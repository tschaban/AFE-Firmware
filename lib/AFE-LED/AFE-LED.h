/*
  LED component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_LED_h
#define _AFE_LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class AFELED {

private:
	byte _gpio;
  boolean _blinking = false;
  unsigned long _interval;
  unsigned long _previousMillis = 0;

public:
  /* Constructor: gpio is GPIO to where LED is connected to */
	AFELED(byte gpio);

  /* Turns on LED */
  void on();

  /* Turn off LED */
	void off();

  /* Blink LED. Duration how lon LED is ON can be set by input parameter (in milli)*/
  void blink(unsigned int duration = 100);

  /* Turns on LED blinking with interval as input paramters. It's in milliseconds */
  void blinkingOn(unsigned long interval);

  /* Turns off LED blinking */
  void blinkingOff();

  /* Method must be added to main loop in order to enable continues LED blinking */
  void loop();
};

#endif
