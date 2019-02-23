/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_RELAY_Structure_h
#define _AFE_RELAY_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-REGULATOR-Structure.h>

/* Relay states */
#define RELAY_ON 1
#define RELAY_OFF 0

struct RELAY_DEFAULTS {
  uint8_t powerOn;
  uint8_t MQTTConnected;
};

struct RELAY {
  uint8_t gpio;
  char name[16];
  float timeToOff;
  uint8_t ledID;
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
  RELAY_DEFAULTS state;
  REGULATOR thermostat;
  REGULATOR humidistat;
  float thermalProtection;
};

#endif
