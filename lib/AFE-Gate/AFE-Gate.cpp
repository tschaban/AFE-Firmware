/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Gate.h"

AFEGate::AFEGate(){};

void AFEGate::begin(uint8_t id, AFEDevice *_Device, AFEDataAccess *_Data) {

#ifdef DEBUG
  Serial << endl << F("INFO: Initializing the Gate: ") << id;
#endif

  Device = _Device;
  Data = _Data;

  gateId = id;
  Data->getConfiguration(gateId,&configuration);
#ifdef DEBUG
  Serial << endl
         << F("INFO: Initializing the gate's relay: ") << configuration.relayId;
#endif
  if (configuration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST) {
    GateRelay.begin(configuration.relayId);
    GateRelay.setTimerUnitToSeconds(false);
    GateRelay.gateId = id;
  }
#ifdef DEBUG
  Serial << endl << F("INFO: Initializing the gate's ") << id << F(", contactrons");
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
         << F("INFO: Number of contactrons to initialize: ")
         << numberOfContractons;
#endif

  for (uint8_t i = 0; i < numberOfContractons; i++) {
    Contactron[i].begin(configuration.contactron.id[i], _Device, _Data);
  }

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
    mqttStateTopic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Gate Initialization completed");
#endif
}

void AFEGate::toggle() {
#ifdef DEBUG
  Serial << endl << F("INFO: Toggling gate");
#endif
  GateRelay.on();
  // Setting Gate state manually is possible only if there is no contactrons
  if (numberOfContractons == 0) {
    Data->saveGateState(gateId, get() == AFE_GATE_CLOSED ? AFE_GATE_OPEN
                                                         : AFE_GATE_CLOSED);
    _event = true;
  }
}

uint8_t AFEGate::getGateStateBasedOnContractons() {
  uint8_t gateState = AFE_GATE_UNKNOWN;
  if (numberOfContractons > 0) {
    uint8_t _state[numberOfContractons];
    for (uint8_t i = 0; i < numberOfContractons; i++) {
      _state[i] = Contactron[i].get();
    }
    if (_state[0] == AFE_CONTACTRON_OPEN) {
      if (numberOfContractons == 2) {
        gateState = _state[1] == AFE_CONTACTRON_OPEN
                        ? configuration.states.state[0]
                        : configuration.states.state[1];
      } else {
        gateState = configuration.states.state[0];
      }
    } else if (_state[0] == AFE_CONTACTRON_CLOSED) {
      if (numberOfContractons == 2) {
        gateState = _state[1] == AFE_CONTACTRON_OPEN
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

void AFEGate::triggerEvent() {
#ifdef DEBUG
  Serial << endl << F("INFO: Gate event triggered externally");
#endif

  _event = true;
}

void AFEGate::getJSON(char *json) {
  uint8_t _state = get();
  sprintf(json, "{\"state\":\":%s\"}",
          (_state == AFE_GATE_OPEN
               ? "open"
               : (_state == AFE_GATE_CLOSED
                      ? "closed"
                      : (_state == AFE_GATE_PARTIALLY_OPEN ? "partiallyOpen"
                                                           : "unknown"))));
}
