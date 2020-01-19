/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_HPMA115S0_Structure_h
#define _AFE_HPMA115S0_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
struct HPMA115S0_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG pm25;
  DOMOTICZ_BASIC_CONFIG pm10;
};
#endif

struct HPMA115S0 {
  char name[17];
  uint32_t interval;
  uint16_t timeToMeasure;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  HPMA115S0_DOMOTICZ domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif  
};

struct HPMA115S0_DATA {
  uint16_t pm10;
  uint16_t pm25;
};

#endif
