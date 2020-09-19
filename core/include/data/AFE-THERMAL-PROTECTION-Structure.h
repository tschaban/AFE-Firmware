/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_THERMAL_PROTECTOR_Structure_h
#define _AFE_THERMAL_PROTECTOR_Structure_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
struct THERMAL_PROTECTOR {
  char name[17];
  uint8_t relayId;
  uint8_t sensorId;
  uint8_t sensorHardware;
  float temperature;
  boolean enabled;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#endif // _AFE_THERMAL_PROTECTOR_Structure_h
