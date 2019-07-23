/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DOMOTICZ_Structure_h
#define _AFE_DOMOTICZ_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-PROTOCOL-Structure.h>
#include <IPAddress.h>

struct DOMOTICZ_BASIC_CONFIG {
  uint32_t idx;
};

struct DOMOTICZ {
  uint8_t protocol;
  char host[41];
  uint16_t port;
  char user[33];
  char password[33];
};

#endif
