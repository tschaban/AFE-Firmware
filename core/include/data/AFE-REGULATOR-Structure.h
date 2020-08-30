/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_REGULATOR_Structure_h
#define _AFE_REGULATOR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


enum REGULATOR_TYPE {
  REGULATOR_THERMOSTAT = 0,
  REGULATOR_HUMIDISTAT = 1
} regulator_t;

struct REGULATOR {
  uint8_t relayId;
  uint8_t sensorId;
  boolean enabled;
  float turnOn;
  boolean turnOnAbove;
  float turnOff;
  boolean turnOffAbove;
};

#endif
