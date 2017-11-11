/*
   Default values
   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_Defaults_h
#define _AFE_Defaults_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-EEPROM.h>
#include <Streaming.h>

class AFEDefaults {
private:
  AFEEEPROM Eeprom;

public:
  AFEDefaults();
  void set();
  void eraseConfiguration();
};
#endif
