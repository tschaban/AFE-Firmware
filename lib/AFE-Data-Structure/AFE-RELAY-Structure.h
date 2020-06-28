/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_RELAY_Structure_h
#define _AFE_RELAY_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct RELAY_DEFAULTS {
  uint8_t powerOn;
  uint8_t MQTTConnected;
};

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
struct THERMAL_PROTECTION {
  float temperature;
  uint8_t sensorId;
};
#endif

struct RELAY {
  uint8_t gpio;
  char name[17];
  float timeToOff;
  uint8_t ledID;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
  RELAY_DEFAULTS state;
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  THERMAL_PROTECTION thermalProtection;
#endif
};

#endif
