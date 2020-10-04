/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_CONTACTRON_Structure_h
#define _AFE_CONTACTRON_Structure_h


#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
#include <AFE-DOMOTICZ-Structure.h>
#endif
#include <AFE-MQTT-Structure.h>

struct CONTACTRON {
  uint8_t gpio;
  byte type = AFE_CONTACTRON_NO;
  uint8_t ledID;
  uint16_t bouncing = AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;
  char name[17];
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED  
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif
