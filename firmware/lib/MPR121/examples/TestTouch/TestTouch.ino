#include <Arduino.h>
#include <MPR121.h>
#include <Streaming.h>

#include "Constants.h"


MPR121 mpr121;

void setup()
{
  Serial.begin(constants::baud);

  mpr121.setupSingleDevice(*constants::wire_ptr,
    constants::device_address,
    constants::fast_mode);

  mpr121.setAllChannelsThresholds(constants::touch_threshold,
    constants::release_threshold);
  mpr121.setDebounce(constants::device_address,
    constants::touch_debounce,
    constants::release_debounce);
  mpr121.setBaselineTracking(constants::device_address,
    constants::baseline_tracking);
  mpr121.setChargeDischargeCurrent(constants::device_address,
    constants::charge_discharge_current);
  mpr121.setChargeDischargeTime(constants::device_address,
    constants::charge_discharge_time);
  mpr121.setFirstFilterIterations(constants::device_address,
    constants::first_filter_iterations);
  mpr121.setSecondFilterIterations(constants::device_address,
    constants::second_filter_iterations);
  mpr121.setSamplePeriod(constants::device_address,
    constants::sample_period);

  mpr121.startChannels(constants::physical_channel_count,
    constants::proximity_mode);
}

void loop()
{
  delay(constants::loop_delay);
  if (!mpr121.communicating(constants::device_address))
  {
    Serial << "mpr121 device not commmunicating!\n\n";
    return;
  }

  uint8_t channel_count = mpr121.getChannelCount();
  Serial << "channel_count: " << channel_count << "\n";
  uint8_t running_channel_count = mpr121.getRunningChannelCount();
  Serial << "running_channel_count: " << running_channel_count << "\n";

  uint16_t touch_status = mpr121.getTouchStatus(constants::device_address);
  Serial << "touch_status: " << _BIN(touch_status) << "\n";
  if (mpr121.overCurrentDetected(touch_status))
  {
    Serial << "Over current detected!\n\n";
    mpr121.startChannels(constants::physical_channel_count,
      constants::proximity_mode);
    return;
  }
  bool any_touched = mpr121.anyTouched(touch_status);
  Serial << "any_touched: " << any_touched << "\n";
  bool device_channel_touched = mpr121.deviceChannelTouched(touch_status,
    constants::channel);
  Serial << "device_channel_touched: " << device_channel_touched << "\n";

  uint16_t out_of_range_status = mpr121.getOutOfRangeStatus(constants::device_address);
  Serial << "out_of_range_status: " << _BIN(out_of_range_status) << "\n";

  bool channel_touched = mpr121.channelTouched(constants::channel);
  Serial << "channel_touched: " << channel_touched << "\n";

  uint16_t channel_filtered_data = mpr121.getChannelFilteredData(constants::channel);
  Serial << "channel_filtered_data: " << channel_filtered_data << "\n";

  uint16_t channel_baseline_data = mpr121.getChannelBaselineData(constants::channel);
  Serial << "channel_baseline_data: " << channel_baseline_data << "\n";

  Serial << "\n";
}
