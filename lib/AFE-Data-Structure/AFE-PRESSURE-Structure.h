/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_PRESSURE_Structure_h
#define _AFE_PRESSURE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct PRESSURE {
  float value;
  float relative;
  float correction;
  uint8_t unit;
};

#endif
