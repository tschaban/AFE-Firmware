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
#include <AFE-LED.h>
// #include <Streaming.h>

class AFEPIR {

private:
  boolean _initialized = false;
  boolean _stateChanged = false;
  boolean state;

  AFELED Led;

public:
  PIR configuration;

  /* Constructors */
  AFEPIR();
  AFEPIR(uint8_t id);

  /* Init PIR */
  void begin(uint8_t id);

  /* Returns PIR State */
  byte get();

  /* Returns PIR MQTT Topic */
  const char *getMQTTTopic();

  /* Returns true is PIR state changed */
  boolean stateChanged();

  /* Methods returns PIR name */
  const char *getName();

  /* Method should be added to main loop to listen for PIR events */
  void listener();
};

#endif
