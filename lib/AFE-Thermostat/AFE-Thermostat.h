/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_Thermostat_h
#define _AFE_Thermostat_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

class AFEThermostat {

private:
  boolean ready = false;
  REGULATOR configuration;
  byte relayState;
  /* Method enables / disables thermostat */
  void enable(boolean state);

public:
  /* Constructors */
  AFEThermostat();

  /* Method initialize thermostat */
  void begin(REGULATOR config);

  /* Method returns true if thermostat is enabled */
  boolean enabled();

  /* Method returns true if event related to thermostat has been captured */
  boolean isReady();

  /* It returns what should be the relay state after thermostat event captured
   */
  byte getRelayState();

  /* Method rises event if thermostat themperatures have been exceeded */
  void listener(float currentTemperature);

  /* Method turns on thermostat */
  void on();

  /* Method turns off thermostat */
  void off();

  /* Method turns thermostat to opposite to current state */
  void toggle();
};

#endif
