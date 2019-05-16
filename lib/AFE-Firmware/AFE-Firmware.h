/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Firmware_h
#define _AFE_Firmware_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmware {

private:


public:
  /* Constructor */
  AFEFirmware();

  /* Returns if firmware version if pro */
  boolean isUnlocked();

/* Returns information about the pro version, incl. limits */
  PRO_VERSION getProVersionInfo();

/* Checking the version */
  void validate();


};

#endif
