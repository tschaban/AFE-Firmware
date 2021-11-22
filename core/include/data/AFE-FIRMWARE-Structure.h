/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_FIRMWARE_Structure_h
#define _AFE_FIRMWARE_Structure_h

#include <arduino.h>

struct FIRMWARE {
  char version[10];
  uint8_t type;
  uint8_t api;
};

#endif
