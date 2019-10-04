/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_AS3935_Structure_h
#define _AFE_AS3935_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-Configuration.h>

struct AS3935 {
  uint8_t i2cAddress;
  uint8_t irqGPIO;
  boolean setNoiseFloorAutomatically;
  int8_t noiseFloor;
  bool indoor;
  uint8_t unit; // Distance
  DOMOTICZ_BASIC_CONFIG domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

struct AS3935_DATA {
  uint8_t type;
  uint8_t distance;
};

#endif
