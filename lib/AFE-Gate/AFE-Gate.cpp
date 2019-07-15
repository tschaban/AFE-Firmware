/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Gate.h"

AFEGate::AFEGate(){};

void AFEGate::begin(uint8_t id) {

#ifdef DEBUG
  Serial << endl << "Initializing the Gate: " << id;
#endif

  gateId = id;
  gateConfiguration = Data.getGateConfiguration(gateId);

#ifdef DEBUG
  Serial << endl << "Initializing the Gate: " << id << ", relay";
#endif

  Relay.begin(gateConfiguration.relayId);
  Relay.setTimerUnitToSeconds(false);

#ifdef DEBUG
  Serial << endl << "Initializing the Gate: " << id << ", contactrons";
#endif

  /* How many contactrons monitors the gate. Default 0 set in class init */
  if (gateConfiguration.contactronId[1] > 0) {
    numberOfContractors = 2;
    Contactron[0].begin(gateConfiguration.contactronId[0]);
    Contactron[1].begin(gateConfiguration.contactronId[1]);
  } else if (gateConfiguration.contactronId[0] > 1) {
    numberOfContractors = 1;
    Contactron[0].begin(gateConfiguration.contactronId[0]);
  }

#ifdef DEBUG
  Serial << endl << "Number of contactrons: " << numberOfContractors;
#endif
}

void AFEGate::toggle() {
  Relay.on();
  // Setting Gate state manually is possible only if there is no contactrons
  if (numberOfContractors == 0) {
    Data.saveGateState(gateId, get() == GATE_CLOSED ? GATE_OPEN : GATE_CLOSED);
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
    return Data.getGateState(gateId);
  } else { // Get gate state based on contactrons
    return getGateStateBasedOnContractons();
  }
}

void AFEGate::listener() {

  Relay.autoTurnOff();

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

const char *AFEGate::getMQTTCommandTopic() {
  if (strlen(gateConfiguration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", gateConfiguration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
  return mqttCommandTopic;
}

const char *AFEGate::getMQTTStateTopic() {
  if (strlen(gateConfiguration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", gateConfiguration.mqtt.topic);
  } else {
    mqttStateTopic[0] = '\0';
  }
  return mqttStateTopic;
}

unsigned long AFEGate::getDomoticzIDX() {
  return gateConfiguration.domoticz.idx;
}
