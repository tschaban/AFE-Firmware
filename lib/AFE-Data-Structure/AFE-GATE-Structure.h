/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_GATE_Structure_h
#define _AFE_GATE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

struct GATES_CURRENT_STATE {
  uint8_t state[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
};

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
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
