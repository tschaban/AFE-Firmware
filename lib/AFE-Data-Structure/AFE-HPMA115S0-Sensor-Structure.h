/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_HPMA115S0_Structure_h
#define _AFE_HPMA115S0_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define HPMA115S0_TYPE_PM25 0
#define HPMA115S0_TYPE_PM10 1

struct HPMA115S0_DOMOTICZ {
  unsigned long pm25;
  unsigned long pm10;
};

struct HPMA115S0 {
  uint16_t interval;
  uint16_t timeToMeasure;
  HPMA115S0_DOMOTICZ idx;
};

struct HPMA115S0_DATA {
  uint16_t pm10;
  uint16_t pm25;
};

#endif
