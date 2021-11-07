/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_CLED_Structure_h
#define _AFE_CLED_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CLED

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#include <arduino.h>

struct CLED_STRIP {
  uint16_t numberOfLEDs;
};

struct CLED_PARAMETERS {
  uint32_t color;
  uint8_t brightness;
};

#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
struct CLED_LIGHT_CONTROLLED_LEVELS_CONFIG {
  uint32_t lightLevel;
  CLED_COLOR color;
};

struct CLED_LIGHT_CONTROLLED_CONFIG {
  uint8_t lightSensorId;
  CLED_BACKLIGHT_CONFIG
  config[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS];
};
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT

struct CLED_EFFECT_BLINKING {
  char name[33];
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;
  unsigned long onTimeout;
  unsigned long offTimeout;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

struct CLED_EFFECT_WAVE {
  char name[33];
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;
  unsigned long timeout;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

struct CLED_EFFECT_FADE_INOUT {
  char name[33];
  CLED_PARAMETERS in;
  CLED_PARAMETERS out;
  unsigned long timeout;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

struct CLED {
  char name[33];
  uint8_t gpio;
  uint16_t ledNumbers;
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_Structure_h
