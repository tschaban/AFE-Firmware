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
//#include <Streaming.h>

class AFEDefaults {
private:
  AFEEEPROM Eeprom;

public:
  AFEDefaults();

  /* Method returns firmware version number */
  const char *getFirmwareVersion();

  /* Method returns firmware version type */
  uint8_t getFirmwareType();

  /* Method erases EEPROM */
  void eraseConfiguration();

  /* Method set default values */
  void set();
};
#endif
