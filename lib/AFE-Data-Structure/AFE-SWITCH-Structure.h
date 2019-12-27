/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_SWITCH_Structure_h
#define _AFE_SWITCH_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct SWITCH {
  uint8_t gpio;
  uint8_t type;
  uint16_t sensitiveness;
  uint8_t functionality;
  uint8_t relayID;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#endif
#ifdef AFE_CONFIG_API_MQTT_ENABLED
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
