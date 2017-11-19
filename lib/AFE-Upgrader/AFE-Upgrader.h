/*
  AFE Firmware for smart home devices build on ESP8266
  Version: T0
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_UPGRADER_h
#define _AFE_UPGRADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>

class AFEUpgrader {

private:
  FIRMWARE FirmwareConfiguration;
  AFEDataAccess Data;
  AFEDefaults Defaults;
  void upgradeTypeOfFirmware();

public:
  /* Constructor */
  AFEUpgrader();
  /* Checks if firmware has been upgraded */
  boolean upgraded();

  /* Perform post upgrade changes */
  void upgrade();
};

#endif
