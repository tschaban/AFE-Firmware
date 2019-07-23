/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#include "AFE-Thermostat.h"

AFEThermostat::AFEThermostat(){};

void AFEThermostat::begin(REGULATOR config) { configuration = config; }

boolean AFEThermostat::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

byte AFEThermostat::getRelayState() { return relayState; }

void AFEThermostat::listener(float currentTemperature) {
  if (configuration.enabled) {
    if (configuration.turnOnAbove &&
        currentTemperature > configuration.turnOn) {
      relayState = AFE_RELAY_ON;
      ready = true;
    } else if (!configuration.turnOnAbove &&
               currentTemperature < configuration.turnOn) {
      relayState = AFE_RELAY_ON;
      ready = true;
    } else if (configuration.turnOffAbove &&
               currentTemperature > configuration.turnOff) {
      relayState = AFE_RELAY_OFF;
      ready = true;
    } else if (!configuration.turnOffAbove &&
               currentTemperature < configuration.turnOff) {
      relayState = AFE_RELAY_OFF;
      ready = true;
    }
  }
}

void AFEThermostat::on() {
  configuration.enabled = true;
  enable(configuration.enabled);
}
void AFEThermostat::off() {
  configuration.enabled = false;
  enable(configuration.enabled);
}
void AFEThermostat::toggle() {
  configuration.enabled ? configuration.enabled = false
                        : configuration.enabled = true;
  enable(configuration.enabled);
}
boolean AFEThermostat::enabled() { return configuration.enabled; }

void AFEThermostat::enable(boolean state) {
  AFEDataAccess Data;
  Data.saveRegulatorState(state, THERMOSTAT_REGULATOR);
}
