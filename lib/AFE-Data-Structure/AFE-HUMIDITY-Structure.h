/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_HUMIDITY_Structure_h
#define _AFE_HUMIDITY_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct HUMIDITY {
  float correction;
};

#endif
