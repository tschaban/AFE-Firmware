/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_TEMPERATURE_Structure_h
#define _AFE_TEMPERATURE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct TEMPERATURE {
  float correction;
  uint8_t unit;
};

#endif
