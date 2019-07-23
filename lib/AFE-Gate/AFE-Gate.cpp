/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Gate.h"

AFEGate::AFEGate(){};

void AFEGate::begin(uint8_t id, AFEDevice *_Device, AFEDataAccess *_Data) {

#ifdef DEBUG
  Serial << endl << "Initializing the Gate: " << id;
#endif

  Device = _Device;
  Data = _Data;

  gateId = id;
  configuration = Data->getGateConfiguration(gateId);
#ifdef DEBUG
  Serial << endl << "Initializing the gate's relay: " << configuration.relayId;
#endif
  if (configuration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST) {
    GateRelay.begin(configuration.relayId);
    GateRelay.setTimerUnitToSeconds(false);
    GateRelay.gateId = id;
  }
#ifdef DEBUG
  Serial << endl << "Initializing the gate's " << id << ", contactrons";
#endif

  /* How many contactrons monitors the gate. Default 0 set in class init
   */

  if (configuration.contactron.id[1] != AFE_HARDWARE_ITEM_NOT_EXIST) {
    numberOfContractons = 2;
  } else if (configuration.contactron.id[0] != AFE_HARDWARE_ITEM_NOT_EXIST) {
    numberOfContractons = 1;
  }

#ifdef DEBUG
  Serial << endl
         << "Number of contactrons to initialize: " << numberOfContractons;
#endif

  for (uint8_t i = 0; i < numberOfContractons; i++) {
    Contactron[i].begin(configuration.contactron.id[i], _Device, _Data);
  }

#ifdef DEBUG
  Serial << endl << "Gate Initialization completed";
#endif
}

void AFEGate::toggle() {
  GateRelay.on();
  // Setting Gate state manually is possible only if there is no contactrons
  if (numberOfContractons == 0) {
    Data->saveGateState(gateId, get() == GATE_CLOSED ? GATE_OPEN : GATE_CLOSED);
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
        gateState = _state[1] == CONTACTRON_OPEN
                        ? configuration.states.state[0]
                        : configuration.states.state[1];
      } else {
        gateState = configuration.states.state[0];
      }
    } else if (_state[0] == CONTACTRON_CLOSED) {
      if (numberOfContractons == 2) {
        gateState = _state[1] == CONTACTRON_OPEN
                        ? configuration.states.state[2]
                        : configuration.states.state[3];
      } else {
        gateState = configuration.states.state[3];
      }
    }
  }
  return gateState;
}

uint8_t AFEGate::get() {
  if (numberOfContractons ==
      0) { // If there is not contactorns then get gate state from EEPROM
    return Data->getGateState(gateId);
  } else { // Get gate state based on contactrons
    return getGateStateBasedOnContractons();
  }
}

uint8_t AFEGate::getNoOfContactrons() { return numberOfContractons; }

uint8_t AFEGate::getContactronId(uint8_t index) {
  return configuration.contactron.id[index];
}

boolean AFEGate::event() {

  GateRelay.autoTurnOff();

  if (_event) {
    _event = false;
    return true;
  } else {
    return false;
  }
}

void AFEGate ::triggerEvent() {
#ifdef DEBUG
  Serial << endl << "Gate event triggered externally";
#endif

  _event = true;
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
