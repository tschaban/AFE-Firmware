/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_DHT_Structure_h
#define _AFE_DHT_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DHT

#include <AFE-Air-Item-Structure.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>


#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
struct DHT_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG temperature;
  DOMOTICZ_BASIC_CONFIG humidity;
  DOMOTICZ_BASIC_CONFIG absoluteHumidity;
  DOMOTICZ_BASIC_CONFIG dewPoint;
  DOMOTICZ_BASIC_CONFIG heatIndex;
  DOMOTICZ_BASIC_CONFIG temperatureHumidity;
  DOMOTICZ_BASIC_CONFIG perception;
  DOMOTICZ_BASIC_CONFIG comfort;
};

#endif


struct DHT {
  char name[33];
  uint8_t gpio;
  uint8_t type;
  uint32_t interval;
  boolean sendOnlyChanges;
  AIR_ITEM_SETTING temperature;
  AIR_ITEM_SETTING humidity;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DHT_DOMOTICZ domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_DHT
#endif // _AFE_DHT_Structure_h
