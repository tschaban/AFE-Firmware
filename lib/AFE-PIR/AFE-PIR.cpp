/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-PIR.h"

AFEPIR::AFEPIR(){};

AFEPIR::AFEPIR(uint8_t id) { begin(id); }

void AFEPIR::begin(uint8_t id) {
  AFEDataAccess Data;
  PIRConfiguration = Data.getPIRConfiguration(id);
  pinMode(PIRConfiguration.gpio, INPUT_PULLUP);
  state = digitalRead(PIRConfiguration.gpio);
  Led.begin(1);
  _initialized = true;
}

boolean AFEPIR::stateChanged() { _stateChanged = false; }

boolean AFEPIR::getState() { return state; }

void AFEPIR::listener() {
  if (_initialized) {
    boolean currentState = digitalRead(PIRConfiguration.gpio);
    if (currentState != state) {
      state = currentState;
      _stateChanged = true;
      state ? Led.on() : Led.off();
    }
  }
}
