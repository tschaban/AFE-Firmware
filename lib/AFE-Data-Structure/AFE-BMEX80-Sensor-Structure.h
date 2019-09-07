/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_BMEX80_Structure_h
#define _AFE_BMEX80_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>

struct BMEX80_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  DOMOTICZ_BASIC_CONFIG gasResistance;
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG pressure;
  DOMOTICZ_BASIC_CONFIG humidity;
};

struct BMEX80 {
  char name[17];
  uint8_t type;
  uint32_t interval;
  uint8_t i2cAddress;
  BMEX80_DOMOTICZ domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

struct BMEX80_DATA {
  float temperature;
  float humidity;
  float pressure;
  float gasResistance;
};

#endif
