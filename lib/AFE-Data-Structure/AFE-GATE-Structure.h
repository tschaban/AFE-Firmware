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

#define GATE_OPEN 0           // Open
#define GATE_CLOSED 1         // Closed
#define GATE_PARTIALLY_OPEN 2 // Being opened
#define GATE_UNKNOWN 9        // Unknown state

struct GATE {
  uint8_t relayId;
  uint8_t contactronId[2];
  uint8_t state[4];
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

#endif
