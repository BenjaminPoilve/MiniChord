// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>
#include <MPR121.h>


namespace constants
{
extern const long baud;

extern TwoWire * wire_ptr;

extern const MPR121::DeviceAddress device_address;
extern const bool fast_mode;

extern const size_t loop_delay;

extern const uint8_t channel;

extern const uint8_t physical_channel_count;
extern const MPR121::ProximityMode proximity_mode;

extern const uint8_t touch_threshold;
extern const uint8_t release_threshold;
extern const uint8_t touch_debounce;
extern const uint8_t release_debounce;

extern const MPR121::BaselineTracking baseline_tracking;
extern const uint8_t charge_discharge_current;
extern const MPR121::ChargeDischargeTime charge_discharge_time;
extern const MPR121::FirstFilterIterations first_filter_iterations;
extern const MPR121::SecondFilterIterations second_filter_iterations;
extern const MPR121::SamplePeriod sample_period;

}
#endif
