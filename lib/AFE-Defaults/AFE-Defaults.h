/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Defaults_h
#define _AFE_Defaults_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Data-Access.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEDefaults {
private:
  AFEDataAccess *Data;

public:
  AFEDefaults();
  void set();
};
#endif
