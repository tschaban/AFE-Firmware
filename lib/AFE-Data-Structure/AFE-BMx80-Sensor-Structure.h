/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_BMx80_Structure_h
#define _AFE_BMx80_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct BMx80_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  DOMOTICZ_BASIC_CONFIG gasResistance;
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG pressure;
  DOMOTICZ_BASIC_CONFIG humidity;
};

struct BMx80 {
  uint8_t type;
  uint16_t interval;
  uint8_t i2cAddress;
  BMx80_DOMOTICZ domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

struct BMx80_DATA {
  float temperature;
  float humidity;
  float pressure;
  float gasResistance;
};

#endif
