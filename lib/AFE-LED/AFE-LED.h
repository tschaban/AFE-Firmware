/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_LED_h
#define _AFE_LED_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_LED

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
  boolean begin(AFEDataAccess *, uint8_t id);

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

#endif // AFE_CONFIG_HARDWARE_LED
#endif // _AFE_LED_h