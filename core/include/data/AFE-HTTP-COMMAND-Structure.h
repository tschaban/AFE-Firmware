/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_HTTP_COMMAND_Structure_h
#define _AFE_HTTP_COMMAND_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct HTTPCOMMAND {
  char device[32];
  char name[17];
  char command[32];
  char source[9];
};

#endif
