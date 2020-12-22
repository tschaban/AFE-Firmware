/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_Structure_h
#define _AFE_LED_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_LED

#include <arduino.h>

struct LED {
  uint8_t gpio;
  boolean changeToOppositeValue;
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  MCP23017 mcp23017;
#endif
};

#endif // AFE_CONFIG_HARDWARE_LED
#endif // _AFE_LED_Structure_h
