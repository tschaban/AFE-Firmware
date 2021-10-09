/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_BMEX80_Structure_h
#define _AFE_BMEX80_Structure_h

#include <AFE-Air-Item-Structure.h>
#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
struct BMEX80_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG humidity;
  DOMOTICZ_BASIC_CONFIG dewPoint;
  DOMOTICZ_BASIC_CONFIG heatIndex;
  DOMOTICZ_BASIC_CONFIG pressure;
  DOMOTICZ_BASIC_CONFIG relativePressure;
  DOMOTICZ_BASIC_CONFIG temperatureHumidity;
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  DOMOTICZ_BASIC_CONFIG gasResistance;
  DOMOTICZ_BASIC_CONFIG iaq;
  DOMOTICZ_BASIC_CONFIG staticIaq;
  DOMOTICZ_BASIC_CONFIG co2Equivalent;
  DOMOTICZ_BASIC_CONFIG breathVocEquivalent;
  DOMOTICZ_BASIC_CONFIG perception;
  DOMOTICZ_BASIC_CONFIG comfort;
  DOMOTICZ_BASIC_CONFIG absoluteHumidity;
};
#endif

struct BMEX80_OVERSAMPLING_SETTINGS {
  uint8_t temperature;
  uint8_t humidity;
  uint8_t pressure;
};

struct BMEX80 {
  char name[17];
  uint8_t type;
  uint32_t interval;
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif  
  uint8_t i2cAddress;
  uint8_t resolution; // for BMP180 only
  AIR_ITEM_SETTING temperature;
  AIR_ITEM_SETTING humidity;
  AIR_ITEM_SETTING pressure;
  int16_t altitude;
  float seaLevelPressure;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  BMEX80_DOMOTICZ domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

struct BMEX80_DATA {
  AIR_ITEM temperature;
  AIR_ITEM humidity;
  AIR_ITEM dewPoint;
  AIR_ITEM heatIndex;
  AIR_ITEM pressure;
  AIR_ITEM relativePressure;
  AIR_ITEM iaq;
  AIR_ITEM staticIaq;
  AIR_ITEM co2Equivalent;
  AIR_ITEM breathVocEquivalent;
  AIR_ITEM gasResistance;
};

#endif
