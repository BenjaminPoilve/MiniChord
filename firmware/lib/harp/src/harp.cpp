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
#else
  void harp::setup(){
    touch_sensor.setupSingleDevice(Wire,MPR121::ADDRESS_5A,true);
    touch_sensor.startAllChannels();
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
    touch_sensor.startAllChannels();

  }


  void harp::update(debouncer (&data_array)[12]){
      uint16_t touch_status = touch_sensor.getTouchStatus(MPR121::ADDRESS_5A);
      bool swapping;
      if (touch_sensor.overCurrentDetected(touch_status)){
        // The chip stops measuring until the flag is cleared. Cleared, it starts again from scratch, as
        // after a plate swap.
        touch_sensor.clearOverCurrentFlag(MPR121::ADDRESS_5A);
        uint16_t none[12] = {0};
        swap_begin("over current detected", none, none, false);
        recalibrate();
        for (uint8_t k = 0; k < 12; k++) swap_lo[k] = 0xFFFF, swap_hi[k] = 0;
        swapping = true;
      } else {
        swapping = swap_watch(touch_status);
      }
      // while a swap is waited out every zone reads as untouched, so the strings held are let go as usual
      for (uint8_t key=0; key < 12; key++){
          data_array[remap_array[key]].set(!swapping && touch_sensor.deviceChannelTouched(touch_status,key));
      }
  }

  // PLATE SWAP
  // A plate taken off or put on with the power on. The chip reads each zone against a resting level (its
  // baseline) that follows a change only a count at a time, and hardly at all while the zone reads as touched.
  // So a plate with more copper put on reads, on every zone at once, as twelve fingers held down: the strings
  // sound and stay held, and nothing new plays, until the baseline crawls down. A plate with less copper (or
  // one taken off) reads above its rest, and a finger has to press far harder to count.
  // So the swap is watched, from the chip's readings every swap_poll_ms:
  //  - coming off: swap_zones zones or more read over swap_lift above their rest. A touch only ever brings a
  //    reading down. Or swap_left_zones zones or more left above their rest, untouched, for swap_left_ms: the
  //    rest of a plate pulled off one header at a time, or a hand lifted after a recalibration made under it.
  //  - going on: once a swap has settled with no plate on, swap_zones zones or more falling swap_drop under
  //    their rest, however slowly the plate is pressed on.
  //  - left stuck: all twelve zones touched and still to within swap_steady_counts for swap_stuck_ms: a plate
  //    put on with the harp not known to be bare (switched on without one). Its strings sound until then.
  // (A plate's landings aren't timed: a hand slapped flat across the strings lands as fast as a plate does.)
  // Then every zone reads as untouched (the strings are let go) until every zone holds still to within
  // swap_steady_counts for swap_settle_ms, the chip is recalibrated there, and the harp plays again
  // swap_after_ms later. Measured on swaps of five plates: a plate coming off reads 31 to 187 counts above
  // its rest, one going on 75 to 189 under, and the bare pins about 904 to 975.
  void harp::swap_begin(const char *why, const uint16_t (&filtered)[12], const uint16_t (&baseline)[12], bool coming_off){
    Serial.printf("Plate swap: %s; channels above (+) or below (-) their rest:", why);
    swap_before = 0;
    for (uint8_t k = 0; k < 12; k++){
      Serial.printf(" %+d", (int)filtered[k] - (int)baseline[k]);
      swap_lo[k] = swap_hi[k] = filtered[k];
      swap_before += baseline[k];
    }
    Serial.println();
    swap_state = SWAP_SETTLING;
    swap_coming_off = coming_off;
    swap_stuck = false;
    swap_steady_for = 0;
    swap_left_for = 0;
    swap_still_seed = true;
  }

  // true while a swap is being waited out
  bool harp::swap_watch(uint16_t touch_status){
    if (swap_state == SWAP_AFTER){
      if (swap_since_after < swap_after_ms) return true;
      swap_state = SWAP_NONE;
      Serial.println("Plate swap: playing again");
      return false;
    }
    if (swap_state == SWAP_NONE && swap_since_poll < swap_poll_ms) return false;
    swap_since_poll = 0;
    uint16_t f[12], b[12];
    touch_sensor.getDeviceAllChannelsData(MPR121::ADDRESS_5A, f, b);
    if (swap_state == SWAP_NONE){
      uint8_t lifted = 0, dropped = 0;
      bool all_down = true, any_down = false;
      for (uint8_t k = 0; k < 12; k++){
        lifted += f[k] > b[k] + swap_lift;
        dropped += f[k] + swap_drop < b[k];
        bool down = touch_sensor.deviceChannelTouched(touch_status,k);
        all_down &= down;
        any_down |= down;
      }
      // all twelve held, and still since they went down
      bool still = all_down;
      if (all_down && swap_still_seed){
        for (uint8_t k = 0; k < 12; k++) swap_still_lo[k] = swap_still_hi[k] = f[k];
        swap_all_still = 0;
        swap_still_seed = false;
      }
      for (uint8_t k = 0; k < 12 && still; k++){
        swap_still_lo[k] = min(swap_still_lo[k], f[k]);
        swap_still_hi[k] = max(swap_still_hi[k], f[k]);
        still = swap_still_hi[k] - swap_still_lo[k] <= swap_steady_counts;
      }
      if (!still) swap_still_seed = true;
      if (lifted < swap_left_zones || any_down) swap_left_for = 0;
      if (lifted >= swap_zones) swap_begin("a plate coming off", f, b, true);
      else if (swap_left_for >= swap_left_ms) swap_begin("zones left above their rest", f, b, true);
      else if (swap_bare && dropped >= swap_zones) swap_begin("a plate going on, onto bare pins", f, b, false);
      else if (still && swap_all_still >= swap_stuck_ms){
        swap_begin("all twelve zones held still", f, b, false);
        swap_stuck = true;
      }
      else return false;
      return true;
    }
    // settling: wait for every zone to hold still, then recalibrate there
    bool steady = true;
    for (uint8_t k = 0; k < 12; k++){
      swap_lo[k] = min(swap_lo[k], f[k]);
      swap_hi[k] = max(swap_hi[k], f[k]);
      steady &= swap_hi[k] - swap_lo[k] <= swap_steady_counts;
    }
    if (!steady){
      swap_steady_for = 0;
      for (uint8_t k = 0; k < 12; k++) swap_lo[k] = swap_hi[k] = f[k];
      return true;
    }
    if (swap_steady_for < swap_settle_ms) return true;
    // No plate on: a plate came off and it settled well above where the plate rested; or it settled near
    // where the bare pins read before (a plate touched to the pins and lifted off again before it settled).
    // The highest bare reading is kept, as a plate pulled off one header at a time settles half bare first.
    // But what comes off right after all twelve were held still may be the hand that held them.
    int32_t sum = 0;
    for (uint8_t k = 0; k < 12; k++) sum += f[k];
    bool off_again = swap_bare_sum && sum + 12 * swap_bare_counts / 2 > swap_bare_sum;
    swap_bare = (swap_coming_off && !swap_after_stuck && sum > swap_before + 12 * swap_bare_counts) || off_again;
    swap_after_stuck = swap_stuck;
    if (swap_bare) swap_bare_sum = max(swap_bare_sum, sum);
    Serial.printf("Plate swap: settled%s; recalibrating at", swap_bare ? ", no plate on" : "");
    for (uint8_t k = 0; k < 12; k++) Serial.printf(" %d", f[k]);
    Serial.println();
    recalibrate();
    swap_state = SWAP_AFTER;
    swap_since_after = 0;
    return true;
  }
#endif
