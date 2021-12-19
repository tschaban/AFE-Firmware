/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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
#ifdef AFE_CONFIG_HARDWARE_RELAY  
  uint8_t relayID;
#endif
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  MCP23017 mcp23017;
#endif  
};

#endif // AFE_CONFIG_HARDWARE_SWITCH
#endif // _AFE_SWITCH_Structure_h
