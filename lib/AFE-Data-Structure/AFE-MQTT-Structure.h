/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
struct MQTT_MESSAGE {
  char *topic;
  byte *content;
  uint16_t length;
};
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
struct MQTT_BASIC_CONFIG {
  char topic[65];
};

#endif

struct MQTT {
  char host[33];
  char ip[16];
  uint16_t port;
  char user[33];
  char password[33];
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  MQTT_BASIC_CONFIG lwt;
#endif
};

#endif
