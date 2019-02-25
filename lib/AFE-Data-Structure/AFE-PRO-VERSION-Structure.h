/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_PRO_VERSION_Structure_h
#define _AFE_PRO_VERSION_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct PRO_VERSION_SERIAL_NUMBER {
  char number[15];
};

struct PRO_VERSION_DEVICE_LIMITS {
  uint8_t available;
  uint8_t used;
};

struct PRO_VERSION {
  PRO_VERSION_SERIAL_NUMBER serial;
  PRO_VERSION_DEVICE_LIMITS limits;
  boolean valid;
  date lastCheck;
};


#endif
