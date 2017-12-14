/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Thermostat_h
#define _AFE_Thermostat_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Relay.h>
class AFEThermostat {

private:
  THERMOSTAT configuration;
  boolean ready = false;
  byte relayState;

public:
  /* Constructors */
  AFEThermostat();

  void begin(THERMOSTAT config);
  boolean isReady();
  byte getRelayState();
  void listener(float currentTemperature);
  void on();
  void off();
  void toggle();
  boolean enabled();
};

#endif
/* DODAC ZAPISYWANIE INFORMACJI O TERMOSTACIE DO EEPROM, NIE JESTEM PEWIEN CZY
 * TO NIE POWINO ZOSTAC PRZENIESIONE NA POZIOM RELAY */
