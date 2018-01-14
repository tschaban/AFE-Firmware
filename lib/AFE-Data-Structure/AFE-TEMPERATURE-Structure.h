/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_TEMPERATURE_Structure_h
#define _AFE_TEMPERATURE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct TEMPERATURE {
  float correction;
  unsigned int interval;
  uint8_t unit;
};

#endif
