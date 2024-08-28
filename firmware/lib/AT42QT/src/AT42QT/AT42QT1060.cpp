// ----------------------------------------------------------------------------
// AT42QT1060.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AT42QT1060.h"


AT42QT1060::Status AT42QT1060::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT1060::STATUS,status.bytes,STATUS_SIZE);
  return status;
}

bool AT42QT1060::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

bool AT42QT1060::anyTouched(Status status)
{
  return status.keys;
}

bool AT42QT1060::touched(Status status,
  uint8_t key)
{
  return bitRead(status.keys,key);
}

void AT42QT1060::enableRelativeDriftCompensation()
{
  write(RegisterAddresses::AT42QT1060::DRIFT_OPTION,ENABLE_RELATIVE_DRIFT_COMPENSATION);
}

void AT42QT1060::disableRelativeDriftCompensation()
{
  write(RegisterAddresses::AT42QT1060::DRIFT_OPTION,DISABLE_RELATIVE_DRIFT_COMPENSATION);
}

uint8_t AT42QT1060::getRecalibrationDelay()
{
  uint8_t rd;
  read(RegisterAddresses::AT42QT1060::RECALIBRATION_DELAY,rd);
  return rd;
}

void AT42QT1060::setRecalibrationDelay(uint8_t rd)
{
  write(RegisterAddresses::AT42QT1060::RECALIBRATION_DELAY,rd);
}

uint8_t AT42QT1060::getKeyDetectThreshold(uint8_t key)
{
  uint8_t threshold;
  readKey(RegisterAddresses::AT42QT1060::KEY_DETECT_THRESHOLD,key,threshold);
  return threshold;
}

void AT42QT1060::setKeyDetectThreshold(uint8_t key,
  uint8_t threshold)
{
  writeKey(RegisterAddresses::AT42QT1060::KEY_DETECT_THRESHOLD,key,threshold);
}

uint8_t AT42QT1060::getMeasurementIntervalCount()
{
  uint8_t interval_count;
  read(RegisterAddresses::AT42QT1060::LOW_POWER_MODE,interval_count);
  return interval_count;
}

void AT42QT1060::setMeasurementIntervalCount(uint8_t interval_count)
{
  write(RegisterAddresses::AT42QT1060::LOW_POWER_MODE,interval_count);
}

AT42QT1060::IOMask AT42QT1060::getIOMask()
{
  IOMask mask;
  read(RegisterAddresses::AT42QT1060::IO_MASK,mask.bytes);
  return mask;
}

void AT42QT1060::setIOMask(IOMask mask)
{
  write(RegisterAddresses::AT42QT1060::IO_MASK,mask.bytes);
}

AT42QT1060::KeyMask AT42QT1060::getKeyMask()
{
  KeyMask mask;
  read(RegisterAddresses::AT42QT1060::KEY_MASK,mask.bytes);
  return mask;
}

void AT42QT1060::setKeyMask(KeyMask mask)
{
  write(RegisterAddresses::AT42QT1060::KEY_MASK,mask.bytes);
}

AT42QT1060::AdjacentKeySuppressionMask AT42QT1060::getAdjacentKeySuppressionMask()
{
  AdjacentKeySuppressionMask mask;
  read(RegisterAddresses::AT42QT1060::KEY_MASK,mask.bytes);
  return mask;
}

void AT42QT1060::setAdjacentKeySuppressionMask(AdjacentKeySuppressionMask mask)
{
  write(RegisterAddresses::AT42QT1060::ADJACENT_KEY_SUPRESSION_MASK,mask.bytes);
}

AT42QT1060::PWMMask AT42QT1060::getPWMMask()
{
  PWMMask mask;
  read(RegisterAddresses::AT42QT1060::PWM_MASK,mask.bytes);
  return mask;
}

void AT42QT1060::setPWMMask(PWMMask mask)
{
  write(RegisterAddresses::AT42QT1060::PWM_MASK,mask.bytes);
}

AT42QT1060::DetectionMask AT42QT1060::getDetectionMask()
{
  DetectionMask mask;
  read(RegisterAddresses::AT42QT1060::DETECTION_MASK,mask.bytes);
  return mask;
}

void AT42QT1060::setDetectionMask(DetectionMask mask)
{
  write(RegisterAddresses::AT42QT1060::DETECTION_MASK,mask.bytes);
}
