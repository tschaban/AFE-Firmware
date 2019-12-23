/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_HPMA115S0_Structure_h
#define _AFE_HPMA115S0_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-DOMOTICZ-Structure.h>
#include <AFE-MQTT-Structure.h>

struct HPMA115S0_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG pm25;
  DOMOTICZ_BASIC_CONFIG pm10;
};

struct HPMA115S0 {
  char name[17];
  uint32_t interval;
  uint16_t timeToMeasure;
  HPMA115S0_DOMOTICZ domoticz;
  MQTT_BASIC_CONFIG mqtt;
};

struct HPMA115S0_DATA {
  uint16_t pm10;
  uint16_t pm25;
};

#endif
