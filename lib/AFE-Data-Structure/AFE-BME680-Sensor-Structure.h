/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_BME680_Structure_h
#define _AFE_BME680_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct BME680 {
  uint8_t type;
  uint16_t interval;
  boolean sendOnlyChanges;
  unsigned long temperatureIdx;
  unsigned long humidityIdx;
  unsigned long pressureIdx;
  unsigned long gasIdx;
  unsigned long temperatureAndHumidityIdx;
};

#endif
