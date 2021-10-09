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

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  DOMOTICZ_BASIC_CONFIG domoticz;
#else
  MQTT_BASIC_CONFIG mqtt;
#endif
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  MCP23017 mcp23017;
#endif  
};

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#endif // _AFE_BINARY_SENSOR_Structure_h
