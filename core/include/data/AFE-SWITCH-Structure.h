/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_SWITCH_Structure_h
#define _AFE_SWITCH_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_SWITCH

#include <arduino.h>

struct SWITCH {
  uint8_t gpio;
  uint8_t type;
  uint16_t sensitiveness;
  uint8_t functionality;
  uint8_t relayID;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
};

#endif // AFE_CONFIG_HARDWARE_SWITCH
#endif // _AFE_SWITCH_Structure_h
