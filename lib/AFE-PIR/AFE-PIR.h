/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_PIR_h
#define _AFE_PIR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-LED.h>
#include <Streaming.h>

class AFEPIR {

private:
  boolean _initialized = false;
  boolean _stateChanged = false;
  boolean state;

  AFELED Led;

public:
  PIR Configuration;

  /* Constructors */
  AFEPIR();
  AFEPIR(uint8_t id);

  /* Init PIR */
  void begin(uint8_t id);

  byte get();
  const char *getMQTTTopic();
  boolean stateChanged();

  void listener();
};

#endif
