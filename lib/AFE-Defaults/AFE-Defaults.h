/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Defaults_h
#define _AFE_Defaults_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDefaults {
private:
  AFEDataAccess *Data;

public:
  AFEDefaults();

  /* Set's the default values for a device, creates configuration files */
  void set();
};
#endif
