/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DHT_Structure_h
#define _AFE_DHT_Structure_h

#include <AFE-HUMIDITY-Structure.h>
#include <AFE-TEMPERATURE-Structure.h>

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct DH {
  uint8_t gpio;
  uint8_t type;
  TEMPERATURE temperature;
  HUMIDITY humidity;
  uint32_t interval;
  boolean sendOnlyChanges;
  boolean publishHeatIndex;
  boolean publishDewPoint;
  unsigned long temperatureIdx;
  unsigned long humidityIdx;
  unsigned long temperatureAndHumidityIdx;
};

#endif
