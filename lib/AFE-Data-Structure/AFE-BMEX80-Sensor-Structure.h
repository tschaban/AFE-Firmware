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
#include <AFE-TEMPERATURE-Structure.h>
#include <AFE-HUMIDITY-Structure.h>
#include <AFE-PRESSURE-Structure.h>

struct BMEX80_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  DOMOTICZ_BASIC_CONFIG gasResistance;
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG pressure;
  DOMOTICZ_BASIC_CONFIG humidity;
};



struct BMEX80_OVERSAMPLING_SETTINGS {
  uint8_t temperature;
  uint8_t humidity;
  uint8_t pressure;
};

struct BME680_GAS_SETTINGS {
  uint16_t temperature;
  uint16_t duration;
};

struct BMEX80_DATA_CORRECTIONS {
  float temperature;
  float humidity;
  float pressure;
  float gasResistance;
};

struct BMEX80_IAQ_ITEM {
  float value;
  uint8_t accuracy;
  uint8_t rating;
  uint8_t unit;
};

struct BMEX80_IAQ {
  BMEX80_IAQ_ITEM iaq;
  BMEX80_IAQ_ITEM staticIaq;
  BMEX80_IAQ_ITEM co2Equivalent;
  BMEX80_IAQ_ITEM breathVocEquivalent;
  BMEX80_IAQ_ITEM gasResistance;
};

struct BMEX80 {
  char name[17];
  uint8_t type;
  uint32_t interval;
  uint8_t i2cAddress;
  float altitude;
  float seaLevelPressure;
  uint8_t temperatureUnit;
  uint8_t powerMode;
  uint8_t resolution; // for BMP180 only
  BMEX80_DOMOTICZ domoticz;
  MQTT_BASIC_CONFIG mqtt;
  BMEX80_DATA_CORRECTIONS corrections;
  BME680_GAS_SETTINGS heater; // for BME680 only
};

struct BMEX80_DATA {
  TEMPERATURE temperature;
  HUMIDITY humidity;
  TEMPERATURE dewPoint;
  PRESSURE pressure;
  BMEX80_IAQ airQuality;
};

#endif
