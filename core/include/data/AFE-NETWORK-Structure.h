/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_NETWORK_Structure_h
#define _AFE_NETWORK_Structure_h

#include <arduino.h>

struct NETWORK {
  char ssid[33];
  char password[33];
  uint8_t isDHCP;
  char ip[16];
  char gateway[16];
  char subnet[16];
  uint8_t noConnectionAttempts;
  uint8_t waitTimeConnections;
  uint8_t waitTimeSeries;
  char ssidBackup[33];
  char passwordBackup[33];
  uint8_t isDHCPBackup;
  char ipBackup[16];
  char gatewayBackup[16];
  char subnetBackup[16];
  uint8_t noFailuresToSwitchNetwork;
};

#endif
