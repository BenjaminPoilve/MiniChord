// ----------------------------------------------------------------------------
// AT42QT2120.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AT42QT2120.h"


AT42QT2120::Status AT42QT2120::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT2120::STATUS,status.bytes,STATUS_SIZE);
  return status;
}

bool AT42QT2120::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

bool AT42QT2120::anyTouched(Status status)
{
  return status.keys;
}

bool AT42QT2120::touched(Status status,
  uint8_t key)
{
  return bitRead(status.keys,key);
}

uint8_t AT42QT2120::getMeasurementIntervalCount()
{
  uint8_t interval_count;
  read(RegisterAddresses::AT42QT2120::LOW_POWER_MODE,interval_count);
  return interval_count;
}

void AT42QT2120::setMeasurementIntervalCount(uint8_t interval_count)
{
  write(RegisterAddresses::AT42QT2120::LOW_POWER_MODE,interval_count);
}

uint8_t AT42QT2120::getTowardsDriftCompensationDuration()
{
  uint8_t tdcd;
  read(RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_COMPENSATION_DURATION,tdcd);
  return tdcd;
}

void AT42QT2120::setTowardsDriftCompensationDuration(uint8_t tdcd)
{
  if (tdcd > DRIFT_COMPENSATION_DURATION_MAX)
  {
    tdcd = DRIFT_COMPENSATION_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::TOWARDS_DRIFT_COMPENSATION_DURATION,tdcd);
}

uint8_t AT42QT2120::getAwayDriftCompensationDuration()
{
  uint8_t adcd;
  read(RegisterAddresses::AT42QT2120::AWAY_DRIFT_COMPENSATION_DURATION,adcd);
  return adcd;
}

void AT42QT2120::setAwayDriftCompensationDuration(uint8_t adcd)
{
  if (adcd > DRIFT_COMPENSATION_DURATION_MAX)
  {
    adcd = DRIFT_COMPENSATION_DURATION_MAX;
  }
  write(RegisterAddresses::AT42QT2120::AWAY_DRIFT_COMPENSATION_DURATION,adcd);
}

uint8_t AT42QT2120::getDetectionIntegrator()
{
  uint8_t di;
  read(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR,di);
  return di;
}

void AT42QT2120::setDetectionIntegrator(uint8_t di)
{
  if (di > DETECTION_INTEGRATOR_MAX)
  {
    di = DETECTION_INTEGRATOR_MAX;
  }
  write(RegisterAddresses::AT42QT2120::DETECTION_INTEGRATOR,di);
}

uint8_t AT42QT2120::getRecalibrationDelay()
{
  uint8_t rd;
  read(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY,rd);
  return rd;
}

void AT42QT2120::setRecalibrationDelay(uint8_t rd)
{
  write(RegisterAddresses::AT42QT2120::RECALIBRATION_DELAY,rd);
}

uint8_t AT42QT2120::getDriftCompensationHoldDuration()
{
  uint8_t dchd;
  read(RegisterAddresses::AT42QT2120::DRIFT_COMPENSATION_HOLD_DURATION,dchd);
  return dchd;
}

void AT42QT2120::setDriftCompensationHoldDuration(uint8_t dchd)
{
  write(RegisterAddresses::AT42QT2120::DRIFT_COMPENSATION_HOLD_DURATION,dchd);
}

bool AT42QT2120::sliderOrWheelEnabled()
{
  SliderOrWheelEnable swe;
  read(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE,swe.bytes);
  return swe.enable;
}

void AT42QT2120::disableSliderAndWheel()
{
  SliderOrWheelEnable swe;
  swe.wheel = 0;
  swe.enable = 0;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE,swe.bytes);
}

void AT42QT2120::enableSlider()
{
  SliderOrWheelEnable swe;
  swe.wheel = 0;
  swe.enable = 1;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE,swe.bytes);
}

void AT42QT2120::enableWheel()
{
  SliderOrWheelEnable swe;
  swe.wheel = 1;
  swe.enable = 1;
  write(RegisterAddresses::AT42QT2120::SLIDER_OR_WHEEL_ENABLE,swe.bytes);
}

uint8_t AT42QT2120::getChargeDuration()
{
  uint8_t cd;
  read(RegisterAddresses::AT42QT2120::CHARGE_DURATION,cd);
  return cd;
}

void AT42QT2120::setChargeDuration(uint8_t cd)
{
  write(RegisterAddresses::AT42QT2120::CHARGE_DURATION,cd);
}

uint8_t AT42QT2120::getKeyDetectThreshold(uint8_t key)
{
  uint8_t threshold;
  readKey(RegisterAddresses::AT42QT2120::KEY_DETECT_THRESHOLD,key,threshold);
  return threshold;
}

void AT42QT2120::setKeyDetectThreshold(uint8_t key,
  uint8_t threshold)
{
  writeKey(RegisterAddresses::AT42QT2120::KEY_DETECT_THRESHOLD,key,threshold);
}

AT42QT2120::KeyControl AT42QT2120::getKeyControl(uint8_t key)
{
  KeyControl key_control;
  readKey(RegisterAddresses::AT42QT2120::KEY_CONTROL,key,key_control.bytes);
  return key_control;
}

void AT42QT2120::setKeyControl(uint8_t key,
  KeyControl key_control)
{
  writeKey(RegisterAddresses::AT42QT2120::KEY_CONTROL,key,key_control.bytes);
}

AT42QT2120::KeyPulseScale AT42QT2120::getKeyPulseScale(uint8_t key)
{
  KeyPulseScale key_pulse_scale;
  readKey(RegisterAddresses::AT42QT2120::KEY_PULSE_SCALE,key,key_pulse_scale.bytes);
  return key_pulse_scale;
}

void AT42QT2120::setKeyPulseScale(uint8_t key,
  KeyPulseScale key_pulse_scale)
{
  writeKey(RegisterAddresses::AT42QT2120::KEY_PULSE_SCALE,key,key_pulse_scale.bytes);
}

uint16_t AT42QT2120::getKeySignal(uint8_t key)
{
  uint16_t signal;
  readKey(RegisterAddresses::AT42QT2120::KEY_SIGNAL,key,signal);
  return signal;
}

uint16_t AT42QT2120::getKeyReference(uint8_t key)
{
  uint16_t reference;
  readKey(RegisterAddresses::AT42QT2120::KEY_REFERENCE,key,reference);
  return reference;
}
