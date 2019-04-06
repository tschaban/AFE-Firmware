/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_SWITCH_Structure_h
#define _AFE_SWITCH_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define SWITCH_TYPE_MONO 0 // Mono stable switch
#define SWITCH_TYPE_BI 1   // Bistable switch

#define SWITCH_FUNCTIONALITY_NONE 0
#define SWITCH_FUNCTIONALITY_MULTI 1
#define SWITCH_FUNCTIONALITY_RELAY 2

struct SWITCH {
  uint8_t gpio;
  uint8_t type;
  uint16_t sensitiveness;
  uint8_t functionality;
  uint8_t relayID;
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

#endif
