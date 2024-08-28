#include <AT42QT2120.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_LOOP_DELAY = 50;

const int LOOP_DELAY = 500;

AT42QT2120 touch_sensor;

unsigned long loop_counter;

void printKeyStatus(AT42QT2120::Status status, uint8_t key_count)
{
  for (uint8_t key=0; key < key_count; ++key)
  {
    if ((key != 0) && (key < 10))
      Serial.print("  ");
    else if (key >= 10)
      Serial.print(" ");
    Serial.print(key);
  }
  Serial.println();
  for (uint8_t key=0; key < key_count; ++key)
  {
    if (key != 0)
      Serial.print("  ");
    Serial.print(touch_sensor.touched(status,key));
  }
  Serial.println();
}

void setup()
{
  Serial.begin(BAUD);

  touch_sensor.begin();

  if (!touch_sensor.communicating())
  {
    return;
  }

  Serial.println("reseting...");
  touch_sensor.reset();
  delay(RESET_DELAY);

  Serial.println("triggerCalibration");
  touch_sensor.triggerCalibration();
  delay(CALIBRATION_LOOP_DELAY);
  while (touch_sensor.calibrating())
  {
    Serial.println("calibrating...");
    delay(CALIBRATION_LOOP_DELAY);
  }
  Serial.println("finished calibrating");

  loop_counter = 0;
}

void loop()
{
  delay(LOOP_DELAY);

  if (!touch_sensor.communicating())
  {
    Serial.println("Not communicating! Is chip connected and powered properly?");
    return;
  }

  uint8_t chip_id = touch_sensor.getChipId();
  Serial.print("chip_id: ");
  Serial.println(chip_id, HEX);

  AT42QT2120::Status status = touch_sensor.getStatus();

  Serial.print("status.anyTouched(): ");
  Serial.println(touch_sensor.anyTouched(status));

  Serial.print("status.overflow: ");
  Serial.println(status.overflow);

  Serial.print("status.keys: ");
  Serial.println(status.keys, BIN);

  printKeyStatus(status,touch_sensor.KEY_COUNT);

  uint8_t interval_count = 2;
  Serial.print("setMeasurementIntervalCount(");
  Serial.print(interval_count);
  Serial.println(")");
  touch_sensor.setMeasurementIntervalCount(interval_count);
  interval_count = touch_sensor.getMeasurementIntervalCount();
  Serial.print("getMeasurementIntervalCount(): ");
  Serial.println(interval_count);

  uint8_t tdcd = 21;
  Serial.print("setTowardsDriftCompensationDuration(");
  Serial.print(tdcd);
  Serial.println(")");
  touch_sensor.setTowardsDriftCompensationDuration(tdcd);
  tdcd = touch_sensor.getTowardsDriftCompensationDuration();
  Serial.print("getTowardsDriftCompensationDuration(): ");
  Serial.print(tdcd);
  Serial.println();;

  uint8_t adcd = 8;
  Serial.print("setAwayDriftCompensationDuration(");
  Serial.print(adcd);
  Serial.println(")");
  touch_sensor.setAwayDriftCompensationDuration(adcd);
  adcd = touch_sensor.getAwayDriftCompensationDuration();
  Serial.print("getAwayDriftCompensationDuration(): ");
  Serial.println(adcd);

  uint8_t di = 5;
  Serial.print("setDetectionIntegrator(");
  Serial.print(di);
  Serial.println(")");
  touch_sensor.setDetectionIntegrator(di);
  di = touch_sensor.getDetectionIntegrator();
  Serial.print("getDetectionIntegrator(): ");
  Serial.println(di);

  uint8_t rd = 250;
  Serial.print("setRecalibrationDelay(");
  Serial.print(rd);
  Serial.println(")");
  touch_sensor.setRecalibrationDelay(rd);
  rd = touch_sensor.getRecalibrationDelay();
  Serial.print("getRecalibrationDelay(): ");
  Serial.println(rd);

  uint8_t dchd = 26;
  Serial.print("setDriftCompensationHoldDuration(");
  Serial.print(dchd);
  Serial.println(")");
  touch_sensor.setDriftCompensationHoldDuration(dchd);
  dchd = touch_sensor.getDriftCompensationHoldDuration();
  Serial.print("getDriftCompensationHoldDuration(): ");
  Serial.println(dchd);

  Serial.println("disableSliderAndWheel()");
  touch_sensor.disableSliderAndWheel();
  bool swe = touch_sensor.sliderOrWheelEnabled();
  Serial.print("sliderOrWheelEnabled(): ");
  Serial.println(swe);

  uint8_t cd = 255;
  Serial.print("setChargeDuration(");
  Serial.print(cd);
  Serial.println(")");
  touch_sensor.setChargeDuration(cd);
  rd = touch_sensor.getChargeDuration();
  Serial.print("getChargeDuration(): ");
  Serial.println(cd);

  uint8_t key = 3;
  uint8_t threshold = 66;
  Serial.print("setKeyDetectThreshold(");
  Serial.print(key);
  Serial.print(",");
  Serial.print(threshold);
  Serial.println(")");
  touch_sensor.setKeyDetectThreshold(key,threshold);
  threshold = touch_sensor.getKeyDetectThreshold(key);
  Serial.print("getKeyDetectThreshold(");
  Serial.print(key);
  Serial.print("): ");
  Serial.println(threshold);

  key = 5;
  AT42QT2120::KeyControl key_control;
  uint8_t aksg = 2;
  key_control.adjacent_key_suppression_group = aksg;
  Serial.print("setKeyControl: key = ");
  Serial.print(key);
  Serial.print(", adjacent_key_suppression_group = ");
  Serial.println(aksg);
  touch_sensor.setKeyControl(key,key_control);
  key_control = touch_sensor.getKeyControl(key);
  aksg = key_control.adjacent_key_suppression_group;
  Serial.print("getKeyControl: key = ");
  Serial.print(key);
  Serial.print(", adjacent_key_suppression_group = ");
  Serial.println(aksg);

  key = 0;
  AT42QT2120::KeyPulseScale key_pulse_scale;
  uint8_t pulse = 0x8;
  uint8_t scale = 0x4;
  key_pulse_scale.pulse = pulse;
  key_pulse_scale.scale = scale;
  Serial.print("setKeyPulseScale: key = ");
  Serial.print(key);
  Serial.print(", pulse = ");
  Serial.print(pulse);
  Serial.print(", scale = ");
  Serial.println(scale);
  touch_sensor.setKeyPulseScale(key,key_pulse_scale);
  key_pulse_scale = touch_sensor.getKeyPulseScale(key);
  pulse = key_pulse_scale.pulse;
  scale = key_pulse_scale.scale;
  Serial.print("getKeyPulseScale: key = ");
  Serial.print(key);
  Serial.print(", pulse = ");
  Serial.print(pulse);
  Serial.print(", scale = ");
  Serial.println(scale);
  pulse = 0;
  scale = 0;
  key_pulse_scale.pulse = pulse;
  key_pulse_scale.scale = scale;
  Serial.print("setKeyPulseScale: key = ");
  Serial.print(key);
  Serial.print(", pulse = ");
  Serial.print(pulse);
  Serial.print(", scale = ");
  Serial.println(scale);
  touch_sensor.setKeyPulseScale(key,key_pulse_scale);

  key = 8;
  uint16_t signal = touch_sensor.getKeySignal(key);
  Serial.print("getKeySignal(");
  Serial.print(key);
  Serial.print("): ");
  Serial.println(signal);

  uint16_t reference = touch_sensor.getKeyReference(key);
  Serial.print("getKeyReference(");
  Serial.print(key);
  Serial.print("): ");
  Serial.println(reference);

  Serial.println();
  ++loop_counter;
}
