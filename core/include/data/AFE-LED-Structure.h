/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_Structure_h
#define _AFE_LED_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_LED

#include <Arduino.h>

struct LED {
  uint8_t gpio;
  boolean changeToOppositeValue;
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  MCP23XXX_ITEM_CONFIG mcp23017;
#endif
};

#endif // AFE_CONFIG_HARDWARE_LED
#endif // _AFE_LED_Structure_h
