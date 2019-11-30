/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_AIR_ITEM_Structure_h
#define _AFE_AIR_ITEM_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct AIR_ITEM_SETTING {
  float correction;
  uint8_t unit;
};

struct AIR_ITEM {
  float value;
  uint8_t accuracy;
  uint8_t rating;
};

#endif
