/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_FS3000_Structure_h
#define _AFE_FS3000_Structure_h

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
struct FS3000_DOMOTICZ {
  DOMOTICZ_BASIC_CONFIG raw;
  DOMOTICZ_BASIC_CONFIG meterPerSecond;
  DOMOTICZ_BASIC_CONFIG milesPerHour;
  DOMOTICZ_BASIC_CONFIG meter3PerSecond;
};
#endif //  AFE_CONFIG_API_DOMOTICZ_ENABLED

struct FS3000_CONFIG {
  char name[33];
  uint32_t interval;
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif
  uint8_t i2cAddress;
  uint8_t range;
  uint16_t r;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  FS3000_DOMOTICZ domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
};

#endif // _AFE_FS3000_Structure_h
