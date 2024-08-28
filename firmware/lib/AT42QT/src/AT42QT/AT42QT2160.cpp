// ----------------------------------------------------------------------------
// AT42QT2160.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AT42QT2160.h"


AT42QT2160::Status AT42QT2160::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT2160::STATUS,status.bytes);
  return status;
}

bool AT42QT2160::anyTouched(Status status)
{
  return status.keys;
}

bool AT42QT2160::touched(Status status,
  uint8_t key)
{
  return bitRead(status.keys,key);
}
