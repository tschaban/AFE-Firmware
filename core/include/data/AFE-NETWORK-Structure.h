/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_NETWORK_Structure_h
#define _AFE_NETWORK_Structure_h

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
};

#endif
