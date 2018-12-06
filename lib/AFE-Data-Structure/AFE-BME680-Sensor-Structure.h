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

struct BME680_DOMOTICZ {
  unsigned long temperatureAndHumidity;
  unsigned long pressure;
  unsigned long gasResistance;
};

struct BME680 {
  uint8_t type;
  uint16_t interval;
  boolean sendOnlyChanges;
  BME680_DOMOTICZ idx;
};

struct BME680_DATA {
  float temperature;
  float humidity;
  float pressure;
  float gasResistance;
};

#endif
