/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_GATE_Structure_h
#define _AFE_GATE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>

struct GATE_CONTACTRONS {
  uint8_t id[2];
};

struct GATE_STATES {
  uint8_t state[4];
};

struct GATE {
  uint8_t relayId;  // Relay ID - controlling the gate
  uint8_t switchId; // Switch ID - triggering the gate
  GATE_CONTACTRONS contactron;
  GATE_STATES states;
  char name[17];
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

#endif
