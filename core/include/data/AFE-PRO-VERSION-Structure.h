/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_PRO_VERSION_Structure_h
#define _AFE_PRO_VERSION_Structure_h

#include <Arduino.h>

struct PRO_VERSION {
  char serial[19];
  boolean valid;
};

#endif
