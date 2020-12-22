/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23017_Structure_h
#define _AFE_MCP23017_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_MCP23017

struct MCP23017_CACHE {
  uint8_t id;
  uint8_t address;
};

struct MCP23017 {
  uint8_t gpio;
  uint8_t address;
};

#endif // AFE_CONFIG_HARDWARE_MCP23017
#endif // _AFE_LED_Structure_h
