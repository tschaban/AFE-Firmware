/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_GATE_Structure_h
#define _AFE_GATE_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_GATE
#include <arduino.h>

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
  DOMOTICZ_BASIC_CONFIG domoticz; // IDX for gate state
  DOMOTICZ_BASIC_CONFIG domoticzControl; // IDX for gate controll
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_GATE

#endif // _AFE_GATE_Structure_h
