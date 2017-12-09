/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Thermostat.h"

AFEThermostat::AFEThermostat() {};

void AFEThermostat::begin(THERMOSTAT config) {
  configuration = config;
}

boolean AFEThermostat::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

byte AFEThermostat::getRelayState() {
  return relayState;
}

void AFEThermostat::listener(float currentTemperature) {
   if (configuration.enabled) {
     if (configuration.temperatureTurnOnAbove && currentTemperature > configuration.temperatureTurnOn) {
       relayState = RELAY_ON;
       ready = true;
     } else if (!configuration.temperatureTurnOnAbove && currentTemperature < configuration.temperatureTurnOn) {
       relayState = RELAY_ON;
       ready = true;
     } else if (configuration.temperatureTurnOffAbove && currentTemperature > configuration.temperatureTurnOff) {
       relayState = RELAY_OFF;
       ready = true;
     } else if (!configuration.temperatureTurnOffAbove && currentTemperature < configuration.temperatureTurnOff) {
       relayState = RELAY_OFF;
       ready = true;
     }
   }
}
