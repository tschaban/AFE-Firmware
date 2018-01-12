/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-PIR.h"

AFEPIR::AFEPIR(){};

AFEPIR::AFEPIR(uint8_t id) { begin(id); }

void AFEPIR::begin(uint8_t id) {
  AFEDataAccess Data;
  Configuration = Data.getPIRConfiguration(id);
  pinMode(Configuration.gpio, INPUT_PULLUP);
  state = digitalRead(Configuration.gpio);
  Led.begin(1);
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
  return digitalRead(Configuration.gpio) == HIGH ? PIR_OPEN : PIR_CLOSE;
}

const char *AFEPIR::getMQTTTopic() { return Configuration.mqttTopic; }

void AFEPIR::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(Configuration.gpio);
    if (currentState != state) {
      state = currentState;
      _stateChanged = true;
      state ? Led.on() : Led.off();
    }
  }
}

const char *AFEPIR::getName() { return Configuration.name; }
