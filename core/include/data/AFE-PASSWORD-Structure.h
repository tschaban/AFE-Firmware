/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_PASSWORD_Structure_h
#define _AFE_PASSWORD_Structure_h

#include <Arduino.h>

struct PASSWORD {
  boolean protect;
  char password[10];
};

#endif
