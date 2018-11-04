/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Gate.h"

AFEGate::AFEGate(){};

void AFEGate::begin() {
  gateConfiguration = Data.getGateConfiguration();

  Relay[0].begin(0);

  // Relay[0].setRelayAfterRestoringPower(); I think it's not required

  Relay[0].setTimerUnitToSeconds(false);

  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Contactron[i].begin(i);
      numberOfContractors++;
    } else {
      break;
    }
  }
}

void AFEGate::toggle() {
  Relay[0].on();
  // Setting Gate state manually is possible only if there is no contactrons
  if (numberOfContractors == 0) {
    Data.saveGateState(get() == GATE_CLOSED ? GATE_OPEN : GATE_CLOSED);
    _event = true;
  }
}

uint8_t AFEGate::getGateStateBasedOnContractons() {
  uint8_t gateState = GATE_UNKNOWN;
  if (numberOfContractors > 0) {
    uint8_t _state[numberOfContractors];
    for (uint8_t i = 0; i < numberOfContractors; i++) {
      _state[i] = Contactron[i].get();
    }
    if (_state[0] == CONTACTRON_OPEN) {
      if (numberOfContractors == 2) {
        gateState = _state[1] == CONTACTRON_OPEN ? gateConfiguration.state[0]
                                                 : gateConfiguration.state[1];
      } else {
        gateState = gateConfiguration.state[0];
      }
    } else if (_state[0] == CONTACTRON_CLOSED) {
      if (numberOfContractors == 2) {
        gateState = _state[1] == CONTACTRON_OPEN ? gateConfiguration.state[2]
                                                 : gateConfiguration.state[3];
      } else {
        gateState = gateConfiguration.state[3];
      }
    }
  }
  return gateState;
}

uint8_t AFEGate::get() {
  if (numberOfContractors ==
      0) { // If there is not contactorns then get gate state from EEPROM
    return Data.getGateState();
  } else { // Get gate state based on contactrons
    return getGateStateBasedOnContractons();
  }
}

void AFEGate::listener() {

  Relay[0].autoTurnOff();

  for (uint8_t i = 0; i < numberOfContractors; i++) {
    Contactron[i].listener();
    if (Contactron[i].changed()) {
      _event = true; // Gate changed the state as contractons changed the state
    }
  }
}

boolean AFEGate::event() {
  if (_event) {
    _event = false;
    return true;
  } else {
    return false;
  }
}

unsigned long AFEGate::getDomoticzIDX() { return gateConfiguration.idx; }
