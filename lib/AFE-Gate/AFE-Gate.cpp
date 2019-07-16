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
  configuration = Data.getGateConfiguration(gateId);
  delay(100);
#ifdef DEBUG
  Serial << endl
         << "Initializing the Gate: " << id
         << ", relay: " << configuration.relayId;
#endif

  Relay.begin(configuration.relayId);
  delay(100);
  Relay.setTimerUnitToSeconds(false);
  delay(100);
#ifdef DEBUG
  Serial << endl << "Initializing the Gate: " << id << ", contactrons.";
#endif

  /* How many contactrons monitors the gate. Default 0 set in class init
   */

  if (configuration.contactronId[1] > 0) {
    numberOfContractons = 2;
  } else if (configuration.contactronId[0] > 1) {
    numberOfContractons = 1;
  }
  delay(100);
#ifdef DEBUG
  Serial << endl
         << "Number of contactrons to initialize: " << numberOfContractons;
#endif

  for (uint8_t i = 0; i < numberOfContractons; i++) {
#ifdef DEBUG
    Serial << endl << " - init : " << i;
#endif
    Contactron[i].begin(configuration.contactronId[i]);
    delay(100);
  }

#ifdef DEBUG
  Serial << endl << "Gate Initialization completed";
#endif
}

void AFEGate::toggle() {
  Relay.on();
  // Setting Gate state manually is possible only if there is no contactrons
  if (numberOfContractons == 0) {
    Data.saveGateState(gateId, get() == GATE_CLOSED ? GATE_OPEN : GATE_CLOSED);
    _event = true;
  }
}

uint8_t AFEGate::getGateStateBasedOnContractons() {
  uint8_t gateState = GATE_UNKNOWN;
  if (numberOfContractons > 0) {
    uint8_t _state[numberOfContractons];
    for (uint8_t i = 0; i < numberOfContractons; i++) {
      _state[i] = Contactron[i].get();
    }
    if (_state[0] == CONTACTRON_OPEN) {
      if (numberOfContractons == 2) {
        gateState = _state[1] == CONTACTRON_OPEN ? configuration.state[0]
                                                 : configuration.state[1];
      } else {
        gateState = configuration.state[0];
      }
    } else if (_state[0] == CONTACTRON_CLOSED) {
      if (numberOfContractons == 2) {
        gateState = _state[1] == CONTACTRON_OPEN ? configuration.state[2]
                                                 : configuration.state[3];
      } else {
        gateState = configuration.state[3];
      }
    }
  }
  return gateState;
}

uint8_t AFEGate::get() {
  if (numberOfContractons ==
      0) { // If there is not contactorns then get gate state from EEPROM
    return Data.getGateState(gateId);
  } else { // Get gate state based on contactrons
    return getGateStateBasedOnContractons();
  }
}

uint8_t AFEGate::getNoOfContactrons() { return numberOfContractons; }

uint8_t AFEGate::getContactronId(uint8_t index) {
  return configuration.contactronId[index];
}

void AFEGate::listener() {

  Relay.autoTurnOff();

  for (uint8_t i = 0; i < numberOfContractons; i++) {
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
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
  return mqttCommandTopic;
}

const char *AFEGate::getMQTTStateTopic() {
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = '\0';
  }
  return mqttStateTopic;
}

unsigned long AFEGate::getDomoticzIDX() { return configuration.domoticz.idx; }
