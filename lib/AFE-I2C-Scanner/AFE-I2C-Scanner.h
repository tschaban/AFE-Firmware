/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_I2C_Scanner_h
#define _AFE_I2C_Scanner_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEI2CScanner {

public:
  AFEI2CScanner();

#ifdef DEBUG
  void scanAll();
#endif

  boolean scan(byte address);
  const char *getName(byte deviceAddress);
};

#endif
