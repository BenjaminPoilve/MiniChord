// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace constants
{
const long baud = 115200;

TwoWire * wire_ptr = &Wire;

const MPR121::DeviceAddress device_address = MPR121::ADDRESS_5A;
const bool fast_mode = true;

const size_t loop_delay = 1000;

const uint8_t channel = 0;

const uint8_t physical_channel_count = 2;
const MPR121::ProximityMode proximity_mode = MPR121::COMBINE_CHANNELS_0_TO_1;

const uint8_t touch_threshold = 40;
const uint8_t release_threshold = 20;
const uint8_t touch_debounce = 1;
const uint8_t release_debounce = 1;

const MPR121::BaselineTracking baseline_tracking = MPR121::BASELINE_TRACKING_INIT_10BIT;
const uint8_t charge_discharge_current = 63;
const MPR121::ChargeDischargeTime charge_discharge_time = MPR121::CHARGE_DISCHARGE_TIME_HALF_US;
const MPR121::FirstFilterIterations first_filter_iterations = MPR121::FIRST_FILTER_ITERATIONS_34;
const MPR121::SecondFilterIterations second_filter_iterations = MPR121::SECOND_FILTER_ITERATIONS_10;
const MPR121::SamplePeriod sample_period = MPR121::SAMPLE_PERIOD_1MS;
}
