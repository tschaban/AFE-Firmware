/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-PIR.h"

AFEPIR::AFEPIR(){};

AFEPIR::AFEPIR(uint8_t id) { begin(id); }

void AFEPIR::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getPIRConfiguration(id);
  pinMode(configuration.gpio, INPUT_PULLUP);
  state = digitalRead(configuration.gpio);
  Led.begin(configuration.ledId);
  _initialized = true;
}

boolean AFEPIR::stateChanged() {
  if (_stateChanged) {
    _stateChanged = false;
    return true;
  } else {
    return false;
  }
}

byte AFEPIR::get() {
  return digitalRead(configuration.gpio) == HIGH ? PIR_OPEN : PIR_CLOSE;
}

const char *AFEPIR::getMQTTTopic() { return configuration.mqttTopic; }

void AFEPIR::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(configuration.gpio);
    if (currentState != state) {
      state = currentState;
      _stateChanged = true;
      state ? Led.on() : Led.off();
    }
  }
}

const char *AFEPIR::getName() { return configuration.name; }
