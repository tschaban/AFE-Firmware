/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_DOMOTICZ_Structure_h
#define _AFE_DOMOTICZ_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-PROTOCOL-Structure.h>
#include <IPAddress.h>

struct DOMOTICZ {
  uint8_t protocol;
  char host[42];
  uint16_t port;
  char user[32];
  char password[32];
};

#endif
