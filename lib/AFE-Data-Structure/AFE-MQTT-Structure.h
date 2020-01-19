/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MQTT_Structure_h
#define _AFE_MQTT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
struct MQTT_MESSAGE {
  char *topic;
  byte *content;
  uint16_t length;
};
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
struct MQTT_BASIC_CONFIG {
  char topic[65];
};

/* Types of items in MQTT Topics cache */
typedef enum {
  AFE_MQTT_DEVICE_RELAY = 0,
  AFE_MQTT_DEVICE_ADC = 1,
  AFE_MQTT_DEVICE_SWITCH = 2,
} afe_mqtt_standard_device_type_t;

/* MQTT Topics cache structure */
struct MQTT_TOPICS_CACHE {
  MQTT_BASIC_CONFIG message;
  uint8_t id;
  afe_mqtt_standard_device_type_t type;
};
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

struct MQTT {
  char host[33];
  char ip[16];
  uint16_t port;
  char user[33];
  char password[33];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG lwt;
#else
  MQTT_BASIC_CONFIG lwt;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
};

#endif
