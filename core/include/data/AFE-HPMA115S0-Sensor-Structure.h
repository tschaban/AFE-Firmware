/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_HPMA115S0_Structure_h
#define _AFE_HPMA115S0_Structure_h

#include <arduino.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
struct HPMA115S0_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG pm25;
  DOMOTICZ_BASIC_CONFIG pm10;
  DOMOTICZ_BASIC_CONFIG whoPM10Norm;
  DOMOTICZ_BASIC_CONFIG whoPM25Norm;
};
#endif

struct HPMA115S0 {
  char name[17];
  uint32_t interval;
  uint16_t timeToMeasure;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  HPMA115S0_DOMOTICZ domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
  float whoPM10Norm;
  float whoPM25Norm;
};

struct HPMA115S0_DATA {
  float pm10;
  float pm25;
  float whoPM10Norm;
  float whoPM25Norm;
};

#endif
