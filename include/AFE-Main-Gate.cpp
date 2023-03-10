/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_GATE

/* ---------Headers ---------*/

void initializeGate(void);
void gateEventsListener(void);

/* --------- Body -----------*/

void initializeGate(void) {
  for (uint8_t i = 0; i < FirmwarePro->Device->configuration.noOfGates; i++) {
    Gate[i].begin(i, FirmwarePro);
    GatesCurrentStates->state[i] = AFE_GATE_UNKNOWN;

    /* Assigning Gate ID to a contactron */
    for (uint8_t j = 0; j < Gate[i].getNoOfContactrons(); j++) {
      Contactron[Gate[i].getContactronId(j)].gateId = i;
#ifdef DEBUG
      Serial << endl
             << F("INFO: GATE: For a ContactronID: ")
             << Gate[i].getContactronId(j) << F(" setting GateID: ") << i;
#endif
    }

#ifdef DEBUG
    Serial << endl << F("INFO: GATE: ") << i << F(" initialized");
#endif
  }
}

/* Method processes gate related events */
void gateEventsListener(void) {
  uint8_t _gateState;
  for (uint8_t gateId = 0; gateId < FirmwarePro->Device->configuration.noOfGates; gateId++) {
    if (Gate[gateId].event()) {
      _gateState = Gate[gateId].get();
      if (GatesCurrentStates->state[gateId] != _gateState) {
        MqttAPI->publishGateState(gateId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishGateState(gateId);
#endif
        GatesCurrentStates->state[gateId] = _gateState;
      }
    }
  }
}

#endif
