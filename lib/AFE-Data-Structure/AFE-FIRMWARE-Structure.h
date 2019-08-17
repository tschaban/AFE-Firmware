/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_FIRMWARE_Structure_h
#define _AFE_FIRMWARE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct FIRMWARE {
  char version[10];
  uint8_t type;
  uint8_t autoUpgrade;
  char upgradeURL[120];
};

#endif
