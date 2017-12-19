/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Humidistat.h"

AFEHumidistat::AFEHumidistat(){};

void AFEHumidistat::begin(uint8_t relayID, RELAYSTAT config) {
  Humidistat.begin(relayID, config);
  _relayID = relayID;
}

boolean AFEHumidistat::isReady() { Humidistat.isReady(); }

byte AFEHumidistat::getRelayState() { return Humidistat.relayState; }

void AFEHumidistat::listener(float currentTemperature) {
  Humidistat.listener(currentTemperature);
}

void AFEHumidistat::on() { Humidistat.on(); }

void AFEHumidistat::off() { Humidistat.off(); }

void AFEHumidistat::toggle() { Humidistat.toggle(); }

boolean AFEHumidistat::enabled() { return Humidistat.configuration.enabled; }

void AFEHumidistat::enable(boolean state) {
  AFEDataAccess Data;
  Data.saveHumidistatState(_relayID, state);
}
