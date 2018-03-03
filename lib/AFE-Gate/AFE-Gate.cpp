/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Gate.h"

AFEGate::AFEGate(){};

void AFEGate::begin() {
  gateConfiguration = Data.getGateConfiguration();

  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Contactron[i].begin(i);
      numberOfContractors++;
    } else {
      break;
    }
  }
}

uint8_t AFEGate::get() {
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

void AFEGate::listener() {

  for (uint8_t i = 0; i < numberOfContractors; i++) {

    Contactron[i].listener();

    if (Contactron[i].changed()) {
      _event = true;
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
