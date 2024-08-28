// ----------------------------------------------------------------------------
// MPR121.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef MPR121_H
#define MPR121_H
#include <Arduino.h>
#include <Wire.h>


class MPR121
{
public:
  MPR121();

  enum DeviceAddress
    {
     ADDRESS_5A = 0x5A,
     ADDRESS_5B = 0x5B,
     ADDRESS_5C = 0x5C,
     ADDRESS_5D = 0x5D,
    };
  enum {PHYSICAL_CHANNELS_PER_DEVICE=12};
  enum {CHANNELS_PER_DEVICE=13};
  enum {DEVICE_COUNT_MAX=4};

  // Convenience method when using a single device
  bool setupSingleDevice(TwoWire & wire=Wire,
    DeviceAddress device_address=ADDRESS_5A,
    bool fast_mode=false);

  // Methods for using a single device or multiple devices
  bool communicating(DeviceAddress device_address);

  enum ProximityMode
    {
     PROXIMITY_MODE_DISABLED = 0b00,
     COMBINE_CHANNELS_0_TO_1 = 0b01,
     COMBINE_CHANNELS_0_TO_3 = 0b10,
     COMBINE_CHANNELS_0_TO_11 = 0b11,
    };
  void startChannels(uint8_t physical_channel_count,
    ProximityMode proximity_mode=PROXIMITY_MODE_DISABLED);
  void startAllChannels(ProximityMode proximity_mode=PROXIMITY_MODE_DISABLED);
  void stopAllChannels();

  uint8_t getChannelCount();
  uint8_t getRunningChannelCount();

  // sets touch and release thresholds either for all channels, or
  // for a specfic channel - higher values = less sensitive
  void setChannelThresholds(uint8_t channel,
    uint8_t touch_threshold,
    uint8_t release_threshold);
  void setAllChannelsThresholds(uint8_t touch_threshold,
    uint8_t release_threshold);

  bool channelTouched(uint8_t channel);

  uint16_t getChannelFilteredData(uint8_t channel);
  uint16_t getChannelBaselineData(uint8_t channel);

  // Methods for using multiple devices
  // Take care when using fast_mode with non-MPR121 devices
  void setWire(TwoWire & wire=Wire,
    bool fast_mode=false);

  void addDevice(DeviceAddress device_address);
  bool setupDevice(DeviceAddress device_address);
  bool setupAllDevices();

  void startChannels(DeviceAddress device_address,
    uint8_t physical_channel_count,
    ProximityMode proximity_mode=PROXIMITY_MODE_DISABLED);
  void startChannelsAllDevices(uint8_t physical_channel_count,
    ProximityMode proximity_mode=PROXIMITY_MODE_DISABLED);
  void startAllChannels(DeviceAddress device_address,
    ProximityMode proximity_mode=PROXIMITY_MODE_DISABLED);
  void stopAllChannels(DeviceAddress device_address);

  uint8_t getDeviceCount();
  uint8_t getDeviceChannelCount();
  uint8_t getRunningChannelCount(DeviceAddress device_address);

  void setDeviceChannelThresholds(DeviceAddress device_address,
    uint8_t device_channel,
    uint8_t touch_threshold,
    uint8_t release_threshold);
  void setAllDeviceChannelsThresholds(DeviceAddress device_address,
    uint8_t touch_threshold,
    uint8_t release_threshold);

  uint16_t getTouchStatus(DeviceAddress device_address);
  bool overCurrentDetected(uint16_t touch_status);
  bool anyTouched(uint16_t touch_status);
  uint8_t getTouchCount(uint16_t touch_status);
  bool deviceChannelTouched(uint16_t touch_status,
    uint8_t device_channel);

  uint16_t getOutOfRangeStatus(DeviceAddress device_address);
  bool deviceChannelOutOfRange(uint16_t out_of_range_status,
    uint8_t device_channel);
  bool autoConfigFail(uint16_t out_of_range_status);
  bool autoReconfigFail(uint16_t out_of_range_status);

  uint16_t getDeviceChannelFilteredData(DeviceAddress device_address,
    uint8_t device_channel);
  uint16_t getDeviceChannelBaselineData(DeviceAddress device_address,
    uint8_t device_channel);

  enum BaselineTracking
    {
     BASELINE_TRACKING_INIT_0 = 0x00, // default
     BASELINE_TRACKING_DISABLED = 0x01,
     BASELINE_TRACKING_INIT_5BIT = 0x02,
     BASELINE_TRACKING_INIT_10BIT = 0x03,
    };
  void setBaselineTracking(DeviceAddress device_address,
    BaselineTracking baseline_tracking);
  void setDebounce(DeviceAddress device_address,
    uint8_t touch_debounce,
    uint8_t release_debounce);

  const static uint8_t CHARGE_DISCHARGE_CURRENT_MIN = 1;
  const static uint8_t CHARGE_DISCHARGE_CURRENT_MAX = 63;
  void setChargeDischargeCurrent(DeviceAddress device_address,
    uint8_t charge_discharge_current);
  void setDeviceChannelChargeDischargeCurrent(DeviceAddress device_address,
    uint8_t device_channel,
    uint8_t charge_discharge_current);
  enum ChargeDischargeTime
    {
     CHARGE_DISCHARGE_TIME_DISABLED = 0x00,
     CHARGE_DISCHARGE_TIME_HALF_US = 0x01, // default
     CHARGE_DISCHARGE_TIME_1US = 0x02,
     CHARGE_DISCHARGE_TIME_2US = 0x03,
     CHARGE_DISCHARGE_TIME_4US = 0x04,
     CHARGE_DISCHARGE_TIME_8US = 0x05,
     CHARGE_DISCHARGE_TIME_16US = 0x06,
     CHARGE_DISCHARGE_TIME_32US = 0x07
    };
  void setChargeDischargeTime(DeviceAddress device_address,
    ChargeDischargeTime charge_discharge_time);
  void setDeviceChannelChargeDischargeTime(DeviceAddress device_address,
    uint8_t device_channel,
    ChargeDischargeTime charge_discharge_time);

  enum FirstFilterIterations
    {
     FIRST_FILTER_ITERATIONS_6 = 0x00, // default
     FIRST_FILTER_ITERATIONS_10 = 0x01,
     FIRST_FILTER_ITERATIONS_18 = 0x02,
     FIRST_FILTER_ITERATIONS_34 = 0x03,
    };
  void setFirstFilterIterations(DeviceAddress device_address,
    FirstFilterIterations first_filter_iterations);
  enum SecondFilterIterations
    {
     SECOND_FILTER_ITERATIONS_4 = 0x00, // default
     SECOND_FILTER_ITERATIONS_6 = 0x01,
     SECOND_FILTER_ITERATIONS_10 = 0x02,
     SECOND_FILTER_ITERATIONS_18 = 0x03,
    };
  void setSecondFilterIterations(DeviceAddress device_address,
    SecondFilterIterations second_filter_iterations);

  // Sets the sample period of the MPR121 - the time between capacitive
  // readings. Higher values consume less power, but are less responsive.
  // sample intervals
  enum SamplePeriod
    {
     SAMPLE_PERIOD_1MS = 0x00,
     SAMPLE_PERIOD_2MS = 0x01,
     SAMPLE_PERIOD_4MS = 0x02,
     SAMPLE_PERIOD_8MS = 0x03,
     SAMPLE_PERIOD_16MS = 0x04, // default
     SAMPLE_PERIOD_32MS = 0x05,
     SAMPLE_PERIOD_64MS = 0x06,
     SAMPLE_PERIOD_128MS = 0x07
    };
  void setSamplePeriod(DeviceAddress device_address,
    SamplePeriod sample_period);

private:
  uint8_t device_count_;
  DeviceAddress device_addresses_[DEVICE_COUNT_MAX];
  ProximityMode proximity_modes_[DEVICE_COUNT_MAX];

  TwoWire * wire_ptr_;
  const static long FAST_MODE_CLOCK_FREQUENCY = 400000;

  const static int DEVICE_INDEX_NONE = -1;
  int deviceAddressToDeviceIndex(DeviceAddress device_address);

  template<typename T>
  void write(DeviceAddress device_address,
    uint8_t register_address,
    T data);
  template<typename T>
  void read(DeviceAddress device_address,
    uint8_t register_address,
    T & data);

  const static uint8_t BITS_PER_BYTE = 8;
  const static uint8_t BITS_PER_TWO_BYTES = 16;
  const static uint8_t BYTE_MAX = 0xFF;
  const static uint16_t TWO_BYTE_MAX = 0xFFFF;

  const static uint16_t OVER_CURRENT_REXT = 0x8000;
  const static uint16_t TOUCH_STATUS_MASK_BASE = 0x1FFF;
  uint16_t touch_status_mask_;

  const static uint16_t OUT_OF_RANGE_STATUS_ACFF = 0x8000;
  const static uint16_t OUT_OF_RANGE_STATUS_ARFF = 0x4000;

  const static uint8_t BASELINE_DATA_BIT_SHIFT = 2;

  // register addresses
  const static uint8_t TOUCH_STATUS_REGISTER_ADDRESS = 0x00;
  const static uint8_t OUT_OF_RANGE_STATUS_REGISTER_ADDRESS = 0x02;

  // filtered data
  const static uint8_t FILTERED_DATA0_REGISTER_ADDRESS = 0x04;

  // baseline data
  const static uint8_t BASELINE_DATA0_REGISTER_ADDRESS = 0x1E;

  // general electrode touch sense baseline filters
  // rising filter
  const static uint8_t MHDR_REGISTER_ADDRESS = 0x2B;
  const static uint8_t NHDR_REGISTER_ADDRESS = 0x2C;
  const static uint8_t NCLR_REGISTER_ADDRESS = 0x2D;
  const static uint8_t FDLR_REGISTER_ADDRESS = 0x2E;

  // falling filter
  const static uint8_t MHDF_REGISTER_ADDRESS = 0x2F;
  const static uint8_t NHDF_REGISTER_ADDRESS = 0x30;
  const static uint8_t NCLF_REGISTER_ADDRESS = 0x31;
  const static uint8_t FDLF_REGISTER_ADDRESS = 0x32;

  // touched filter
  const static uint8_t NHDT_REGISTER_ADDRESS = 0x33;
  const static uint8_t NCLT_REGISTER_ADDRESS = 0x34;
  const static uint8_t FDLT_REGISTER_ADDRESS = 0x35;

  // proximity electrode touch sense baseline filters
  // rising filter
  const static uint8_t MHDPROXR_REGISTER_ADDRESS = 0x36;
  const static uint8_t NHDPROXR_REGISTER_ADDRESS = 0x37;
  const static uint8_t NCLPROXR_REGISTER_ADDRESS = 0x38;
  const static uint8_t FDLPROXR_REGISTER_ADDRESS = 0x39;

  // falling filter
  const static uint8_t MHDPROXF_REGISTER_ADDRESS = 0x3A;
  const static uint8_t NHDPROXF_REGISTER_ADDRESS = 0x3B;
  const static uint8_t NCLPROXF_REGISTER_ADDRESS = 0x3C;
  const static uint8_t FDLPROXF_REGISTER_ADDRESS = 0x3D;

  // touched filter
  const static uint8_t NHDPROXT_REGISTER_ADDRESS = 0x3E;
  const static uint8_t NCLPROXT_REGISTER_ADDRESS = 0x3F;
  const static uint8_t FDLPROXT_REGISTER_ADDRESS = 0x40;

  // // electrode touch and release thresholds
  const static uint8_t TOUCH_THRESHOLD0_REGISTER_ADDRESS = 0x41;
  const static uint8_t RELEASE_THRESHOLD0_REGISTER_ADDRESS = 0x42;

  // debounce settings
  const static uint8_t DEBOUNCE_REGISTER_ADDRESS = 0x5B;
  union DebounceConfiguration
  {
    struct Fields
    {
      uint8_t touch_debounce : 3;
      uint8_t space0 : 1;
      uint8_t release_debounce : 3;
      uint8_t space1 : 1;
    } fields;
    uint8_t uint8;
  };

  // // configuration registers
  const static uint8_t AFE1_REGISTER_ADDRESS = 0x5C;
  const static uint8_t AFE1_REGISTER_DEFAULT = 0x10;
  union AFE1Configuration
  {
    struct Fields
    {
      uint8_t charge_discharge_current : 6;
      uint8_t first_filter_iterations : 2;
    } fields;
    uint8_t uint8;
  };
  const static uint8_t AFE2_REGISTER_ADDRESS = 0x5D;
  const static uint8_t AFE2_REGISTER_DEFAULT = 0x24;
  uint8_t afe2_register_values_[DEVICE_COUNT_MAX];
  union AFE2Configuration
  {
    struct Fields
    {
      uint8_t sample_period : 3;
      uint8_t second_filter_iterations : 2;
      uint8_t charge_discharge_time : 3;
    } fields;
    uint8_t uint8;
  };
  const static uint8_t ECR_REGISTER_ADDRESS = 0x5E;
  union ElectrodeConfiguration
  {
    struct Fields
    {
      uint8_t electrode_enable : 4;
      uint8_t proximity_enable : 2;
      uint8_t calibration_lock : 2;
    } fields;
    uint8_t uint8;
  };

  // electrode currents
  const static uint8_t CDC0_REGISTER_ADDRESS = 0x5F;

  // electrode charge times
  const static uint8_t CDT0_REGISTER_ADDRESS = 0x6C;
  const static uint8_t CDT_DIVISOR = 2;
  const static uint8_t CDT_OFFSET = 4;

  // GPIO
  const static uint8_t CTL0_REGISTER_ADDRESS = 0x73;
  const static uint8_t CTL1_REGISTER_ADDRESS = 0x74;
  const static uint8_t DAT_REGISTER_ADDRESS = 0x75;
  const static uint8_t DIR_REGISTER_ADDRESS = 0x76;
  const static uint8_t EN_REGISTER_ADDRESS = 0x77;
  const static uint8_t SET_REGISTER_ADDRESS = 0x78;
  const static uint8_t CLR_REGISTER_ADDRESS = 0x79;
  const static uint8_t TOG_REGISTER_ADDRESS = 0x7A;

  // auto-config
  const static uint8_t ACCR0_REGISTER_ADDRESS = 0x7B;
  const static uint8_t ACCR1_REGISTER_ADDRESS = 0x7C;
  const static uint8_t USL_REGISTER_ADDRESS = 0x7D;
  const static uint8_t LSL_REGISTER_ADDRESS = 0x7E;
  const static uint8_t TL_REGISTER_ADDRESS = 0x7F;

  // soft reset
  const static uint8_t SRST_REGISTER_ADDRESS = 0x80;
  const static uint8_t SRST_REGISTER_VALUE = 0x63;

  // PWM
  const static uint8_t PWM0 = 0x81;
  const static uint8_t PWM1 = 0x82;
  const static uint8_t PWM2 = 0x83;
  const static uint8_t PWM3 = 0x84;

  struct Settings
  {
    uint8_t touch_threshold;
    uint8_t release_threshold;

    // general electrode touch sense baseline filters
    // rising filter
    uint8_t MHDR;
    uint8_t NHDR;
    uint8_t NCLR;
    uint8_t FDLR;

    // falling filter
    uint8_t MHDF;
    uint8_t NHDF;
    uint8_t NCLF;
    uint8_t FDLF;

    // touched filter
    uint8_t NHDT;
    uint8_t NCLT;
    uint8_t FDLT;

    // proximity electrode touch sense baseline filters
    // rising filter
    uint8_t MHDPROXR;
    uint8_t NHDPROXR;
    uint8_t NCLPROXR;
    uint8_t FDLPROXR;

    // falling filter
    uint8_t MHDPROXF;
    uint8_t NHDPROXF;
    uint8_t NCLPROXF;
    uint8_t FDLPROXF;

    // touched filter
    uint8_t NHDPROXT;
    uint8_t NCLPROXT;
    uint8_t FDLPROXT;

    // debounce settings
    uint8_t DEBOUNCE;

    // configuration registers
    uint8_t ECR;

    // auto-configuration registers
    uint8_t ACCR0;
    uint8_t ACCR1;
    uint8_t USL;
    uint8_t LSL;
    uint8_t TL;

    // default values in initialisation list
    Settings():
      touch_threshold(40),
      release_threshold(20),
      MHDR(0x01),
      NHDR(0x01),
      NCLR(0x10),
      FDLR(0x20),
      MHDF(0x01),
      NHDF(0x01),
      NCLF(0x10),
      FDLF(0x20),
      NHDT(0x01),
      NCLT(0x10),
      FDLT(0xFF),
      MHDPROXR(0x0F),
      NHDPROXR(0x0F),
      NCLPROXR(0x00),
      FDLPROXR(0x00),
      MHDPROXF(0x01),
      NHDPROXF(0x01),
      NCLPROXF(0xFF),
      FDLPROXF(0xFF),
      NHDPROXT(0x00),
      NCLPROXT(0x00),
      FDLPROXT(0x00),
      DEBOUNCE(0x11),
      ECR(0xC0), // default to fast baseline startup, no electrodes enabled, no proximity
      ACCR0(0x00),
      ACCR1(0x00),
      USL(0x00),
      LSL(0x00),
      TL(0x00)
    {};
  };
  const Settings default_settings_;

  ElectrodeConfiguration electrode_configuration_backup_;
  void pauseChannels(DeviceAddress device_address);
  void resumeChannels(DeviceAddress device_address);

  uint8_t channelToDeviceIndex(uint8_t channel);
  DeviceAddress channelToDeviceAddress(uint8_t channel);
  uint8_t channelToDeviceChannel(uint8_t channel);

  // applies a complete array of settings from a
  // Settings variable useful if you want to do a bulk setup of the device
  void applySettings(DeviceAddress device_address,
    const Settings & settings);

  void clearOverCurrentFlag(DeviceAddress device_address);
};

#include "MPR121/MPR121Definitions.h"

#endif
