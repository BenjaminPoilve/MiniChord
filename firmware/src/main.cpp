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
potentiometer chord_pot(POT_CHORD_PIN);
potentiometer harp_pot(POT_HARP_PIN);
potentiometer mod_pot(POT_MOD_PIN);
LittleFS_Program myfs; // to save the settings
bool color_led_blink_state = true;
//>>CHORD DEFINITION<<
//for each chord, we first have the 4 notes of the chord, then decoration that might be used in specific modes
uint8_t major[7] = {0, 4, 7, 12, 2, 5, 9};  
uint8_t minor[7] = {0, 3, 7, 12, 2, 5, 8};
uint8_t seventh[7] = {0, 4, 10, 7, 2, 5, 9};
uint8_t maj_seventh[7] = {0, 4, 11, 7, 2, 5, 9};
uint8_t min_seventh[7] = {0, 3, 10, 7, 2, 5, 8};
uint8_t aug[7] = {0, 4, 8, 12, 2, 5, 9};
uint8_t dim[7] = {0, 3, 6, 12, 2, 5, 9};
uint8_t root_button[7] = {0, 4, 2, 5, 9, 11, 7}; // defines the fundamental of each row, ie C, E, D, F, A, B, G
float c_frequency = 130.81;                      // for 440Hz A
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
bool continuous_chord = false; // wether the chord is held continuously. Controlled by the "hold" button
bool rythm_mode = false;
IntervalTimer note_timer[4]; // timers for delayed chord enveloppe

//>>SWITCHING LOGIC PARAMETERS<<
uint8_t note_slash_level = 0;     // the level we are replacing in the chord when slashing (usually the fundamental)
bool retrigger_chord = true;      // wether or not to retrigger the enveloppe when the chord is switched within current line (including when selecting slash chord)
bool change_held_strings = false; // to control wether hold strings change with chord:

//>>SYSEX PARAMETERS<<
// SYSEX midi message are used to control up to 256 synthesis parameters.
const uint16_t parameter_size = 256;
const uint8_t preset_number = 12;
int16_t default_bank_sysex_parameters[preset_number][parameter_size] = {
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
int16_t current_sysex_parameters[parameter_size] = {0, 0, 50, 50, 50, 38, 0, 0, 0, 0, 40, 35, 64, 32, 40, 0, 40, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 9, 5, 10, 10, 80, 2000, 10, 75, 30, 140, 100, 160, 90, 25, 75, 700, 15, 80, 0, 300, 0, 100, 0, 500, 0, 0, 40, 200, 35, 40, 200, 35, 10, 0, 700, 100, 50, 0, 0, 100, 0, 30, 50, 100, 40, 836, 80, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 11, 100, 35, 0, 200, 25, 0, 100, 4, 50, 50, 50, 50, 0, 0, 1, 100, 100, 16, 0, 10, 30, 25, 80, 600, 4, 0, 300, 0, 100, 15, 20, 30, 80, 325, 10, 450, 54, 100, 150, 0, 300, 0, 0, 100, 0, 200, 0, 1, 0, 5, 45, 400, 75, 100, 10, 0, 0, 675, 100, 30, 0, 0, 100, 0, 43, 0, 36, 52, 33, 699, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
// >10 and <20 are limited access, for example the potentiometer settings (we don't want a pot to control another pot)
int8_t chord_pot_alternate_control = 10;
int8_t chord_pot_alternate_range = 11;
int8_t harp_pot_alternate_control = 12;
int8_t harp_pot_alternate_range = 13;
int8_t mod_pot_main_control = 14;
int8_t mod_pot_main_range = 15;
int8_t mod_pot_alternate_control = 16;
int8_t mod_pot_alternate_range = 17;
// 20-39 are global parameters (switching logic, global reverb etc.)
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
// for the chord
AudioEffectEnvelope *chord_vibrato_envelope_array[4] = {&voice1_vibrato_envelope, &voice2_vibrato_envelope, &voice3_vibrato_envelope, &voice4_vibrato_envelope};
AudioSynthWaveformModulated *chord_osc_1_array[4] = {&voice1_osc1, &voice2_osc1, &voice3_osc1, &voice4_osc1};
AudioSynthWaveformModulated *chord_osc_2_array[4] = {&voice1_osc2, &voice2_osc2, &voice3_osc2, &voice4_osc2};
AudioSynthWaveformModulated *chord_osc_3_array[4] = {&voice1_osc1, &voice2_osc3, &voice3_osc3, &voice4_osc3};
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
// shuffling arrays and index for the harp
int8_t harp_shuffling_array[6][12] = {
    //each number indicates the note for the string 0-6 are taken within the chord pattern. 
    //the /10 number indicates the octave
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32},
    {4, 1, 0, 2, 14, 11, 10, 12, 24, 21, 20, 22},
    {5, 2, 0, 1, 15, 12, 10, 11, 25, 22, 20, 21},
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32},
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32},
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32}};
int8_t harp_shuffling_selection = 0;
int8_t chord_shuffling_array[6][7] = {
    //each number indicates the note for the string 0-6 are taken within the chord pattern. 
    //the /10 number indicates the octave
    {0, 1, 2, 3, 4, 5, 6},
    {0, 1, 2, 3, 10, 12, 15},
    {10, 11, 12, 13, 2, 5, 6},
    {0, 1, 2, 3, 4, 5, 6},
    {0, 1, 2, 3, 4, 5, 6},
    {0, 1, 2, 3, 4, 5, 6}};
int8_t chord_shuffling_selection = 0;
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

//>>AUTO RYTHM<<
u_int8_t rythm_pattern[16] = {};
u_int16_t rythm_bpm = 80;
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

uint8_t rythm_limit_change_to_every = 2; // when we allow the chord change
elapsedMillis since_last_button_push;
bool note_change_flag[7] = {false, false, false, false, false, false, false};
uint8_t rythm_freeze_current_chord_notes[7]; // this array is needed because we need to handle the situation when a
uint8_t rythm_loop_length = 16;
u_int8_t current_selected_voice=0; //increment at each voice steal to rotate amongst voices;

//-->>FUNCTION THAT NEED ANNOUNCING
void save_config(int bank_number, bool default_save);
void load_config(int bank_number);
void recalculate_timer();
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp);
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp);

void calculate_ws_array();

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
  analogWrite(R_LED_PIN, r * 255);
  analogWrite(G_LED_PIN, g * 115);
  analogWrite(B_LED_PIN, b * 70);
  return;
}

//-->>UTILITIES FOR SYSEX HANDLING
void control_command(uint8_t command, uint8_t parameter) {
  switch (command) {
  case 0: // SIGNAL TO SEND BACK ALL DATA
    Serial.println("Reporting all data");
    int8_t midi_data_array[parameter_size * 2];
    for (int i = 0; i < parameter_size; i++) {
      midi_data_array[2 * i] = current_sysex_parameters[i] % 128;
      midi_data_array[2 * i + 1] = current_sysex_parameters[i] / 128;
    }
    usbMIDI.sendSysEx(parameter_size * 2, (const uint8_t *)&midi_data_array);
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

  default:
    break;
  }
}
// the autogenerated code (see ./generator for the script)
#include <sysex_handler.h>
void processMIDI(void) {
  byte type;
  type = usbMIDI.getType();
  if (type == usbMIDI.SystemExclusive && usbMIDI.getSysExArrayLength() == 6) {
    sysex_controler_connected=true; //we can say for sure a controller is connected
    const byte *data = usbMIDI.getSysExArray();
    int adress = data[1] + 128 * data[2];
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

//-->>TIMER FUNCTIONS
// function to handle the delayed chord activation
void blink_color_led() {
  set_led_color(bank_led_hue, 1.0, color_led_blink_state ? 1.0 : 0.2);
  color_led_blink_state = !color_led_blink_state;
}
void play_single_note(int i, IntervalTimer *timer) {
  timer->end();
  chord_vibrato_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
}

void play_note_selected_duration(int i){
  chord_vibrato_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  note_off_timing[i]=0;
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
void set_chord_voice_frequency(uint8_t i, uint16_t current_note) {
  float note_freq = c_frequency/2.0 * pow(2, (current_note) / 12.0); //down one octave to let more possibilities with the shuffling array
  AudioNoInterrupts();
  chords_vibrato_lfo.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
  chords_tremolo_lfo.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
  // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
  // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
  chord_voice_filter_array[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
  chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * note_freq);
  chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * note_freq);
  chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * note_freq);
  // chord_voice_filter_array[i]->frequency(1*freq);
  AudioInterrupts();
}
// setting the harp
void set_harp_voice_frequency(uint8_t i, uint16_t current_note) {
  float note_freq = c_frequency * pow(2, (current_note) / 12.0);
  AudioNoInterrupts();
  string_waveform_array[i]->frequency(note_freq);
  string_filter_array[i]->frequency(string_filter_base_freq + note_freq * string_filter_keytrack);
  // string_vibrato_1.offset(0);
  AudioInterrupts();
}
// function to calculate the frequency of individual chord notes
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp) {
  uint8_t note = 0;
  uint8_t level = chord_shuffling_array[chord_shuffling_selection][voice];
  // only slash the selected level of the chord (note, will be ignored if >2)
  if (slashed && level % 10 == note_slash_level) {
    note = (12 * int(level / 10) + float(root_button[slash_value]) + sharp * 1.0);
  } else {
    note = (12 * int(level / 10) + float(root_button[fundamental]) + sharp * 1.0 + float((*current_chord)[level % 10]));
  }
  return note;
}
// function to calculate the level of individual harp touch
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp) {
  uint8_t note = 0;
  uint8_t level = harp_shuffling_array[harp_shuffling_selection][string];
  // only slash the selected level of the chord (note, will be ignored if >2)
  if (slashed && level % 10 == note_slash_level) {
    note = (12 * int(level / 10) + float(root_button[slash_value]) + sharp * 1.0);
  } else {
    note = (12 * int(level / 10) + float(root_button[fundamental]) + sharp * 1.0 + float((*current_chord)[level % 10]));
  }
  return note;
}

//-->>RYTHM MODE UTILITIES
void rythm_tick_function() {
  if (rythm_current_step % rythm_limit_change_to_every == 0) {
    for (int i = 0; i < 7; i++) {
      rythm_freeze_current_chord_notes[i] = current_applied_chord_notes[i];
      note_change_flag[i] = true;
    }
  }
  // handling the led pattern
  uint8_t active_modulus = 1;
  uint8_t possible_pattern[4] = {3, 2};
  for (uint8_t i = 0; i < sizeof(possible_pattern) / sizeof(uint8_t); i++) {
    if (rythm_loop_length % possible_pattern[i] == 0) {
      active_modulus = possible_pattern[i];
      break;
    }
  }
  analogWrite(RYTHM_LED_PIN, (220 * (rythm_current_step % rythm_limit_change_to_every == 0) + 15) * (rythm_current_step % active_modulus == 0));
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
  for (int i = 0; i < 7; i++) {
    if (result & (1 << i)) {
      if (note_change_flag[i] == true) {
        set_chord_voice_frequency(current_selected_voice, rythm_freeze_current_chord_notes[i]);
        note_change_flag[i] = false;
      }
      play_note_selected_duration(current_selected_voice);
      current_selected_voice=(current_selected_voice+1)%4;
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

void save_config(int bank_number, bool default_save) {
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
    String return_data = serialize(default_bank_sysex_parameters[bank_number], parameter_size);
    dataFile.println(return_data);
  } else {
    Serial.println("Saving current settings");
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
  digitalWrite(_MUTE_PIN, LOW); // muting the DAC
  File entry = myfs.open(bank_name[bank_number]);
  if (entry) {
    String data_string = "";
    while (entry.available()) {
      data_string += char(entry.read());
    }
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
  control_command(0, 0); // tell itself to update the remote controller if present
  chord_pot.force_update();
  harp_pot.force_update();
  mod_pot.force_update();
  digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
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
  


  // the delay passthrough
  string_delay_mix.gain(0, 1);
  chord_delay_mix.gain(0, 1);
  // simple mixers
  for (int i = 0; i < 3; i++) {
    string_mixer_array[i]->gain(0, 0.25);
    string_mixer_array[i]->gain(1, 0.25);
    string_mixer_array[i]->gain(2, 0.25);
    string_mixer_array[i]->gain(3, 0.25);
  }
  for (int i = 0; i < 4; i++) {
    chord_voice_mixer_array[i]->gain(0, 0.25);
    chord_voice_mixer_array[i]->gain(1, 0.25);
    chord_voice_mixer_array[i]->gain(2, 0.25);
    chord_noise_array[i]->amplitude(0.5);
  }
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
  Serial.println("Initialisation complete");
}

void loop() {
  //>>Looking for incoming midi message
  if (usbMIDI.read()) {
    processMIDI();
  }
  //>>Updating the debouncers
  hold_button.set(digitalRead(HOLD_BUTTON_PIN));
  up_button.set(digitalRead(UP_PGM_PIN));
  down_button.set(digitalRead(DOWN_PGM_PIN));
  LBO_flag.set(digitalRead(BATT_LBO_PIN));
  chord_matrix.update(chord_matrix_array);
  //>>handling low battery blink indicator
  uint8_t LBO_transition = LBO_flag.read_transition();
  if (LBO_transition == 1) {
    color_led_blink_timer.priority(200);
    color_led_blink_timer.begin(blink_color_led, 100000);
  } else if (LBO_transition == 2) {
    color_led_blink_timer.end();
    set_led_color(bank_led_hue, 1.0, 1.0);
  }
  //>>Handlind the hold button functions
  uint8_t hold_transition = hold_button.read_transition();
  if (hold_transition == 2) {
    if (!rythm_mode) {
      Serial.println("Switching mode");
      continuous_chord = !continuous_chord;
      analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
      trigger_chord = true;
    } else {
      //>>push tempo management
      if (since_last_button_push > 100 && since_last_button_push < 2000) {  // check that we are inside the BPM range
        rythm_bpm = (rythm_bpm + 60 * 1000 / (since_last_button_push)) / 2; // we push for full note
        Serial.print("Updating the BPM to:");
        Serial.println(rythm_bpm);
        recalculate_timer();
        if (current_long_period) {
          rythm_timer.update(long_timer_period);
        } else {
          rythm_timer.update(short_timer_period);
        }
      }
    }
    since_last_button_push = 0;
  }
  //>>Handling the long hold to switch to rythm mode
  if (hold_transition == 1) {
    if (since_last_button_push > 800) {
      Serial.println("Long push, switching rythm mode");
      rythm_mode = !rythm_mode;
      continuous_chord = false;
      analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
      if (rythm_mode) {
        for (int i = 0; i < 7; i++) {
          current_applied_chord_notes[i] = current_chord_notes[i];
          rythm_freeze_current_chord_notes[i] = current_chord_notes[i]; // circulate the notes to the right buffers
        }
      }
    }
  }

  //>>Handling the preset change interface
  if (up_button.read_transition() > 1) {
    Serial.println("Switching to next preset");
    if(!sysex_controler_connected){
      save_config(current_bank_number, false); // saving to remember alternate pot position when not connnected
    }    current_bank_number = (current_bank_number + 1) % 12;
    load_config(current_bank_number);
  }
  if (down_button.read_transition() > 1) {
    Serial.println("Switching to last preset");
    if(!sysex_controler_connected){
      save_config(current_bank_number, false); // saving to remember alternate pot position when not connnected
    }
    current_bank_number = (current_bank_number - 1);
    if (current_bank_number == -1) {
      current_bank_number = 11;
    }
    load_config(current_bank_number);
  }

  //>>Handling the rythm mode timers start and stop
  if (!rythm_timer_running && rythm_mode) {
    Serial.println("Starting rythm timers");
    rythm_timer.begin(rythm_tick_function, short_timer_period);
    rythm_timer_running = true;
    rythm_timer.update(long_timer_period);
    current_long_period = true;
  }
  if (rythm_timer_running && !rythm_mode) {
    Serial.println("Stopping rythm timers");
    rythm_timer.end();
    rythm_timer_running = false;
  }
  //>>Handling the turning off of notes in rythm mode (mandatory because we are missing one timer to do it cleanly)
  if(rythm_mode){
    for (int i = 0; i < 4; i++) {
      if(note_off_timing[i]>note_pushed_duration){
        chord_vibrato_envelope_array[i]->noteOff();
        chord_envelope_array[i]->noteOff();
        chord_envelope_filter_array[i]->noteOff();
      }
    }
  }

  //>>Handling the potentiometer mode
  bool alternate = chord_matrix_array[0].read_value(); // use the sharp as potentiometer alt selection
  chord_pot.update_parameter(alternate);
  harp_pot.update_parameter(alternate);
  mod_pot.update_parameter(alternate);

  //>>Handling of chords logic
  // If not button is active in touch mode, then turn everything off
  if (!continuous_chord && !rythm_mode) {
    bool one_button_active = false;
    for (int i = 1; i < 22; i++) {
      one_button_active = one_button_active || chord_matrix_array[i].read_value();
    }
    if (!(one_button_active)) {
      AudioNoInterrupts();
      for (int i = 0; i < 4; i++) {
        chord_vibrato_envelope_array[i]->noteOff();
        chord_envelope_array[i]->noteOff();
        chord_envelope_filter_array[i]->noteOff();
      }
      AudioInterrupts();
    }
  }
  // If there is a line currently active, then start the update logic
  if (current_line >= 0) {
    fundamental = current_line; // this is our active line
    slash_chord = false;
    // let's check if we have a slashed chord and if that's the case, which
    for (int i = 1; i < 22; i++) {
      bool value = chord_matrix_array[i].read_value();
      if (value) {
        int slash_line = (i - 1) / 3;
        if (slash_line != current_line) {
          slash_chord = true;
          slash_value = slash_line;
        }
      }
    }
    // detect which buttons are active within our line
    bool button_maj = chord_matrix_array[1 + current_line * 3].read_value();
    bool button_min = chord_matrix_array[2 + current_line * 3].read_value();
    bool button_seventh = chord_matrix_array[3 + current_line * 3].read_value();
    if (!(button_maj || button_seventh || button_min)) {
      current_line = -1; // if no button is, then line is no more active. we get out of that loop

    } else {
      // depending on the active button identify the current chord
      if (button_maj && !button_min && !button_seventh) {
        current_chord = &major;
      }
      if (!button_maj && button_min && !button_seventh) {
        current_chord = &minor;
      }
      if (!button_maj && !button_min && button_seventh) {
        current_chord = &seventh;
      }
      if (button_maj && !button_min && button_seventh) {
        current_chord = &maj_seventh;
      }
      if (!button_maj && button_min && button_seventh) {
        current_chord = &min_seventh;
      }
      if (button_maj && button_min && !button_seventh) {
        current_chord = &dim;
      }
      if (button_maj && button_min && button_seventh) {
        current_chord = &aug;
      }
      // We can now calculate the target notes
      for (int i = 0; i < 7; i++) {
        current_chord_notes[i] = calculate_note_chord(i, slash_chord, sharp_active);
      }
      // But that target should only be applied if we have an action from the user: the push of a button
      if (button_pushed) {
        Serial.println("Updating frequences");
        if (!rythm_mode) { // if we are not in rythm mode we can then directly apply the frequency
          for (int i = 0; i < 4; i++) {
            set_chord_voice_frequency(i, current_chord_notes[i]);
          }
        } else { // but in rythm mode it depends on were we are on the loop. We push it to a buffer
          for (int i = 0; i < 7; i++) {
            current_applied_chord_notes[i] = current_chord_notes[i];
          }
        }
        for (int i = 0; i < 12; i++) { // In any case we update the harp frequency
          current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
          if (change_held_strings) {
            for (int i = 0; i < 12; i++) {
              if (string_enveloppe_array[i]->isSustain()) { // change the frequency if we are in the sustain part
                set_harp_voice_frequency(i, current_harp_notes[i]);
              }
            }
          }
        }
      }
      if ((trigger_chord || (button_pushed && retrigger_chord)) && !rythm_mode) { // if there is a explicit signal to trigger the enveloppe, or we are in a situation where trigger is needed, we do it
        note_timer[0].begin([] { play_single_note(0, &note_timer[0]); }, 10);          // those allow for delayed triggering
        note_timer[1].begin([] { play_single_note(1, &note_timer[1]); }, 10 + inter_string_delay + random(random_delay));
        note_timer[2].begin([] { play_single_note(2, &note_timer[2]); }, 10 + inter_string_delay * 2 + random(random_delay));
        note_timer[3].begin([] { play_single_note(3, &note_timer[3]); }, 10 + inter_string_delay * 3 + random(random_delay));
        trigger_chord = false;
      }
      button_pushed = false; // in any case after that loop, we can reset button pushed
    }
  }
  // Now let's read the button transitions
  int sharp_transition = chord_matrix_array[0].read_transition(); // first the sharp
  if (sharp_transition > 1 && current_line != -1) {               // want to trigger the button pushed, only if buttons are selected
    button_pushed = true;
  }
  sharp_active = chord_matrix_array[0].read_value(); // in any case we record the current value
  for (int i = 1; i < 22; i++) {                     // now the rest of the chord buttons
    int value = chord_matrix_array[i].read_transition();
    if (value > 1) { // a button was indeed pushed
      button_pushed = true;
      if (current_line == -1) {
        current_line = (i - 1) / 3; // if no line is currently active, we have a new base line
        if (!continuous_chord) {
          trigger_chord = true;
        }
      }
    }
  }

  //>>Handling the harp functions, once the frequency array is defined the the chords
  harp_sensor.update(harp_array);
  for (int i = 0; i < 12; i++) {
    int value = harp_array[i].read_transition();
    if (value > 1) {
      set_harp_voice_frequency(i, current_harp_notes[i]);
      AudioNoInterrupts();
      envelope_string_vibrato_1.noteOn();
      string_enveloppe_filter_array[i]->noteOn();
      string_enveloppe_array[i]->noteOn();
      AudioInterrupts();
    }
    value = harp_array[i].read_value(); //weirdly if we use the debouncer some noteOff seems to be ignored
    if (value == 0) {
      AudioNoInterrupts();
      string_enveloppe_filter_array[i]->noteOff();
      string_enveloppe_array[i]->noteOff();
      AudioInterrupts();
    }
  }
}
