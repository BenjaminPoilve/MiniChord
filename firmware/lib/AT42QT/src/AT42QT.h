// ----------------------------------------------------------------------------
// AT42QT.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AT42QT_H
#define AT42QT_H
#include <Arduino.h>
#include <Wire.h>


template<typename RegisterAddress>
class AT42QT
{
public:
  AT42QT(uint8_t device_address,
    uint8_t chip_id,
    uint8_t key_count,
    TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1);

  void begin();
  void hardReset();
  uint8_t getChipId();
  bool communicating();

  typedef void (*Callback)();
  void attachChangeCallback(const Callback callback);

  void triggerCalibration();
  void reset();

private:
  const uint8_t device_address_;
  const uint8_t chip_id_;
  const uint8_t key_count_;
  TwoWire * const wire_ptr_;
  const int8_t change_pin_;
  const int8_t reset_pin_;

  const static uint8_t BITS_PER_BYTE = 8;
  const static uint8_t BYTE_MAX = 0xFF;

protected:
  const uint8_t NONZERO_VALUE = 1;

  template<typename Data>
  void write(RegisterAddress register_address,
    const Data & data);

  template<typename Data>
  void write(RegisterAddress register_address,
    const Data & data,
    uint8_t data_size);

  template<typename Data>
  void writeKey(RegisterAddress base_register_address,
    uint8_t key,
    const Data & data);

  template<typename Data>
  void read(RegisterAddress register_address,
    Data & data);

  template<typename Data>
  void read(RegisterAddress register_address,
    Data & data,
    uint8_t data_size);

  template<typename Data>
  void readKey(RegisterAddress base_register_address,
    uint8_t key,
    Data & data);

private:
  uint8_t verifyKey(uint8_t key);

  template<typename Data>
  void writeRegisterBlock(uint8_t register_address_number,
    const Data & data,
    uint8_t data_size);

  template<typename Data>
  void readRegisterBlock(uint8_t register_address_number,
    Data & data,
    uint8_t data_size);


};
#include "AT42QT/AT42QTDefinitions.h"
#endif
