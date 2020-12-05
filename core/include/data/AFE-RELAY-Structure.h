/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_RELAY_Structure_h
#define _AFE_RELAY_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_RELAY

#include <arduino.h>

struct RELAY_DEFAULTS {
  uint8_t powerOn;
  uint8_t MQTTConnected;
};

struct RELAY {
  uint8_t gpio;
  char name[17];
  float timeToOff;
 #ifdef AFE_CONFIG_HARDWARE_LED
  uint8_t ledID;
#endif
  uint8_t triggerSignal;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
  RELAY_DEFAULTS state;
};

#endif // AFE_CONFIG_HARDWARE_RELAY
#endif // _AFE_RELAY_Structure_h
