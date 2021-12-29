/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_TSL2561_Structure_h
#define _AFE_TSL2561_Structure_h

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
struct TSL2561_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG ir;
  DOMOTICZ_BASIC_CONFIG illuminance;
  DOMOTICZ_BASIC_CONFIG broadband;
};
#endif //  AFE_CONFIG_API_DOMOTICZ_ENABLED

struct TSL2561 {
  char name[17];
  uint32_t interval;
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif
  uint8_t i2cAddress;
  uint8_t sensitiveness;
  uint8_t gain;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  TSL2561_DOMOTICZ domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // _AFE_TSL2561_Structure_h
