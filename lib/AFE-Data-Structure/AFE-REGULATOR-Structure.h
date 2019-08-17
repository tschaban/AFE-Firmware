/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_REGULATOR_Structure_h
#define _AFE_REGULATOR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define THERMOSTAT_REGULATOR 0
#define HUMIDISTAT_REGULATOR 1

struct REGULATOR {
  boolean enabled;
  float turnOn;
  boolean turnOnAbove;
  float turnOff;
  boolean turnOffAbove;
};

#endif
