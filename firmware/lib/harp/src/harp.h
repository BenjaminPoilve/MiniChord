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
  // Hover: the chip's thirteenth channel reads all twelve pads together as one
  // large antenna, which feels a hand above the plate before it lands. Its
  // filtered reading, which falls as a hand comes near; 0 if there is none. The
  // chip's own baseline is only kept to four counts, coarser than a hand a few
  // centimetres away moves it, so the baseline is the caller's to keep.
  uint16_t read_proximity();
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
    // The twelve pads read together hold about twelve times one pad's charge,
    // so the half microsecond that suits a pad leaves them far below the chip's
    // range. They get a charge of their own, found at calibration.
    static const uint8_t proximity_channel = 12;
    void set_proximity_charge();
  #endif


};

#endif