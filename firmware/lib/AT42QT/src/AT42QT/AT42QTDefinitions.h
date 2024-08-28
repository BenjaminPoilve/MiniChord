// ----------------------------------------------------------------------------
// AT42QTDefinitions.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AT42QT_DEFINITIONS_H
#define AT42QT_DEFINITIONS_H


template<typename RegisterAddress>
AT42QT<RegisterAddress>::AT42QT(uint8_t device_address,
  uint8_t chip_id,
  uint8_t key_count,
  TwoWire & wire,
  int8_t change_pin,
  int8_t reset_pin) :
device_address_(device_address),
chip_id_(chip_id),
key_count_(key_count),
wire_ptr_(&wire),
change_pin_(change_pin),
reset_pin_(reset_pin)
{
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::begin()
{
  if(change_pin_ >= 0)
  {
    pinMode(change_pin_,INPUT_PULLUP);
  }

  if(reset_pin_ >= 0)
  {
    pinMode(reset_pin_,OUTPUT);
    digitalWrite(reset_pin_,HIGH);
  }

  wire_ptr_->begin();
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::hardReset()
{
  if(reset_pin_ < 0) return;
  digitalWrite(reset_pin_,LOW);
  delay(1);
  digitalWrite(reset_pin_,HIGH);
}

template<typename RegisterAddress>
uint8_t AT42QT<RegisterAddress>::getChipId()
{
  uint8_t chip_id;
  read(RegisterAddress::CHIP_ID,chip_id);
  return chip_id;
}

template<typename RegisterAddress>
bool AT42QT<RegisterAddress>::communicating()
{
  uint8_t chip_id = getChipId();
  return chip_id == chip_id_;
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::attachChangeCallback(const Callback callback)
{
  if(change_pin_ >= 0)
  {
    attachInterrupt(digitalPinToInterrupt(change_pin_),callback,FALLING);
  }
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::triggerCalibration()
{
  write(RegisterAddress::CALIBRATE,NONZERO_VALUE);
}

template<typename RegisterAddress>
void AT42QT<RegisterAddress>::reset()
{
  write(RegisterAddress::RESET,NONZERO_VALUE);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::write(RegisterAddress register_address,
  const Data & data)
{
  uint8_t data_size = sizeof(data);
  writeRegisterBlock(static_cast<uint8_t>(register_address),data,data_size);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::write(RegisterAddress register_address,
  const Data & data,
  uint8_t data_size)
{
  writeRegisterBlock(static_cast<uint8_t>(register_address),data,data_size);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::writeKey(RegisterAddress base_register_address,
  uint8_t key,
  const Data & data)
{
  uint8_t data_size = sizeof(data);
  key = verifyKey(key);
  uint8_t register_address_number = static_cast<uint8_t>(base_register_address);
  register_address_number += key * data_size;
  writeRegisterBlock(register_address_number,data,data_size);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::read(RegisterAddress register_address,
  Data & data)
{
  uint8_t data_size = sizeof(data);
  readRegisterBlock(static_cast<uint8_t>(register_address),data,data_size);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::read(RegisterAddress register_address,
  Data & data,
  uint8_t data_size)
{
  readRegisterBlock(static_cast<uint8_t>(register_address),data,data_size);
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::readKey(RegisterAddress base_register_address,
  uint8_t key,
  Data & data)
{
  uint8_t data_size = sizeof(data);
  key = verifyKey(key);
  uint8_t register_address_number = static_cast<uint8_t>(base_register_address);
  register_address_number += key * data_size;
  readRegisterBlock(register_address_number,data,data_size);
}

template<typename RegisterAddress>
uint8_t AT42QT<RegisterAddress>::verifyKey(uint8_t key)
{
  if (key >= key_count_)
  {
    key = key_count_ - 1;
  }
  return key;
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::writeRegisterBlock(uint8_t register_address_number,
  const Data & data,
  uint8_t data_size)
{
  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(register_address_number);
  for (uint8_t byte_n=0; byte_n<data_size; ++byte_n)
  {
    wire_ptr_->write((data >> (BITS_PER_BYTE * byte_n)) & BYTE_MAX);
  }
  wire_ptr_->endTransmission();
}

template<typename RegisterAddress>
template<typename Data>
void AT42QT<RegisterAddress>::readRegisterBlock(uint8_t register_address_number,
  Data & data,
  uint8_t data_size)
{
  wire_ptr_->beginTransmission(device_address_);
  wire_ptr_->write(register_address_number);
  wire_ptr_->endTransmission(false);

  wire_ptr_->requestFrom(device_address_,data_size);
  data = 0;
  for (uint8_t byte_n=0; byte_n<data_size; ++byte_n)
  {
    data |= (wire_ptr_->read()) << (BITS_PER_BYTE * byte_n);
  }
}

#endif
