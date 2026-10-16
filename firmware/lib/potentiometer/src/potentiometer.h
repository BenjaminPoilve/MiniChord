#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Arduino.h" 
#include <functional>
#include "parameter_lookup.h"


class potentiometer{
  private:
  std::function<void(int, int)> apply_audio_parameter;
  public:
  potentiometer(int pot_pin);
  void setup(int main_adress,float main_range,int alternate_adress,float alternate_range, int16_t *current_sysex_parameters_pointer, int alternate_initial_value, const std::function<void(int, int)> & apply_audio_parameter, int alternate_storage_adress);
  void set_main(int adress);
  void set_main_range(float range);
  void set_alternate(int adress);
  void set_alternate_range(float range);
  void set_alternate_default(int alternate_initial_value);
  void force_update();
  bool update_parameter(bool alternate_flag); //the flag tells us whether the modifier button was pushed
  // Pickup: the next switch between main and alternate leaves the newly active function where it
  // was until the knob reaches the position it was last set from (or crosses it), so the function
  // doesn't jump to wherever the knob happens to be. Knob layer asks for it; holding the modifier
  // doesn't, and behaves as it always has.
  void pickup_on_next_switch();
  private:
  //Memory variable
  int potentiometer_smoothed_value=0; //smoothing of the analog reading
  int potentiometer_old_value=0; //old value to detect if we meet threshold
  int alternate_value=0;
  int main_position=-1;       // the reading the main function was last set from (-1: not yet)
  bool last_alternate=false;  // which function was active on the last update
  bool pickup_next=false;     // the next switch waits for pickup
  bool waiting=false;         // the active function is waiting for the knob to reach it
  int8_t waiting_side=0;      // which side of that position the knob was on when it started waiting
  
  uint16_t old_main_output=0;
  uint16_t old_alternate_output=0;

  //The Potentiometer target values 
  int main_adress;
  float main_range;
  int alternate_adress;
  float alternate_range;
  int alternate_storage_adress;
  int old_main_adress_value=0;
  //Potentiometer hardware parameters
  int pot_pin;
  int16_t dead_zone=20;
  int16_t threshold=20;
    // Discrete (integer) targets are mapped across the bounds declared in
    // parameters.json rather than scaled around the stored value.
    bool declared_bounds(int adress, int16_t &min_out, int16_t &max_out);
    int16_t quantise(int16_t proposed, int16_t lo, int16_t hi, int adress);
    int16_t last_discrete_output = 0;  // last value emitted for a discrete target
    int16_t last_discrete_reading = 0; // smoothed reading when it was latched
    int last_discrete_adress = -1;     // target the latch refers to

  //To access initial value
  int16_t *current_sysex_parameters_pointer;
};

#endif