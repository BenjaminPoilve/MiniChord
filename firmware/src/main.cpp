#include "audio_definition.h"
#include "def.h"
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
int version_ID=12; //to be read 00.03, stored at adress 7 in memory
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

uint8_t alt_chord_layout = 0;   // 0 = standard chords, 1 = the assignable layout


// Every chord the instrument can make, in one list, so a button combination can
// be pointed at any of them rather than at a fixed table.
uint8_t (*chord_catalogue[18])[7] = {
  &major, &minor, &seventh, &maj_seventh, &min_seventh, &dim, &aug,
  &maj_sixth, &min_sixth, &full_dim, &half_dim,
  &sus_fourth, &sus_second, &seventh_sus,
  &major_ninth, &minor_ninth, &added_ninth, &six_nine
};
const uint8_t chord_catalogue_size = 18;

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
const int8_t base_notes[7] = {11, 4, 9, 2, 7, 0, 5}; // Base note offsets for buttons in key of C (relative to C4 = MIDI 60), in hardware order B, E, A, D, G, C, F
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
const int8_t scale_root_offsets[12] = {
  0, 7, 2, 9, 4, 11, // C, G, D, A, E, B
  5, 10, 3, 8, 1, 6  // F, Bb, Eb, Ab, Db, Gb
};

// Fixed scales for modes 1-7, semitones from the root
const uint8_t scale_intervals[7][8] = {
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
const uint8_t chord_scale_intervals[15][8] = {
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
  {0, 2, 3, 5, 7, 8, 10, 0}  // 14: Aeolian, minor
};
const uint8_t chord_scale_lengths[15] = {5, 5, 5, 5, 5, 8, 6, 8, 8, 8, 7, 7, 7, 7, 7};



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
// SYSEX midi message are used to control up to 512 synthesis parameters, in two pages of 256.
//
// Page 0 (0-255) is every setting there was before the array grew, and stays exactly as it was
// everywhere, so no existing preset changes: the preset files, the factory presets below, the
// dump an editor reads on command 0, and preset codes. Page 1 (256-511) is for new settings. A
// preset file holds all 512 now, but one saved before holds 256, and loading starts every slot at
// its default (parameter_default) before the file is read over it, so page 1 comes up at its
// defaults; firmware from before the growth reads the first 256 of a longer file and keeps page 0.
// Page 1 goes to an editor only when asked (command 7), with a header, so no tool that knows only
// page 0 can take it for a dump. 382, 383, 510 and 511 stay unused: their low byte is a universal
// SysEx id (0x7E, 0x7F), as the first byte of a write.
const uint16_t parameter_size = 512;
const uint16_t page_size = 256;
const uint8_t preset_number = 12;
static_assert(sizeof(parameter_page1_defaults) / sizeof(parameter_page1_defaults[0]) == parameter_size - page_size, "parameter_lookup.h is for another array size: run the generator");
// The factory presets' page 0; page 1's defaults are generated, the same for every bank
// (parameter_page1_defaults in parameter_lookup.h, from parameters.json)
int16_t default_bank_sysex_parameters[preset_number][page_size] = {
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,43,0,50,37,38,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,64,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,10,1,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,4,25,8,3,29,18,65,488,3,159,25,70,5,18,4,26,6,1,77,0,587,32,0,390,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,0,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,1659,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,1436,118,100,50,32,107,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,30,0,1,1,87,33,62,12,80,67,0,0,0,0,0,0,0,0,0,0,0,6,3,8,8,12,42,1855,1,42,20,217,3,35,83,59,2658,1,185,0,282,14,0,247,11,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,23,0,0,755,195,82,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,13,0,100,23,8,200,0,0,50,0,50,18,32,50,0,0,10,66,353,44,1452,1,569,16,141,32,83,28,48,54,1,0,698,82,56,0,579,0,28,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,100,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,76,92,100,184,100,85,100,0,0,60,0,1,0,14,0,0,0,42,46,0,0,0,0,0,0,0,0,0,0,3,10,8,11,42,30,61,2137,1,106,22,140,4,35,83,24,1956,1,139,0,282,0,0,247,10,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,57,0,1,858,70,66,100,100,0,0,0,0,127,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,8,200,11,0,134,10,0,100,7,50,59,15,50,0,15,10,10,45,61,1489,1,652,16,84,32,21,15,33,19,1,0,490,0,109,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,40,78,21,0,162,16,4,100,1000,639,140,100,47,100,85,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,110,0,0,0,57,0,0,0,67,58,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,85,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,60,47,56,159,100,157,100,0,0,138,1,0,0,0,0,0,0,27,74,0,0,0,0,0,0,0,0,0,0,4,14,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,200,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,10,11,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,35,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,2101,70,100,6,22,55,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,61,100,130,100,83,100,0,0,175,0,0,0,0,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,12,9,11,1,1,70,2141,1,383,17,302,101,103,34,30,1,12,0,0,367,17,0,363,5,1,1,1,86,1,1,0,1,1,1,1,8,0,0,70,0,0,0,100,0,33,50,0,5000,70,100,0,0,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,9,200,13,6,99,6,0,50,27,50,50,50,50,0,0,345,1,1,80,800,32,2200,0,70,1,1,1,100,1,1,0,30,0,0,0,166,0,6,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,59,100,0,80,16,4,100,100,2366,70,100,40,23,57,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,4,4,16,16,12,0,1,6,8,6,0,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,220,0,0,0,0,0,0,86,55,63,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,84,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,100,47,56,159,100,157,100,0,0,253,1,0,0,0,0,0,0,0,59,0,0,0,0,0,0,0,0,0,0,4,15,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,12,76,1,22,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,158,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,10,0,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,22,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,532,70,100,6,82,96,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,149,69,76,100,85,79,184,100,0,0,266,0,0,0,0,0,0,0,65,70,0,0,0,0,0,0,0,0,0,0,0,5,0,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,0,424,1360,70,46,0,0,100,60,81,0,0,996,125,82,61,71,0,0,0,0,108,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,20,12,100,23,0,200,15,8,100,0,50,45,17,0,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,857,70,42,100,100,158,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,55,61,100,184,100,85,100,0,0,310,0,0,0,86,0,56,87,37,60,0,0,0,0,0,0,0,0,0,0,4,15,12,36,42,30,59,1410,1,30,33,140,4,35,83,24,615,1,94,0,282,15,0,247,0,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,40,0,1,858,70,46,100,82,0,0,0,0,124,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,16,8,200,18,0,134,21,0,200,2,50,59,15,50,18,15,10,66,43,35,711,1,271,16,84,32,83,733,31,54,1,0,698,0,220,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,34,75,0,0,162,16,4,100,1000,1889,116,49,100,42,129,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,340,1,0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,4,25,3,3,29,18,65,488,3,159,25,70,5,18,4,26,40,1,77,0,587,32,0,715,11,76,1,1,100,1,1,68,17,14,22,20,17,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,1,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,706,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,678,118,100,50,32,168,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
}; 
int16_t current_sysex_parameters[parameter_size] = {0,0,50,50,512,512,512,1,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,0,0,0,0,0,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,38,0,0,80,16,4,94,753,474,70,5,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
const int8_t chord_note_floor = 12;  // below this the chord voices turn to mud
const int8_t chord_note_ceiling = 96;
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
bool rythm_timer_running = false;
volatile bool rythm_clock_synced = false;   // the rhythm following a MIDI clock (see the clock in processMIDI)
IntervalTimer rythm_timer;       // the rhythm's clock, every millisecond (see ACCOMPANIMENT)
IntervalTimer note_off_timer[4]; // timers for delayed chord enveloppe
IntervalTimer color_led_blink_timer;
// The rhythm LED's flash: lit on the step or the beat (acc_step, acc_beat), let go 200 ms on by the
// rhythm's own clock (acc_tick), so lighting it no longer starts a timer from inside an interrupt
volatile bool rythm_led_lit = false;
volatile uint32_t rythm_led_lit_at = 0;
elapsedMicros last_midi_clock_in;
// The accompaniment's settings (274-280), see ACCOMPANIMENT
uint8_t acc_style = 0;          // 0 the sixteen steps of rythm pattern, else a written part (accompaniment_styles.h)
uint8_t acc_bass_part = 0;      // voice 0: 0 plays the style's part, 1 holds the chord's note, 2 rests
uint8_t acc_chord_part = 0;     // voices 1-3, the same
uint8_t acc_follow = 0;         // 0 plays on, 1 only while a chord is held, 2 that and the bar starts on the press
uint8_t acc_change = 0;         // a new chord comes in: 0 on the next note, 1 on the beat, 2 on the bar
uint8_t acc_articulation = 100; // % of each written note's length
uint8_t acc_accents = 100;      // % of the written difference between loud and soft notes
uint8_t tap_counts = 0;         // tap tempo counts (281): 0 a tap a beat, 1 a tap an eighth; see acc_tap
volatile double acc_phase_error = 0;   // 24ths of a beat the rhythm is to catch up (or fall back) to a tap
// MIDI clock out (266): 0 off, 1 on port 1, 2 on port 2, 3 on both; see the clock in acc_tick
uint8_t midi_clock_out = 0;
volatile uint8_t midi_clock_started = 0;      // the ports a Start has gone to and no Stop since (bit 0 port 1, bit 1 port 2)
volatile uint32_t midi_clock_heard_ms = 0;    // when a MIDI clock last came in, rhythm mode or not
volatile bool midi_clock_heard = false;

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
  uint8_t realtime;  // a system real-time message (clock, start, stop), its type; 0 for a note
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
    midi_queue[midi_queue_head].realtime = 0;
    midi_queue_head = next;
  } else {
    midi_queue_dropped++;
  }
  if (!primask) __enable_irq();
}

// The same for a system real-time message: usbMIDI.Clock, Start or Stop
void queue_midi_realtime(uint8_t type, uint8_t cable) {
  uint32_t primask;
  __asm__ volatile("mrs %0, primask" : "=r"(primask));
  __disable_irq();
  uint8_t next = (midi_queue_head + 1) & (MIDI_QUEUE_SIZE - 1);
  if (next != midi_queue_tail) {
    midi_queue[midi_queue_head].cable = cable;
    midi_queue[midi_queue_head].realtime = type;
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
    e.realtime = midi_queue[midi_queue_tail].realtime;
    midi_queue_tail = (midi_queue_tail + 1) & (MIDI_QUEUE_SIZE - 1);
    if (e.realtime) {   // a clock goes straight out: the pacing is for synths reading notes
      usbMIDI.sendRealTime(e.realtime, e.cable);
      continue;
    }
    if (sent) delayMicroseconds(midi_buffer_delay); // pacing for slower hardware synths
    if (e.note_on) {
      usbMIDI.sendNoteOn(e.note, e.velocity, e.channel, e.cable);
    } else {
      usbMIDI.sendNoteOff(e.note, e.velocity, e.channel, e.cable);
    }
    sent = true;
  }
  // No send_now() here. The packet a message leaves half full goes out on the next USB start-of-frame
  // (125 us at 480 Mbit, 1 ms at 12), from the SOF interrupt. Flushing it from here as well froze the
  // USB controller under load: the core's usb_midi_flush_output() has no lock, the interrupt's flush
  // could land inside this one, and the descriptor the hardware had just taken was prepared again and
  // linked to itself.
}

//-->>FUNCTION THAT NEED ANNOUNCING
void save_config(int bank_number, bool default_save);
void load_config(int bank_number);
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp);
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp);
void set_chord_voice_frequency(uint8_t i, int16_t current_note);
void refresh_chord_filter();
// the note frequency each chord voice is currently sounding, kept so the filter
// corner can be recomputed for a voice without touching anything else about it
float chord_voice_note_freq[4] = {0, 0, 0, 0};
void calculate_ws_array();
void acc_settings_changed();
void acc_tap();
void acc_tap_reset();
void acc_clock_out_changed();
void apply_acc_voice_gain(uint8_t i, float gain);
void acc_midi_start();
void acc_midi_stop();
void acc_midi_clock();
uint8_t acc_held_mask();
void stop_chord_voices(uint8_t mask);

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
// A setting's factory default in a bank: page 0's from the bank's factory preset, page 1's generated
static inline int16_t parameter_default(int bank_number, uint16_t i) {
  return i < page_size ? default_bank_sysex_parameters[bank_number][i] : parameter_page1_defaults[i - page_size];
}

// A page of settings to an editor. Page 0 is the dump every editor has always read, recognised by
// its length alone, so it stays exactly that; the others carry a header (7D, the id for
// non-commercial use, then 6D and the page) and are a few bytes longer, which an editor that
// knows only page 0 drops.
void send_parameter_page(uint8_t page) {
  if (page >= parameter_size / page_size) return;
  const uint8_t header = page == 0 ? 0 : 3;
  uint8_t midi_data_array[3 + page_size * 2];
  midi_data_array[0] = 0x7D;
  midi_data_array[1] = 0x6D;
  midi_data_array[2] = page;
  for (uint16_t i = 0; i < page_size; i++) {
    int16_t parameter_value = constrain(current_sysex_parameters[page * page_size + i], 0, 16383); // a sysex byte only carries 7 bits
    midi_data_array[header + 2 * i] = parameter_value % 128;
    midi_data_array[header + 2 * i + 1] = parameter_value / 128;
  }
  usbMIDI.sendSysEx(header + page_size * 2, midi_data_array, 0);
}

void control_command(uint8_t command, uint8_t parameter) {
  switch (command) {
  case 0: // SIGNAL TO SEND BACK ALL DATA: page 0, as it always was, two bytes a setting and no header
    Serial.println("Reporting all data");
    send_parameter_page(0);
    break;
  case 7: // page 1 and on, for an editor that knows them: 7D 6D <page>, then two bytes a setting
    send_parameter_page(parameter);
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
// A parameter write is F0 <adress lo> <adress hi> <value lo> <value hi> F7. Universal system
// exclusive messages are six bytes long too, F0 <7E or 7F> <device id> <sub id 1> <sub id 2> F7,
// and hosts and DAWs send them unprompted, so they must not be read as a parameter write. Their
// first byte can't tell them apart: it is also the low byte of adresses 126, 127, 254 and 255.
// A device id of 2 or more already makes the adress 382 or more, past the parameter array, so
// only messages sent to device 0 or 1 need telling apart; most go to 0x7F, "all call". Those
// are recognised by their sub ids: identity request (7E 06 01), GM system on/off (7E 09 01-03)
// and MIDI machine control commands (7F 06 01-7F). The price is that writes of the same shape
// are dropped: 134 and 137 to adress 126 or 254 (frequency multiplier 2 of 1.34 or 1.37, a
// neighbouring slider step still lands), 265 and 393 to them, and 6+128*n (n>0) to 127 or 255,
// which amplitude 3 (0 to 100) never sends. Rarer six-byte universal messages sent to device 0
// or 1, such as the sample dump handshakes (7E 7B-7F), still read as a write to those adresses.
bool is_universal_sysex(const byte *data) {
  if (data[1] == 0x7E) {
    return (data[3] == 0x06 && data[4] == 0x01) || (data[3] == 0x09 && data[4] >= 0x01 && data[4] <= 0x03);
  }
  if (data[1] == 0x7F) {
    return data[3] == 0x06 && data[4] >= 0x01;
  }
  return false;
}

void processMIDI(void) {
  byte type;
  type = usbMIDI.getType();
  if (type == usbMIDI.SystemExclusive && usbMIDI.getSysExArrayLength() == 6) {
    const byte *data = usbMIDI.getSysExArray();
    if (!is_universal_sysex(data)) {
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
  // Following a MIDI clock (24 a beat), see ACCOMPANIMENT: Start begins the pattern on the next
  // clock, as MIDI has it; Stop, or the clock going quiet for half a second (rythm_clock_watch),
  // hands the rhythm back to the minichord's own tempo.
  if(type==usbMIDI.Start && rythm_mode){
    acc_midi_start();
    last_midi_clock_in = 0;
    Serial.println("Start received");
  }
  if(type==usbMIDI.Stop && rythm_mode){
    acc_midi_stop();
  }
  if(type==usbMIDI.Clock){   // the minichord keeps its own clock to itself while another is about
    midi_clock_heard = true;
    midi_clock_heard_ms = millis();
  }
  if(type==usbMIDI.Clock && rythm_mode){
    uint32_t us = last_midi_clock_in;
    last_midi_clock_in = 0;
    // the tempo from the clock's spacing; only a real interval counts (25 to 1250 BPM), not the
    // first clock after a gap
    if (us > 2000 && us < 100000) {
      rythm_bpm = constrain((rythm_bpm * 10 + 60e6f / (24.0f * us)) / 11.0f, 30.0f, 300.0f);
    }
    acc_midi_clock();
  }
}

//-->>TIMER FUNCTIONS
// function to handle the delayed chord activation
void play_single_note(int i, IntervalTimer *timer) {
  timer->end();
  apply_acc_voice_gain(i, 1);   // a voice the rhythm played softer, now holding the chord
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
  queue_midi(true, midi_base_note_transposed+ current_applied_chord_notes[i],chord_attack_velocity,chord_channel, chord_port);
  chord_started_notes[i]=midi_base_note_transposed+ current_applied_chord_notes[i];
}

// The MIDI note a chord voice's note goes out as. A bass line under the chord can go below note 0
// (see ACCOMPANIMENT); anything else is worked out exactly as it always was.
uint8_t chord_midi_note(int16_t note) {
  if (note >= 0) return midi_base_note_transposed + note;
  return (uint8_t)constrain(midi_base_note_transposed + note, 0, 127);
}

// How much of its note level (131-134) each chord voice plays at: a style's accents (see
// ACCOMPANIMENT), 1 for everything else
float acc_voice_gain[4] = {1, 1, 1, 1};
void apply_acc_voice_gain(uint8_t i, float gain) {
  if (acc_voice_gain[i] == gain) return;
  acc_voice_gain[i] = gain;
  chord_voice_mixer.gain(i, current_sysex_parameters[131 + i] / 100.0 * gain);
}

// A note of the rhythm on chord voice i, from its timer's interrupt: firmness 0-1 sets how loud it
// plays here, down to 24 dB, and goes out as its MIDI velocity (on chord velocity's scale)
void acc_play(uint8_t i, int16_t note, float firmness) {
  apply_acc_voice_gain(i, powf(10.0f, -1.2f * (1 - firmness) * (1 - firmness)));
  // ISR context: queue only, never touch usbMIDI here. The old note goes first, so moving the
  // voice to its new one doesn't also send the move (set_chord_voice_frequency) just before the
  // strike sends it again.
  if(chord_started_notes[i]!=0){
    queue_midi(false, chord_started_notes[i],chord_release_velocity,chord_channel, chord_port);
    chord_started_notes[i]=0;}
  set_chord_voice_frequency(i, note);
  chord_vibrato_envelope_array[i]->noteOn();
  chord_vibrato_dc_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  chord_voice_released[i] = false;
  uint8_t velocity = constrain((int)lroundf(chord_attack_velocity * firmness), 1, 127);
  chord_started_notes[i] = chord_midi_note(note);
  queue_midi(true, chord_started_notes[i], velocity, chord_channel, chord_port);
}

// A note of the rhythm let go, from its timer's interrupt or the loop. As in stop_chord_notes(): the
// check and the flag are one step, so a note starting in between isn't marked released without
// ever being.
void acc_release(uint8_t i) {
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

void set_chord_voice_frequency(uint8_t i, int16_t current_note) {
  float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
  if(glide_length>0){
        //ok so first we need to set the "middle note". Keep in mind that the signal will be +/-1 and will go +/- 2 octaves (frequencyModulation(2), hence the /24.0 below)
    //let's do a trick to select a middle note: get the level (relative to the C) and the note and do a modulo 
    int note_level=12*chord_octave_change-3*12+current_note+transpose_semitones;
    int base_octave =chord_octave_change-2+(chord_shuffling_array[chord_shuffling_selection][i])/10;
    int middle_note=base_octave*12+transpose_semitones; 
    int note_delta=note_level-middle_note;
    float middle_freq=c_frequency*pow(2,middle_note/12.0);

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
    chord_freq_dc_array[i]->amplitude(note_delta/24.0,glide_length);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }else{
    float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
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
  // (play_single_note, acc_tick), so it must queue rather than send.
  if(chord_started_notes[i]!=0 && chord_started_notes[i]!=chord_midi_note(current_note)){
    //we need to change the note without triggering the change, ie a pitch bend
    queue_midi(false, chord_started_notes[i],chord_release_velocity,chord_channel, chord_port);
    chord_started_notes[i]=0;
    queue_midi(true, chord_midi_note(current_note),chord_attack_velocity,chord_channel, chord_port);
    chord_started_notes[i]=chord_midi_note(current_note);
  }
}
// setting the harp
void set_harp_voice_frequency(uint8_t i, uint16_t current_note) {
  float note_freq =  pow(2,harp_octave_change)*c_frequency/4 * pow(2, (current_note+transpose_semitones) / 12.0);
  float transient_freq =  64.0*c_frequency/4 *pow(2, ((current_note+transpose_semitones)%12+transient_note_level) / 12.0);
  AudioNoInterrupts();
  string_waveform_array[i]->frequency(note_freq);
  string_transient_waveform_array[i]->frequency(transient_freq);
  string_filter_array[i]->frequency(string_filter_base_freq + note_freq * string_filter_keytrack);
  // string_vibrato_1.offset(0);
  AudioInterrupts();
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
    note += 12; // Move up one octave if the note is shifted "on top"
  }
  int8_t num_accidentals = key_signatures[key];   // Apply key signature (sharps or flats)
  if (key <= KEY_SIG_B) { // Sharp keys (C, G, D, A, E, B)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == sharp_notes[num_accidentals - 1][i]) {
        note += 1; // Add sharp
      }
    }
  } else { // Flat keys (F, Bb, Eb, Ab, Db, Gb)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == flat_notes[num_accidentals - 1][i]) {
        note -= 1; // Add flat
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
    uint8_t t = (*chord)[i] % 12;
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
  return tones[k % n] + 12 * (k / n);
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
    case 1: return (voice == 2) ? -12 : 0;                        // drop 2
    case 2: return (voice == 1) ? -12 : 0;                        // drop 3
    case 3: return (voice == 2 || voice == 0) ? -12 : 0;          // drop 2 and 4
    case 4: return (voice == 0) ? -12 : ((voice == 3) ? 12 : 0);  // spread the outer voices
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
    int8_t slash_offset = sharp ? (flat_button_modifier ? -1 : 1) : 0;
    int16_t slash_note = 12 * (level / 10)
      + get_root_button(key_signature_selection, chord_frame_shift, slash_value)
      + slash_offset;
    int16_t moved = (int16_t)note + shift;
    if (moved < slash_note && (moved % 12) != (slash_note % 12)) return note;
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
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * 1.0);
    } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * 1.0);
    }
  } else {
    if (!flat_button_modifier) {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * 1.0 + chord_tone_offset(level, voice));
      note = apply_chord_spacing(note, voice, level, slashed, sharp);
      } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * 1.0 + chord_tone_offset(level, voice));
      note = apply_chord_spacing(note, voice, level, slashed, sharp);    
    }
  }
  return note;
}


enum ChordType {
  CHORD_MAJOR, CHORD_MINOR, CHORD_SEVENTH, CHORD_MAJ_SEVENTH, CHORD_MIN_SEVENTH,
  CHORD_DIM, CHORD_AUG, CHORD_MAJ_SIXTH, CHORD_MIN_SIXTH, CHORD_FULL_DIM, CHORD_UNKNOWN
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
    scale_root = (scale_root + 12 - 3) % 12; // relative minor, a minor third down
  }
  return scale_root + scale_intervals[scale_index][scale_degree] + (octave * 12) + 12;
}

// Modes 8 and 9: a scale chosen to suit the chord being held, rooted on it.
uint8_t calculate_chord_specific_note(uint8_t string, uint8_t root_note, int8_t sharp_offset,
                                      uint8_t (*chord)[7], bool use_pentatonic) {
  uint8_t scale_index = get_chord_scale_index(get_chord_type(chord), use_pentatonic);
  uint8_t scale_length = chord_scale_lengths[scale_index];
  uint8_t octave = string / scale_length;
  uint8_t scale_degree = string % scale_length;
  return root_note + sharp_offset + chord_scale_intervals[scale_index][scale_degree] + (octave * 12);
}

uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp) {
  if (chromatic_harp_mode) {
    return string + 24; // Chromatic mode
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
    int8_t sharp_offset = sharp ? (flat_button_modifier ? -1 : 1) : 0;
    return calculate_chord_specific_note(string, root_note, sharp_offset, current_chord,
                                         scalar_harp_selection == 9);
  }

  // Mode 0, the existing chord-following behaviour, unchanged
  uint8_t note = 0;
  uint8_t level = harp_shuffling_array[harp_shuffling_selection][string];
  if (slashed && level % 10 == note_slash_level) {
    if (!flat_button_modifier) {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * 1.0);
    } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * 1.0);
    }
  } else {
    if (!flat_button_modifier) {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * 1.0 + (*current_chord)[level % 10]);
    } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * 1.0 + (*current_chord)[level % 10]);

    }
  }
  return note;
}
//-->>RYTHM MODE UTILITIES
/* ---- ACCOMPANIMENT -----------------------------------------------------------
 *
 * Rhythm mode plays the chord voices in time. It used to be one thing: sixteen
 * steps, each a set of the chord's seven notes, all the same length and
 * loudness. It still is, as rhythm style 0, and it plays as it always did. The
 * other styles are written parts (accompaniment_styles.h): an Alberti bass, a
 * waltz, a walking bass, a strummed guitar, and so on, each a few bars of notes
 * that know what they are -- the root, the fifth below it, the top of the
 * chord, a step into the next chord -- so they follow whatever chord is held,
 * in any key, with their own lengths, accents and strums.
 *
 * One clock runs it all: the rhythm timer, every millisecond, moving a position
 * counted in 24ths of a beat (MIDI clock's unit) at the tempo, and playing each
 * note whose time it passes. The old timer fired once a step and worked out the
 * next step's length; this one has a time for everything, so a tempo change or
 * a tap moves on smoothly, a shuffle is just where the off eighth (or sixteenth)
 * falls, and following a MIDI clock is keeping the position from running past
 * the clock's latest tick. The timer only plays notes and lets them go; MIDI
 * is queued, as everywhere (see MIDI OUTPUT QUEUE). It is the only timer the
 * rhythm uses: the LED's flash is let go by it too.
 *
 * The chord: acc_snapshot() records the chord as the player has just made it
 * (its voicing, root, bass and tones) whenever it is built; the rhythm plays
 * from its own copy, taken on the next note, the next beat or the next bar
 * (rhythm chord change). A chord pressed a moment after notes fell due -- the
 * player meant it for the beat and was late -- moves those notes to it
 * (acc_catch_late), as it would on an instrument a player strikes.
 *
 * Voice 0 is the bass part and voices 1-3 the chord part; either can play the
 * style, hold the chord as rhythm mode never could (a bass line walking under a
 * sustained pad), or rest. With rhythm follows hands, the parts sound only
 * while a chord is held, the clock running on underneath so they come back in
 * time, or with the bar starting from the press.
 */
#include "accompaniment_styles.h"

#define ACC_NO_NOTE -32768

// The chord as the rhythm sees it, in semitones
struct acc_chord_t {
  bool valid;
  int16_t v[4];        // the four chord voices' notes, lowest first
  int16_t root, bass;  // pitch classes
  int16_t third, fifth, seventh, second, fourth, sixth;   // above the root
};
acc_chord_t acc_latest = {};    // as the player last made it (from the loop, acc_snapshot)
acc_chord_t acc_now = {};       // as the rhythm plays it (taken from acc_latest as rhythm chord change says)
volatile uint16_t acc_latest_serial = 0;

double acc_pos = 0;                     // where the rhythm has got to, in 24ths of a beat since it began: every note before it has played
volatile double acc_clock_pos = 0;      // under a MIDI clock, where its latest tick fell
volatile bool acc_clock_waiting = false;   // under a MIDI clock, waiting for the tick it starts on
volatile bool acc_clock_restart = false;   // ... which begins the pattern (Start), rather than the next beat
volatile bool acc_restart = false;      // the next millisecond begins the pattern over (rhythm follows hands 2)
volatile bool acc_hands = false;        // a chord is held (from the loop)
volatile uint32_t acc_latch_ms = 0;     // when the rhythm last took a new chord on the beat or the bar

// each voice's latest note of the part
struct acc_voice_t {
  bool off_pending;   // to let go at off_at
  uint32_t off_at;
  bool due;           // a note of the part fell due, at `at`: struck, or not while the hands were off
  bool struck;
  uint32_t at;
  uint8_t role, length, velocity;
  int8_t octave;
  int16_t note, before;   // the note it played and the one before, for approaches
};
acc_voice_t acc_voice[4];

// notes of a strum waiting their string's turn
struct acc_rolled_t {
  bool used;
  uint32_t at;
  uint8_t voice, role, length, velocity;
  int8_t octave;
};
#define ACC_ROLL_MAX 8
acc_rolled_t acc_rolled[ACC_ROLL_MAX];

static inline uint8_t acc_style_index() { return acc_style < acc_style_count ? acc_style : 0; }
static inline int16_t acc_pc(int16_t n) { return ((n % 12) + 12) % 12; }
// the highest note of pitch class pc at or below ceiling
static inline int16_t acc_at_or_below(int16_t pc, int16_t ceiling) { return ceiling - acc_pc(ceiling - pc); }
static inline float acc_ms_per_tick() { return 60000.0f / (rythm_bpm * ACC_BEAT); }

// The voices the rhythm leaves to hold the chord as it is played outside rhythm mode
FLASHMEM uint8_t acc_held_mask() {
  if (acc_style_index() == 0) return 0;
  return (acc_bass_part == 1 ? 0x1 : 0) | (acc_chord_part == 1 ? 0xE : 0);
}

// From the loop, whenever a chord is built (update_chord_notes)
FLASHMEM void acc_snapshot() {
  acc_chord_t c;
  c.valid = true;
  for (uint8_t i = 0; i < 4; i++) c.v[i] = current_chord_notes[i];
  for (uint8_t i = 1; i < 4; i++) {   // lowest first: inversion and spacing can reorder the voices
    for (uint8_t j = i; j > 0 && c.v[j] < c.v[j - 1]; j--) {
      int16_t t = c.v[j]; c.v[j] = c.v[j - 1]; c.v[j - 1] = t;
    }
  }
  // as calculate_note_chord() builds it
  int16_t sharp = sharp_active ? (flat_button_modifier ? -1 : 1) : 0;
  const uint8_t *tones = *current_chord;
  c.root = acc_pc(get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp + tones[0]);
  c.bass = slash_chord ? acc_pc(get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp) : c.root;
  // The chord's tones in its first four slots are not in order (a seventh chord keeps its fifth in
  // the fourth), so the fifth is whichever is nearest a perfect fifth and the seventh whichever is
  // above a major sixth; the second, fourth and sixth are the slots after them.
  const int16_t fifth = 7, sixth_and_more = 9;
  c.third = acc_pc(tones[1] - tones[0]);
  c.fifth = fifth;
  c.seventh = 12;   // none: the octave
  int16_t nearest = 32767;
  for (uint8_t s = 1; s < 4; s++) {
    int16_t a = acc_pc(tones[s] - tones[0]);
    if (abs(a - fifth) < nearest) { nearest = abs(a - fifth); c.fifth = a; }
    if (a >= sixth_and_more) c.seventh = a;
  }
  c.second = acc_pc(tones[4] - tones[0]);
  c.fourth = acc_pc(tones[5] - tones[0]);
  c.sixth = acc_pc(tones[6] - tones[0]);
  noInterrupts();
  acc_latest = c;
  acc_latest_serial++;
  interrupts();
}

static inline void acc_take_chord() {
  if (acc_latest.valid) acc_now = acc_latest;
}

// The note a role plays in the chord the rhythm is playing; previous is the voice's last note
int16_t acc_note(uint8_t role, int8_t octave, int16_t previous) {
  const acc_chord_t &c = acc_now;
  int16_t root = acc_at_or_below(c.root, c.v[0]);
  int16_t n;
  switch (role) {
    case ACC_V1: case ACC_V2: case ACC_V3: case ACC_V4: n = c.v[role - ACC_V1]; break;
    case ACC_BASS: n = acc_at_or_below(c.bass, c.v[0]); break;
    case ACC_THIRD: n = root + c.third; break;
    case ACC_FIFTH: n = root + c.fifth; break;
    case ACC_SIXTH: n = root + c.sixth; break;
    case ACC_SEVENTH: n = root + c.seventh; break;
    case ACC_FLAT7: n = root + 10; break;
    case ACC_SECOND: n = root + c.second; break;
    case ACC_FOURTH: n = root + c.fourth; break;
    case ACC_APPROACH: {
      // a step from the next chord's bass (the one held, or waiting to come in) where that chord
      // will put it, from the side the voice is coming from: G to D flat to C, E to B to C
      const acc_chord_t &next = acc_latest.valid ? acc_latest : c;
      int16_t target = acc_at_or_below(next.bass, next.v[0]) + octave * 12;
      return previous != ACC_NO_NOTE && previous > target ? target + 1 : target - 1;
    }
    default: n = root; break;   // ACC_ROOT
  }
  return n + octave * 12;
}

// A note of a style's part on voice v, now
void acc_strike(uint8_t v, uint8_t role, int8_t octave, uint8_t length, uint8_t velocity) {
  acc_voice_t &a = acc_voice[v];
  int16_t note = acc_note(role, octave, a.note);
  float firmness = (127 - (127 - velocity) * acc_accents / 100.0f) / 127.0f;
  acc_play(v, note, firmness);
  a.before = a.note;
  a.note = note;
  a.struck = true;
  a.off_pending = length != 0;   // 0 rings until the voice plays again
  if (a.off_pending) a.off_at = millis() + (uint32_t)(length * acc_articulation / 100.0f * acc_ms_per_tick());
}

// A note of a style falls due
void acc_event(const acc_event_t &ev) {
  uint8_t v = ev.voice;
  if ((v == 0 ? acc_bass_part : acc_chord_part) != 0) return;   // holding the chord, or resting
  if (acc_change == 0) acc_take_chord();
  if (!acc_now.valid) return;   // no chord played yet
  acc_voice_t &a = acc_voice[v];
  a.due = true;
  a.struck = false;
  a.at = millis();
  a.role = ev.role; a.octave = ev.octave; a.length = ev.length; a.velocity = ev.velocity;
  if (acc_follow && !acc_hands) return;
  if (ev.roll == 0) {
    acc_strike(v, ev.role, ev.octave, ev.length, ev.velocity);
    return;
  }
  // a later string of a strum: inter-string delay a string, the whole strum within a quarter beat.
  // It counts as played (acc_catch_late moves it, it doesn't play it again).
  a.struck = true;
  float string_ms = fminf(inter_string_delay / 1000.0f, 15000.0f / rythm_bpm / 3);
  for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) {
    if (acc_rolled[r].used) continue;
    acc_rolled[r] = acc_rolled_t{true, millis() + (uint32_t)lroundf(ev.roll * string_ms), v, ev.role, ev.length, ev.velocity, ev.octave};
    return;
  }
  acc_strike(v, ev.role, ev.octave, ev.length, ev.velocity);   // no room to wait: with the rest
}

// A step of rhythm style 0, the sixteen steps as they always played: every chord note the step's
// bits name, the fourth to seventh on voices 1 to 3, for note pushed duration
void acc_step(uint8_t step) {
  rythm_current_step = step;
  if (step % rythm_limit_change_to_every == 0) {
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
  analogWrite(RYTHM_LED_PIN, (220 * (step % rythm_limit_change_to_every == 0) + 15) * (step % active_modulus == 0));
  rythm_led_lit_at = millis();   // off 200 ms on (rythm_led_settle)
  rythm_led_lit = true;
  if (acc_follow && !acc_hands) return;
  u_int8_t result = rythm_pattern[step];
  for (int i = 6; i >= 0; i--) {
    if (result & (1 << i)) {
      uint8_t v = i < 4 ? i : i - 3;
      acc_play(v, rythm_freeze_current_chord_notes[i], 1);
      acc_voice[v].off_pending = true;
      acc_voice[v].off_at = millis() + note_pushed_duration;
    }
  }
}

// A beat begins
void acc_beat(int64_t b) {
  const acc_style_t &st = acc_styles[acc_style_index()];
  if (st.count == 0) return;   // the sixteen steps light the LED themselves
  bool bar = b % st.beats_per_bar == 0;
  if (acc_change == 2 ? bar : acc_change == 1) {
    acc_take_chord();
    acc_latch_ms = millis();
  }
  analogWrite(RYTHM_LED_PIN, bar ? 235 : 15);
  rythm_led_lit_at = millis();
  rythm_led_lit = true;
}

// Where a note written at t (24ths of a beat from the start) plays: the second of each pair of
// swing units moves by the shuffle, later above 1 and earlier below, as the old steps did
static inline double acc_swung(int64_t t, uint8_t unit) {
  if (t % unit == 0 && (t / unit) % 2 == 1) return t + (shuffle - 1) * unit;
  return t;
}

// Everything that falls from t0 up to t1
void acc_window(double t0, double t1) {
  // beats first, so a chord coming in on the beat does before the beat's notes
  for (int64_t b = (int64_t)ceil(t0 / ACC_BEAT); b * ACC_BEAT < t1; b++) acc_beat(b);
  const acc_style_t &st = acc_styles[acc_style_index()];
  if (st.count == 0) {
    for (int64_t k = (int64_t)floor(t0 / 12) - 1; k <= (int64_t)floor(t1 / 12) + 1; k++) {
      if (k < 0) continue;
      double at = acc_swung(k * 12, 12);
      if (at >= t0 && at < t1) acc_step(k % rythm_loop_length);
    }
    return;
  }
  for (uint8_t e = 0; e < st.count; e++) {
    const acc_event_t &ev = st.events[e];
    int64_t n = (int64_t)floor((t0 - ev.at) / st.length);
    for (int64_t m = n; m <= n + 1; m++) {
      int64_t t = ev.at + m * st.length;
      if (t < 0) continue;
      double at = acc_swung(t, st.swing_unit);
      if (at >= t0 && at < t1) acc_event(ev);
    }
  }
}

// MIDI clock out (266): the rhythm's clock on to whatever is plugged in, so a drum machine or a
// computer plays in time with the minichord. A tick goes out each time the rhythm passes a 24th
// of a beat (acc_tick, from the same clock that plays the notes, so they stay together); a Start
// comes just before the tick that begins a bar, so what follows starts its bar with ours; a Stop
// when rhythm mode ends, the setting changes, or the pattern starts over from a press (and a Start
// on its first tick). Nothing goes out while another clock is coming in, or has in the last
// second: the minichord follows that one instead (see the clock in processMIDI), and two clocks
// would fight.
static inline bool midi_clock_out_free() {
  return midi_clock_out && !rythm_clock_synced && !(midi_clock_heard && millis() - midi_clock_heard_ms < 1000);
}

static void midi_clock_send(uint8_t type, uint8_t ports) {
  for (uint8_t cable = 0; cable < 2; cable++) {
    if (ports & (1 << cable)) queue_midi_realtime(type, cable);
  }
}

// a Stop to the ports a Start went to; from the timer, or the loop with it held off
static void midi_clock_stop() {
  if (!midi_clock_started) return;
  midi_clock_send(usbMIDI.Stop, midi_clock_started);
  midi_clock_started = 0;
}

// the ticks from t0 up to t1, before the notes there so a receiver has its tick when they arrive
static void midi_clock_window(double t0, double t1) {
  if (!midi_clock_out_free()) {
    midi_clock_stop();
    return;
  }
  uint16_t bar = ACC_BEAT * acc_styles[acc_style_index()].beats_per_bar;
  for (int64_t t = (int64_t)ceil(t0); t < t1; t++) {
    if (!midi_clock_started && t % bar == 0) {
      midi_clock_started = midi_clock_out & 3;
      midi_clock_send(usbMIDI.Start, midi_clock_started);
    }
    midi_clock_send(usbMIDI.Clock, midi_clock_out & 3);
  }
}

// The setting changed: a Stop where the clock was going; it starts again on the next bar
FLASHMEM void acc_clock_out_changed() {
  noInterrupts();
  midi_clock_stop();
  interrupts();
}

// The rhythm timer, every millisecond
void acc_tick() {
  uint32_t now = millis();
  if (rythm_led_lit && now - rythm_led_lit_at >= 200) {
    rythm_led_lit = false;
    analogWrite(RYTHM_LED_PIN, 0);
  }
  for (uint8_t v = 0; v < 4; v++) {
    if (acc_voice[v].off_pending && (int32_t)(now - acc_voice[v].off_at) >= 0) {
      acc_voice[v].off_pending = false;
      acc_release(v);
    }
  }
  for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) {
    acc_rolled_t &n = acc_rolled[r];
    if (!n.used || (int32_t)(now - n.at) < 0) continue;
    n.used = false;
    if (!acc_follow || acc_hands) acc_strike(n.voice, n.role, n.octave, n.length, n.velocity);
  }
  if (acc_restart) {
    acc_restart = false;
    acc_pos = 0;
    acc_phase_error = 0;
    midi_clock_stop();   // and a Start on the tick that begins it again
  }
  double step = rythm_bpm * ACC_BEAT / 60000.0;
  if (acc_phase_error != 0) {   // toward a tap: up to 30% quicker or slower until there (acc_tap)
    double c = acc_phase_error;
    if (c > 0.3 * step) c = 0.3 * step;
    if (c < -0.3 * step) c = -0.3 * step;
    step += c;
    acc_phase_error = acc_phase_error - c;
  }
  double t1 = acc_pos + step;
  if (rythm_clock_synced) {
    if (acc_clock_waiting) return;
    // as far as the clock's next tick and no further, and at least to its latest
    if (t1 > acc_clock_pos + 1) t1 = acc_clock_pos + 1;
    if (t1 <= acc_clock_pos) t1 = acc_clock_pos + 1e-6;
  }
  if (t1 <= acc_pos) return;
  midi_clock_window(acc_pos, t1);
  acc_window(acc_pos, t1);
  acc_pos = t1;
}

// Rhythm mode comes on
FLASHMEM void acc_begin() {
  noInterrupts();
  rythm_clock_synced = false;
  acc_clock_waiting = false;
  acc_restart = false;
  acc_pos = 0;
  acc_phase_error = 0;
  acc_tap_reset();
  rythm_current_step = 0;
  for (uint8_t v = 0; v < 4; v++) acc_voice[v] = acc_voice_t{false, 0, false, false, 0, 0, 0, 0, 0, ACC_NO_NOTE, ACC_NO_NOTE};
  for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) acc_rolled[r].used = false;
  acc_now = acc_latest;
  acc_hands = current_line >= 0;
  interrupts();
  stop_chord_voices(0xF & ~acc_held_mask());   // what was ringing gives way to the parts
  Serial.print("Rhythm style: ");
  Serial.println(acc_styles[acc_style_index()].name);
  rythm_timer.priority(254);
  rythm_timer.begin(acc_tick, 1000);
  rythm_timer_running = true;
}

// and goes off
FLASHMEM void acc_end() {
  rythm_timer.end();
  rythm_timer_running = false;
  rythm_clock_synced = false;
  midi_clock_stop();
  for (uint8_t v = 0; v < 4; v++) acc_voice[v].off_pending = false;
  for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) acc_rolled[r].used = false;
  // voices the rhythm played softer are back to full for the chords (play_single_note does the
  // same for one still ringing out, when it next plays)
  for (uint8_t v = 0; v < 4; v++) {
    noInterrupts();
    if (!chord_envelope_array[v]->isActive()) apply_acc_voice_gain(v, 1);
    interrupts();
  }
  rythm_led_lit = false;
}

// A style, part or follow setting changed: the voices the rhythm now plays start from their part
FLASHMEM void acc_settings_changed() {
  if (!rythm_mode) return;
  noInterrupts();
  for (uint8_t v = 0; v < 4; v++) acc_voice[v].off_pending = false;
  for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) acc_rolled[r].used = false;
  interrupts();
  stop_chord_voices(0xF & ~acc_held_mask());
}

// MIDI Start: the pattern begins on the next clock, as MIDI has it
FLASHMEM void acc_midi_start() {
  noInterrupts();
  rythm_clock_synced = true;
  acc_clock_waiting = true;
  acc_clock_restart = true;
  interrupts();
}

// MIDI Stop: on from where it is, at the minichord's own tempo
FLASHMEM void acc_midi_stop() {
  rythm_clock_synced = false;
}

// A MIDI clock tick
FLASHMEM void acc_midi_clock() {
  noInterrupts();
  if (!rythm_clock_synced) {   // a clock already running when rhythm mode came on: a beat starts on this tick
    rythm_clock_synced = true;
    acc_clock_waiting = true;
    acc_clock_restart = false;
  }
  acc_phase_error = 0;
  if (acc_clock_waiting) {
    acc_clock_waiting = false;
    acc_pos = acc_clock_restart ? 0 : ceil(acc_pos / ACC_BEAT) * ACC_BEAT;
    acc_clock_pos = acc_pos;
  } else {
    acc_clock_pos = acc_clock_pos + 1;
  }
  interrupts();
}

// from the loop: a clock gone quiet without a Stop (the computer's player closed, the cable out)
FLASHMEM void rythm_clock_watch() {
  if (rythm_mode && rythm_clock_synced && last_midi_clock_in > 500000) {
    rythm_clock_synced = false;
    acc_clock_waiting = false;
  }
}

// Tap tempo: the hold button tapped in rhythm mode. Each tap was a sixth of the way from the tempo
// to the one tapped, so it took ten taps to get near, and a run of taps at the pulse the pattern
// moves in (eighths, as often as not) dragged the tempo up while the pattern ran away from the
// hand. Now the second tap sets the tempo and the next few refine it: the time across the last few
// taps (up to four intervals). With tap tempo counts (281) at eighths, two taps make a beat, and
// the tempo is taken across pairs of taps, so a shuffle's long and short eighths still make an
// even beat. A tap far off the run (40% from the beat it implies), or after more than two seconds,
// begins a new run. And each tap from the second draws the beat (or the eighth, where the shuffle
// puts it) halfway to where it fell: the rhythm hurries or holds back, up to 30%, until it is
// there, so no note is skipped or played twice, and a slightly uneven hand settles in the middle
// of its taps rather than being chased. Under a MIDI clock the clock has the tempo.
#define TAP_RUN 5
uint32_t tap_us[TAP_RUN];   // the run's taps, latest last
uint8_t tap_count = 0;

FLASHMEM void acc_tap_reset() { tap_count = 0; }

// ms a beat from the first `taps` taps of the run: across whole beats when taps are eighths and
// there are two or more intervals, so a shuffle's long and short eighths even out
static float tap_beat(uint8_t taps) {
  uint8_t intervals = taps - 1;
  if (tap_counts && intervals >= 2) intervals &= ~1;
  float per_beat = tap_counts ? 2 : 1;
  return (tap_us[taps - 1] - tap_us[taps - 1 - intervals]) / 1000.0f * per_beat / intervals;
}

FLASHMEM void acc_tap() {
  uint32_t now = micros();
  if (rythm_clock_synced || (tap_count > 0 && now - tap_us[tap_count - 1] > 2000000)) tap_count = 0;
  if (tap_count == TAP_RUN) {   // keep the latest
    memmove(tap_us, tap_us + 1, sizeof(uint32_t) * (TAP_RUN - 1));
    tap_count--;
  }
  tap_us[tap_count++] = now;
  if (rythm_clock_synced || tap_count < 2) return;
  // this tap's own beat (its interval, or with eighths the two taps to it) against the run's
  // before it: far off, a new run from the tap before this one
  if (tap_count >= 3) {
    uint8_t own = (tap_counts && tap_count >= 3) ? 2 : 1;
    float per_beat = tap_counts ? 2 : 1;
    float this_beat = (now - tap_us[tap_count - 1 - own]) / 1000.0f * per_beat / own;
    float before = tap_beat(tap_count - 1);
    if (fabsf(this_beat - before) > 0.4f * before) {
      tap_us[0] = tap_us[tap_count - 2];
      tap_us[1] = now;
      tap_count = 2;
    }
  }
  float beat_ms = tap_beat(tap_count);
  if (beat_ms < 200 || beat_ms > 2000) return;   // 30 to 300 bpm
  rythm_bpm = 60000.0f / beat_ms;
  noInterrupts();
  // where the tap was: the button is believed after 10 ms of steady contact (the debouncer)
  double at = acc_pos - 10 * rythm_bpm * ACC_BEAT / 60000.0;
  // the nearest beat, or with eighths the nearest eighth as the shuffle places it
  double beat = floor(at / ACC_BEAT) * ACC_BEAT;
  double nearest = (at - beat < ACC_BEAT / 2) ? beat : beat + ACC_BEAT;
  if (tap_counts) {
    double off = beat + ACC_BEAT / 2 * shuffle;
    if (fabs(at - off) < fabs(at - nearest)) nearest = off;
  }
  acc_phase_error = (nearest - at) / 2;
  interrupts();
  Serial.print("Updating the BPM to: ");
  Serial.println(rythm_bpm);
}

// A chord that arrives just after notes of the part fell due -- meant for the beat, a moment late --
// moves them to it, or plays them, if the hands were off when they fell due. With the chord coming
// in on the beat or the bar, it takes that beat or bar if it is this late for it, not the next.
FLASHMEM void acc_catch_late() {
  float grace = fminf(100.0f, 15000.0f / rythm_bpm);   // a quarter of a beat, at most 100 ms
  noInterrupts();
  bool take = acc_change == 0 || millis() - acc_latch_ms <= grace;
  if (take) acc_take_chord();
  for (uint8_t v = 0; v < 4 && acc_now.valid; v++) {
    acc_voice_t &a = acc_voice[v];
    if (!a.due || millis() - a.at > grace) continue;
    if (!a.struck) {
      acc_strike(v, a.role, a.octave, a.length, a.velocity);
    } else if (take && chord_envelope_array[v]->isActive() && !chord_voice_released[v]) {
      int16_t note = acc_note(a.role, a.octave, a.before);
      if (note != a.note) {
        set_chord_voice_frequency(v, note);
        a.note = note;
      }
    }
  }
  interrupts();
}

// From the loop, in rhythm mode, once the chord buttons have been read
FLASHMEM void acc_update() {
  static uint16_t seen = 0;
  bool hands = current_line >= 0;
  bool caught = false;
  if (hands != acc_hands) {
    acc_hands = hands;
    if (acc_follow && !hands) {   // let go: the parts stop, and the chord held with them
      noInterrupts();
      for (uint8_t v = 0; v < 4; v++) acc_voice[v].off_pending = false;
      for (uint8_t r = 0; r < ACC_ROLL_MAX; r++) acc_rolled[r].used = false;
      interrupts();
      stop_chord_voices(0xF);
    } else if (acc_follow == 2 && hands && !rythm_clock_synced) {
      acc_restart = true;   // the bar starts with the press
      caught = true;
    } else if (acc_follow && hands) {
      acc_catch_late();
      caught = true;
    }
  }
  if (acc_latest_serial != seen) {
    seen = acc_latest_serial;
    if (!caught && acc_style_index() != 0) acc_catch_late();
  }
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
    Serial.println(bank_name[bank_number]);
    static int16_t factory[parameter_size];
    for (uint16_t i = 0; i < parameter_size; i++) factory[i] = parameter_default(bank_number, i);
    String return_data = serialize(factory, parameter_size);
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
    // every slot from its default first, so a slot the file doesn't hold (all of page 1, in a
    // preset saved before the array grew) comes up at its default, not the last preset's
    for (uint16_t i = 0; i < parameter_size; i++) current_sysex_parameters[i] = parameter_default(bank_number, i);
    deserialize(data_string, current_sysex_parameters);
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
      queue_midi(true, midi_base_note_transposed + current_harp_notes[i], harp_attack_velocity, harp_channel, harp_port);
      harp_started_notes[i] = midi_base_note_transposed + current_harp_notes[i];
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
    for (int i = 0; i < 7; i++) {
      current_chord_notes[i] = calculate_note_chord(i, slash_chord, sharp_active);
    }
    acc_snapshot();
    Serial.println("Updating frequencies");
    if (!rythm_mode && !trigger_chord && !retrigger_chord) {
      for (int i = 0; i < 4; i++) {
        set_chord_voice_frequency(i, current_chord_notes[i]);
      }
    } else {
      for (int i = 0; i < 7; i++) {
        current_applied_chord_notes[i] = current_chord_notes[i];
      }
      // in rhythm mode, the voices holding the chord move to the new one as they would outside it
      if (rythm_mode && !trigger_chord && !retrigger_chord) {
        uint8_t held = acc_held_mask();
        for (int i = 0; i < 4; i++) {
          if (held & (1 << i)) set_chord_voice_frequency(i, current_chord_notes[i]);
        }
      }
    }
  }
}

void update_harp_notes() {
  if (button_pushed) {
    for (int i = 0; i < 12; i++) {
      current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
      if (change_held_strings && harp_started_notes[i] != 0) {
        queue_midi(false, harp_started_notes[i], harp_release_velocity, harp_channel, harp_port);
        queue_midi(true, midi_base_note_transposed + current_harp_notes[i], harp_attack_velocity, harp_channel, harp_port);
        harp_started_notes[i] = midi_base_note_transposed + current_harp_notes[i];
        if (string_enveloppe_array[i]->isSustain()) {
          set_harp_voice_frequency(i, current_harp_notes[i]);
        }
      }
    }
  }
}

void stop_chord_notes() {
  stop_chord_voices(0xF);
}

// Lets go of the chord voices in mask (bit i, voice i)
void stop_chord_voices(uint8_t mask) {
  // Cancel pending retrigger timers — prevents NoteOn firing after NoteOff already sent
  for (int i = 0; i < 4; i++) if (mask & (1 << i)) note_timer[i].end();
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++) {
    if (!(mask & (1 << i))) continue;
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
      acc_tap();
    }
    since_last_button_push = 0;
  } else if (hold_transition == 1 && since_last_button_push > 800) {
    Serial.println("Long push, switching rhythm mode");
    rythm_mode = !rythm_mode;
    continuous_chord = false;
    analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
    if (rythm_mode) {
      Serial.println("Starting rhythm timers");
      acc_begin();
    } else {
      Serial.println("Stopping rhythm timers");
      acc_end();
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
  uint8_t voices = rythm_mode ? acc_held_mask() : 0xF;   // in rhythm mode, the voices it leaves to hold the chord
  if ((trigger_chord || (button_pushed && retrigger_chord)) && voices) {
    Serial.println("Triggering chord notes");
    for (int i = 0; i < 4; i++) {
      note_timer[i].priority(253);
    }
    if (voices & 1) note_timer[0].begin([] { play_single_note(0, &note_timer[0]); }, 10+chord_retrigger_release*1000);          // those allow for delayed triggering
    if (voices & 2) note_timer[1].begin([] { play_single_note(1, &note_timer[1]); }, 10 +chord_retrigger_release*1000+ inter_string_delay + random(random_delay));
    if (voices & 4) note_timer[2].begin([] { play_single_note(2, &note_timer[2]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 2 + random(random_delay));
    if (voices & 8) note_timer[3].begin([] { play_single_note(3, &note_timer[3]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 3 + random(random_delay));
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

  rythm_clock_watch();

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
  if (rythm_mode) acc_update();   // after the chord is built, so a press is heard with its chord

  // Handle chord button transitions
  handle_chords_button();

  // Handle harp functions
  handle_harp();

  // The only point at which this firmware transmits MIDI. Must stay last, and must
  // stay in loop() -- see the MIDI OUTPUT QUEUE comment above.
  drain_midi_queue();
}
