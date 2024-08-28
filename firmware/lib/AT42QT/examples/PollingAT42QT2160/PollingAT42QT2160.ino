#include <AT42QT2160.h>


const long BAUD = 115200;
const int RESET_DELAY = 2000;
const int CALIBRATION_DELAY = 2000;

const int LOOP_DELAY = 500;

AT42QT2160 touch_sensor;

unsigned long loop_counter;

void printKeyStatus(AT42QT2160::Status status, uint8_t key_count)
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
  delay(CALIBRATION_DELAY);
  Serial.println("finished calibrating");

  loop_counter = 0;
}

void loop()
{
  delay(LOOP_DELAY);

  if (!touch_sensor.communicating())
  {
    Serial.print("Not communicating! Is chip connected and powered properly?");
    return;
  }

  uint8_t chip_id = touch_sensor.getChipId();
  Serial.print("chip_id: ");
  Serial.println(chip_id, HEX);

  AT42QT2160::Status status = touch_sensor.getStatus();
  uint16_t keys = status.keys;

  Serial.print("status.keys: ");
  Serial.println(keys, BIN);

  printKeyStatus(status,touch_sensor.KEY_COUNT);

  Serial.println();
  ++loop_counter;
}
