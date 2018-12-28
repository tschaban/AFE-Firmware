/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_BMEx80_Structure_h
#define _AFE_BMEx80_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct BMEx80_DOMOTICZ {
  unsigned long temperatureHumidityPressure;
  unsigned long gasResistance;
  unsigned long temperature;
  unsigned long pressure;
  unsigned long humidity;
};

struct BMEx80 {
  uint8_t type;
  uint16_t interval;
  boolean sendOnlyChanges;
  BMEx80_DOMOTICZ idx;
};

struct BMEx80_DATA {
  float temperature;
  float humidity;
  float pressure;
  float gasResistance;
};

#endif
