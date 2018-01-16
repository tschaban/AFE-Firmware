/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Thermostat.h"

AFEThermostat::AFEThermostat(){};

void AFEThermostat::begin(uint8_t relayID, REGULATOR config) {
  configuration = config;
  _relayID = relayID;
}

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
      relayState = RELAY_ON;
      ready = true;
    } else if (!configuration.turnOnAbove &&
               currentTemperature < configuration.turnOn) {
      relayState = RELAY_ON;
      ready = true;
    } else if (configuration.turnOffAbove &&
               currentTemperature > configuration.turnOff) {
      relayState = RELAY_OFF;
      ready = true;
    } else if (!configuration.turnOffAbove &&
               currentTemperature < configuration.turnOff) {
      relayState = RELAY_OFF;
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
  Data.saveThermostatState(_relayID, state);
}
