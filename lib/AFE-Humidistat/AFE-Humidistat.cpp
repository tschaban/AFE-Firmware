/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Humidistat.h"

AFEHumidistat::AFEHumidistat(){};

void AFEHumidistat::begin(uint8_t relayID, REGULATOR config) {
  configuration = config;
  _relayID = relayID;
}

boolean AFEHumidistat::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

byte AFEHumidistat::getRelayState() { return relayState; }

void AFEHumidistat::listener(float currentTemperature) {
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

void AFEHumidistat::on() {
  configuration.enabled = true;
  enable(configuration.enabled);
}
void AFEHumidistat::off() {
  configuration.enabled = false;
  enable(configuration.enabled);
}
void AFEHumidistat::toggle() {
  configuration.enabled ? configuration.enabled = false
                        : configuration.enabled = true;
  enable(configuration.enabled);
}
boolean AFEHumidistat::enabled() { return configuration.enabled; }

void AFEHumidistat::enable(boolean state) {
  AFEDataAccess Data;
  Data.saveHumidistatState(_relayID, state);
}
