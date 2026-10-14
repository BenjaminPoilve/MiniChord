// The rhythm mode's accompaniment styles (rhythm style, address 274), as written parts for the
// four chord voices. See ACCOMPANIMENT in main.cpp for how they are played.
//
// Each event is one note: when it falls in the pattern, which chord voice plays it, what note it
// is, and how long, loud and late (in a strum) it is. Time is counted in 24ths of a beat, as MIDI
// clock counts it: a beat is 24, an eighth 12, a sixteenth 6. A pattern repeats every `length`.
//
// The note is a role, worked out from the chord as it is when the note plays, so a part follows
// whatever the player holds, in any key:
//   ACC_V1 to ACC_V4  the chord voices as they are voiced, lowest first, so inversions and spacing
//                     shape the figure
//   ACC_ROOT          the chord's root, at or below its lowest voice: the bass register
//   ACC_BASS          the same for the chord's bass note, which a slash chord changes
//   ACC_THIRD ...     the chord's own tones above that root: its third, its fifth (the tone nearest
//                     a perfect fifth, so a diminished chord's flat five), its seventh (the octave
//                     when it has none); and the chord's second, fourth and sixth
//   ACC_FLAT7         a minor seventh above the root whatever the chord, as the blues has it
//   ACC_APPROACH      a step from the next chord's bass, from the side the voice is coming from, to
//                     lead into it (the next chord is the one held: with chord change on the beat or
//                     the bar, the one waiting to come in)
// octave moves any of them by whole octaves. length 0 lets the note ring until the voice plays
// again. roll delays the note by that many strings of a strum (inter-string delay each).
//
// Voice 0 is the bass part and voices 1-3 the chord part (rhythm bass and rhythm chords, 275 and
// 276).

#ifndef ACCOMPANIMENT_STYLES_H
#define ACCOMPANIMENT_STYLES_H

enum {
  ACC_V1, ACC_V2, ACC_V3, ACC_V4,
  ACC_ROOT, ACC_BASS, ACC_THIRD, ACC_FIFTH, ACC_SIXTH, ACC_SEVENTH, ACC_FLAT7, ACC_SECOND, ACC_FOURTH,
  ACC_APPROACH
};

struct acc_event_t {
  uint16_t at;        // 24ths of a beat from the start of the pattern
  uint8_t voice;      // chord voice 0-3
  uint8_t role;       // ACC_*
  int8_t octave;
  uint8_t length;     // 24ths of a beat, 0 rings on
  uint8_t velocity;   // 1-127
  uint8_t roll;       // strings of a strum to wait
};

struct acc_style_t {
  const char *name;
  uint16_t length;          // 24ths of a beat
  uint8_t beats_per_bar;
  uint8_t swing_unit;       // what the shuffle (190) swings: 12 eighths, 6 sixteenths
  uint8_t count;
  const acc_event_t *events;
};

#define ACC_BEAT 24

// the chord part's three voices together, as a pianist's right hand or a guitar's top strings
#define ACC_CHORD(at, len, vel) \
  {(at), 1, ACC_V2, 0, (len), (vel), 0}, {(at), 2, ACC_V3, 0, (len), (vel), 0}, {(at), 3, ACC_V4, 0, (len), (vel), 0}
// a guitar's downstroke, lowest string first, and its upstroke, from the top and missing the bass
#define ACC_DOWN(at, len, vel) \
  {(at), 0, ACC_V1, 0, (len), (vel), 0}, {(at), 1, ACC_V2, 0, (len), (vel), 1}, \
  {(at), 2, ACC_V3, 0, (len), (vel), 2}, {(at), 3, ACC_V4, 0, (len), (vel), 3}
#define ACC_UP(at, len, vel) \
  {(at), 3, ACC_V4, 0, (len), (vel), 0}, {(at), 2, ACC_V3, 0, (len), (vel), 1}, {(at), 1, ACC_V2, 0, (len), (vel), 2}

// 1. Alberti bass: lowest, top, middle, top in sixteenths, the classical left hand (Mozart's C major
// sonata). Three voices, the harp free for the tune; an inversion makes it move as a hand does.
const acc_event_t acc_alberti[] PROGMEM = {
  {0, 0, ACC_V1, 0, 7, 100, 0}, {6, 2, ACC_V3, 0, 7, 72, 0}, {12, 1, ACC_V2, 0, 7, 84, 0}, {18, 2, ACC_V3, 0, 7, 72, 0},
};

// 2. Waltz: oom-pah-pah, the bass alternating root and the fifth below bar to bar
const acc_event_t acc_waltz[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 20, 110, 0}, ACC_CHORD(24, 10, 78), ACC_CHORD(48, 10, 70),
  {72, 0, ACC_FIFTH, -1, 20, 100, 0}, ACC_CHORD(96, 10, 78), ACC_CHORD(120, 10, 70),
};

// 3. Boom-chick: root, chord, fifth below, chord; country, folk, a two-beat
const acc_event_t acc_boom_chick[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 18, 110, 0}, ACC_CHORD(24, 8, 88), {48, 0, ACC_FIFTH, -1, 18, 100, 0}, ACC_CHORD(72, 8, 88),
};

// 4. Walking bass: root, third, fifth and a chromatic step into the next chord, under a Charleston
// comp (the beat and the and of two). Swing it with the shuffle.
const acc_event_t acc_walking[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 22, 105, 0}, {24, 0, ACC_THIRD, 0, 22, 88, 0}, {48, 0, ACC_FIFTH, 0, 22, 96, 0}, {72, 0, ACC_APPROACH, 0, 22, 88, 0},
  ACC_CHORD(0, 10, 90), ACC_CHORD(36, 8, 78),
};

// 5. Boogie: the rock and roll rhythm guitar, root under a fifth rocking to the sixth, and in the
// second bar out to the flat seventh. Two voices; shuffle it for a boogie, straight for Chuck Berry.
#define ACC_BOOGIE_ROOT(at, vel) {(at), 0, ACC_ROOT, 0, 10, (vel), 0}
#define ACC_BOOGIE(at, role, vel) {(at), 1, (role), 0, 10, (vel), 0}
const acc_event_t acc_boogie[] PROGMEM = {
  ACC_BOOGIE_ROOT(0, 100), ACC_BOOGIE(0, ACC_FIFTH, 96), ACC_BOOGIE_ROOT(12, 76), ACC_BOOGIE(12, ACC_FIFTH, 72),
  ACC_BOOGIE_ROOT(24, 92), ACC_BOOGIE(24, ACC_SIXTH, 90), ACC_BOOGIE_ROOT(36, 76), ACC_BOOGIE(36, ACC_SIXTH, 72),
  ACC_BOOGIE_ROOT(48, 96), ACC_BOOGIE(48, ACC_FIFTH, 92), ACC_BOOGIE_ROOT(60, 76), ACC_BOOGIE(60, ACC_FIFTH, 72),
  ACC_BOOGIE_ROOT(72, 92), ACC_BOOGIE(72, ACC_SIXTH, 90), ACC_BOOGIE_ROOT(84, 76), ACC_BOOGIE(84, ACC_SIXTH, 72),
  ACC_BOOGIE_ROOT(96, 100), ACC_BOOGIE(96, ACC_FIFTH, 96), ACC_BOOGIE_ROOT(108, 76), ACC_BOOGIE(108, ACC_FIFTH, 72),
  ACC_BOOGIE_ROOT(120, 92), ACC_BOOGIE(120, ACC_SIXTH, 90), ACC_BOOGIE_ROOT(132, 76), ACC_BOOGIE(132, ACC_SIXTH, 72),
  ACC_BOOGIE_ROOT(144, 96), ACC_BOOGIE(144, ACC_FLAT7, 92), ACC_BOOGIE_ROOT(156, 76), ACC_BOOGIE(156, ACC_FLAT7, 72),
  ACC_BOOGIE_ROOT(168, 92), ACC_BOOGIE(168, ACC_SIXTH, 90), ACC_BOOGIE_ROOT(180, 76), ACC_BOOGIE(180, ACC_SIXTH, 72),
};

// 6. Travis picking: the thumb alternating bass and a middle string on the beats, the fingers on
// the top strings between, pinched with the bass on the one
const acc_event_t acc_travis[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 22, 105, 0}, {0, 3, ACC_V4, 0, 12, 84, 0}, {12, 2, ACC_V3, 0, 10, 68, 0},
  {24, 1, ACC_V2, 0, 22, 90, 0}, {36, 3, ACC_V4, 0, 10, 74, 0},
  {48, 0, ACC_FIFTH, -1, 22, 100, 0}, {60, 2, ACC_V3, 0, 10, 68, 0},
  {72, 1, ACC_V2, 0, 22, 90, 0}, {84, 3, ACC_V4, 0, 10, 74, 0},
};

// 7. Strummed guitar: down, down-up, up-down-up, the campfire pattern. The strings ring until
// struck again; the inter-string delay sets how spread each strum is.
const acc_event_t acc_strum[] PROGMEM = {
  ACC_DOWN(0, 0, 110), ACC_DOWN(24, 0, 92), ACC_UP(36, 0, 72), ACC_UP(60, 0, 72), ACC_DOWN(72, 0, 92), ACC_UP(84, 0, 72),
};

// 8. Bossa nova: the bass on one and three, each led into from the and before, and the guitar's
// two-bar syncopation over it
const acc_event_t acc_bossa[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 34, 105, 0}, {36, 0, ACC_FIFTH, -1, 10, 82, 0}, {48, 0, ACC_FIFTH, -1, 34, 95, 0}, {84, 0, ACC_ROOT, 0, 10, 82, 0},
  {96, 0, ACC_ROOT, 0, 34, 105, 0}, {132, 0, ACC_FIFTH, -1, 10, 82, 0}, {144, 0, ACC_FIFTH, -1, 34, 95, 0}, {180, 0, ACC_ROOT, 0, 10, 82, 0},
  ACC_CHORD(0, 9, 88), ACC_CHORD(36, 9, 78), ACC_CHORD(72, 9, 80), ACC_CHORD(120, 9, 78), ACC_CHORD(156, 9, 80),
};

// 9. Arpeggio up, in sixteenths, each note ringing a little into the next
const acc_event_t acc_arp_up[] PROGMEM = {
  {0, 0, ACC_V1, 0, 8, 100, 0}, {6, 1, ACC_V2, 0, 8, 78, 0}, {12, 2, ACC_V3, 0, 8, 86, 0}, {18, 3, ACC_V4, 0, 8, 78, 0},
};

// 10. Arpeggio up and down: six notes against the four sixteenths of a beat, so it turns over
// across the bar as an arpeggiator's does
const acc_event_t acc_arp_up_down[] PROGMEM = {
  {0, 0, ACC_V1, 0, 8, 100, 0}, {6, 1, ACC_V2, 0, 8, 78, 0}, {12, 2, ACC_V3, 0, 8, 82, 0},
  {18, 3, ACC_V4, 0, 8, 88, 0}, {24, 2, ACC_V3, 0, 8, 78, 0}, {30, 1, ACC_V2, 0, 8, 74, 0},
};

// 11. Ballad: the pop piano, root and fifth held in the bass under chords pulsing in eighths
const acc_event_t acc_ballad[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 46, 100, 0}, {48, 0, ACC_FIFTH, 0, 46, 88, 0},
  ACC_CHORD(0, 10, 84), ACC_CHORD(12, 10, 60), ACC_CHORD(24, 10, 72), ACC_CHORD(36, 10, 60),
  ACC_CHORD(48, 10, 80), ACC_CHORD(60, 10, 60), ACC_CHORD(72, 10, 72), ACC_CHORD(84, 10, 60),
};

// 12. Reggae: the skank, short chords on every and, over a bass that walks the chord
const acc_event_t acc_reggae[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 18, 100, 0}, {36, 0, ACC_THIRD, 0, 8, 80, 0}, {48, 0, ACC_FIFTH, 0, 18, 95, 0},
  {78, 0, ACC_SIXTH, 0, 4, 72, 0}, {84, 0, ACC_FIFTH, 0, 8, 80, 0},
  ACC_CHORD(12, 6, 95), ACC_CHORD(36, 6, 88), ACC_CHORD(60, 6, 95), ACC_CHORD(84, 6, 88),
};

// 13. 6/8 arpeggio: up and back down in eighths, two pulses of three to the bar (House of the
// Rising Sun). Counted in beats of two eighths, three to the bar, so the tempo is a quarter note's.
const acc_event_t acc_six_eight[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 22, 100, 0}, {12, 1, ACC_V2, 0, 22, 74, 0}, {24, 2, ACC_V3, 0, 22, 80, 0},
  {36, 3, ACC_V4, 0, 22, 88, 0}, {48, 2, ACC_V3, 0, 22, 76, 0}, {60, 1, ACC_V2, 0, 22, 72, 0},
};

// 14. Habanera: the dotted bass of the tango and Carmen, in two
const acc_event_t acc_habanera[] PROGMEM = {
  {0, 0, ACC_ROOT, 0, 16, 105, 0}, {18, 0, ACC_FIFTH, -1, 6, 80, 0}, {24, 0, ACC_THIRD, 0, 12, 92, 0}, {36, 0, ACC_FIFTH, 0, 12, 88, 0},
  ACC_CHORD(24, 10, 76),
};

#define ACC_STYLE(name, length, beats, swing, events) {name, length, beats, swing, sizeof(events) / sizeof(acc_event_t), events}
const acc_style_t acc_styles[] PROGMEM = {
  {"pattern", 0, 4, 12, 0, nullptr},   // 0: the sixteen steps of rythm pattern (220-235), as they always were
  ACC_STYLE("alberti", 24, 4, 6, acc_alberti),
  ACC_STYLE("waltz", 144, 3, 12, acc_waltz),
  ACC_STYLE("boom-chick", 96, 4, 12, acc_boom_chick),
  ACC_STYLE("walking bass", 96, 4, 12, acc_walking),
  ACC_STYLE("boogie", 192, 4, 12, acc_boogie),
  ACC_STYLE("travis picking", 96, 4, 12, acc_travis),
  ACC_STYLE("strum", 96, 4, 12, acc_strum),
  ACC_STYLE("bossa nova", 192, 4, 12, acc_bossa),
  ACC_STYLE("arpeggio up", 24, 4, 6, acc_arp_up),
  ACC_STYLE("arpeggio up and down", 36, 4, 6, acc_arp_up_down),
  ACC_STYLE("ballad", 96, 4, 12, acc_ballad),
  ACC_STYLE("reggae", 96, 4, 12, acc_reggae),
  ACC_STYLE("6/8 arpeggio", 72, 3, 12, acc_six_eight),
  ACC_STYLE("habanera", 48, 2, 12, acc_habanera),
};
const uint8_t acc_style_count = sizeof(acc_styles) / sizeof(acc_style_t);

#endif
