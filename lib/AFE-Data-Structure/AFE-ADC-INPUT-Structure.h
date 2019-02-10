/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_ADC_INPUT_Structure_h
#define _AFE_ADC_INPUT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct ADCINPUT_DOMOTICZ {
  unsigned long raw;
  unsigned long percent;
  unsigned long voltage;
};

struct ADCINPUT_DATA {
  uint16_t raw;
  float percent;
  double voltage;
};

struct ADCINPUT {
  uint8_t gpio;
  uint16_t interval;
  uint8_t numberOfSamples;
  ADCINPUT_DOMOTICZ idx;
};

#endif
