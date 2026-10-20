#ifndef HARP_H
#define HARP_H
#include "Arduino.h" 
//1=ATQT2120, 0=MPR121
#define CAP_CHIP 0

#if CAP_CHIP==1
  #include <AT42QT2120.h>
#else
  #include <MPR121.h>
#endif
#include <debouncer.h>

class harp{
  public:
  harp();

  void setup();
  void recalibrate();
  void update(debouncer (&data_array)[12]);

  private:
  #if CAP_CHIP==1
    AT42QT2120 touch_sensor;
    int remap_array[12]={3,4,5,6,7,8,9,10,11,2,1,0};
    int calibrated_val_array[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    int threshold=12;
    float hysteresis=1.6;
  #else
    MPR121 touch_sensor;
    int remap_array[12]={11,9,7,6,8,10,0,2,4,5,3,1};
    //int remap_array[12]={1,3,5,4,2,0,10,8,7,11,9,6};
    const uint8_t touch_threshold = 40;
    const uint8_t release_threshold = 20;
    const uint8_t touch_debounce = 1;
    const uint8_t release_debounce = 1;
    const MPR121::BaselineTracking baseline_tracking = MPR121::BASELINE_TRACKING_INIT_10BIT;
    const uint8_t charge_discharge_current = 63;
    const MPR121::ChargeDischargeTime charge_discharge_time = MPR121::CHARGE_DISCHARGE_TIME_HALF_US;
    const MPR121::FirstFilterIterations first_filter_iterations = MPR121::FIRST_FILTER_ITERATIONS_6;
    const MPR121::SecondFilterIterations second_filter_iterations = MPR121::SECOND_FILTER_ITERATIONS_4;
    const MPR121::SamplePeriod sample_period = MPR121::SAMPLE_PERIOD_1MS;

    // A plate taken off or put on with the power on, waited out and recalibrated (harp.cpp)
    bool swap_watch(uint16_t touch_status);
    void swap_begin(const char *why, const uint16_t (&filtered)[12], const uint16_t (&baseline)[12], bool coming_off);
    const uint16_t swap_poll_ms = 20;
    const int16_t swap_lift = 15;          // counts above rest, on swap_zones zones: a plate coming off
    const int16_t swap_drop = 30;          // counts under rest, on swap_zones zones of bare pins: a plate going on
    const uint8_t swap_zones = 10;
    const uint8_t swap_left_zones = 3;     // zones left above their rest, untouched ...
    const uint16_t swap_left_ms = 300;     // ... this long: the rest of a plate coming off
    const uint16_t swap_stuck_ms = 3000;   // all twelve held and still this long: a plate gone on unseen
    const uint8_t swap_steady_counts = 4;  // still, to within this many counts ...
    const uint16_t swap_settle_ms = 300;   // ... this long: settled, recalibrated there
    const uint16_t swap_after_ms = 60;     // after recalibrating, how long the zones still read as untouched
    const int16_t swap_bare_counts = 20;   // counts a zone, on average, a settled swap reads above the plate before it: bare pins
    enum SwapState { SWAP_NONE, SWAP_SETTLING, SWAP_AFTER };
    SwapState swap_state = SWAP_NONE;
    elapsedMillis swap_since_poll, swap_steady_for, swap_since_after, swap_all_still, swap_left_for;
    uint16_t swap_lo[12], swap_hi[12], swap_still_lo[12], swap_still_hi[12];
    bool swap_still_seed = true;
    bool swap_coming_off = false;          // this swap began as a plate coming off
    bool swap_bare = false;                // the last swap settled with no plate on
    bool swap_stuck = false;               // this swap began as all twelve held still ...
    bool swap_after_stuck = false;         // ... and the last one did
    int32_t swap_before = 0;               // the readings' sum before a plate came off (its rest)
    int32_t swap_bare_sum = 0;             // the readings' sum on the bare pins, once a swap has settled there
  #endif


};

#endif