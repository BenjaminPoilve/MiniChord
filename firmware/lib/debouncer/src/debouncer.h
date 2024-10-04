#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include "Arduino.h" 

class debouncer{
  public:
  debouncer();
  void set(bool set_value);
  uint8_t read_transition();
  bool read_value();
  private:
  u_int16_t debounce_value=1000;
  bool flag=false; //flag warns that there has been a change in value that was not yet accounted for 
  elapsedMicros last_update=0;
  bool value=false;
};

#endif