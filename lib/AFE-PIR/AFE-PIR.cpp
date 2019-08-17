/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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

byte AFEPIR::get() {
  return digitalRead(configuration.gpio) == HIGH ? PIR_OPEN : PIR_CLOSE;
}

boolean AFEPIR::stateChanged() {
  if (_stateChanged) {
    _stateChanged = false;
    return true;
  } else {
    return false;
  }
}

boolean AFEPIR::motionDetected() {
  return configuration.type == PIR_NO ? (get() == PIR_CLOSE ? true : false)
                                      : (get() == PIR_OPEN ? true : false);
}

const char *AFEPIR::getMQTTTopic() { return configuration.mqttTopic; }

void AFEPIR::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(configuration.gpio);
    if (currentState != state) {
      state = currentState;
      _stateChanged = true;
      motionDetected() ? Led.on() : Led.off();
    }
  }
}

const char *AFEPIR::getName() { return configuration.name; }

unsigned long AFEPIR::getDomoticzIDX() { return configuration.idx; }
