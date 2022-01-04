/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_DS18B20_Structure_h
#define _AFE_DS18B20_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DS18B20

#include <DallasTemperature.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
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
  uint8_t resolution;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_DS18B20
#endif // _AFE_DS18B20_Structure_h
