/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_DS18B20_Structure_h
#define _AFE_DS18B20_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DS18B20

#include <DallasTemperature.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

typedef DeviceAddress DS18B20Addresses[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20];

struct DS18B20 {
  char name[17];
  uint8_t gpio;
  DeviceAddress address;
  float correction;
  uint32_t interval;
  uint8_t unit;
  boolean sendOnlyChanges;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_DS18B20
#endif // _AFE_DS18B20_Structure_h
