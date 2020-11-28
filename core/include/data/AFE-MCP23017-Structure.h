/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23017_Structure_h
#define _AFE_MCP23017_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_MCP23017

struct MCP23017 {
  uint8_t address;
  uint8_t gpio;
};

#endif // AFE_CONFIG_HARDWARE_MCP23017
#endif // _AFE_LED_Structure_h
