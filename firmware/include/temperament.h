// Temperament arithmetic, kept free of Arduino and audio code so it can be
// checked on a computer (generator/temperament_test.cpp) as well as used by
// the firmware. The tables themselves are generated: see temperament_profiles.h.
#pragma once
#include <stdint.h>
#include <math.h>
#include "temperament_profiles.h"

// Frequency ratio of a note in temperament t, with the octave divided into edo
// steps. In the twelve-note temperaments the note is nudged by a few cents; in
// 19 and 31 the octave is simply divided differently. Temperament 0 with
// edo 12 returns exactly the expression the firmware has always used, so the
// default path is bit-for-bit unchanged. The note may be fractional (glide).
inline double temperament_ratio(uint8_t t, uint8_t edo, double note) {
  if (edo != 12) return pow(2, note / (double)edo);
  if (t == 0 || t >= temperament_count) return pow(2, note / 12.0);
  int n = (int)floor(note);
  int pitch_class = ((n % 12) + 12) % 12;
  return pow(2, (note + temperament_profiles[t].cents[pitch_class] / 100.0) / 12.0);
}
