/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_DOMOTICZ_Structure_h
#define _AFE_DOMOTICZ_Structure_h

#include <AFE-Configuration.h>
#include <Arduino.h>

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-COLOR-Structure.h>
#endif

struct DOMOTICZ_BASIC_CONFIG {
  uint32_t idx = AFE_DOMOTICZ_DEFAULT_IDX;
};

struct DOMOTICZ_MQTT_COMMAND {
  DOMOTICZ_BASIC_CONFIG domoticz;
  uint8_t nvalue;
  char svalue[AFE_CONFIG_MQTT_CMD_SVALUE_LENGTH];
#ifdef AFE_CONFIG_HARDWARE_CLED
  CLED_PARAMETERS led;
#endif
};

typedef enum {
#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFE_DOMOTICZ_DEVICE_RELAY = 0,
#endif // AFE_CONFIG_HARDWARE_RELAY
#ifdef AFE_CONFIG_HARDWARE_GATE
  AFE_DOMOTICZ_DEVICE_GATE = 1,
#endif // AFE_CONFIG_HARDWARE_GATE
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  AFE_DOMOTICZ_DEVICE_REGULATOR = 2,
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  AFE_DOMOTICZ_DEVICE_THERMAL_PROTECTOR = 3,
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef AFE_CONFIG_HARDWARE_CLED
  AFE_DOMOTICZ_DEVICE_CLED = 4,
  AFE_DOMOTICZ_DEVICE_CLED_EFFECT = 5,
#endif // AFE_CONFIG_HARDWARE_CLED
} afe_domoticz_device_type_t;

struct DOMOTICZ_IDX_CACHE {
  DOMOTICZ_BASIC_CONFIG domoticz;
  uint8_t id;
  afe_domoticz_device_type_t type;
};

struct DOMOTICZ {
  uint8_t protocol;
  char host[41];
  uint16_t port;
  char user[33];
  char password[33];
};

#endif
