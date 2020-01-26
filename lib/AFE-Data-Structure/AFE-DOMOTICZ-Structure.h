/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DOMOTICZ_Structure_h
#define _AFE_DOMOTICZ_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>

struct DOMOTICZ_BASIC_CONFIG {
  uint32_t idx = AFE_DOMOTICZ_DEFAULT_IDX;
};

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
struct DOMOTICZ_MQTT_COMMAND {
  DOMOTICZ_BASIC_CONFIG domoticz;
  uint8_t nvalue;
};

typedef enum {
  AFE_DOMOTICZ_DEVICE_RELAY = 0
} afe_domoticz_device_type_t;


struct DOMOTICZ_IDX_CACHE {
  DOMOTICZ_BASIC_CONFIG domoticz;
  uint8_t id;
  afe_domoticz_device_type_t type; 
};
#endif //AFE_CONFIG_API_PROCESS_REQUESTS


struct DOMOTICZ {
  uint8_t protocol;
  char host[41];
  uint16_t port;
  char user[33];
  char password[33];
};

#endif
