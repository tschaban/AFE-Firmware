/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_REGULATOR_Structure_h
#define _AFE_REGULATOR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct REGULATOR {
  boolean enabled;
  float turnOn;
  boolean turnOnAbove;
  float turnOff;
  boolean turnOffAbove;
};

#endif
