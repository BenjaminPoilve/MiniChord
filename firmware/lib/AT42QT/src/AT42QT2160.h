// ----------------------------------------------------------------------------
// AT42QT2160.h
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AT42QT2160_H
#define AT42QT2160_H
#include "AT42QT.h"
#include "AT42QT/RegisterAddresses.h"


class AT42QT2160 : public AT42QT<RegisterAddresses::AT42QT2160>
{
public:
  static const uint8_t DEVICE_ADDRESS = 0x0D;
  static const uint8_t CHIP_ID = 0x11;
  static const uint8_t KEY_COUNT = 16;

  AT42QT2160(TwoWire & wire=Wire,
    int8_t change_pin=-1,
    int8_t reset_pin=-1) :
  AT42QT<RegisterAddresses::AT42QT2160>(DEVICE_ADDRESS,
    CHIP_ID,
    KEY_COUNT,
    wire,
    change_pin,
    reset_pin)
  {
  }

  union Status
  {
    struct
    {
      uint64_t slider : 1;
      uint64_t common_change : 1;
      uint64_t space0 : 4;
      uint64_t cycle_overrun : 1;
      uint64_t was_reset : 1;
      uint64_t keys : 16;
      uint64_t slider_position : 8;
      uint64_t gpio : 8;
      uint64_t space1 : 8;
    };
    uint64_t bytes;
  };
  static const uint8_t STATUS_SIZE = 5;
  Status getStatus();

  bool anyTouched(Status status);
  bool touched(Status status,
    uint8_t key);

};

#endif
