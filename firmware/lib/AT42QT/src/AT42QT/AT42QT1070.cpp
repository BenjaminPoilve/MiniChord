// ----------------------------------------------------------------------------
// AT42QT1070.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AT42QT1070.h"


AT42QT1070::Status AT42QT1070::getStatus()
{
  Status status;
  read(RegisterAddresses::AT42QT1070::STATUS,status.bytes,STATUS_SIZE);
  return status;
}

bool AT42QT1070::calibrating()
{
  Status status = getStatus();
  return status.calibrating;
}

bool AT42QT1070::anyTouched(Status status)
{
  return status.keys;
}

bool AT42QT1070::touched(Status status,
  uint8_t key)
{
  return bitRead(status.keys,key);
}
