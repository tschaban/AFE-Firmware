/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_h
#define _AFE_LED_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-LED-Structure.h>

class AFELED {

private:
  LED LEDConfiguration;
  boolean _initialized = false;
  boolean blinking = false;
  unsigned long interval;
  unsigned long previousMillis = 0;

  /* Method turns on/off LED */
  void set(uint8_t state);

public:
  /* Constructor */
  AFELED();

  /* Constructor with GPIO init. GPIO is GPIO to where LED is connected to */
  AFELED(uint8_t id);

  /* Constructor: gpio is GPIO to where LED is connected to */
  void begin(uint8_t id);

  /* Turns on LED */
  void on();

  /* Turn off LED */
  void off();

  /* Method change the LED to opposite state */
  void toggle();

  /* Blink LED. Duration how lon LED is ON can be set by input parameter (in
   * milli)*/
  void blink(unsigned int duration = 100);

  /* Turns on LED blinking with interval as input paramters. It's in
   * milliseconds */
  void blinkingOn(unsigned long blinking_interval);

  /* Turns off LED blinking */
  void blinkingOff();

  /* Returns true if LED is blinking, false if not */
  boolean isBlinking();

  /* Method must be added to main loop in order to enable continues LED blinking
   */
  void loop();
};

#endif
