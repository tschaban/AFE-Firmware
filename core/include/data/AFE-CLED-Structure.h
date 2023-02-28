/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_CLED_Structure_h
#define _AFE_CLED_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CLED

#include <AFE-COLOR-Structure.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

#include <Arduino.h>

struct CLED_STRIP {
  uint16_t numberOfLEDs;
};

/* @TODO T5 
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
*/

struct CLED_EFFECT_BLINKING {
  char name[33];
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;
  uint32_t onTimeout;
  uint32_t offTimeout;
};

struct CLED_EFFECT_WAVE {
  char name[33];
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;
  uint32_t timeout;
};

struct CLED_EFFECT_FADE_INOUT {
  char name[33];
  CLED_PARAMETERS in;
  CLED_PARAMETERS out;
  uint32_t timeout;
};

struct CLED {
  char name[33];
  uint8_t gpio;
  uint16_t ledNumbers;
  CLED_PARAMETERS on;
  CLED_PARAMETERS off;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG cled;
  DOMOTICZ_BASIC_CONFIG effect;
#else
  MQTT_TOPIC cled;
  MQTT_TOPIC effect;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  uint8_t brightnessConversion;
#endif  
#endif
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_Structure_h
