#include "harp.h"


harp::harp(){}  


#if CAP_CHIP==1
  void harp::setup(){
    touch_sensor.begin();
    if (!touch_sensor.communicating()){
      Serial.println("Harp not communicating");
      return;
    }
  }

  void harp::recalibrate(){
      Serial.println("reseting...");
    touch_sensor.reset();
    delay(1000);
    Serial.println("triggerCalibration");
    touch_sensor.triggerCalibration();
    delay(50);
    while (touch_sensor.calibrating())
    {
      Serial.println("calibrating...");
      delay(50);
    }
    Serial.println("finished calibrating"); 
    touch_sensor.setMeasurementIntervalCount(1);
    touch_sensor.setDetectionIntegrator(2); 
    touch_sensor.setTowardsDriftCompensationDuration(26);
    touch_sensor.setAwayDriftCompensationDuration(8);
    touch_sensor.setRecalibrationDelay(0);
    AT42QT2120::KeyPulseScale key_pulse_scale;
    uint8_t pulse = 0x0;
    uint8_t scale = 0x0;
    key_pulse_scale.pulse = pulse;
    key_pulse_scale.scale = scale;
    for (uint8_t key=0; key < touch_sensor.KEY_COUNT; ++key){
      touch_sensor.setKeyDetectThreshold(key,threshold);
      touch_sensor.setKeyPulseScale(key,key_pulse_scale);
    }
  }


  void harp::update(debouncer (&data_array)[12]){
      AT42QT2120::Status status = touch_sensor.getStatus();
      uint8_t key_count= touch_sensor.KEY_COUNT;
      for (uint8_t key=0; key < key_count; ++key){
          data_array[remap_array[key]].set(touch_sensor.touched(status,key));
      }
  }

  // This chip has no proximity channel
  uint16_t harp::read_proximity(){ return 0; }
#else
  void harp::setup(){
    touch_sensor.setupSingleDevice(Wire,MPR121::ADDRESS_5A,true);
    touch_sensor.startAllChannels(MPR121::COMBINE_CHANNELS_0_TO_11);
    if (!touch_sensor.communicating(MPR121::ADDRESS_5A)){
      Serial.println("Harp not communicating");
      return;
    }
  }

  void harp::recalibrate(){
    Serial.println("Recalibrating Harp");
    touch_sensor.setAllChannelsThresholds(touch_threshold,
    release_threshold);
    touch_sensor.setDebounce(MPR121::ADDRESS_5A,
    touch_debounce,
    release_debounce);
    touch_sensor.setBaselineTracking(MPR121::ADDRESS_5A,
    baseline_tracking);
    touch_sensor.setChargeDischargeCurrent(MPR121::ADDRESS_5A,
    charge_discharge_current);
    touch_sensor.setChargeDischargeTime(MPR121::ADDRESS_5A,
    charge_discharge_time);
    touch_sensor.setFirstFilterIterations(MPR121::ADDRESS_5A,
    first_filter_iterations);
    touch_sensor.setSecondFilterIterations(MPR121::ADDRESS_5A,
    second_filter_iterations);
    touch_sensor.setSamplePeriod(MPR121::ADDRESS_5A,
    sample_period);
    touch_sensor.startAllChannels(MPR121::COMBINE_CHANNELS_0_TO_11);
    set_proximity_charge();
  }


  void harp::update(debouncer (&data_array)[12]){
      uint16_t touch_status = touch_sensor.getTouchStatus(MPR121::ADDRESS_5A);
      if (touch_sensor.overCurrentDetected(touch_status)){
        Serial.println("Over current detected!\n\n");
        touch_sensor.startAllChannels(MPR121::COMBINE_CHANNELS_0_TO_11);
        return;
      }
      for (uint8_t key=0; key < 12; key++){
          data_array[remap_array[key]].set(touch_sensor.deviceChannelTouched(touch_status,key));
      }
  }

  // Sets the proximity channel's charge so its reading sits high in the chip's
  // range: the counts a hand moves it grow with the charge, and the top of the
  // range (about 800 at 3.3 V) is where the chip starts reading out of range.
  // Doubles the charge time until the reading passes the target, then trims the
  // current to land on it. On the stock plate: 2 us reads about 590, so 4 us at
  // about 38 uA.
  void harp::set_proximity_charge(){
    const uint16_t target = 720;
    const uint16_t saturated = 1000;   // past the top it reads full scale, whatever the charge
    uint16_t previous = 0;
    for (uint8_t cdt = 1; cdt <= 7; cdt++){
      touch_sensor.setDeviceChannelChargeDischargeCurrent(MPR121::ADDRESS_5A, proximity_channel, charge_discharge_current);
      touch_sensor.setDeviceChannelChargeDischargeTime(MPR121::ADDRESS_5A, proximity_channel, (MPR121::ChargeDischargeTime)cdt);
      delay(30);   // the filters settle
      uint16_t level = touch_sensor.getDeviceChannelFilteredData(MPR121::ADDRESS_5A, proximity_channel);
      if (level < target && cdt < 7){
        previous = level;
        continue;
      }
      // the reading grows with the current: scale it to the target, estimating a
      // saturated reading from the step before, at half the time
      uint16_t full = (level >= saturated && previous) ? previous * 2 : level;
      uint8_t current = full ? constrain((uint32_t)charge_discharge_current * target / full, 1, charge_discharge_current) : charge_discharge_current;
      touch_sensor.setDeviceChannelChargeDischargeCurrent(MPR121::ADDRESS_5A, proximity_channel, current);
      delay(30);
      Serial.printf("Hover charge: %.1f us at %d uA, reads %d\n", (1 << (cdt - 1)) * 0.5, current,
        touch_sensor.getDeviceChannelFilteredData(MPR121::ADDRESS_5A, proximity_channel));
      return;
    }
  }

  uint16_t harp::read_proximity(){
    return touch_sensor.getDeviceChannelFilteredData(MPR121::ADDRESS_5A, proximity_channel);
  }
#endif
