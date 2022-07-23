/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_MCP23017_Structure_h
#define _AFE_MCP23017_Structure_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX


struct MCP23XXX_ITEM_CONFIG {
  uint8_t id;
  uint8_t gpio;
};

struct MCP23XXX {
  char name[33];
#ifdef AFE_ESP32
  uint8_t wirePortId;
#endif
  uint8_t address;
};

#endif // AFE_CONFIG_HARDWARE_MCP23XXX
#endif // _AFE_LED_Structure_h
