/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_CLED_Structure_h
#define _AFE_CLED_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>


#include <arduino.h>

struct CLED {
  uint8_t gpio;
  uint8_t colorOrder;
  uint8_t chipset;
  int ledNumber;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif  
};

#if defined(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT) || defined(AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT)
struct CLED_EFFECT {
  uint32_t color;
  uint16_t time;
  uint8_t brightness;
};

struct CLED_EFFECTS {
  CLED_EFFECT effect[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_EFFECTS];
};

#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT 
struct CLED_BACKLIGHT_CONFIG {

  uint32_t luxLevel;
  uint32_t color;
  uint8_t brightness;  
};

struct CLED_BACKLIGHT {
  uint8_t lightSensorId;
  CLED_BACKLIGHT_CONFIG config[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS]; 
};
#endif // AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT 


#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT


#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_Structure_h
