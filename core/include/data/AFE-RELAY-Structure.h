/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
  RELAY_DEFAULTS state;
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  MCP23XXX_ITEM_CONFIG mcp23017;
#endif  
};

#endif // AFE_CONFIG_HARDWARE_RELAY
#endif // _AFE_RELAY_Structure_h
