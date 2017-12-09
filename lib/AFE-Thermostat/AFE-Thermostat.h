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


};

#endif
