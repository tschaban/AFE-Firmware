/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct MQTT_MESSAGE {
  char *topic;
  byte *content;
  uint16_t length;
};


struct MQTT_BASIC_CONFIG {
  char topic[65];
};

struct MQTT {
  char host[33];
  char ip[16];
  uint16_t port;
  char user[33];
  char password[33];
  MQTT_BASIC_CONFIG lwt;
};

#endif
