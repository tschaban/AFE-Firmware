/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#endif // _AFE_THERMAL_PROTECTOR_Structure_h
