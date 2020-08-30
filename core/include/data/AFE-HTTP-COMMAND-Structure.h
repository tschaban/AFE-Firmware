/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_HTTP_COMMAND_Structure_h
#define _AFE_HTTP_COMMAND_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct HTTPCOMMAND {
  char device[16];
  char name[16];
  char command[32];
  char source[9];
};

#endif
