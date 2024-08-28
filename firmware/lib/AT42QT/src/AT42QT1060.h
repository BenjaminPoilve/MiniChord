// ----------------------------------------------------------------------------
// AT42QT1060.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AT42QT1060_H
#define AT42QT1060_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT1060 : public AT42QT<RegisterAddresses::AT42QT1060>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x12;
  static const uint8_t CHIP_ID = 0x31;
  static const uint8_t KEY_COUNT = 6;

  AT42QT1060(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT1060>(DEVICE_ADDRESS,
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
      uint16_t keys : 6;
      uint16_t space0 : 1;
      uint16_t calibrating : 1;
      uint16_t inputs : 7;
      uint16_t space1 : 1;
    };
    uint16_t bytes;
  };
  static const uint8_t STATUS_SIZE = 2;
  Status getStatus();
  bool calibrating();

  bool anyTouched(Status status);
  bool touched(Status status,
    uint8_t key);

  static const uint8_t ENABLE_RELATIVE_DRIFT_COMPENSATION = 0;
  static const uint8_t DISABLE_RELATIVE_DRIFT_COMPENSATION = 1;
  // default = disabled
  void enableRelativeDriftCompensation();
  void disableRelativeDriftCompensation();

  // default = 40
  uint8_t getRecalibrationDelay();
  void setRecalibrationDelay(uint8_t rd);

  // default = 10
  // do not use zero as this causes a key to go into detection when
  // its signal is equal to its reference
  uint8_t getKeyDetectThreshold(uint8_t key);
  void setKeyDetectThreshold(uint8_t key,
    uint8_t threshold);

  // each interval is 16ms
  // an interval_count of 4 equals 16ms*4=64ms between measurements
  // power down device by writing a zero interval_count
  // wake device by resetting or writing a nonzero interval_count
  // longer intervals yield lower power consumption
  // default = 2
  uint8_t getMeasurementIntervalCount();
  void setMeasurementIntervalCount(uint8_t interval_count);

  union IOMask
  {
    struct
    {
      uint8_t io_0 : 1;
      uint8_t io_1 : 1;
      uint8_t io_2 : 1;
      uint8_t io_3 : 1;
      uint8_t io_4 : 1;
      uint8_t io_5 : 1;
      uint8_t io_6 : 1;
      uint8_t space : 1;
    };
    uint8_t bytes;
  };
  static const uint8_t IO_OUTPUT = 1;
  static const uint8_t IO_INPUT = 0;
  IOMask getIOMask();
  void setIOMask(IOMask mask);

  union KeyMask
  {
    struct
    {
      uint8_t key_0 : 1;
      uint8_t key_1 : 1;
      uint8_t key_2 : 1;
      uint8_t key_3 : 1;
      uint8_t key_4 : 1;
      uint8_t key_5 : 1;
      uint8_t space : 1;
      uint8_t calibrating : 1;
    };
    uint8_t bytes;
  };
  static const uint8_t ENABLES_CHANGE_TRANSITION = 1;
  static const uint8_t DISABLES_CHANGE_TRANSITION = 0;
  KeyMask getKeyMask();
  void setKeyMask(KeyMask mask);

  union AdjacentKeySuppressionMask
  {
    struct
    {
      uint8_t key_0 : 1;
      uint8_t key_1 : 1;
      uint8_t key_2 : 1;
      uint8_t key_3 : 1;
      uint8_t key_4 : 1;
      uint8_t key_5 : 1;
      uint8_t space : 2;
    };
    uint8_t bytes;
  };
  static const uint8_t INCLUDE_IN_ADJACENT_KEY_SUPPRESSION = 1;
  static const uint8_t EXCLUDE_FROM_ADJACENT_KEY_SUPPRESSION = 0;
  AdjacentKeySuppressionMask getAdjacentKeySuppressionMask();
  void setAdjacentKeySuppressionMask(AdjacentKeySuppressionMask mask);

  union PWMMask
  {
    struct
    {
      uint8_t io_0 : 1;
      uint8_t io_1 : 1;
      uint8_t io_2 : 1;
      uint8_t io_3 : 1;
      uint8_t io_4 : 1;
      uint8_t io_5 : 1;
      uint8_t io_6 : 1;
      uint8_t space : 1;
    };
    uint8_t bytes;
  };
  static const uint8_t ENABLE_PWM = 1;
  static const uint8_t DISABLE_PWM = 0;
  PWMMask getPWMMask();
  void setPWMMask(PWMMask mask);

  union DetectionMask
  {
    struct
    {
      uint8_t io_0 : 1;
      uint8_t io_1 : 1;
      uint8_t io_2 : 1;
      uint8_t io_3 : 1;
      uint8_t io_4 : 1;
      uint8_t io_5 : 1;
      uint8_t io_6 : 1;
      uint8_t space : 1;
    };
    uint8_t bytes;
  };
  static const uint8_t ENABLE_DETECTION = 1;
  static const uint8_t DISABLE_DETECTION = 0;
  DetectionMask getDetectionMask();
  void setDetectionMask(DetectionMask mask);
};

#endif
