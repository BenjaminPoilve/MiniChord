#include "audio_definition.h"
#include "def.h"
#include "temperament.h"
#include <AT42QT2120.h>
#include <Arduino.h>
#include <Audio.h>
#include <LittleFS.h>
#include <SPI.h>
#include <Wire.h>
#include <button_matrix.h>
#include <debouncer.h>
#include <harp.h>
#include <potentiometer.h>

//>>SOFWTARE VERSION 
const uint16_t firmware_version_adress = 7;   // where the writing firmware's version is stamped
void apply_preset_version(int bank_number);
int version_ID=11; //to be read 00.03, stored at adress 7 in memory
//>>BUTTON ARRAYS<<
debouncer harp_array[12];
debouncer chord_matrix_array[22];

//>>HARDWARE SETUP<<
harp harp_sensor;
button_matrix chord_matrix(SHIFT_DATA_PIN, SHIFT_STORAGE_CLOCK_PIN, SHIFT_CLOCK_PIN, READ_MATRIX_1_PIN, READ_MATRIX_2_PIN, READ_MATRIX_3_PIN);
debouncer hold_button;
debouncer up_button;
debouncer down_button;
debouncer LBO_flag;
bool flag_save_needed=false; //to know if we need to save the preset
potentiometer chord_pot(POT_CHORD_PIN);
potentiometer harp_pot(POT_HARP_PIN);
potentiometer mod_pot(POT_MOD_PIN);
LittleFS_Program myfs; // to save the settings
float color_led_blink_val = 1.0;
bool led_blinking_flag = false;
float led_attenuation = 0.0; 



//>>CHORD DEFINITION<<
//for each chord, we first have the 4 notes of the chord, then decoration that might be used in specific modes
/* ---- octave divisions -------------------------------------------------------
 *
 * Temperaments 10, 11 and 12 divide the octave into 19, 24 and 31 steps instead
 * of twelve, so every table of note numbers has a version per division: chords,
 * scales, the chord scales of the scale-per-chord harp, the root offsets and
 * the button base notes. apply_temperament() copies the live division's set
 * over the working tables, and EDO, sharp_step and transpose_steps carry the
 * arithmetic everywhere a twelve used to be hardcoded. Intervals are the
 * nearest approximation of each just target in the division: in 31 a major
 * third is 10 steps (387 cents, one cent from 5:4) and a fifth 18 (697).
 *
 * 24 is the odd one out: not a meantone but twelve with a note between each
 * pair, the quarter-tones. Its tables are twelve's doubled, and the modifier
 * moves a letter by one step instead of two (modifier_step), so it plays the
 * note between the frets, while key signatures still sharpen by a semitone.
 */
const uint8_t edo_steps[4] = {12, 19, 31, 24};
const uint8_t edo_sharp[4] = {1, 1, 2, 2};   // how far a key signature's sharp moves a letter: one step in 12 and 19, two in 31 and 24
// How far the MODIFIER moves a letter. In 12, 19 and 31 it is the chromatic semitone, the same as a
// key signature's sharp. In 24 it is one step, a quarter-tone: 24 holds twelve inside it, so a
// semitone modifier would only double twelve, while a quarter-tone one reaches the twelve notes in
// between. Key signatures still move a letter by the semitone (edo_sharp), so G major is G major.
const uint8_t edo_modifier[4] = {1, 1, 2, 1};
uint8_t EDO = 12;             // steps per octave of the live division
uint8_t edo_index = 0;        // which of the four divisions is live
uint8_t sharp_step = 1;       // how far a key signature's sharp or flat moves a letter
uint8_t modifier_step = 1;    // how far the modifier moves one: the same, but a quarter-tone in 24
uint8_t transpose_steps = 0;  // transposition in steps of the live division; equal to transpose_semitones in 12
const int8_t edo_base_notes[4][7] = {
  {11, 4, 9, 2, 7, 0, 5},
  {17, 6, 14, 3, 11, 0, 8},
  {28, 10, 23, 5, 18, 0, 13},
  {22, 8, 18, 4, 14, 0, 10}
};
const int8_t edo_scale_root_offsets[4][12] = {
  {0, 7, 2, 9, 4, 11, 5, 10, 3, 8, 1, 6},
  {0, 11, 3, 14, 6, 17, 8, 16, 5, 13, 2, 10},
  {0, 18, 5, 23, 10, 28, 13, 26, 8, 21, 3, 16},
  {0, 14, 4, 18, 8, 22, 10, 20, 6, 16, 2, 12}
};
const uint8_t edo_scale_intervals[4][7][8] = {
  {{0, 2, 4, 5, 7, 9, 11, 0}, {0, 2, 4, 7, 9, 0, 0, 0}, {0, 2, 3, 7, 10, 0, 0, 0}, {0, 2, 4, 5, 7, 8, 9, 11}, {0, 2, 3, 5, 7, 8, 10, 0}, {0, 2, 3, 5, 7, 8, 11, 0}, {0, 2, 3, 7, 10, 0, 0, 0}},
  {{0, 3, 6, 8, 11, 14, 17, 0}, {0, 3, 6, 11, 14, 0, 0, 0}, {0, 3, 5, 11, 16, 0, 0, 0}, {0, 3, 6, 8, 11, 13, 14, 17}, {0, 3, 5, 8, 11, 13, 16, 0}, {0, 3, 5, 8, 11, 13, 17, 0}, {0, 3, 5, 11, 16, 0, 0, 0}},
  {{0, 5, 10, 13, 18, 23, 28, 0}, {0, 5, 10, 18, 23, 0, 0, 0}, {0, 5, 8, 18, 26, 0, 0, 0}, {0, 5, 10, 13, 18, 21, 23, 28}, {0, 5, 8, 13, 18, 21, 26, 0}, {0, 5, 8, 13, 18, 21, 28, 0}, {0, 5, 8, 18, 26, 0, 0, 0}},
  {{0, 4, 8, 10, 14, 18, 22, 0}, {0, 4, 8, 14, 18, 0, 0, 0}, {0, 4, 6, 14, 20, 0, 0, 0}, {0, 4, 8, 10, 14, 16, 18, 22}, {0, 4, 6, 10, 14, 16, 20, 0}, {0, 4, 6, 10, 14, 16, 22, 0}, {0, 4, 6, 14, 20, 0, 0, 0}}
};
// Rows 5, 6 and 9 carry the same named tones as the diminished and augmented
// chords they serve: the whole tone row by its names, and the octatonic (dim)
// and offset diminished sixth (dim7) rows with the chord's own G-flat and
// B-double-flat in place of F-sharp and A, so the harp in modes 8 and 9 plays
// the chord it is under. Row 15, locrian, is the utonal tetrad's (a just chord,
// m7b5 in twelve), by its names too.
const uint8_t edo_chord_scale_intervals[4][16][8] = {
  {{0, 2, 4, 7, 9, 0, 0, 0}, {0, 2, 4, 6, 9, 0, 0, 0}, {0, 3, 5, 7, 10, 0, 0, 0}, {0, 2, 4, 7, 10, 0, 0, 0}, {0, 3, 5, 7, 9, 0, 0, 0}, {0, 1, 3, 4, 6, 7, 9, 10}, {0, 2, 4, 6, 8, 10, 0, 0}, {0, 2, 4, 5, 7, 8, 9, 11}, {0, 2, 3, 5, 7, 8, 9, 11}, {0, 2, 3, 4, 6, 7, 9, 11}, {0, 2, 4, 5, 7, 9, 11, 0}, {0, 2, 3, 5, 7, 9, 10, 0}, {0, 2, 4, 6, 7, 9, 11, 0}, {0, 2, 4, 5, 7, 9, 10, 0}, {0, 2, 3, 5, 7, 8, 10, 0}, {0, 1, 3, 5, 6, 8, 10, 0}},
  {{0, 3, 6, 11, 14, 0, 0, 0}, {0, 3, 6, 9, 14, 0, 0, 0}, {0, 5, 8, 11, 16, 0, 0, 0}, {0, 3, 6, 11, 16, 0, 0, 0}, {0, 5, 8, 11, 14, 0, 0, 0}, {0, 2, 5, 6, 10, 11, 14, 16}, {0, 3, 6, 9, 12, 15, 0, 0}, {0, 3, 6, 8, 11, 13, 14, 17}, {0, 3, 5, 8, 11, 13, 14, 17}, {0, 3, 5, 6, 10, 11, 15, 17}, {0, 3, 6, 8, 11, 14, 17, 0}, {0, 3, 5, 8, 11, 14, 16, 0}, {0, 3, 6, 9, 11, 14, 17, 0}, {0, 3, 6, 8, 11, 14, 16, 0}, {0, 3, 5, 8, 11, 13, 16, 0}, {0, 2, 5, 8, 10, 13, 16, 0}},
  {{0, 5, 10, 18, 23, 0, 0, 0}, {0, 5, 10, 15, 23, 0, 0, 0}, {0, 8, 13, 18, 26, 0, 0, 0}, {0, 5, 10, 18, 26, 0, 0, 0}, {0, 8, 13, 18, 23, 0, 0, 0}, {0, 3, 8, 10, 16, 18, 23, 26}, {0, 5, 10, 15, 20, 25, 0, 0}, {0, 5, 10, 13, 18, 21, 23, 28}, {0, 5, 8, 13, 18, 21, 23, 28}, {0, 5, 8, 10, 16, 18, 24, 28}, {0, 5, 10, 13, 18, 23, 28, 0}, {0, 5, 8, 13, 18, 23, 26, 0}, {0, 5, 10, 15, 18, 23, 28, 0}, {0, 5, 10, 13, 18, 23, 26, 0}, {0, 5, 8, 13, 18, 21, 26, 0}, {0, 3, 8, 13, 16, 21, 26, 0}},
  {{0, 4, 8, 14, 18, 0, 0, 0}, {0, 4, 8, 12, 18, 0, 0, 0}, {0, 6, 10, 14, 20, 0, 0, 0}, {0, 4, 8, 14, 20, 0, 0, 0}, {0, 6, 10, 14, 18, 0, 0, 0}, {0, 2, 6, 8, 12, 14, 18, 20}, {0, 4, 8, 12, 16, 20, 0, 0}, {0, 4, 8, 10, 14, 16, 18, 22}, {0, 4, 6, 10, 14, 16, 18, 22}, {0, 4, 6, 8, 12, 14, 18, 22}, {0, 4, 8, 10, 14, 18, 22, 0}, {0, 4, 6, 10, 14, 18, 20, 0}, {0, 4, 8, 12, 14, 18, 22, 0}, {0, 4, 8, 10, 14, 18, 20, 0}, {0, 4, 6, 10, 14, 16, 20, 0}, {0, 2, 6, 10, 12, 16, 20, 0}}
};
const uint8_t edo_major[4][7] = {{0, 4, 7, 12, 2, 5, 9}, {0, 6, 11, 19, 3, 8, 14}, {0, 10, 18, 31, 5, 13, 23}, {0, 8, 14, 24, 4, 10, 18}};
const uint8_t edo_minor[4][7] = {{0, 3, 7, 12, 1, 5, 8}, {0, 5, 11, 19, 2, 8, 13}, {0, 8, 18, 31, 3, 13, 21}, {0, 6, 14, 24, 2, 10, 16}};
const uint8_t edo_maj_sixth[4][7] = {{0, 4, 7, 9, 2, 5, 12}, {0, 6, 11, 14, 3, 8, 19}, {0, 10, 18, 23, 5, 13, 31}, {0, 8, 14, 18, 4, 10, 24}};
const uint8_t edo_min_sixth[4][7] = {{0, 3, 7, 9, 1, 5, 12}, {0, 5, 11, 14, 2, 8, 19}, {0, 8, 18, 23, 3, 13, 31}, {0, 6, 14, 18, 2, 10, 24}};
const uint8_t edo_seventh[4][7] = {{0, 4, 10, 7, 2, 5, 9}, {0, 6, 16, 11, 3, 8, 14}, {0, 10, 26, 18, 5, 13, 23}, {0, 8, 20, 14, 4, 10, 18}};
const uint8_t edo_maj_seventh[4][7] = {{0, 4, 11, 7, 2, 5, 9}, {0, 6, 17, 11, 3, 8, 14}, {0, 10, 28, 18, 5, 13, 23}, {0, 8, 22, 14, 4, 10, 18}};
const uint8_t edo_min_seventh[4][7] = {{0, 3, 10, 7, 1, 5, 8}, {0, 5, 16, 11, 2, 8, 13}, {0, 8, 26, 18, 3, 13, 21}, {0, 6, 20, 14, 2, 10, 16}};
// The diminished and augmented tones are the NAMED intervals, like every other
// table here: a diminished fifth is G-flat (16 of 31, 10 of 19), not F-sharp
// (15, 9); a diminished seventh B-double-flat (24, 15), not A (23, 14); an
// augmented fifth G-sharp (20, 12), not A-flat (21, 13). These had been
// generated by rounding cents, which lands on the neighbouring enharmonic --
// the pitches twelve cannot tell apart and 19 and 31 can.
const uint8_t edo_aug[4][7] = {{0, 4, 8, 12, 2, 5, 9}, {0, 6, 12, 19, 3, 8, 14}, {0, 10, 20, 31, 5, 13, 23}, {0, 8, 16, 24, 4, 10, 18}};
const uint8_t edo_dim[4][7] = {{0, 3, 6, 12, 2, 5, 9}, {0, 5, 10, 19, 3, 8, 14}, {0, 8, 16, 31, 5, 13, 23}, {0, 6, 12, 24, 4, 10, 18}};
const uint8_t edo_full_dim[4][7] = {{0, 3, 6, 9, 2, 5, 12}, {0, 5, 10, 15, 3, 8, 19}, {0, 8, 16, 24, 5, 13, 31}, {0, 6, 12, 18, 4, 10, 24}};
// The alternate layout's chords (firmware #130), from the same interval names.
// Without these they kept their twelve-note numbers in 19 and 31, read there as
// steps: a 31-EDO sus4 of 0 5 7 is a second and a quarter-octave, not F and G.
const uint8_t edo_half_dim[4][7] = {{0, 3, 6, 10, 2, 5, 8}, {0, 5, 10, 16, 3, 8, 13}, {0, 8, 16, 26, 5, 13, 21}, {0, 6, 12, 20, 4, 10, 16}};
const uint8_t edo_sus_fourth[4][7] = {{0, 5, 7, 12, 2, 9, 10}, {0, 8, 11, 19, 3, 14, 16}, {0, 13, 18, 31, 5, 23, 26}, {0, 10, 14, 24, 4, 18, 20}};
const uint8_t edo_sus_second[4][7] = {{0, 2, 7, 12, 5, 9, 4}, {0, 3, 11, 19, 8, 14, 6}, {0, 5, 18, 31, 13, 23, 10}, {0, 4, 14, 24, 10, 18, 8}};
const uint8_t edo_seventh_sus[4][7] = {{0, 5, 10, 7, 2, 9, 4}, {0, 8, 16, 11, 3, 14, 6}, {0, 13, 26, 18, 5, 23, 10}, {0, 10, 20, 14, 4, 18, 8}};
const uint8_t edo_major_ninth[4][7] = {{0, 4, 11, 2, 7, 5, 9}, {0, 6, 17, 3, 11, 8, 14}, {0, 10, 28, 5, 18, 13, 23}, {0, 8, 22, 4, 14, 10, 18}};
const uint8_t edo_minor_ninth[4][7] = {{0, 3, 10, 2, 7, 5, 8}, {0, 5, 16, 3, 11, 8, 13}, {0, 8, 26, 5, 18, 13, 21}, {0, 6, 20, 4, 14, 10, 16}};
const uint8_t edo_added_ninth[4][7] = {{0, 4, 7, 2, 5, 9, 11}, {0, 6, 11, 3, 8, 14, 17}, {0, 10, 18, 5, 13, 23, 28}, {0, 8, 14, 4, 10, 18, 22}};
const uint8_t edo_six_nine[4][7] = {{0, 4, 9, 2, 7, 5, 11}, {0, 6, 14, 3, 11, 8, 17}, {0, 10, 23, 5, 18, 13, 28}, {0, 8, 18, 4, 14, 10, 22}};
/* The chords the divided octaves are for: built from ratios rather than letter
 * names, the intervals twelve notes cannot spell. Steps are round(cents * EDO /
 * 1200) of each ratio, and every row is in all four divisions so a layout slot
 * does not break when the temperament changes. In 31 the harmonic seventh is a
 * cent from 7:4 and the neutral third a cent from 11:9, and no tone is more than
 * 15 cents off; in 19 the septimal tones hold within 22 cents, but 19 has no
 * neutral intervals, so a neutral third rounds onto the major third there; in 24
 * the neutral intervals are the quarter-tones. In 12 most of them collapse onto
 * chords the catalogue already has -- the subminor seventh onto m7, the utonal
 * tetrad onto m7b5 -- which is the point of having the divisions at all.
 *
 * The four voices take the first four entries; the remaining tones of the larger
 * sonorities live in the extras, where the rhythm voices and the harp spell them.
 */
const uint8_t edo_neutral[4][7] = {{0, 3, 7, 12, 2, 5, 9}, {0, 6, 11, 19, 3, 8, 14}, {0, 9, 18, 31, 5, 13, 23}, {0, 7, 14, 24, 4, 10, 18}};      // 1 11/9 3/2
const uint8_t edo_harmonic_7th[4][7] = {{0, 4, 10, 7, 2, 5, 9}, {0, 6, 15, 11, 3, 8, 14}, {0, 10, 25, 18, 5, 13, 23}, {0, 8, 19, 14, 4, 10, 18}}; // 4:5:6:7
const uint8_t edo_subminor[4][7] = {{0, 3, 7, 12, 2, 5, 9}, {0, 4, 11, 19, 3, 8, 14}, {0, 7, 18, 31, 5, 13, 23}, {0, 5, 14, 24, 4, 10, 18}};     // 6:7:9
const uint8_t edo_supermajor[4][7] = {{0, 4, 7, 12, 2, 5, 9}, {0, 7, 11, 19, 3, 8, 14}, {0, 11, 18, 31, 5, 13, 23}, {0, 9, 14, 24, 4, 10, 18}};   // 14:18:21
const uint8_t edo_subminor_seventh[4][7] = {{0, 3, 7, 10, 2, 5, 9}, {0, 4, 11, 15, 3, 7, 14}, {0, 7, 18, 25, 5, 12, 23}, {0, 5, 14, 19, 4, 9, 18}};   // 1 7/6 3/2 7/4 | 9/8 21/16 5/3
const uint8_t edo_utonal_tetrad[4][7] = {{0, 3, 6, 10, 2, 5, 8}, {0, 4, 9, 15, 3, 8, 13}, {0, 7, 15, 25, 5, 13, 21}, {0, 5, 12, 19, 4, 10, 16}};    // 1 7/6 7/5 7/4 | 9/8 4/3 8/5
const uint8_t edo_harmonic_ninth[4][7] = {{0, 2, 4, 10, 7, 6, 8}, {0, 3, 6, 15, 11, 9, 13}, {0, 5, 10, 25, 18, 14, 22}, {0, 4, 8, 19, 14, 11, 17}};  // 1 9/8 5/4 7/4 | 3/2 11/8 13/8
const uint8_t edo_neutral_seventh[4][7] = {{0, 3, 7, 10, 2, 6, 9}, {0, 6, 11, 17, 2, 9, 13}, {0, 9, 18, 27, 4, 14, 22}, {0, 7, 14, 21, 3, 11, 17}};   // 1 11/9 3/2 11/6 | 12/11 11/8 18/11
const uint8_t edo_otonal_hexad[4][7] = {{0, 4, 7, 10, 2, 6, 12}, {0, 6, 11, 15, 3, 9, 19}, {0, 10, 18, 25, 5, 14, 31}, {0, 8, 14, 19, 4, 11, 24}}; // 1 5/4 3/2 7/4 | 9/8 11/8 2
// The just augmented is stacked pure major thirds. Its last extra is 125/64,
// the next third up: in twelve it rounds onto the octave, which is the closure
// that makes the equal tempered chord symmetric; in nineteen and thirty one it
// does not, so the harp plays the note that shows the chord no longer closes.
const uint8_t edo_just_augmented[4][7] = {{0, 4, 8, 12, 2, 6, 12}, {0, 6, 12, 19, 3, 9, 18}, {0, 10, 20, 31, 5, 15, 30}, {0, 8, 15, 24, 4, 12, 23}};  // 1 5/4 25/16 2 | 9/8 45/32 125/64
// In nineteen the septimal fourth 21/16 rounds onto the same step as the 9/7
// third, so the extras take the plain 4/3, as the subminor seventh does 5/3.
const uint8_t edo_supermajor_seventh[4][7] = {{0, 4, 7, 11, 2, 5, 9}, {0, 7, 11, 18, 4, 8, 15}, {0, 11, 18, 29, 6, 13, 24}, {0, 9, 14, 23, 5, 10, 19}}; // 1 9/7 3/2 27/14 | 8/7 4/3 12/7

uint8_t major[7] = {0, 4, 7, 12, 2, 5, 9};  // After the four notes of the chord (fundamental, third, fifth of seven, and octave of fifth, the next notes are the second fourth and sixth)
uint8_t minor[7] = {0, 3, 7, 12, 1, 5, 8};
uint8_t maj_sixth[7] = {0, 4, 7, 9, 2, 5, 12};
uint8_t min_sixth[7] = {0, 3, 7, 9, 1, 5, 12};
uint8_t seventh[7] = {0, 4, 10, 7, 2, 5, 9};
uint8_t maj_seventh[7] = {0, 4, 11, 7, 2, 5, 9};
uint8_t min_seventh[7] = {0, 3, 10, 7, 1, 5, 8};
uint8_t aug[7] = {0, 4, 8, 12, 2, 5, 9};
uint8_t dim[7] = {0, 3, 6, 12, 2, 5, 9};
uint8_t full_dim[7] = {0, 3, 6, 9, 2, 5, 12};
//now the chords used for the alternate layout
uint8_t sus_fourth[7]   = {0, 5, 7, 12, 2, 9, 10};  // sus4
uint8_t sus_second[7]   = {0, 2, 7, 12, 5, 9, 4};   // sus2
uint8_t seventh_sus[7]  = {0, 5, 10, 7, 2, 9, 4};   // 7sus4
uint8_t major_ninth[7]  = {0, 4, 11, 2, 7, 5, 9};   // maj9, no fifth
uint8_t minor_ninth[7]  = {0, 3, 10, 2, 7, 5, 8};   // min9, no fifth
uint8_t added_ninth[7]  = {0, 4, 7, 2, 5, 9, 11};   // add9
uint8_t six_nine[7]     = {0, 4, 9, 2, 7, 5, 11};   // 6/9
uint8_t half_dim[7]     = {0, 3, 6, 10, 2, 5, 8};   // m7b5
// The just chords, for the divided octaves; their ratios and rounding are in
// the edo_ tables above.
uint8_t supermajor[7]         = {0, 4, 7, 12, 2, 5, 9};    // 14:18:21
uint8_t subminor[7]           = {0, 3, 7, 12, 2, 5, 9};    // 6:7:9
uint8_t neutral[7]            = {0, 3, 7, 12, 2, 5, 9};    // 1 11/9 3/2
uint8_t harmonic_7th[7]       = {0, 4, 10, 7, 2, 5, 9};    // 4:5:6:7
uint8_t neutral_seventh[7]    = {0, 3, 7, 10, 2, 6, 9};    // 1 11/9 3/2 11/6
uint8_t subminor_seventh[7]   = {0, 3, 7, 10, 2, 5, 9};    // 12:14:18:21
uint8_t utonal_tetrad[7]      = {0, 3, 6, 10, 2, 5, 8};    // 1/7:1/6:1/5:1/4
uint8_t harmonic_ninth[7]     = {0, 2, 4, 10, 7, 6, 8};    // 4:5:6:7:9, the fifth in the extras
uint8_t otonal_hexad[7]       = {0, 4, 7, 10, 2, 6, 12};   // 4:5:6:7, the 9 and 11 in the extras
uint8_t just_augmented[7]     = {0, 4, 8, 12, 2, 6, 12};   // 16:20:25
uint8_t supermajor_seventh[7] = {0, 4, 7, 11, 2, 5, 9};    // 14:18:21:27

uint8_t alt_chord_layout = 0;   // 0 = standard chords, 1 = the assignable layout


// Every chord the instrument can make, in one list, so a button combination can
// be pointed at any of them rather than at a fixed table.
uint8_t (*chord_catalogue[29])[7] = {
  &major, &minor, &seventh, &maj_seventh, &min_seventh, &dim, &aug,
  &maj_sixth, &min_sixth, &full_dim, &half_dim,
  &sus_fourth, &sus_second, &seventh_sus,
  &major_ninth, &minor_ninth, &added_ninth, &six_nine,
  // The just chords keep the order of their twelve-tone counterparts: triads
  // first (supermajor as major, subminor as minor, neutral as the third triad
  // twelve does not have), then the sevenths tracking dominant, major and minor
  // -- 11/6 sits nearer the major seventh than the minor, so the neutral seventh
  // takes the major seat -- then the utonal tetrad where m7b5 sits, the harmonic
  // ninth with the ninths, and the hexad. The just augmented fills the augmented
  // seat, and the supermajor seventh is the septimal major seventh.
  &supermajor, &subminor, &neutral, &harmonic_7th,
  &neutral_seventh, &subminor_seventh, &utonal_tetrad, &harmonic_ninth, &otonal_hexad,
  &just_augmented, &supermajor_seventh
};
const uint8_t chord_catalogue_size = 29;

// One parameter per button combination, so a layout is part of the preset.
const uint8_t alt_slot_adress[7] = {202, 203, 204, 205, 206, 207, 208};

// What each slot plays when its parameter is 0. That matters for compatibility:
// a preset saved before these addresses existed holds 0 in all of them, and
// should still give the suspended and extended set rather than seven majors.
const uint8_t alt_slot_default[7] = {11, 12, 13, 14, 15, 16, 17};

uint8_t (*alt_chord_for(uint8_t slot))[7];
uint8_t key_signature_selection = 0; // 0=C, 1=G, 2=D, 3=A, 4=E, 5=B, 6=F, 7=Bb, 8=Eb, 9=Ab, 10=Db, 11=Gb
enum KeySig { // Enums for KeySigs
  KEY_SIG_C, KEY_SIG_G, KEY_SIG_D, KEY_SIG_A, KEY_SIG_E, KEY_SIG_B,
  KEY_SIG_F, KEY_SIG_Bb, KEY_SIG_Eb, KEY_SIG_Ab, KEY_SIG_Db, KEY_SIG_Gb
};
enum Button { // Button enum in hardware order: B, E, A, D, G, C, F
  BTN_B, BTN_E, BTN_A, BTN_D, BTN_G, BTN_C, BTN_F
};
enum FrameShift { //Enums for chord frame shifts
  FRAMESHIFT_0, FRAMESHIFT_1,FRAMESHIFT_2,FRAMESHIFT_3,FRAMESHIFT_4,FRAMESHIFT_5,FRAMESHIFT_6
};
int8_t base_notes[7] = {11, 4, 9, 2, 7, 0, 5}; // Base note offsets for buttons in key of C (relative to C4 = MIDI 60), in hardware order B, E, A, D, G, C, F
const int8_t key_offsets[12] = {0, 7, 2, 9, 4, 11, 5, 10, 3, 8, 1, 6}; // Circle of fifths: semitone offset for each key’s root note relative to C: C, G, D, A, E, B, F, Bb, Eb, Ab, Db, Gb
const int8_t key_signatures[12] = {0, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6}; // Number of sharps or flats for each key: Sharps for C, G, D, A, E, B; flats for F, Bb, Eb, Ab, Db, Gb
const int8_t sharp_notes[6][6] = { // Notes affected by sharps in each key, in hardware order (B, E, A, D, G, C, F)
  {BTN_F},          // 1 sharp: F#
  {BTN_F, BTN_C},   // 2 sharps: F#, C#
  {BTN_F, BTN_C, BTN_G}, // 3 sharps: F#, C#, G#
  {BTN_F, BTN_C, BTN_G, BTN_D}, // 4 sharps: F#, C#, G#, D#
  {BTN_F, BTN_C, BTN_G, BTN_D, BTN_A}, // 5 sharps: F#, C#, G#, D#, A#
  {BTN_F, BTN_C, BTN_G, BTN_D, BTN_A, BTN_E} // 6 sharps: F#, C#, G#, D#, A#, E#
};
const int8_t flat_notes[6][6] = { // Notes affected by flats in each key, in hardware order (B, E, A, D, G, C, F)
  {BTN_B},          // 1 flat: Bb
  {BTN_B, BTN_E},   // 2 flats: Bb, Eb
  {BTN_B, BTN_E, BTN_A}, // 3 flats: Bb, Eb, Ab
  {BTN_B, BTN_E, BTN_A, BTN_D}, // 4 flats: Bb, Eb, Ab, Db
  {BTN_B, BTN_E, BTN_A, BTN_D, BTN_G}, // 5 flats: Bb, Eb, Ab, Db, Gb
  {BTN_B, BTN_E, BTN_A, BTN_D, BTN_G, BTN_C} // 6 flats: Bb, Eb, Ab, Db, Gb, Cb
};

float c_frequency = 130.81;                      // for C3

//>>SCALAR HARP MODE<<
// 0 follows the chord as before. 1-7 are fixed scales rooted on the key. 8 and 9
// pick a scale to suit whichever chord is currently held, 9 being the pentatonic
// version of 8.
uint8_t scalar_harp_selection = 0;

// Tonic pitch class for each key signature, in the order of the KeySig enum
int8_t scale_root_offsets[12] = {
  0, 7, 2, 9, 4, 11, // C, G, D, A, E, B
  5, 10, 3, 8, 1, 6  // F, Bb, Eb, Ab, Db, Gb
};

// Fixed scales for modes 1-7, semitones from the root
uint8_t scale_intervals[7][8] = {
  {0, 2, 4, 5, 7, 9, 11, 0}, // 1: Major (Ionian)
  {0, 2, 4, 7, 9, 0, 0, 0},  // 2: Major Pentatonic
  {0, 2, 3, 7, 10, 0, 0, 0}, // 3: Minor Pentatonic
  {0, 2, 4, 5, 7, 8, 9, 11}, // 4: Diminished 6th
  {0, 2, 3, 5, 7, 8, 10, 0}, // 5: Relative Natural Minor
  {0, 2, 3, 5, 7, 8, 11, 0}, // 6: Relative Harmonic Minor
  {0, 2, 3, 7, 10, 0, 0, 0}  // 7: Relative Minor Pentatonic
};
const uint8_t scale_lengths[7] = {7, 5, 5, 8, 7, 7, 5};

// Scales chosen per chord type for modes 8 and 9
uint8_t chord_scale_intervals[16][8] = {
  {0, 2, 4, 7, 9, 0, 0, 0},  //  0: Major Pentatonic, major chord
  {0, 2, 4, 6, 9, 0, 0, 0},  //  1: Lydian Pentatonic, major seventh
  {0, 3, 5, 7, 10, 0, 0, 0}, //  2: Minor Pentatonic, minor
  {0, 2, 4, 7, 10, 0, 0, 0}, //  3: Mixolydian Pentatonic, dominant seventh
  {0, 3, 5, 7, 9, 0, 0, 0},  //  4: Dorian Pentatonic, minor seventh
  {0, 1, 3, 4, 6, 7, 9, 10}, //  5: Octatonic, diminished
  {0, 2, 4, 6, 8, 10, 0, 0}, //  6: Whole Tone, augmented
  {0, 2, 4, 5, 7, 8, 9, 11}, //  7: Diminished 6th, major sixth
  {0, 2, 3, 5, 7, 8, 9, 11}, //  8: Diminished 6th Minor, minor sixth
  {0, 2, 3, 4, 6, 7, 9, 11}, //  9: Offset Diminished 6th, full diminished
  {0, 2, 4, 5, 7, 9, 11, 0}, // 10: Ionian, major
  {0, 2, 3, 5, 7, 9, 10, 0}, // 11: Dorian, minor seventh
  {0, 2, 4, 6, 7, 9, 11, 0}, // 12: Lydian, major seventh
  {0, 2, 4, 5, 7, 9, 10, 0}, // 13: Mixolydian, dominant seventh
  {0, 2, 3, 5, 7, 8, 10, 0}, // 14: Aeolian, minor
  {0, 1, 3, 5, 6, 8, 10, 0}  // 15: Locrian, the utonal tetrad (an m7b5 in twelve)
};
const uint8_t chord_scale_lengths[16] = {5, 5, 5, 5, 5, 8, 6, 8, 8, 8, 7, 7, 7, 7, 7, 7};
// The letter degree each scale entry names (0 = root, 1 = second, 2 = third, ...
// 6 = seventh), so a just chord's tone can take the place of the scale's own
// third or seventh rather than whichever entry happens to lie nearest it (see
// substitute_chord_tones). The degrees are the same in every division; only the
// steps are retuned. The octatonic, whole tone and diminished sixth rows name a
// degree twice or not at all, so they are left unlabelled (DEG_NONE) and fall
// back to the nearest entry.
#define DEG_NONE 255
const uint8_t chord_scale_degrees[16][8] = {
  {0, 1, 2, 4, 5}, {0, 1, 2, 3, 5}, {0, 2, 3, 4, 6}, {0, 1, 2, 4, 6}, {0, 2, 3, 4, 5},
  {DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE},
  {DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE},
  {DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE},
  {DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE},
  {DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE, DEG_NONE},
  {0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 4, 5, 6},
  {0, 1, 2, 3, 4, 5, 6}
};



uint8_t chord_octave_change=4;
uint8_t harp_octave_change=4;
uint8_t chord_frame_shift=0;
uint8_t transpose_semitones=0;                       // to use to transpose the instrument, number of semitones
uint8_t (*current_chord)[7] = &major;            // the array holding the current chord
uint8_t current_chord_notes[7];                  // the array for the note calculation within the chord, calculate 7 of them for the arpeggiator mode
uint8_t current_applied_chord_notes[7];          // the array for the note calculation within the chord
uint8_t current_harp_notes[12];                  // the array for the note calculation within the string

//>>SWITCHING LOGIC GLOBAL VARIABLES<<
int8_t current_line = -1;      // holds the current selected line of button, -1 if nothing is on
int8_t fundamental = 0;        // holds the value of the last selected line, hence the fundamental
uint8_t slash_value = 0;       // stores the "slash", ie when a different alternative note is selected
bool slash_chord = false;      // flag for when a slashed chord is currently activated
bool button_pushed = false;    // flag for when any button has been pushed during the main loop
bool trigger_chord = false;    // flag to trigger the enveloppe of the chord
bool sharp_active = false;     // flag for when the sharp is active
bool flat_button_modifier= false; //flag to set the modifier to flat instead of sharp
bool continuous_chord = false; // wether the chord is held continuously. Controlled by the "hold" button
bool rythm_mode = false;
bool barry_harris_mode = false;
IntervalTimer note_timer[4]; // timers for delayed chord enveloppe
// Whether each chord voice has been released since its last note on. A release
// has to reach a voice in whatever stage it is in, not only once it has
// settled into sustain, and remembering that it was sent lets the loop, which
// runs the check every pass, send it once.
volatile bool chord_voice_released[4] = {true, true, true, true};
bool inhibit_button=false;

//>>SWITCHING LOGIC PARAMETERS<<
uint8_t note_slash_level = 0;     // the level we are replacing in the chord when slashing (usually the fundamental)
bool retrigger_chord = true;      // wether or not to retrigger the enveloppe when the chord is switched within current line (including when selecting slash chord)
bool change_held_strings = false; // to control wether hold strings change with chord:
bool chromatic_harp_mode = false; // to switch the harp to chromatic mode
//>>SYSEX PARAMETERS<<
// SYSEX midi message are used to control up to 256 synthesis parameters.
const uint16_t parameter_size = 256;
const uint8_t preset_number = 12;
int16_t default_bank_sysex_parameters[preset_number][parameter_size] = {
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,43,0,50,37,38,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,64,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,10,1,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,4,25,8,3,29,18,65,488,3,159,25,70,5,18,4,26,6,1,77,0,587,32,0,390,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,4,7,0,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,1659,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,1436,118,100,50,32,107,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,30,0,1,1,87,33,62,12,80,67,0,0,0,0,0,0,0,0,0,0,0,6,3,8,8,12,42,1855,1,42,20,217,3,35,83,59,2658,1,185,0,282,14,0,247,11,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,23,0,0,755,195,82,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,1,13,0,100,23,8,200,0,0,50,0,50,18,32,50,0,0,10,66,353,44,1452,1,569,16,141,32,83,28,48,54,1,0,698,82,56,0,579,0,28,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,100,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,76,92,100,184,100,85,100,0,0,60,0,1,0,14,0,0,0,42,46,0,0,0,0,0,0,0,0,0,0,3,10,8,11,42,30,61,2137,1,106,22,140,4,35,83,24,1956,1,139,0,282,0,0,247,10,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,57,0,1,858,70,66,100,100,0,0,0,0,127,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,0,17,8,200,11,0,134,10,0,100,7,50,59,15,50,0,15,10,10,45,61,1489,1,652,16,84,32,21,15,33,19,1,0,490,0,109,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,40,78,21,0,162,16,4,100,1000,639,140,100,47,100,85,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,110,0,0,0,57,0,0,0,67,58,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,85,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,60,47,56,159,100,157,100,0,0,138,1,0,0,0,0,0,0,27,74,0,0,0,0,0,0,0,0,0,0,4,14,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,200,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,4,10,11,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,35,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,2101,70,100,6,22,55,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,61,100,130,100,83,100,0,0,175,0,0,0,0,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,12,9,11,1,1,70,2141,1,383,17,302,101,103,34,30,1,12,0,0,367,17,0,363,5,1,1,1,86,1,1,0,1,1,1,1,8,0,0,70,0,0,0,100,0,33,50,0,5000,70,100,0,0,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,5,9,9,200,13,6,99,6,0,50,27,50,50,50,50,0,0,345,1,1,80,800,32,2200,0,70,1,1,1,100,1,1,0,30,0,0,0,166,0,6,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,59,100,0,80,16,4,100,100,2366,70,100,40,23,57,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,4,4,16,16,12,0,1,6,8,6,0,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,220,0,0,0,0,0,0,86,55,63,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,84,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,100,47,56,159,100,157,100,0,0,253,1,0,0,0,0,0,0,0,59,0,0,0,0,0,0,0,0,0,0,4,15,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,12,76,1,22,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,158,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,4,10,0,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,22,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,532,70,100,6,82,96,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,149,69,76,100,85,79,184,100,0,0,266,0,0,0,0,0,0,0,65,70,0,0,0,0,0,0,0,0,0,0,0,5,0,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,0,424,1360,70,46,0,0,100,60,81,0,0,996,125,82,61,71,0,0,0,0,108,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,1,20,12,100,23,0,200,15,8,100,0,50,45,17,0,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,857,70,42,100,100,158,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,55,61,100,184,100,85,100,0,0,310,0,0,0,86,0,56,87,37,60,0,0,0,0,0,0,0,0,0,0,4,15,12,36,42,30,59,1410,1,30,33,140,4,35,83,24,615,1,94,0,282,15,0,247,0,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,40,0,1,858,70,46,100,82,0,0,0,0,124,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,1,16,8,200,18,0,134,21,0,200,2,50,59,15,50,18,15,10,66,43,35,711,1,271,16,84,32,83,733,31,54,1,0,698,0,220,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,34,75,0,0,162,16,4,100,1000,1889,116,49,100,42,129,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,340,1,0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,4,25,3,3,29,18,65,488,3,159,25,70,5,18,4,26,40,1,77,0,587,32,0,715,11,76,1,1,100,1,1,68,17,14,22,20,17,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,1,3,1,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,706,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,678,118,100,50,32,168,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
}; 
int16_t current_sysex_parameters[parameter_size] = {0,0,50,50,512,512,512,1,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,0,0,0,0,0,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,0,0,0,0,0,0,0,0,0,0,4400,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,38,0,0,80,16,4,94,753,474,70,5,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char *bank_name[preset_number] = {"a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt", "g.txt", "h.txt", "i.txt", "j.txt", "k.txt", "l.txt"};
int8_t current_bank_number = 0;
float bank_led_hue = 0;
// Reserved SYSEX adresses
// =0 is for the control command
// 1 is the bank ID
// >2 and <10 are protected
int8_t harp_volume_sysex = 2;
int8_t chord_volume_sysex = 3;
int8_t chord_pot_alternate_storage = 4;
int8_t harp_pot_alternate_storage = 5;
int8_t mod_pot_alternate_storage = 6;

// >10 and <21 are limited access, for example the potentiometer settings (we don't want a pot to control another pot sysex adress or range)
int8_t chord_pot_alternate_control = 10;
int8_t chord_pot_alternate_range = 11;
int8_t harp_pot_alternate_control = 12;
int8_t harp_pot_alternate_range = 13;
int8_t mod_pot_main_control = 14;
int8_t mod_pot_main_range = 15;
int8_t mod_pot_alternate_control = 16;
int8_t mod_pot_alternate_range = 17;
// 21-39 are global parameters (switching logic, global reverb etc.)
// 40-119 are harp parameters
// 120-219 are chord parameters
// 220-235 are rythm patterns
bool sysex_controler_connected=false; //bool to remember if there is a controller that is connected to avoid saving any change

//>>AUDIO OBJECT ARRAYS<<
// for the strings
AudioSynthWaveformModulated *string_waveform_array[12] = {&waveform_string_1, &waveform_string_2, &waveform_string_3, &waveform_string_4, &waveform_string_5, &waveform_string_6, &waveform_string_7, &waveform_string_8, &waveform_string_9, &waveform_string_10, &waveform_string_11, &waveform_string_12};
AudioEffectEnvelope *string_enveloppe_array[12] = {&envelope_string_1, &envelope_string_2, &envelope_string_3, &envelope_string_4, &envelope_string_5, &envelope_string_6, &envelope_string_7, &envelope_string_8, &envelope_string_9, &envelope_string_10, &envelope_string_11, &envelope_string_12};
AudioEffectEnvelope *string_enveloppe_filter_array[12] = {&envelope_filter_1, &envelope_filter_2, &envelope_filter_3, &envelope_filter_4, &envelope_filter_5, &envelope_filter_6, &envelope_filter_7, &envelope_filter_8, &envelope_filter_9, &envelope_filter_10, &envelope_filter_11, &envelope_filter_12};
AudioMixer4 *string_mixer_array[3] = {&string_mix_1, &string_mix_2, &string_mix_3};
AudioFilterStateVariable *string_filter_array[12] = {&filter_string_1, &filter_string_2, &filter_string_3, &filter_string_4, &filter_string_5, &filter_string_6, &filter_string_7, &filter_string_8, &filter_string_9, &filter_string_10, &filter_string_11, &filter_string_12};
AudioSynthWaveform *string_transient_waveform_array[12] = {&waveform_transient_1, &waveform_transient_2, &waveform_transient_3, &waveform_transient_4, &waveform_transient_5, &waveform_transient_6, &waveform_transient_7, &waveform_transient_8, &waveform_transient_9, &waveform_transient_10, &waveform_transient_11, &waveform_transient_12};
AudioEffectEnvelope *string_transient_envelope_array[12] = {&envelope_transient_1, &envelope_transient_2, &envelope_transient_3, &envelope_transient_4, &envelope_transient_5, &envelope_transient_6, &envelope_transient_7, &envelope_transient_8, &envelope_transient_9, &envelope_transient_10, &envelope_transient_11, &envelope_transient_12};
AudioMixer4 *transient_mixer_array[3] = {&transient_mix_1, &transient_mix_2, &transient_mix_3};
// for the chord
AudioEffectEnvelope *chord_vibrato_envelope_array[4] = {&voice1_vibrato_envelope, &voice2_vibrato_envelope, &voice3_vibrato_envelope, &voice4_vibrato_envelope};
AudioEffectEnvelope *chord_vibrato_dc_envelope_array[4] = {&voice1_vibrato_dc_envelope, &voice2_vibrato_dc_envelope, &voice3_vibrato_dc_envelope, &voice4_vibrato_dc_envelope};
AudioMixer4 *chord_vibrato_mixer_array[4] = {&voice1_vibrato_mixer, &voice2_vibrato_mixer, &voice3_vibrato_mixer, &voice4_vibrato_mixer};
AudioSynthWaveformModulated *chord_osc_1_array[4] = {&voice1_osc1, &voice2_osc1, &voice3_osc1, &voice4_osc1};
AudioSynthWaveformModulated *chord_osc_2_array[4] = {&voice1_osc2, &voice2_osc2, &voice3_osc2, &voice4_osc2};
AudioSynthWaveformModulated *chord_osc_3_array[4] = {&voice1_osc3, &voice2_osc3, &voice3_osc3, &voice4_osc3};
AudioSynthWaveformDc *chord_freq_dc_array[4]= {&voice1_frequency_dc, &voice2_frequency_dc, &voice3_frequency_dc, &voice4_frequency_dc};
AudioSynthNoiseWhite *chord_noise_array[4] = {&voice1_noise, &voice2_noise, &voice3_noise, &voice4_noise};
AudioMixer4 *chord_voice_mixer_array[4] = {&voice1_mixer, &voice2_mixer, &voice3_mixer, &voice4_mixer};
AudioFilterStateVariable *chord_voice_filter_array[4] = {&voice1_filter, &voice2_filter, &voice3_filter, &voice4_filter};
AudioEffectEnvelope *chord_envelope_filter_array[4] = {&voice1_envelope_filter, &voice2_envelope_filter, &voice3_envelope_filter, &voice4_envelope_filter};
AudioEffectMultiply *chord_tremolo_mult_array[4] = {&voice1_tremolo_mult, &voice2_tremolo_mult, &voice3_tremolo_mult, &voice4_tremolo_mult};
AudioEffectEnvelope *chord_envelope_array[4] = {&voice1_envelope, &voice2_envelope, &voice3_envelope, &voice4_envelope};

//>>SYNTHESIS VARIABLE<<
// waveshaper shape
float wave_shape[257] = {};
float ws_sin_param = 1;
// waveform array 
int8_t waveform_array[12] = {
    0, //WAVEFORM_SINE
    1, //WAVEFORM_SAWTOOTH
    2, //WAVEFORM_SQUARE
    3, //WAVEFORM_TRIANGLE
    12, //WAVEFORM_BANDLIMIT_PULSE
    5, //WAVEFORM_PULSE
    6, //WAVEFORM_SAWTOOTH_REVERSE
    7, //WAVEFORM_SAMPLE_HOLD 
    8, //WAVEFORM_TRIANGLE_VARIABLE
    9, //WAVEFORM_BANDLIMIT_SAWTOOTH
    10,//WAVEFORM_BANDLIMIT_SAWTOOTH_REVERSE
    11, //WAVEFORM_BANDLIMIT_SQUARE
}; 
// shuffling arrays and index for the harp
int8_t harp_shuffling_array[7][12] = {
    //each number indicates the note for the string 0-6 are taken within the chord pattern. 
    //the /10 number indicates the octave
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32},
    {4, 1, 0, 2, 14, 11, 10, 12, 24, 21, 20, 22}, //add the seconds
    {5, 2, 0, 1, 15, 12, 10, 11, 25, 22, 20, 21}, //add the fourth
    {6, 2, 0, 1, 16, 12, 10, 11, 26, 22, 20, 21}, //add the sixth
    {0, 1, 2, 3, 10, 11, 12, 13, 20, 21, 22, 23}, //replaced octave by barry_harris shuffling array 
    {0, 4, 1, 5, 2, 6, 10, 14, 11, 15, 12, 16}, //chromatic
    {0, 10, 20, 1, 11, 21, 2, 12, 22, 3, 13, 23}}; //special array for keymaster/barry_harris combo
int8_t harp_shuffling_selection = 0;
int8_t transient_note_level=0; //level of the note of the transient in the scale;
int8_t chord_shuffling_array[6][7] = {
    //each number indicates the note for the voice 0-6 are taken within the chord pattern. In normal mode, only 0-3 is used, and 4-6 is available in rythm mode 
    //the /10 number indicates the octave
    {0, 1, 2, 3, 4, 5, 6}, //normal 
    {10, 11, 12, 13, 14, 15, 16},//one octave up with chromatics
    {10, 11, 12, 13, 0, 2, 3},//one octave up with low chord notes
    {10, 11, 12, 13, 2, 5, 6},//one octave up with low fifth and low chromatics
    {10, 11, 12, 13, 2, 15, 16},//one octave up with low fifth and high chromatics
    {20, 21, 22, 23, 24, 25, 26}};//two octave up
int8_t chord_shuffling_selection = 0;
uint8_t chord_inversion = 0; // 0 = root position, 1-3 = successive inversions
uint8_t chord_spacing = 0;   // 0 = close, 1 = drop 2, 2 = drop 3, 3 = drop 2+4, 4 = spread
int16_t chord_note_floor = 12;    // one and eight octaves in the live division,  // below this the chord voices turn to mud
int16_t chord_note_ceiling = 96;  // set with it in apply_temperament
// retrigger release for chord delayed note
int chord_retrigger_release=0;
int glide_length=0;
// strings filter parameters
float string_filter_keytrack = 0;
int string_filter_base_freq = 0;
// lfo for chord parameters
float chord_vibrato_base_freq = 0;
float chord_vibrato_keytrack = 0;
float chord_tremolo_base_freq = 0;
float chord_tremolo_keytrack = 0;
float chord_filter_base_freq = 0;
float chord_filter_keytrack = 0;
// frequency mutlipliers for chord
float osc_1_freq_multiplier = 1;
float osc_2_freq_multiplier = 1;
float osc_3_freq_multiplier = 1;
// string delay_parameter
u_int32_t inter_string_delay = 30000;
u_int32_t random_delay = 10000;
// pan for audio output 
float pan=1;
float reverb_dry_proportion=0.6; //to avoid drop in volume in full reverb, keep some part of the dry signal in

//>>AUTO RYTHM<<
u_int8_t rythm_pattern[16] = {};
float rythm_bpm = 80;
u_int8_t rythm_current_step = 0;
u_int16_t note_pushed_duration = 30;
float shuffle = 1;
u_int32_t long_timer_period = shuffle * (60 * 1000 * 1000) / (2 * rythm_bpm);
u_int32_t short_timer_period = 2 * (60 * 1000 * 1000) / (2 * rythm_bpm) - long_timer_period;
bool current_long_period = true;
bool rythm_timer_running = false;
IntervalTimer rythm_timer;       // that gives the general rythm
IntervalTimer note_off_timer[4]; // timers for delayed chord enveloppe
IntervalTimer led_timer;
IntervalTimer color_led_blink_timer;
elapsedMillis note_off_timing[4];
elapsedMicros last_midi_clock_in;
int midi_clock_current_step=0;

uint8_t rythm_limit_change_to_every = 2; // when we allow the chord change
elapsedMillis since_last_button_push;
elapsedMillis last_key_change;

uint8_t rythm_freeze_current_chord_notes[7]; // this array is needed because we need to handle the situation when a
uint8_t rythm_loop_length = 16;
u_int8_t current_selected_voice=0; //increment at each voice steal to rotate amongst voices;

//-->>MIDI PARAMETERS
uint8_t chord_port=0;
uint8_t chord_channel=1;
uint8_t chord_attack_velocity=127;
uint8_t chord_release_velocity=20;
uint8_t chord_started_notes[4]={0,0,0,0};                   
uint8_t harp_port=1;
uint8_t harp_channel=1;
uint8_t harp_attack_velocity=127; 
uint8_t harp_release_velocity=20;
uint8_t harp_started_notes[12]={0,0,0,0,0,0,0,0,0,0,0,0};    
uint8_t midi_base_note=48; // for C3
uint8_t midi_base_note_transposed=midi_base_note; //to handle note transposition
uint midi_buffer_delay=300; //in microseconds, helps compatibility with some hardware devices

//-->>MIDI OUTPUT QUEUE
// usbMIDI is not reentrant; calling it from both loop() and a PIT ISR corrupts its
// transmit state. Rule: only drain_midi_queue(), called at the end of loop(), touches
// usbMIDI. Everything else enqueues via queue_midi(), safe from ISR context.
#define MIDI_QUEUE_SIZE 256 // power of two, max 256 for uint8_t indices
#define MIDI_DRAIN_MAX_PER_LOOP 16 // caps how long a drain can hold up loop()
struct midi_event_t {
  uint8_t note;
  uint8_t velocity;
  uint8_t channel;
  uint8_t cable;
  bool note_on;
};
volatile midi_event_t midi_queue[MIDI_QUEUE_SIZE];
volatile uint8_t midi_queue_head = 0; // written by producers
volatile uint8_t midi_queue_tail = 0; // written by loop() only
volatile uint32_t midi_queue_dropped = 0; // diagnostic: events lost to a full queue

// Safe to call from any context, including an ISR. Drops the event if the queue is
// full rather than blocking -- blocking is what caused the original fault.
void queue_midi(bool note_on, uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable) {
  uint32_t primask;
  __asm__ volatile("mrs %0, primask" : "=r"(primask));
  __disable_irq();
  uint8_t next = (midi_queue_head + 1) & (MIDI_QUEUE_SIZE - 1);
  if (next != midi_queue_tail) {
    midi_queue[midi_queue_head].note = note;
    midi_queue[midi_queue_head].velocity = velocity;
    midi_queue[midi_queue_head].channel = channel;
    midi_queue[midi_queue_head].cable = cable;
    midi_queue[midi_queue_head].note_on = note_on;
    midi_queue_head = next;
  } else {
    midi_queue_dropped++;
  }
  if (!primask) __enable_irq();
}

// Called from loop() only. The single point at which this firmware talks to usbMIDI.
void drain_midi_queue() {
  bool sent = false;
  uint8_t budget = MIDI_DRAIN_MAX_PER_LOOP;

  while (midi_queue_tail != midi_queue_head && budget > 0) {
    budget--;
    midi_event_t e;
    e.note = midi_queue[midi_queue_tail].note;
    e.velocity = midi_queue[midi_queue_tail].velocity;
    e.channel = midi_queue[midi_queue_tail].channel;
    e.cable = midi_queue[midi_queue_tail].cable;
    e.note_on = midi_queue[midi_queue_tail].note_on;
    midi_queue_tail = (midi_queue_tail + 1) & (MIDI_QUEUE_SIZE - 1);
    if (sent) delayMicroseconds(midi_buffer_delay); // pacing for slower hardware synths
    if (e.note_on) {
      usbMIDI.sendNoteOn(e.note, e.velocity, e.channel, e.cable);
    } else {
      usbMIDI.sendNoteOff(e.note, e.velocity, e.channel, e.cable);
    }
    sent = true;
  }
  if (sent) usbMIDI.send_now();
}

//-->>FUNCTION THAT NEED ANNOUNCING
void save_config(int bank_number, bool default_save);
void load_config(int bank_number);
void recalculate_timer();
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp);
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp);
void set_chord_voice_frequency(uint8_t i, uint16_t current_note);
void apply_temperament(uint8_t t);

/* ---- temperament ------------------------------------------------------------
 *
 * Equal temperament divides the octave into twelve identical steps: every key
 * sounds the same and no interval but the octave is quite in tune. Temperaments
 * 1 to 9 move each of the twelve pitch classes by a few cents, the way a
 * keyboard was tuned before equal temperament took over. Temperaments 10 and 11
 * keep none of the twelve notes at all: they divide the octave into 19 and 31
 * equal steps, where the meantone thirds live and the septimal intervals twelve
 * notes cannot spell become playable.
 *
 * The offsets are in temperament_profiles.h, generated by
 * generator/temperaments.py from how each temperament is built (its tempered
 * fifths, its ratios, or its division); the arithmetic is in temperament.h,
 * where generator/temperament_test.cpp can check it on a computer. A has no
 * offset in the twelve-note ones, so A sounds at the master tuning pitch there.
 * The tuning is fixed to the keyboard, so which keys are sweet does not follow
 * the key signature.
 *
 * In the twelve-note temperaments only the sound changes: note numbers, and so
 * MIDI out, are untouched. In 19 and 31 the note numbers themselves are steps
 * of the division, so MIDI out goes through midi_out_note() below, which
 * rescales to the nearest semitone: the recording lands in the right register
 * and is audibly an approximation, rather than silently absurd.
 */
uint8_t temperament_selection = 0;

// Frequency ratio of a note in the selected temperament and division.
static inline double temper_ratio(double note) {
  return temperament_ratio(temperament_selection, EDO, note);
}

// a minor third in the live division, for the relative-minor harp modes
static inline int8_t minor_third_steps() {
  return edo_index == 0 ? 3 : edo_index == 1 ? 5 : edo_index == 2 ? 8 : 6;
}

// MIDI note numbers are integers, so a 31st of an octave has nowhere to go.
static inline uint8_t midi_out_note(int16_t note) {
  int32_t v = (edo_index == 0) ? note : (int32_t)lroundf(note * 12.0f / (float)EDO);
  if (v < 0) v = 0;
  if (v > 127) v = 127;
  return (uint8_t)v;
}

// the harmonic context the note arrays were last built with, so they can be
// rebuilt when the division changes even though no button is down
bool chord_context_sharp = false;
bool chord_context_slashed = false;
void update_chord_notes();
void update_harp_notes();
void retune_active_voices();
void refresh_chord_filter();
// the note frequency each chord voice is currently sounding, kept so the filter
// corner can be recomputed for a voice without touching anything else about it
float chord_voice_note_freq[4] = {0, 0, 0, 0};
uint16_t chord_voice_current_note[4] = {0, 0, 0, 0};   // last note each chord voice was tuned to, so a tuning change can re-apply it
uint16_t harp_voice_current_note[12] = {0};           // same for the harp strings
void calculate_ws_array();
void rythm_tick_function();
void set_chord_voice_frequency(uint8_t i, uint16_t current_note);
void set_harp_voice_frequency(uint8_t i, uint16_t current_note);

// Re-applies the stored note of every sounding voice, so a reference-pitch
// change lands on held notes too. Same-division only: apply_temperament() does
// its own remap, since note numbers change meaning across a division change.
void retune_active_voices() {
  for (uint8_t i = 0; i < 4; i++) {
    noInterrupts();
    if (chord_envelope_array[i]->isActive()) {
      set_chord_voice_frequency(i, chord_voice_current_note[i]);
    }
    interrupts();
  }
  for (uint8_t i = 0; i < 12; i++) {
    noInterrupts();
    if (string_enveloppe_array[i]->isActive()) {
      set_harp_voice_frequency(i, harp_voice_current_note[i]);
    }
    interrupts();
  }
}

//-->>LED HSV CALCULATION
// function to calculate led RGB value, thank you SO
void set_led_color(float h, float s, float v) {
  double hh, p, q, t, ff;
  long i;
  double r, g, b;
  if (s <= 0.0) {
    r = v;
    g = v;
    b = v;
    analogWrite(R_LED_PIN, 0);
    analogWrite(G_LED_PIN, 0);
    analogWrite(B_LED_PIN, 0);
    return;
  }
  hh = h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));
  switch (i) {
  case 0:
    r = v;
    g = t;
    b = p;
    break;
  case 1:
    r = q;
    g = v;
    b = p;
    break;
  case 2:
    r = p;
    g = v;
    b = t;
    break;
  case 3:
    r = p;
    g = q;
    b = v;
    break;
  case 4:
    r = t;
    g = p;
    b = v;
    break;
  case 5:
  default:
    r = v;
    g = p;
    b = q;
    break;
  }
  analogWrite(R_LED_PIN, r * 200);
  analogWrite(G_LED_PIN, g * 115);
  analogWrite(B_LED_PIN, b * 70);
  return;
}

//-->>UTILITIES FOR SYSEX HANDLING
void control_command(uint8_t command, uint8_t parameter) {
  switch (command) {
  case 0: // SIGNAL TO SEND BACK ALL DATA
    Serial.println("Reporting all data");
    uint8_t midi_data_array[parameter_size * 2];
    for (int i = 0; i < parameter_size; i++) {
      int16_t parameter_value = constrain(current_sysex_parameters[i], 0, 16383); // a sysex byte only carries 7 bits
      midi_data_array[2 * i] = parameter_value % 128;
      midi_data_array[2 * i + 1] = parameter_value / 128;
    }
    usbMIDI.sendSysEx(parameter_size * 2, midi_data_array,0);
    break;
  case 1: // SIGNAL TO WIPE MEMORY
    Serial.println("Wiping memory");
    digitalWrite(_MUTE_PIN, LOW); // muting the DAC
    myfs.quickFormat();
    current_bank_number = 0;
    load_config(current_bank_number);
    digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
    break;
  case 2: // saving bank
    Serial.print("Saving to bank: ");
    Serial.println(parameter);
    save_config(parameter, false);
    break;
  case 3: // setting bank to default
    Serial.print("Saving to bank: ");
    Serial.println(parameter);
    current_bank_number = parameter;
    save_config(parameter, true);
    break;
  case 4: // loading a bank, so a remote can read every preset in turn
    if (parameter < preset_number) {
      Serial.print("Loading bank: ");
      Serial.println(parameter);
      current_bank_number = parameter;
      load_config(current_bank_number);
      set_led_color(bank_led_hue, 1.0, 1 - led_attenuation);
    }
    break;

  default:
    break;
  }
}
// the autogenerated code (see ./generator for the script)
#include <sysex_handler.h>
#include <parameter_introduction.h>
void processMIDI(void) {
  byte type;
  type = usbMIDI.getType();
  if (type == usbMIDI.SystemExclusive && usbMIDI.getSysExArrayLength() == 6) {
    const byte *data = usbMIDI.getSysExArray();
    // Universal system exclusive messages (identity request, GM on/off, master volume...) start
    // with 0x7E or 0x7F and are six bytes long too. Hosts and DAWs send them unprompted, so they
    // must not be read as a parameter write.
    if (data[1] != 0x7E && data[1] != 0x7F) {
      int adress = data[1] + 128 * data[2];
      if (adress < parameter_size) { // an adress can reach 16383, the parameter array holds 256
        sysex_controler_connected=true; //the message was meant for us, so a controller is connected
        if (adress == 0) { // it is a control command
          control_command(data[3], data[4]);
        } else {
          Serial.print("Received instruction on adress:");
          Serial.print(adress);
          int value = data[3] + 128 * data[4];
          Serial.print(" with value:");
          Serial.println(value);
          current_sysex_parameters[adress] = value;
          apply_audio_parameter(adress, value);
        }
      }
    }
  }
  if(type==usbMIDI.Start && rythm_mode){
    rythm_current_step=0;
    midi_clock_current_step=0;
    rythm_tick_function();
    Serial.println("Start received");
    rythm_timer.end();
  }
  if(type==usbMIDI.Stop && rythm_mode){
    rythm_timer.begin(rythm_tick_function, short_timer_period);
  }


  if(type==usbMIDI.Clock && rythm_mode){
    //here we want half of the cycle to be synced with the midi clock, and half with the calculated internal clock, so we can still have shuffle
    //recalculate the BPM
    rythm_bpm=(rythm_bpm*10+(1000*1000*60/last_midi_clock_in)/24)/11.0;
    last_midi_clock_in=0;
    midi_clock_current_step+=1;
    recalculate_timer();   
    //once every two beat, we sync
    if(midi_clock_current_step==24){
      rythm_timer.begin(rythm_tick_function, short_timer_period);
      rythm_tick_function();
      midi_clock_current_step=0;
    }
    //We disable the timer to avoid having it trigger the tick too early when we arrive at the sync beat 
    if(midi_clock_current_step>18){
      rythm_timer.end();
    }

   
  
   
  }
}

//-->>TIMER FUNCTIONS
// function to handle the delayed chord activation
void play_single_note(int i, IntervalTimer *timer) {
  timer->end();
  set_chord_voice_frequency(i, current_applied_chord_notes[i]);
  chord_vibrato_envelope_array[i]->noteOn();
  chord_vibrato_dc_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  chord_voice_released[i] = false;
  // ISR context: queue only, never touch usbMIDI here.
  if(chord_started_notes[i]!=0){
    queue_midi(false, chord_started_notes[i],chord_release_velocity,chord_channel, chord_port);
    chord_started_notes[i]=0;}
  queue_midi(true, midi_base_note_transposed+ midi_out_note(current_applied_chord_notes[i]),chord_attack_velocity,chord_channel, chord_port);
  chord_started_notes[i]=midi_base_note_transposed+ midi_out_note(current_applied_chord_notes[i]);
}

void play_note_selected_duration(int i,int current_note){
  chord_vibrato_envelope_array[i]->noteOn();
  chord_vibrato_dc_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  chord_voice_released[i] = false;
  note_off_timing[i]=0;
  // ISR context: queue only, never touch usbMIDI here.
  if(chord_started_notes[i]!=0){
    queue_midi(false, chord_started_notes[i],chord_release_velocity,chord_channel, chord_port);
    chord_started_notes[i]=0;}
  queue_midi(true, midi_base_note_transposed+midi_out_note(current_note),chord_attack_velocity,chord_channel, chord_port);
  chord_started_notes[i]=midi_base_note_transposed+midi_out_note(current_note);
}

void turn_off_led(IntervalTimer *timer) {
  timer->end();
  analogWrite(RYTHM_LED_PIN, 0);
}

//-->>AUDIO HELPER FUNCTIONS
// calculationg the ws array
void calculate_ws_array() {
  for (int i = 0; i < 257; i++) {
    float current_x = (i / 256.0 - 0.5) * 2.0 * PI;
    wave_shape[i] = sin(current_x);
    for (int j = 0; j < ws_sin_param; j++) {
      wave_shape[i] = sin(wave_shape[i] * PI);
    }
  }
}
// setting the pad_frequency
/* Recompute only the filter corner for voices that are sounding.
 *
 * Deliberately NOT set_chord_voice_frequency: that also retunes three
 * oscillators, restarts the glide ramp, and sends a MIDI note off and on when
 * the voice's note has moved. Called on every step of a knob sweep, that would
 * restart the portamento continuously and spray note messages down the wire.
 * This touches the filter and nothing else.
 */
void refresh_chord_filter() {
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++) {
    if (chord_envelope_array[i]->isActive()) {
      chord_voice_filter_array[i]->frequency(chord_voice_note_freq[i] * chord_filter_keytrack + chord_filter_base_freq);
    }
  }
  AudioInterrupts();
}

void set_chord_voice_frequency(uint8_t i, uint16_t current_note) {
  chord_voice_current_note[i] = current_note;
  float note_freq = pow(2,chord_octave_change)*c_frequency/8 * temper_ratio(current_note+transpose_steps); //down one octave to let more possibilities with the shuffling array
  if(glide_length>0){
        //ok so first we need to set the "middle note". Keep in mind that the signal will be +/-1 and will go +/- 2 octaves (frequencyModulation(2), hence the /24.0 below)
    //let's do a trick to select a middle note: get the level (relative to the C) and the note and do a modulo 
    int note_level=EDO*chord_octave_change-3*EDO+current_note+transpose_steps;
    int base_octave =chord_octave_change-2+(chord_shuffling_array[chord_shuffling_selection][i])/10;
    int middle_note=base_octave*EDO+transpose_steps; 
    int note_delta=note_level-middle_note;
    float middle_freq=c_frequency*temper_ratio(middle_note);

    AudioNoInterrupts();
    chord_voice_note_freq[i] = note_freq;
    chords_vibrato_lfo.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * middle_freq);
    chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * middle_freq);
    chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * middle_freq);
    // The oscillators sit on the middle note and the DC offset reaches the
    // voice's own note through frequencyModulation(2), two octaves per unit.
    // note_delta/24 is that distance only in equal temperament with twelve
    // steps; anywhere else the offset has to come from the real ratio, or every
    // glide voice lands on an equal-tempered pitch (and in 19 or 31 steps, on
    // an unrelated one, since a step is read as a semitone).
    float glide_offset = (temperament_selection == 0)
      ? note_delta/24.0
      : log2f(note_freq / middle_freq) / 2.0f;
    chord_freq_dc_array[i]->amplitude(glide_offset,glide_length);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }else{
    float note_freq = pow(2,chord_octave_change)*c_frequency/8 * temper_ratio(current_note+transpose_steps); //down one octave to let more possibilities with the shuffling array
    AudioNoInterrupts();
    chord_voice_note_freq[i] = note_freq;
    chords_vibrato_lfo.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * note_freq);
    chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * note_freq);
    chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * note_freq);
    chord_freq_dc_array[i]->amplitude(0,0);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }

  // Reached from BOTH the main loop (update_chord_notes) and PIT ISR context
  // (play_single_note, rythm_tick_function), so it must queue rather than send.
  if(chord_started_notes[i]!=0 && chord_started_notes[i]!=midi_base_note_transposed+midi_out_note(current_note)){
    //we need to change the note without triggering the change, ie a pitch bend
    queue_midi(false, chord_started_notes[i],chord_release_velocity,chord_channel, chord_port);
    chord_started_notes[i]=0;
    queue_midi(true, midi_base_note_transposed+midi_out_note(current_note),chord_attack_velocity,chord_channel, chord_port);
    chord_started_notes[i]=midi_base_note_transposed+ midi_out_note(current_note);
  }
}
// setting the harp
void set_harp_voice_frequency(uint8_t i, uint16_t current_note) {
  harp_voice_current_note[i] = current_note;
  float note_freq =  pow(2,harp_octave_change)*c_frequency/4 * temper_ratio(current_note+transpose_steps);
  float transient_freq =  64.0*c_frequency/4 *temper_ratio((current_note+transpose_steps)%EDO+transient_note_level);
  // the transient is an interval above the string's own note, so it moves with it
  AudioNoInterrupts();
  string_waveform_array[i]->frequency(note_freq);
  string_transient_waveform_array[i]->frequency(transient_freq);
  string_filter_array[i]->frequency(string_filter_base_freq + note_freq * string_filter_keytrack);
  // string_vibrato_1.offset(0);
  AudioInterrupts();
}
/* Re-apply every sounding voice's own note after c_frequency changes, so master
 * tuning is audible on a held chord while the control moves. Each voice is
 * retuned to the note it last played, so the note number does not move and no
 * MIDI is sent. Interrupts are held per voice because the rhythm and delayed
 * chord timers can retune a voice from ISR context; without the lock, a timer
 * could move the voice to a new note between reading the stored note and
 * re-applying it, and this would put the old note back. */

// Selecting a temperament retunes what is sounding, so a held or sustained
// chord moves to the new tuning rather than waiting for the next note. The note
// numbers do not change, only their pitch, so no MIDI is sent.
void apply_temperament(uint8_t t) {
  if (t >= temperament_count) t = 0;
  uint8_t previous_edo = EDO;
  temperament_selection = t;
  edo_index = temperament_profiles[t].edo_index;
  EDO = edo_steps[edo_index];
  sharp_step = edo_sharp[edo_index];
  modifier_step = edo_modifier[edo_index];
  transpose_steps = (transpose_semitones * EDO + 6) / 12; // one semitone of transposition is EDO/12 steps here
  chord_note_floor = EDO;      // the spacing rails are octaves, so they move with the division
  chord_note_ceiling = 8 * EDO;
  memcpy(base_notes, edo_base_notes[edo_index], sizeof(base_notes));
  memcpy(scale_root_offsets, edo_scale_root_offsets[edo_index], sizeof(scale_root_offsets));
  memcpy(scale_intervals, edo_scale_intervals[edo_index], sizeof(scale_intervals));
  memcpy(chord_scale_intervals, edo_chord_scale_intervals[edo_index], sizeof(chord_scale_intervals));
  memcpy(major, edo_major[edo_index], 7);
  memcpy(minor, edo_minor[edo_index], 7);
  memcpy(maj_sixth, edo_maj_sixth[edo_index], 7);
  memcpy(min_sixth, edo_min_sixth[edo_index], 7);
  memcpy(seventh, edo_seventh[edo_index], 7);
  memcpy(maj_seventh, edo_maj_seventh[edo_index], 7);
  memcpy(min_seventh, edo_min_seventh[edo_index], 7);
  memcpy(aug, edo_aug[edo_index], 7);
  memcpy(dim, edo_dim[edo_index], 7);
  memcpy(full_dim, edo_full_dim[edo_index], 7);
  memcpy(half_dim, edo_half_dim[edo_index], 7);
  memcpy(sus_fourth, edo_sus_fourth[edo_index], 7);
  memcpy(sus_second, edo_sus_second[edo_index], 7);
  memcpy(seventh_sus, edo_seventh_sus[edo_index], 7);
  memcpy(major_ninth, edo_major_ninth[edo_index], 7);
  memcpy(minor_ninth, edo_minor_ninth[edo_index], 7);
  memcpy(added_ninth, edo_added_ninth[edo_index], 7);
  memcpy(six_nine, edo_six_nine[edo_index], 7);
  memcpy(neutral, edo_neutral[edo_index], 7);
  memcpy(harmonic_7th, edo_harmonic_7th[edo_index], 7);
  memcpy(subminor, edo_subminor[edo_index], 7);
  memcpy(supermajor, edo_supermajor[edo_index], 7);
  memcpy(subminor_seventh, edo_subminor_seventh[edo_index], 7);
  memcpy(utonal_tetrad, edo_utonal_tetrad[edo_index], 7);
  memcpy(harmonic_ninth, edo_harmonic_ninth[edo_index], 7);
  memcpy(neutral_seventh, edo_neutral_seventh[edo_index], 7);
  memcpy(otonal_hexad, edo_otonal_hexad[edo_index], 7);
  memcpy(just_augmented, edo_just_augmented[edo_index], 7);
  memcpy(supermajor_seventh, edo_supermajor_seventh[edo_index], 7);

  /* Every note number just changed meaning: in 31 a fifth is 18 rather than 7.
   * So the note arrays are recomputed unconditionally — update_chord_notes and
   * update_harp_notes are both gated on button_pushed, which a held chord does
   * not set — and then pushed into whatever is still sounding, the same way a
   * master tuning change is.
   *
   * set_chord_voice_frequency also sends a MIDI note off and on when the note
   * number moves, and going from 12 to 31 moves all of them, so a held chord
   * retriggers over MIDI here. That is right for a pitch change of this size and
   * matches what master tuning already does, but it is why this belongs on a
   * deliberate setting change and nowhere near a knob sweep.
   */
  //
  // The recalculation used to run only while a chord button was down. A chord
  // still sounding after release, or held by the hold button, kept its old note
  // numbers, which were then read in the new division: a fifth of 7 is nearly a
  // quarter-octave lower in 31, and 18 from 31 read in 12 lands an octave and a
  // half up. Recalculate from the context the notes were last built with
  // instead of the live buttons, which a released chord no longer holds.
  //
  // Each sounding voice is then moved to the NEW number for the note it was
  // playing, found by position in the old arrays, rather than to
  // current_chord_notes[voice]: in rhythm mode a voice plays whichever chord
  // degree the pattern gave it, not the one at its own index.
  bool division_changed = (EDO != previous_edo);
  uint8_t old_chord_notes[7], old_harp_notes[12];
  memcpy(old_chord_notes, current_chord_notes, sizeof(old_chord_notes));
  memcpy(old_harp_notes, current_harp_notes, sizeof(old_harp_notes));
  for (int i = 0; i < 7; i++) current_chord_notes[i] = calculate_note_chord(i, chord_context_slashed, chord_context_sharp);
  for (int i = 0; i < 12; i++) current_harp_notes[i] = calculate_note_harp(i, chord_context_slashed, chord_context_sharp);

  auto remap = [](const uint8_t *from, const uint8_t *to, uint8_t n, uint16_t note, uint16_t &out) {
    for (uint8_t j = 0; j < n; j++) {
      if (from[j] == note) { out = to[j]; return true; }
    }
    out = note;
    return false;
  };

  // the notes the rhythm engine plays from next, updated with timer interrupts
  // held so a step never reads half of each
  noInterrupts();
  for (int i = 0; i < 7; i++) {
    uint16_t n;
    remap(old_chord_notes, current_chord_notes, 7, current_applied_chord_notes[i], n);
    current_applied_chord_notes[i] = n;
    remap(old_chord_notes, current_chord_notes, 7, rythm_freeze_current_chord_notes[i], n);
    rythm_freeze_current_chord_notes[i] = n;
  }
  interrupts();

  // A voice whose note is not in the old arrays is left at the pitch it has
  // when the division changes, since its number means nothing in the new one.
  for (int i = 0; i < 4; i++) {
    noInterrupts();
    if (chord_envelope_array[i]->isActive()) {
      uint16_t n;
      if (remap(old_chord_notes, current_chord_notes, 7, chord_voice_current_note[i], n) || !division_changed) {
        set_chord_voice_frequency(i, n);
      }
    }
    interrupts();
  }
  for (int i = 0; i < 12; i++) {
    if (string_enveloppe_array[i]->isActive()) {
      uint16_t n;
      if (remap(old_harp_notes, current_harp_notes, 12, harp_voice_current_note[i], n) || !division_changed) {
        set_harp_voice_frequency(i, n);
      }
    }
  }
  update_chord_notes();
  update_harp_notes();
}
// Function to compute MIDI note offset dynamically with circular frame shift
int8_t get_root_button(uint8_t key, uint8_t shift, uint8_t button) { 
  int8_t note = base_notes[button]; // Start with base note in C (e.g., B = 11, E = 4, ..., F = 5)
  // Apply circular frame shift: move notes C, D, E, F, G, A, B up an octave based on shift
  // Map button to musical note index (C=0, D=1, E=2, F=3, G=4, A=5, B=6)
  int8_t musical_index;
  switch (button) {
    case BTN_B: musical_index = 6; break; // B
    case BTN_E: musical_index = 2; break; // E
    case BTN_A: musical_index = 5; break; // A
    case BTN_D: musical_index = 1; break; // D
    case BTN_G: musical_index = 4; break; // G
    case BTN_C: musical_index = 0; break; // C
    case BTN_F: musical_index = 3; break; // F
    default: musical_index = 0; // Should not happen
  }
  if (musical_index < shift) {
    note += EDO; // Move up one octave if the note is shifted "on top"
  }
  int8_t num_accidentals = key_signatures[key];   // Apply key signature (sharps or flats)
  if (key <= KEY_SIG_B) { // Sharp keys (C, G, D, A, E, B)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == sharp_notes[num_accidentals - 1][i]) {
        note += sharp_step; // Add sharp
      }
    }
  } else { // Flat keys (F, Bb, Eb, Ab, Db, Gb)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == flat_notes[num_accidentals - 1][i]) {
        note -= sharp_step; // Add flat
      }
    }
  }

  return note; //No need to constrain here
}
// function to calculate the frequency of individual chord notes
// Collects the distinct tones of the current chord, reduced into a single octave
// and sorted low to high. The first four entries of a chord table are the chord
// proper, so a triad whose fourth entry is the octave yields three tones while a
// seventh or sixth chord yields four. Returns how many were found.
uint8_t collect_chord_tones(uint8_t (*chord)[7], uint8_t *tones) {
  uint8_t n = 0;
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t t = (*chord)[i] % EDO;
    bool duplicate = false;
    for (uint8_t j = 0; j < n; j++) {
      if (tones[j] == t) duplicate = true;
    }
    if (!duplicate) tones[n++] = t;
  }
  for (uint8_t i = 1; i < n; i++) { // insertion sort, n is at most 4
    uint8_t key = tones[i];
    int8_t j = i - 1;
    while (j >= 0 && tones[j] > key) { tones[j + 1] = tones[j]; j--; }
    tones[j + 1] = key;
  }
  return n;
}

// Semitone offset of a voice for the current inversion. Voices stack upward
// through the repeating chord tones, so inversion N starts that stack N steps
// higher. Working from pitch rather than from the chord table's index order
// matters: seventh chords list the seventh before the fifth, so rotating
// indices would not produce an inversion.
int16_t inverted_voice_offset(uint8_t (*chord)[7], uint8_t voice, uint8_t inversion) {
  uint8_t tones[4];
  uint8_t n = collect_chord_tones(chord, tones);
  if (n == 0) return 0;
  uint8_t k = voice + inversion;
  return tones[k % n] + EDO * (k / n);
}

// Offset of a chord tone for this voice. The four chord voices follow the
// inversion; the extra voices used in rythm mode keep the shuffling array's
// own choice of added tones.
// How far a voice moves for the current spacing. Drop voicings take a voice
// down an octave to open the chord out; the numbering counts from the top, so
// "drop 2" is the second voice down. Once the inversion step has run the voices
// are in pitch order, which is what makes this expressible per voice.
int8_t chord_spacing_shift(uint8_t voice) {
  switch (chord_spacing) {
    case 1: return (voice == 2) ? -EDO : 0;                         // drop 2
    case 2: return (voice == 1) ? -EDO : 0;                         // drop 3
    case 3: return (voice == 2 || voice == 0) ? -EDO : 0;           // drop 2 and 4
    case 4: return (voice == 0) ? -EDO : ((voice == 3) ? EDO : 0);  // spread the outer voices
    default: return 0;
  }
}

// Moves a voice for the current spacing, but only when there is room. A drop
// that would take the chord below the usable range is simply not made, so the
// voicing narrows at the extremes rather than wrapping into noise.
uint8_t apply_chord_spacing(uint8_t note, uint8_t voice, uint8_t level, bool slashed, bool sharp) {
  if (chord_spacing == 0 || voice >= 4 || level % 10 >= 4) return note;
  int8_t shift = chord_spacing_shift(voice);
  if (shift == 0) return note;
  if (shift < 0 && (int16_t)note + shift < chord_note_floor) return note;
  if (shift > 0 && (int16_t)note + shift > chord_note_ceiling) return note;

  // A slash chord names its own bass, so a dropped voice must not end up
  // underneath it. Another octave of the slash root is fine, and thickens it;
  // any other tone below would turn a C/G into something closer to a C/E.
  if (slashed && shift < 0) {
    int8_t slash_offset = sharp ? (flat_button_modifier ? -modifier_step : modifier_step) : 0;
    int16_t slash_note = EDO * (level / 10)
      + get_root_button(key_signature_selection, chord_frame_shift, slash_value)
      + slash_offset;
    int16_t moved = (int16_t)note + shift;
    if (moved < slash_note && (moved % EDO) != (slash_note % EDO)) return note;
  }
  return note + shift;
}

int16_t chord_tone_offset(uint8_t level, uint8_t voice) {
  // Spacing needs the voices in pitch order, so it uses the same sorted path as
  // inversion. At inversion 0 that reorders which oscillator plays which note
  // without changing the notes themselves, so nothing sounds different.
  if ((chord_inversion > 0 || chord_spacing > 0) && voice < 4 && level % 10 < 4) {
    return inverted_voice_offset(current_chord, voice, chord_inversion);
  }
  return (*current_chord)[level % 10];
}

uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp) {
  uint8_t note = 0;
  uint8_t level = chord_shuffling_array[chord_shuffling_selection][voice];
  if (slashed && level % 10 == note_slash_level) {
    if (!flat_button_modifier) {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * modifier_step);
    } else {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * modifier_step);
    }
  } else {
    if (!flat_button_modifier) {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * modifier_step + chord_tone_offset(level, voice));
      note = apply_chord_spacing(note, voice, level, slashed, sharp);
      } else {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * modifier_step + chord_tone_offset(level, voice));
      note = apply_chord_spacing(note, voice, level, slashed, sharp);    
    }
  }
  return note;
}


enum ChordType {
  CHORD_MAJOR, CHORD_MINOR, CHORD_SEVENTH, CHORD_MAJ_SEVENTH, CHORD_MIN_SEVENTH,
  CHORD_DIM, CHORD_AUG, CHORD_MAJ_SIXTH, CHORD_MIN_SIXTH, CHORD_FULL_DIM,
  // the just chords, kept together so is_ratio_chord() can test a range
  CHORD_NEUTRAL, CHORD_HARMONIC_7TH, CHORD_SUBMINOR, CHORD_SUPERMAJOR,
  CHORD_SUBMINOR_SEVENTH, CHORD_UTONAL_TETRAD, CHORD_HARMONIC_NINTH,
  CHORD_NEUTRAL_SEVENTH, CHORD_OTONAL_HEXAD,
  CHORD_JUST_AUGMENTED, CHORD_SUPERMAJOR_SEVENTH,
  CHORD_UNKNOWN
};

ChordType get_chord_type(uint8_t (*chord)[7]) {
  if (chord == &major)       return CHORD_MAJOR;
  if (chord == &minor)       return CHORD_MINOR;
  if (chord == &seventh)     return CHORD_SEVENTH;
  if (chord == &maj_seventh) return CHORD_MAJ_SEVENTH;
  if (chord == &min_seventh) return CHORD_MIN_SEVENTH;
  if (chord == &dim)         return CHORD_DIM;
  if (chord == &aug)         return CHORD_AUG;
  if (chord == &maj_sixth)   return CHORD_MAJ_SIXTH;
  if (chord == &min_sixth)   return CHORD_MIN_SIXTH;
  if (chord == &full_dim)    return CHORD_FULL_DIM;
  if (chord == &neutral)      return CHORD_NEUTRAL;
  if (chord == &harmonic_7th) return CHORD_HARMONIC_7TH;
  if (chord == &subminor)     return CHORD_SUBMINOR;
  if (chord == &supermajor)   return CHORD_SUPERMAJOR;
  if (chord == &subminor_seventh)   return CHORD_SUBMINOR_SEVENTH;
  if (chord == &utonal_tetrad)      return CHORD_UTONAL_TETRAD;
  if (chord == &harmonic_ninth)     return CHORD_HARMONIC_NINTH;
  if (chord == &neutral_seventh)    return CHORD_NEUTRAL_SEVENTH;
  if (chord == &otonal_hexad)       return CHORD_OTONAL_HEXAD;
  if (chord == &just_augmented)     return CHORD_JUST_AUGMENTED;
  if (chord == &supermajor_seventh) return CHORD_SUPERMAJOR_SEVENTH;
  return CHORD_UNKNOWN;
}

// Which scale suits the chord currently held. Pentatonic variants are used in
// mode 9; the diminished sixth scales suit the sixth and diminished chords in
// both modes.
//
// This does not need to test barry_harris_mode. handle_chord_type() already
// substitutes maj_sixth, min_sixth and full_dim for major, minor and dim when
// that mode is on, so the chord arriving here has the Barry Harris harmonisation
// baked in and maps to the diminished sixth scales by type alone.
uint8_t get_chord_scale_index(ChordType chord_type, bool use_pentatonic) {
  switch (chord_type) {
    case CHORD_MAJOR:        return use_pentatonic ? 0 : 10;
    case CHORD_MAJ_SEVENTH:  return use_pentatonic ? 1 : 12;
    case CHORD_MINOR:        return use_pentatonic ? 2 : 14;
    case CHORD_SEVENTH:      return use_pentatonic ? 3 : 13;
    case CHORD_MIN_SEVENTH:  return use_pentatonic ? 4 : 11;
    case CHORD_DIM:          return 5;
    case CHORD_AUG:          return 6;
    case CHORD_MAJ_SIXTH:    return 7;
    case CHORD_MIN_SIXTH:    return 8;
    case CHORD_FULL_DIM:     return 9;
    // The just chords, each on the scale of its twelve-tone counterpart, which
    // substitute_chord_tones() then retunes to the chord's own tones. Each row is
    // chosen so it has a seat for every tone the chord defines, so the retuning
    // replaces like with like. The neutral triad takes the major rows and its
    // neutral third the third's seat: a neutral third is still the chord's third,
    // and the harp plays the chord's tones.
    case CHORD_NEUTRAL:      return use_pentatonic ? 0 : 10;
    case CHORD_HARMONIC_7TH: return use_pentatonic ? 3 : 13;  // as a dominant: mixolydian
    case CHORD_SUBMINOR:     return use_pentatonic ? 2 : 14;  // leans minor
    case CHORD_SUPERMAJOR:   return use_pentatonic ? 0 : 10;  // leans major
    // The subminor seventh takes the minor pentatonic rather than m7's dorian
    // one, which voices a sixth in place of the seventh that is the whole point
    // of this chord. The utonal tetrad is m7b5 in twelve: locrian, and in mode 9
    // the minor pentatonic, whose fifth its 7/5 replaces. The neutral seventh
    // sits in the major seventh's seat but has no major third or seventh to keep,
    // so it takes the major rows and both its neutral tones move in; the
    // pentatonic has no seventh, so 11/6 takes the sixth's place, its nearest.
    case CHORD_SUBMINOR_SEVENTH: return use_pentatonic ? 2 : 11;   // as m7: dorian
    case CHORD_UTONAL_TETRAD:    return use_pentatonic ? 2 : 15;   // as m7b5: locrian
    case CHORD_HARMONIC_NINTH:   return use_pentatonic ? 3 : 13;   // as a dominant ninth
    case CHORD_NEUTRAL_SEVENTH:  return use_pentatonic ? 0 : 10;
    case CHORD_OTONAL_HEXAD:     return use_pentatonic ? 3 : 13;   // 4:5:6:7 is a dominant
    // The just augmented keeps the augmented chord's whole tone scale, which
    // has no letter degrees; its thirds already fall on whole tone steps in
    // every division, so nothing moves. The supermajor seventh takes the major
    // seventh's lydian rows; the pentatonic has no seventh, so 27/14 takes the
    // sixth's place, as the neutral seventh's 11/6 does.
    case CHORD_JUST_AUGMENTED:     return 6;
    case CHORD_SUPERMAJOR_SEVENTH: return use_pentatonic ? 1 : 12;
    default:                 return use_pentatonic ? 0 : 10;
  }
}

// Modes 1-7: a fixed scale rooted on the key signature, ignoring the chord.
uint8_t calculate_static_scale_note(uint8_t string, uint8_t mode, uint8_t key) {
  uint8_t scale_index = mode - 1;
  uint8_t scale_length = scale_lengths[scale_index];
  uint8_t octave = string / scale_length;
  uint8_t scale_degree = string % scale_length;
  uint8_t scale_root = scale_root_offsets[key];
  if (mode >= 5 && mode <= 7) {
    scale_root = (scale_root + EDO - minor_third_steps()) % EDO; // relative minor, a minor third down
  }
  return scale_root + scale_intervals[scale_index][scale_degree] + (octave * EDO) + EDO;
}

// Modes 8 and 9: a scale chosen to suit the chord being held, rooted on it.
//
// The just chords then retune that scale to their own tones: each of the
// chord's four voices replaces the scale entry naming the same degree -- its
// third the scale's third, its seventh the scale's seventh -- so the harp's
// third or seventh is the chord's own instead of the diatonic stand-in a step
// away (a 10-step third against the supermajor's 11 in 31, mixolydian's 26
// against the harmonic seventh's 25), and passing notes stay diatonic. A tone
// with no seat of its degree in the scale (a seventh over a pentatonic that has
// none) takes the nearest entry, the upper on a tie.
//
// Seating by degree rather than by nearness matters wherever a chord tone falls
// midway between two scale entries: in 19 the supermajor third sits one step
// above the major third and one below the fourth, and would otherwise take the
// fourth's place, leaving the harp both thirds and no fourth.
//
// The classical chords never reach this: their scales were designed around
// their tones, including the pentatonic rows that voice a sixth in place of a
// seventh on purpose.
bool is_ratio_chord(ChordType chord_type) {
  return chord_type >= CHORD_NEUTRAL && chord_type <= CHORD_SUPERMAJOR_SEVENTH;
}

// Degrees of each just chord's four voices, in table order; a voice that
// repeats a pitch class already placed (the octave doubling a triad's root) is
// skipped.
const uint8_t *ratio_chord_degrees(ChordType chord_type) {
  static const uint8_t triad[4]    = {0, 2, 4, 0};   // root, third, fifth, octave
  static const uint8_t dominant[4] = {0, 2, 6, 4};   // harmonic 7th lists its seventh before its fifth
  static const uint8_t seventh[4]  = {0, 2, 4, 6};
  static const uint8_t ninth[4]    = {0, 1, 2, 6};   // harmonic ninth: root, ninth, third, seventh
  switch (chord_type) {
    case CHORD_HARMONIC_7TH:   return dominant;
    case CHORD_HARMONIC_NINTH: return ninth;
    case CHORD_SUBMINOR_SEVENTH: case CHORD_UTONAL_TETRAD:
    case CHORD_NEUTRAL_SEVENTH:  case CHORD_OTONAL_HEXAD:
    case CHORD_SUPERMAJOR_SEVENTH:
      return seventh;
    default: return triad;
  }
}

void substitute_chord_tones(uint8_t *scale, uint8_t scale_length, uint8_t scale_index,
                            uint8_t (*chord)[7], ChordType chord_type) {
  const uint8_t *degrees = ratio_chord_degrees(chord_type);
  uint8_t placed[4];
  uint8_t placed_count = 0;
  for (uint8_t voice = 0; voice < 4; voice++) {
    uint8_t tone = (*chord)[voice] % EDO;
    bool seen = false;
    for (uint8_t i = 0; i < placed_count; i++) {
      if (placed[i] == tone) seen = true;
    }
    if (seen) continue;
    placed[placed_count++] = tone;

    uint8_t seat = 255;
    for (uint8_t j = 0; j < scale_length; j++) {
      if (chord_scale_degrees[scale_index][j] == degrees[voice]) { seat = j; break; }
    }
    if (seat == 255) {
      uint8_t best_distance = 255;
      for (uint8_t j = 0; j < scale_length; j++) {
        uint8_t distance = (scale[j] > tone) ? scale[j] - tone : tone - scale[j];
        if (distance < best_distance || (distance == best_distance && scale[j] > tone)) {
          best_distance = distance;
          seat = j;
        }
      }
    }
    scale[seat] = tone;
  }
}

uint8_t calculate_chord_specific_note(uint8_t string, uint8_t root_note, int8_t sharp_offset,
                                      uint8_t (*chord)[7], bool use_pentatonic) {
  ChordType chord_type = get_chord_type(chord);
  uint8_t scale_index = get_chord_scale_index(chord_type, use_pentatonic);
  uint8_t scale_length = chord_scale_lengths[scale_index];
  uint8_t octave = string / scale_length;
  uint8_t scale_degree = string % scale_length;
  if (is_ratio_chord(chord_type)) {
    uint8_t scale[8];
    memcpy(scale, chord_scale_intervals[scale_index], 8); // work on a copy, never the live table
    substitute_chord_tones(scale, scale_length, scale_index, chord, chord_type);
    return root_note + sharp_offset + scale[scale_degree] + (octave * EDO);
  }
  return root_note + sharp_offset + chord_scale_intervals[scale_index][scale_degree] + (octave * EDO);
}

uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp) {
  if (chromatic_harp_mode) {
    // Chromatic mode: one step a string from C two octaves up, in whatever
    // division is live (24 steps in 12, 38 in 19, 62 in 31).
    return 2 * EDO + string;
  }

  // Modes 1-7 ignore the chord entirely and run a fixed scale from the key
  if (scalar_harp_selection >= 1 && scalar_harp_selection <= 7) {
    return calculate_static_scale_note(string, scalar_harp_selection, key_signature_selection);
  }

  // Modes 8 and 9 keep the chord's root but choose the scale to suit its type
  if (scalar_harp_selection == 8 || scalar_harp_selection == 9) {
    uint8_t root_note = slashed
      ? get_root_button(key_signature_selection, chord_frame_shift, slash_value)
      : get_root_button(key_signature_selection, chord_frame_shift, fundamental);
    int8_t sharp_offset = sharp ? (flat_button_modifier ? -modifier_step : modifier_step) : 0;
    return calculate_chord_specific_note(string, root_note, sharp_offset, current_chord,
                                         scalar_harp_selection == 9);
  }

  // Mode 0, the existing chord-following behaviour, unchanged
  uint8_t note = 0;
  uint8_t level = harp_shuffling_array[harp_shuffling_selection][string];
  if (slashed && level % 10 == note_slash_level) {
    if (!flat_button_modifier) {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * modifier_step);
    } else {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * modifier_step);
    }
  } else {
    if (!flat_button_modifier) {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * modifier_step + (*current_chord)[level % 10]);
    } else {
      note = (EDO * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * modifier_step + (*current_chord)[level % 10]);

    }
  }
  return note;
}
//-->>RYTHM MODE UTILITIES
void rythm_tick_function() {
  //this function seems a bit long for a timed one. Maybe try to offload some logic somewhere else? 
  if (rythm_current_step % rythm_limit_change_to_every == 0) {
    for (int i = 0; i < 7; i++) {
      rythm_freeze_current_chord_notes[i] = current_applied_chord_notes[i];
    }
  }
  // handling the led pattern
  uint8_t active_modulus = 1;
  uint8_t possible_pattern[2] = {3, 2};
  for (uint8_t i = 0; i < sizeof(possible_pattern) / sizeof(uint8_t); i++) {
    if (rythm_loop_length % possible_pattern[i] == 0) {
      active_modulus = possible_pattern[i];
      break;
    }
  }
  analogWrite(RYTHM_LED_PIN, (220 * (rythm_current_step % rythm_limit_change_to_every == 0) + 15) * (rythm_current_step % active_modulus == 0));
  led_timer.priority(255);
  led_timer.begin([] { turn_off_led(&led_timer); }, 200000); 
  if (current_long_period) {
    rythm_timer.update(short_timer_period);
    current_long_period = false;
  } else {
    rythm_timer.update(long_timer_period);
    current_long_period = true;
  }
  u_int8_t result;
  result = rythm_pattern[rythm_current_step];
  for (int i = 6; i >= 0; i--) {
    if (result & (1 << i)) {
      int current_voice=0;
      if(i<4){
        current_voice=i;
      }else{
        current_voice=i-3;
      }
      set_chord_voice_frequency(current_voice, rythm_freeze_current_chord_notes[i]);
      play_note_selected_duration(current_voice, rythm_freeze_current_chord_notes[i]);
    }
  }
  rythm_current_step = (rythm_current_step + 1) % rythm_loop_length;
}

void recalculate_timer() {
  long_timer_period = shuffle * (60 * 1000 * 1000) / (2 * rythm_bpm);
  short_timer_period = 2 * (60 * 1000 * 1000) / (2 * rythm_bpm) - long_timer_period;
}

//--->>FILE HANDLING UTILITIES
String serialize(int16_t data_array[], u_int16_t array_size) {
  String dataString = "0,";
  dataString += String(current_bank_number); // to save the number of the bank for the online display
  dataString += ",";
  for (u_int16_t i = 2; i < array_size; i++) {
    dataString += String(data_array[i]);
    dataString += ",";
  }
  return dataString;
}

void deserialize(String input, int16_t data_array[]) {
  int len = input.length() + 1;
  char string[len];
  char *p;
  input.toCharArray(string, len);
  p = strtok(string, ",");
  int i = 0;
  while (p && i < parameter_size) {
    data_array[i] = atoi(p);
    p = strtok(NULL, ",");
    i++;
  }
}

/* ---- preset versioning ------------------------------------------------------
 *
 * Every preset stores all 256 slots, so an address that did not exist when the
 * preset was written reads back as a stored 0 — indistinguishable from someone
 * having set it to 0 on purpose. That is why a new parameter whose sensible
 * default is not 0 cannot be added without changing what existing presets sound
 * like, and why anything with a non-zero default has had to live outside the
 * preset array.
 *
 * The pieces to fix it were already here and not wired together: version_ID has
 * always been declared, the comment has always said address 7, parameters.json
 * has always carried introduction_version, and the editor already reads a
 * firmware version from address 7. Nothing ever wrote it.
 *
 * So: stamp the version into address 7 on save, and on load give any parameter
 * newer than the stored version its factory default instead of the stored 0.
 * Presets written before this change carry 0 there, which reads as "older than
 * everything" and defaults anything introduced after version 0 — which is the
 * right answer for them.
 */
void apply_preset_version(int bank_number) {
  const int16_t stored_version = current_sysex_parameters[firmware_version_adress];
  /* A 0 here means the preset was written straight from the factory defaults and
   * never saved by a player: the handler for address 7 has existed since version
   * 2 and ignores the incoming value, so any preset a firmware has saved carries
   * the version that saved it. Defaulting such a bank is therefore right rather
   * than destructive — it is a factory bank, and this restores it to the factory
   * values of the running firmware. */
  if (stored_version >= version_ID) {
    return;   // written by this firmware or newer: nothing to migrate
  }
  uint16_t restored = 0;
  for (uint16_t i = 2; i < parameter_size; i++) {
    if (i == firmware_version_adress) continue;
    if (parameter_introduction[i] > stored_version) {
      current_sysex_parameters[i] = default_bank_sysex_parameters[bank_number][i];
      restored++;
    }
  }
  current_sysex_parameters[firmware_version_adress] = version_ID;
  if (restored) {
    Serial.print("Preset written by version ");
    Serial.print(stored_version);
    Serial.print(", defaulted ");
    Serial.print(restored);
    Serial.println(" parameter(s) introduced since");
  }
}

void save_config(int bank_number, bool default_save) {
  if (bank_number < 0 || bank_number >= preset_number) {
    Serial.printf("Error: Invalid bank_number %d in save_config\n", bank_number);
    return;
  }
  digitalWrite(_MUTE_PIN, LOW); // muting the DAC
  current_bank_number=bank_number; //save to correctly write in the memory 
  AudioNoInterrupts();
  // myfs.quickFormat();  // performs a quick format of the created di
  myfs.remove(bank_name[bank_number]);
  File dataFile = myfs.open(bank_name[bank_number], FILE_WRITE);

  if (default_save) {
    // if we need to put the default in memory
    Serial.println("Writing the default file");
    default_bank_sysex_parameters[bank_number][firmware_version_adress] = version_ID;
    Serial.println(bank_name[bank_number]);
    String return_data = serialize(default_bank_sysex_parameters[bank_number], parameter_size);
    dataFile.println(return_data);
  } else {
    Serial.println("Saving current settings");
    for (u_int16_t i = 0; i < parameter_size; i++) {
          Serial.println(current_sysex_parameters[i]);
    }
    dataFile.println(serialize(current_sysex_parameters, parameter_size));
  }
  Serial.print("Saved preset: ");
  Serial.println(dataFile.name());
  dataFile.close();

  load_config(current_bank_number); //we do a full reload to initialise values
  
  // add something to set config_bit in the parameters to zero
  AudioInterrupts();
  digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
}

void load_config(int bank_number) {
  if (bank_number < 0 || bank_number >= preset_number) {
    Serial.printf("Error: Invalid bank_number %d in save_config\n", bank_number);
    return;
  }
  //digitalWrite(_MUTE_PIN, LOW); // muting the DAC
  //Turn off chords notes
  for (int i = 0; i < 4; i++) {
    chord_vibrato_envelope_array[i]->noteOff();
    chord_vibrato_dc_envelope_array[i]->noteOff();
    chord_envelope_array[i]->noteOff();
    chord_envelope_filter_array[i]->noteOff();
    chord_voice_released[i] = true;
  }
  trigger_chord = true; //to be ready to retrigger if needed

  File entry = myfs.open(bank_name[bank_number]);
  if (entry) {
    String data_string = "";
    while (entry.available()) {
      data_string += char(entry.read());
    }
    deserialize(data_string, current_sysex_parameters);
    apply_preset_version(bank_number);
    Serial.print("Loaded preset: ");
    Serial.println(entry.name());
    entry.close();
  } else {
    entry.close();
    Serial.print("No preset, writing factory default");
    save_config(bank_number, true); // reboot with default value
  }
  // Loading the potentiometer
  chord_pot.setup(chord_volume_sysex, 100, current_sysex_parameters[chord_pot_alternate_control], current_sysex_parameters[chord_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[chord_pot_alternate_storage],apply_audio_parameter,chord_pot_alternate_storage);
  harp_pot.setup(harp_volume_sysex, 100, current_sysex_parameters[harp_pot_alternate_control], current_sysex_parameters[harp_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[harp_pot_alternate_storage],apply_audio_parameter,harp_pot_alternate_storage);
  mod_pot.setup(current_sysex_parameters[mod_pot_main_control], current_sysex_parameters[mod_pot_main_range], current_sysex_parameters[mod_pot_alternate_control], current_sysex_parameters[mod_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[mod_pot_alternate_storage],apply_audio_parameter,mod_pot_alternate_storage);
  Serial.println("pot setup done");
  for (int i = 1; i < parameter_size; i++) {
    apply_audio_parameter(i, current_sysex_parameters[i]);
  }
  if (sysex_controler_connected) {
    control_command(0, 0); // push state to a connected remote controller
  }
  chord_pot.force_update();
  harp_pot.force_update();
  mod_pot.force_update();
  flag_save_needed=false;
  //digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising audio parameters");
  AudioMemory(1200);
  //>>STATIC AUDIO PARAMETERS
  // the waveshaper
  calculate_ws_array();
  chord_waveshape.shape(wave_shape, 257);
  string_waveshape.shape(wave_shape, 257);
  //the base DC value for strings
  filter_dc.amplitude(1);
  // the delay passthrough
  string_delay_mix.gain(0, 1);
  chord_delay_mix.gain(0, 1);
  // simple mixers
  string_vibrato_mixer.gain(0,0.5);
  string_vibrato_mixer.gain(1,0.5);
  envelope_string_vibrato_dc.sustain(0);
  for (int i = 0; i < 3; i++) {
    string_mixer_array[i]->gain(0, 1);
    string_mixer_array[i]->gain(1, 1);
    string_mixer_array[i]->gain(2, 1);
    string_mixer_array[i]->gain(3, 1);
    transient_mixer_array[i]->gain(0, 1);
    transient_mixer_array[i]->gain(1, 1);
    transient_mixer_array[i]->gain(2, 1);
    transient_mixer_array[i]->gain(3, 1);
  }
  for (int i = 0; i < 4; i++) {
    chord_voice_mixer_array[i]->gain(0, 1);
    chord_voice_mixer_array[i]->gain(1, 1);
    chord_voice_mixer_array[i]->gain(2, 1);
    chord_noise_array[i]->amplitude(0.5);
    //we hardcode the frequency modulation. Now intensity of the effect will be depending on the mixer gain 
    chord_osc_1_array[i]->frequencyModulation(2);
    chord_osc_2_array[i]->frequencyModulation(2);
    chord_osc_3_array[i]->frequencyModulation(2);
    //Now the max value of the vibrato is 0.25 for each component and we add 0.5 for pitch selection. With the multiplication by freqmodulation of 2, we maintain the rate we had before. 
    chord_vibrato_mixer_array[i]->gain(1,0.5); 

    chord_vibrato_dc_envelope_array[i]->sustain(0); //for the pitch bend no need for sustain
    transient_full_mix.gain(i, 1);
    all_string_mix.gain(i, 1);
  }
  for(int i=0;i<12;i++){
    string_transient_envelope_array[i]->sustain(0);//don't need sustain for the transient
  }
  all_string_mix.gain(3,0.02); //for the transient

  // initialising the rest of the hardware
  chord_matrix.setup();
  harp_sensor.setup();
  harp_sensor.recalibrate();
  pinMode(BATT_LBO_PIN, INPUT);
  pinMode(DOWN_PGM_PIN, INPUT);
  pinMode(UP_PGM_PIN, INPUT);
  pinMode(HOLD_BUTTON_PIN, INPUT);
  if (continuous_chord) {
    analogWrite(RYTHM_LED_PIN, 255);
  }
  // loading the preset
  Serial.println("Initialising filesystem");
  if (!myfs.begin(1024 * 1024)) { // Need to check that size
    Serial.printf("Error starting %s\n", "Program flash DISK");
    while (1) {
      set_led_color(0, 1.0, 1.0); // turn red light
    }
  }
  Serial.println("Loading the preset");
  load_config(current_bank_number);
  // initializing the strings
  for (int i = 0; i < 12; i++) {
    current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
  }
  //Checking the battery 
  LBO_flag.set(digitalRead(BATT_LBO_PIN));
  uint8_t LBO_value = LBO_flag.read_value();
  if (LBO_value == 0) {
    led_blinking_flag=true;
  }


  Serial.println("Initialisation complete");
  digitalWrite(_MUTE_PIN, HIGH);
}

void handle_chords_button() {
  int sharp_transition = chord_matrix_array[0].read_transition();
  if (sharp_transition > 1 && current_line != -1) {
    button_pushed = true;
  }
  sharp_active = chord_matrix_array[0].read_value();

  for (int i = 1; i < 22; i++) {
    int value = chord_matrix_array[i].read_transition();
    if (value > 1 && !inhibit_button) {
      button_pushed = true;
      Serial.print("Button pushed: ");
      Serial.println(i);
      if (current_line == -1) {
        current_line = (i - 1) / 3;
        if (!continuous_chord) {
          trigger_chord = true;
        }
      }
    }
  }
}

void handle_harp() {
  harp_sensor.update(harp_array);
  for (int i = 0; i < 12; i++) {
    int value = harp_array[i].read_transition();
    if (value == 2) {
      set_harp_voice_frequency(i, current_harp_notes[i]);
      AudioNoInterrupts();
      envelope_string_vibrato_lfo.noteOn();
      envelope_string_vibrato_dc.noteOn();
      string_enveloppe_filter_array[i]->noteOn();
      string_enveloppe_array[i]->noteOn();
      string_transient_envelope_array[i]->noteOn();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        queue_midi(false, harp_started_notes[i], harp_release_velocity, harp_channel, harp_port);
      }
      queue_midi(true, midi_base_note_transposed + midi_out_note(current_harp_notes[i]), harp_attack_velocity, harp_channel, harp_port);
      harp_started_notes[i] = midi_base_note_transposed + midi_out_note(current_harp_notes[i]);
    } else if (value == 1) {
      AudioNoInterrupts();
      string_enveloppe_array[i]->noteOff();
      string_transient_envelope_array[i]->noteOff();
      string_enveloppe_filter_array[i]->noteOff();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        queue_midi(false, harp_started_notes[i], harp_release_velocity, harp_channel, harp_port);
        harp_started_notes[i] = 0;
      }
    }
  }
}

uint8_t (*alt_chord_for(uint8_t slot))[7] {
  int16_t index = current_sysex_parameters[alt_slot_adress[slot]];
  if (index <= 0 || index > chord_catalogue_size) index = alt_slot_default[slot];
  else index -= 1;   // 1 selects the first catalogue entry, so 0 stays free for the default
  return chord_catalogue[index];
}

void handle_chord_type(bool button_maj, bool button_min, bool button_seventh) {
  if (!(button_maj || button_min || button_seventh)) {
    current_line = -1;
    return;
  }
  if (alt_chord_layout) {
    if (button_maj && !button_min && !button_seventh)            current_chord = alt_chord_for(0);
    else if (!button_maj && button_min && !button_seventh)       current_chord = alt_chord_for(1);
    else if (!button_maj && !button_min && button_seventh)       current_chord = alt_chord_for(2);
    else if (button_maj && !button_min && button_seventh)        current_chord = alt_chord_for(3);
    else if (!button_maj && button_min && button_seventh)        current_chord = alt_chord_for(4);
    else if (button_maj && button_min && !button_seventh)        current_chord = alt_chord_for(5);
    else if (button_maj && button_min && button_seventh)         current_chord = alt_chord_for(6);
    return;
  }

  if (button_maj && !button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &maj_sixth : &major;
  } else if (!button_maj && button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &min_sixth : &minor;
  } else if (!button_maj && !button_min && button_seventh) {
    current_chord = &seventh;
  } else if (button_maj && !button_min && button_seventh) {
    current_chord = &maj_seventh;
  } else if (!button_maj && button_min && button_seventh) {
    current_chord = &min_seventh;
  } else if (button_maj && button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &full_dim : &dim;
  } else if (button_maj && button_min && button_seventh) {
    current_chord = &aug;
  }
}

void detect_slash() {
  slash_chord = false;
  for (int i = 1; i < 22; i++) {
    if (chord_matrix_array[i].read_value()) {
      int slash_line = (i - 1) / 3;
      if (slash_line != current_line) {
        slash_chord = true;
        slash_value = slash_line;
      }
    }
  }
}

void update_chord_notes() {
  if (button_pushed) {
    chord_context_sharp = sharp_active;
    chord_context_slashed = slash_chord;
    for (int i = 0; i < 7; i++) {
      current_chord_notes[i] = calculate_note_chord(i, slash_chord, sharp_active);
    }
    Serial.println("Updating frequencies");
    if (!rythm_mode && !trigger_chord && !retrigger_chord) {
      for (int i = 0; i < 4; i++) {
        set_chord_voice_frequency(i, current_chord_notes[i]);
      }
    } else {
      for (int i = 0; i < 7; i++) {
        current_applied_chord_notes[i] = current_chord_notes[i];
      }
    }
  }
}

void update_harp_notes() {
  if (button_pushed) {
    chord_context_sharp = sharp_active;
    chord_context_slashed = slash_chord;
    for (int i = 0; i < 12; i++) {
      current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
      if (change_held_strings && harp_started_notes[i] != 0) {
        queue_midi(false, harp_started_notes[i], harp_release_velocity, harp_channel, harp_port);
        queue_midi(true, midi_base_note_transposed + midi_out_note(current_harp_notes[i]), harp_attack_velocity, harp_channel, harp_port);
        harp_started_notes[i] = midi_base_note_transposed + midi_out_note(current_harp_notes[i]);
        if (string_enveloppe_array[i]->isSustain()) {
          set_harp_voice_frequency(i, current_harp_notes[i]);
        }
      }
    }
  }
}

void stop_chord_notes() {
  // Cancel pending retrigger timers — prevents NoteOn firing after NoteOff already sent
  for (int i = 0; i < 4; i++) note_timer[i].end();
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++) {
    // Released in any stage, not only sustain: a chord let go during a long
    // attack, hold or decay used to play that stage out before it started to
    // release, so the release came seconds after the hand did. The sustain
    // test stays as the safety net it always was: a voice caught in its
    // retrigger ramp ignores a note off, and it is released once it settles.
    noInterrupts();
    if (chord_envelope_array[i]->isSustain() || (chord_envelope_array[i]->isActive() && !chord_voice_released[i])) {
      chord_vibrato_envelope_array[i]->noteOff();
      chord_vibrato_dc_envelope_array[i]->noteOff();
      chord_envelope_array[i]->noteOff();
      chord_envelope_filter_array[i]->noteOff();
      chord_voice_released[i] = true;
    }
    interrupts();
    // Sent regardless of the internal envelope: an external synth holds the note
    // until it receives the Note Off.
    if (chord_started_notes[i] != 0) {
      queue_midi(false, chord_started_notes[i], chord_release_velocity, chord_channel, chord_port);
      chord_started_notes[i] = 0;
    }
  }
  AudioInterrupts();
}

void handle_rhythm_mode() {
  for (int i = 0; i < 4; i++) {
    if (note_off_timing[i] > note_pushed_duration) {
      // As in stop_chord_notes(). The rhythm timer starts notes from an
      // interrupt, so the check and the flag are one step: a note starting in
      // between would otherwise be marked released without ever being.
      noInterrupts();
      if (chord_envelope_array[i]->isSustain() || (chord_envelope_array[i]->isActive() && !chord_voice_released[i])) {
        chord_vibrato_envelope_array[i]->noteOff();
        chord_vibrato_dc_envelope_array[i]->noteOff();
        chord_envelope_array[i]->noteOff();
        chord_envelope_filter_array[i]->noteOff();
        chord_voice_released[i] = true;
      }
      interrupts();
      // See stop_chord_notes().
      if (chord_started_notes[i] != 0) {
        queue_midi(false, chord_started_notes[i], chord_release_velocity, chord_channel, chord_port);
        chord_started_notes[i] = 0;
      }
    }
  }
}

void handle_continuous_mode() {
  bool one_button_active = false;
  int line_accumulator[3] = {0, 0, 0};
  for (int i = 1; i < 22; i++) {
    bool active = chord_matrix_array[i].read_value();
    one_button_active |= active;
    if (active) {
      line_accumulator[i % 3]++;
    }
  }
  if (line_accumulator[0] > 2 || line_accumulator[1] > 2 || line_accumulator[2] > 2) {
    current_line = -1;
    inhibit_button = true;
  }
  if (!one_button_active) {
    inhibit_button = false;
    stop_chord_notes();
  }
}

void handle_hold_button() {
  uint8_t hold_transition = hold_button.read_transition();
  if (hold_transition == 2) {
    if (!rythm_mode) {
      Serial.println("Switching mode");
      continuous_chord = !continuous_chord;
      analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
      if (current_line == -1) {
        trigger_chord = true;
      }
    } else {
      if (since_last_button_push > 100 && since_last_button_push < 2000) {
        rythm_bpm = (rythm_bpm * 5.0 + 60 * 1000 / since_last_button_push) / 6.0;
        Serial.print("Updating the BPM to: ");
        Serial.println(rythm_bpm);
        recalculate_timer();
        rythm_timer.update(current_long_period ? long_timer_period : short_timer_period);
      }
    }
    since_last_button_push = 0;
  } else if (hold_transition == 1 && since_last_button_push > 800) {
    Serial.println("Long push, switching rhythm mode");
    rythm_mode = !rythm_mode;
    continuous_chord = false;
    analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
    if (rythm_mode) {
      rythm_current_step = 0;
      Serial.println("Starting rhythm timers");
      rythm_timer.priority(254);
      rythm_timer.begin(rythm_tick_function, short_timer_period);
      rythm_timer_running = true;
      rythm_timer.update(long_timer_period);
      current_long_period = true;
    } else {
      Serial.println("Stopping rhythm timers");
      rythm_timer.end();
      rythm_timer_running = false;
    }
  }
}

void handle_preset_change() {
  if (up_button.read_transition() > 1) {
    Serial.println("Switching to next preset");
    if (!sysex_controler_connected && flag_save_needed) {
      save_config(current_bank_number, false);
    }
    current_bank_number = (current_bank_number + 1) % 12;
    load_config(current_bank_number);
  }
  if (down_button.read_transition() > 1) {
    Serial.println("Switching to last preset");
    if (!sysex_controler_connected && flag_save_needed) {
      save_config(current_bank_number, false);
    }
    current_bank_number = (current_bank_number - 1);
    if (current_bank_number == -1) {
      current_bank_number = 11;
    }
    load_config(current_bank_number);
  }
}

void handle_low_battery() {
  uint8_t LBO_transition = LBO_flag.read_transition();
  if (LBO_transition == 1) {
    led_blinking_flag = true;
  } else if (LBO_transition == 2) {
    led_blinking_flag = false;
    set_led_color(bank_led_hue, 1.0, 1 - led_attenuation);
  }
  if (led_blinking_flag) {
    set_led_color(bank_led_hue, 1.0, 0.6 + 0.4 * sin(color_led_blink_val));
    color_led_blink_val += 0.005;
  }
}

void trigger_chord_notes() {
  if ((trigger_chord || (button_pushed && retrigger_chord)) && !rythm_mode) {
    Serial.println("Triggering chord notes");
    for (int i = 0; i < 4; i++) {
      note_timer[i].priority(253);
    }
    note_timer[0].begin([] { play_single_note(0, &note_timer[0]); }, 10+chord_retrigger_release*1000);          // those allow for delayed triggering
    note_timer[1].begin([] { play_single_note(1, &note_timer[1]); }, 10 +chord_retrigger_release*1000+ inter_string_delay + random(random_delay));
    note_timer[2].begin([] { play_single_note(2, &note_timer[2]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 2 + random(random_delay));
    note_timer[3].begin([] { play_single_note(3, &note_timer[3]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 3 + random(random_delay));
    trigger_chord = false;
  }
  button_pushed = false;
}

void loop() {
  // Process incoming MIDI messages
  if (usbMIDI.read()) {
    processMIDI();
  }
  // Check sysex controller connection
  if (sysex_controler_connected && bitRead(USB1_PORTSC1, 7)) {
    sysex_controler_connected = false;
  }

  // Update debouncers
  hold_button.set(digitalRead(HOLD_BUTTON_PIN));
  up_button.set(digitalRead(UP_PGM_PIN));
  down_button.set(digitalRead(DOWN_PGM_PIN));
  LBO_flag.set(digitalRead(BATT_LBO_PIN));
  chord_matrix.update(chord_matrix_array);

  // Handle low battery indicator
  handle_low_battery();

  // Handle hold button for mode switching and rhythm
  handle_hold_button();

  // Handle preset changes
  handle_preset_change();

  // Handle rhythm mode note-off timing
  if (rythm_mode) {
    handle_rhythm_mode();
  }

  // Handle potentiometer updates
  bool alternate = chord_matrix_array[0].read_value();
  flag_save_needed |= chord_pot.update_parameter(alternate);
  flag_save_needed |= harp_pot.update_parameter(alternate);
  flag_save_needed |= mod_pot.update_parameter(alternate);

  // Handle continuous mode logic
  if (!continuous_chord && !rythm_mode) {
    handle_continuous_mode();
  }

  // Handle chord logic
  if (current_line >= 0) {
    fundamental = current_line;
    detect_slash();
    bool button_maj = chord_matrix_array[1 + current_line * 3].read_value();
    bool button_min = chord_matrix_array[2 + current_line * 3].read_value();
    bool button_seventh = chord_matrix_array[3 + current_line * 3].read_value();
    handle_chord_type(button_maj, button_min, button_seventh);
    update_chord_notes(); // Replaced updateNotes() with update_chord_notes()
    update_harp_notes();  // Added call to update_harp_notes()
    trigger_chord_notes();
  }

  // Handle chord button transitions
  handle_chords_button();

  // Handle harp functions
  handle_harp();

  // The only point at which this firmware transmits MIDI. Must stay last, and must
  // stay in loop() -- see the MIDI OUTPUT QUEUE comment above.
  drain_midi_queue();
}
