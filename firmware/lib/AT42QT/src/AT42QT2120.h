// ----------------------------------------------------------------------------
// AT42QT2120.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AT42QT2120_H
#define AT42QT2120_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT2120 : public AT42QT<RegisterAddresses::AT42QT2120>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x1C;
  static const uint8_t CHIP_ID = 0x3E;
  static const uint8_t KEY_COUNT = 12;

  AT42QT2120(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT2120>(DEVICE_ADDRESS,
    CHIP_ID,
    KEY_COUNT,
    wire,
    change_pin,
    reset_pin)
  {
  }

  union Status
  {
    struct
    {
      uint32_t any_key_touched : 1;
      uint32_t slider_or_wheel : 1;
      uint32_t space0 : 4;
      uint32_t overflow : 1;
      uint32_t calibrating : 1;
      uint32_t keys: 12;
      uint32_t space1 : 4;
      uint32_t slider_or_wheel_position : 8;
    };
    uint32_t bytes;
  };
  static const uint8_t STATUS_SIZE = 4;
  Status getStatus();
  bool calibrating();

  bool anyTouched(Status status);
  bool touched(Status status,
    uint8_t key);

  // each interval is 16ms
  // an interval_count of 4 equals 16ms*4=64ms between measurements
  // power down device by writing a zero interval_count
  // wake device by resetting or writing a nonzero interval_count
  // longer intervals yield lower power consumption
  // default = 1
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);

  // increments of 0.16s
  // should be more than four times the measurement interval count
  // default = 20
  static const uint8_t DRIFT_COMPENSATION_DURATION_MAX = 127;
  uint8_t getTowardsDriftCompensationDuration();
  void setTowardsDriftCompensationDuration(uint8_t tdd);

  // increments of 0.16s
  // should be more than four times the measurement interval count
  // default = 5
  uint8_t getAwayDriftCompensationDuration();
  void setAwayDriftCompensationDuration(uint8_t add);

  // number of consecutive measurements that must be confirmed as having passed
  // the key threshold value before that key is registered as being in detect
  // default = 4
  static const uint8_t DETECTION_INTEGRATOR_MAX = 32;
  uint8_t getDetectionIntegrator();
  void setDetectionIntegrator(uint8_t di);

  // increments of 0.16s
  // automatic recalibration delay after extended unintentional touch
  // default = 255
  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t rd);

  // increments of 0.16s
  // default = 25
  uint8_t getDriftCompensationHoldDuration();
  void setDriftCompensationHoldDuration(uint8_t dhd);

  union SliderOrWheelEnable
  {
    struct
    {
      uint8_t space0 : 6;
      uint8_t wheel : 1;
      uint8_t enable : 1;
    };
    uint8_t bytes;
  };
  // when slider or wheel is enabled only channels 0,1,2 can be used
  // default = disabled
  bool sliderOrWheelEnabled();
  void disableSliderAndWheel();
  void enableSlider();
  void enableWheel();

  // prolongs the charge-transfer period of signal acquisition
  // allows full charge-transfer for keys with heavy Rs/Cx loading
  // increments of 1us
  // default = 0
  uint8_t getChargeDuration();
  void setChargeDuration(uint8_t cd);

  // default = 10
  // do not use zero as this causes a key to go into detection when
  // its signal is equal to its reference
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key,
    uint8_t threshold);

  union KeyControl
  {
    struct
    {
      uint8_t enable_key_output : 1;
      uint8_t key_output : 1;
      uint8_t adjacent_key_suppression_group : 2;
      uint8_t guard : 1;
      uint8_t space : 3;
    };
    uint8_t bytes;
  };
  KeyControl getKeyControl(uint8_t key);
  void setKeyControl(uint8_t key,
    KeyControl key_control);

  union KeyPulseScale
  {
    struct
    {
      uint8_t scale : 4;
      uint8_t pulse : 4;
    };
    uint8_t bytes;
  };
  KeyPulseScale getKeyPulseScale(uint8_t key);
  void setKeyPulseScale(uint8_t key,
    KeyPulseScale key_pulse_scale);

  uint16_t getKeySignal(uint8_t key);
  uint16_t getKeyReference(uint8_t key);
};

#endif
