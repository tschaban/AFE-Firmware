/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_TLS2561_Structure_h
#define _AFE_TLS2561_Structure_h

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
struct TLS2561_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG ir;
  DOMOTICZ_BASIC_CONFIG illuminance;
  DOMOTICZ_BASIC_CONFIG broadband;
};
#endif //  AFE_CONFIG_API_DOMOTICZ_ENABLED

struct TLS2561 {
  char name[17];
  uint32_t interval;
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif
  uint8_t i2cAddress;
  uint8_t sensitiveness;
  uint8_t gain;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  TLS2561_DOMOTICZ domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif // _AFE_TLS2561_Structure_h
