/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_NETWORK_Structure_h
#define _AFE_NETWORK_Structure_h

#include <arduino.h>

struct NETWORK_SETTING {
  char ssid[33];
  char password[33];
  uint8_t isDHCP;
  char ip[16];
  char gateway[16];
  char subnet[16];
  char dns1[16];
  char dns2[16];
};

struct NETWORK {
  NETWORK_SETTING primary;
  NETWORK_SETTING secondary;
  uint8_t noConnectionAttempts;
  uint8_t waitTimeConnections;
  uint8_t waitTimeSeries;
  uint8_t noFailuresToSwitchNetwork;
#if !defined(ESP32)
  uint8_t radioMode;
  float outputPower;
#endif
};

#endif
