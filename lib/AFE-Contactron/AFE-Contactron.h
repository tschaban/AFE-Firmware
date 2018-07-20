/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Contactron_h
#define _AFE_Contactron_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-CONTACTRON-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-LED.h>
//#include <Streaming.h>

class AFEContactron {

private:
  CONTACTRON ContactronConfiguration;
  boolean _initialized = false;
  boolean state;            // It stores actual contactron state
  byte _state;              // This contains contactron state to return
  boolean _changed = false; // True if contractor changed state

  unsigned long startTime = 0;
  char mqttTopic[50];

  AFELED ContactronLed;

  void convert();

public:
  /* Constructors */
  AFEContactron();
  AFEContactron(uint8_t id);

  void begin(uint8_t id);

  /* Method returns contactorn state */
  byte get();

  /* Methods returns contactron name */
  const char *getName();

  /* Method returns MQTT topic for this contactron */
  const char *getMQTTTopic();

  /* Method returns true if cotactron state ahs changed */
  boolean changed();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();
};

#endif
