// Checks the temperament arithmetic in include/temperament.h on a computer.
//
//   g++ -std=c++17 -O2 -I../include temperament_test.cpp -o /tmp/temperament_test && /tmp/temperament_test
//
// It uses the firmware's own header and generated table, and the same
// expressions main.cpp uses for chord voices, the glide path and the harp.
#include <cmath>
#include <cstdio>
#include <cstring>
#include "temperament.h"

static int failures = 0;
#define CHECK(cond, ...) do { if (!(cond)) { failures++; if (failures <= 20) { printf("FAIL: "); printf(__VA_ARGS__); printf("\n"); } } } while (0)

static double cents(double ratio) { return 1200.0 * log2(ratio); }

int main() {
  const float c_frequency = 130.81f;

  // 1. Equal is the old expression, bit for bit.
  for (int note = -60; note <= 200; note++) {
    double old_ratio = pow(2, note / 12.0);
    double now = temperament_ratio(0, 12, note);
    CHECK(memcmp(&old_ratio, &now, sizeof(double)) == 0, "equal temperament differs from the old expression at note %d", note);
  }

  for (uint8_t t = 0; t < temperament_count; t++) {
    if (temperament_profiles[t].edo_index == 0)
      CHECK(temperament_profiles[t].cents[9] == 0, "temperament %d: A has an offset", t);
    for (int note = 0; note <= 160; note++) {
      double r = temperament_ratio(t, 12, note);
      // 2. Each note sits at its table offset from equal temperament.
      double expected = note * 100.0 + temperament_profiles[t].cents[note % 12];
      CHECK(fabs(cents(r) - expected) < 1e-6, "temperament %d note %d: %.6f cents, expected %.6f", t, note, cents(r), expected);
      // 3. Octaves stay pure.
      if (note >= 12) CHECK(fabs(cents(r / temperament_ratio(t, 12, note - 12)) - 1200.0) < 1e-6, "temperament %d note %d: octave not pure", t, note);
      // 4. A is at its equal-tempered pitch, so the master tuning applies to it.
      if (note % 12 == 9) CHECK(fabs(cents(r) - note * 100.0) < 1e-9, "temperament %d: A at note %d moved", t, note);
    }
  }

  // 5. The glide path lands every voice on its tempered pitch. Mirrors
  //    set_chord_voice_frequency: oscillators on the equal-tempered middle note,
  //    reached through frequencyModulation(2), i.e. two octaves per unit of DC.
  int glide_cases = 0;
  double worst_glide_error = 0, widest_offset = 0;
  for (uint8_t t = 0; t < temperament_count; t++)
    for (int chord_octave_change = -2; chord_octave_change <= 2; chord_octave_change++)
      for (int level = 0; level <= 3; level++)             // the shuffling row's octave for this voice
        for (int transpose = 0; transpose <= 12; transpose++)
          for (int tone = 0; tone < 28; tone++) {             // root, chord tone and sharp within that octave
            int current_note = 12 * level + tone;
            float note_freq = pow(2, chord_octave_change) * c_frequency / 8 * temperament_ratio(t, 12, current_note + transpose);
            int note_level = 12 * chord_octave_change - 3 * 12 + current_note + transpose;
            int base_octave = chord_octave_change - 2 + level;
            int middle_note = base_octave * 12 + transpose;
            int note_delta = note_level - middle_note;
            float middle_freq = c_frequency * pow(2, middle_note / 12.0);
            float glide_offset = (t == 0) ? note_delta / 24.0 : log2(note_freq / middle_freq) / 2.0;
            double reached = middle_freq * pow(2, 2.0 * glide_offset);
            double error = fabs(cents(reached / note_freq));
            if (error > worst_glide_error) worst_glide_error = error;
            if (fabs(glide_offset) > widest_offset) widest_offset = fabs(glide_offset);
            glide_cases++;
          }
  // float precision on the device side is about 0.001 cent at these values
  CHECK(worst_glide_error < 0.01, "glide lands %.4f cents from the tempered pitch", worst_glide_error);
  // the DC source is clamped to +/-1, so the offset has to stay inside it
  CHECK(widest_offset <= 1.0, "glide needs a DC offset of %.3f, outside +/-1", widest_offset);

  // 6. Before the glide fix, every temperament other than equal came out equal.
  double old_glide_error = 0;
  for (uint8_t t = 1; t < temperament_count; t++)
    for (int current_note = 0; current_note < 12; current_note++) {
      float note_freq = c_frequency / 8 * temperament_ratio(t, 12, current_note);
      int note_delta = -36 + current_note + 24;
      float middle_freq = c_frequency * pow(2, -24 / 12.0);
      double reached = middle_freq * pow(2, 2.0 * (note_delta / 24.0));
      old_glide_error = fmax(old_glide_error, fabs(cents(reached / note_freq)));
    }

  // 7. The harp transient keeps its interval to the tempered string: the
  // firmware computes it as temper_ratio(note % EDO + level), so relative to the
  // string it is the level minus the octaves stripped by the modulo, whatever
  // the temperament does to the pitch classes involved.
  for (uint8_t t = 0; t < temperament_count; t++)
    for (int note = 0; note < 48; note++)
      for (int level = 0; level <= 24; level += 7) {
        uint8_t edo = (temperament_profiles[t].edo_index == 0) ? 12
                    : (temperament_profiles[t].edo_index == 1) ? 19 : 31;
        double string = temperament_ratio(t, edo, note);
        double transient = temperament_ratio(t, edo, note % edo + level);
        double interval = cents(transient / string);
        double expected = (level - (note - note % edo)) * 1200.0 / edo;
        if (temperament_profiles[t].edo_index == 0) {
          // in twelve notes the string and the transient can sit on different
          // pitch classes, so the interval bends by up to the widest spread in
          // any offset table (39 cents, quarter-comma meantone's C# to D#)
          CHECK(fabs(interval - expected) < 40.0, "temperament %d note %d level %d: transient %.2f cents from expected", t, note, level, interval - expected);
        } else {
          CHECK(fabs(interval - expected) < 1e-9, "%d-EDO note %d level %d: transient off by %.6f cents", edo, note, level, interval - expected);
        }
      }

  // The divided octaves: temperaments 10 (19-EDO) and 11 (31-EDO).
  // A step is exactly 1200/N cents whatever the twelve-note tables say,
  // the octave is exact, and the note may be fractional (the glide path).
  struct { uint8_t t; int edo; } divisions[] = {{10, 19}, {11, 31}};
  for (auto d : divisions) {
    for (int note = -d.edo; note <= 4 * d.edo; note++) {
      double want = note * 1200.0 / d.edo;
      double got = cents(temperament_ratio(d.t, d.edo, note));
      CHECK(fabs(got - want) < 1e-9, "%d-EDO note %d: %f cents, wanted %f", d.edo, note, got, want);
    }
    CHECK(fabs(temperament_ratio(d.t, d.edo, d.edo) - 2.0) < 1e-12, "%d-EDO octave is not 2:1", d.edo);
    double half = cents(temperament_ratio(d.t, d.edo, 0.5));
    CHECK(fabs(half - 600.0 / d.edo) < 1e-9, "%d-EDO fractional note broke: %f", d.edo, half);
  }
  // The intervals the divisions are chosen for.
  CHECK(fabs(cents(temperament_ratio(10, 19, 5)) - 315.789) < 0.001, "19-EDO minor third");      // 0.6c from 6:5
  CHECK(fabs(cents(temperament_ratio(11, 31, 10)) - 387.097) < 0.001, "31-EDO major third");     // 0.8c from 5:4
  CHECK(fabs(cents(temperament_ratio(11, 31, 25)) - 967.742) < 0.001, "31-EDO harmonic seventh"); // 1.1c from 7:4

  if (failures) {
    printf("temperament test: %d failure(s)\n", failures);
    return 1;
  }
  printf("temperament test: %d temperaments; equal is bit-identical to the old expression; "
         "every note within 1e-6 cents of its table offset, octaves pure, A untouched; "
         "%d glide cases land within %.4f cents (largest DC offset %.3f of 1), where the old "
         "glide path was up to %.1f cents off\n",
         temperament_count, glide_cases, worst_glide_error, widest_offset, old_glide_error);
  return 0;
}
