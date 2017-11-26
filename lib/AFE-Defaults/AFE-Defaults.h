/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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
  const char *getFirmwareVersion();
  uint8_t getFirmwareType();
  void eraseConfiguration();
};
#endif
