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

struct HPMA115S0 {
  uint16_t interval;
  boolean sendOnlyChanges;
  uint16_t timeToMeasure;
  unsigned long idxPM25;
  unsigned long idxPM10;
};

#endif
