/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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

struct CLED_EFFECT {
  uint32_t color;
  uint16_t time;
  uint8_t brightness;
};

struct CLED_EFFECTS {
  CLED_EFFECT effect[AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_EFFECTS];
};

#endif // AFE_CONFIG_HARDWARE_CLED
#endif // _AFE_CLED_Structure_h
