/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_BH1750_Structure_h
#define _AFE_BH1750_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct BH1750 {
  uint16_t interval;
  uint8_t i2cAddress;
  uint8_t mode;
  unsigned long idx;
};

#endif
