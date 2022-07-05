/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_BINARY_SENSOR_Structure_h
#define _AFE_BINARY_SENSOR_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

#include <arduino.h>

struct BINARY_SENSOR {
  char name[17];
  uint8_t gpio;
  uint16_t bouncing;

  boolean revertSignal;
  boolean sendAsSwitch;
  boolean internalPullUp;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_TOPIC mqtt;
#endif
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  MCP23XXX_ITEM_CONFIG mcp23017;
#endif  
};

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#endif // _AFE_BINARY_SENSOR_Structure_h
