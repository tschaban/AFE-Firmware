/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_Humidistat_h
#define _AFE_Humidistat_h

#include <Arduino.h>
#include <AFE-Data-Access.h>

class AFEHumidistat {

  /* Class is just an interface to the AFE-thermostat which has the same
   * functionality */

private:
  AFEDataAccess *Data;
  boolean ready = false;
  REGULATOR *configuration = new REGULATOR;
  byte relayState;
  /* Method enables / disables thermostat */
  void enable(boolean state);

public:
  /* Constructors */
  AFEHumidistat(AFEDataAccess *);

  /* Method initialize humidistat */
  void begin(REGULATOR *config);

  /* Method returns true if humidistat is enabled */
  boolean enabled();

  /* Method returns true if event related to humidistat has been captured */
  boolean isReady();

  /* It returns what should be the relay state after humidistat event captured
   */
  byte getRelayState();

  /* Method rises event if thermostat humidistat have been exceeded */
  void listener(float currentHumidity);

  /* Method turns on humidistat */
  void on();

  /* Method turns off humidistat */
  void off();

  /* Method turns humidistat to opposite to current state */
  void toggle();
};

#endif
