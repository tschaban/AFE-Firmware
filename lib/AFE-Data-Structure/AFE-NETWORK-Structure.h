/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_NETWORK_Structure_h
#define _AFE_NETWORK_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct NETWORK {
  char ssid[32];
  char password[32];
  uint8_t isDHCP;
  IPAddress ip;
  IPAddress gateway;
  IPAddress subnet;
  uint8_t noConnectionAttempts;
  uint8_t waitTimeConnections;
  uint8_t waitTimeSeries;
};

#endif
