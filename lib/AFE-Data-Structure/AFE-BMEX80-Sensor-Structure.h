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
#include <AFE-Air-Item-Structure.h>

struct BMEX80_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  DOMOTICZ_BASIC_CONFIG gasResistance;
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG pressure;
  DOMOTICZ_BASIC_CONFIG relativePressure;
  DOMOTICZ_BASIC_CONFIG humidity;
  DOMOTICZ_BASIC_CONFIG iaq;
  DOMOTICZ_BASIC_CONFIG staticIaq;
  DOMOTICZ_BASIC_CONFIG co2Equivalent;
  DOMOTICZ_BASIC_CONFIG breathVocEquivalent;
};



struct BMEX80_OVERSAMPLING_SETTINGS {
  uint8_t temperature;
  uint8_t humidity;
  uint8_t pressure;
};

struct BMEX80 {
  char name[17];
  uint8_t type;
  uint32_t interval;
  uint8_t i2cAddress;
  uint8_t powerMode;
  uint8_t resolution; // for BMP180 only
  AIR_ITEM_SETTING temperature;
  AIR_ITEM_SETTING humidity;
  AIR_ITEM_SETTING pressure;
  float altitude = 211;
  float seaLevelPressure;
  BMEX80_DOMOTICZ domoticz;
  MQTT_BASIC_CONFIG mqtt;

};

struct BMEX80_DATA {
  AIR_ITEM temperature;
  AIR_ITEM humidity;
  AIR_ITEM dewPoint;
  AIR_ITEM pressure;
  AIR_ITEM relativePressure;
  AIR_ITEM iaq;
  AIR_ITEM staticIaq;
  AIR_ITEM co2Equivalent;
  AIR_ITEM breathVocEquivalent;
  AIR_ITEM gasResistance;
};

#endif
