/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_GATE

/* ---------Headers ---------*/

void gateEventsListener(void);

/* --------- Body -----------*/


/* Method processes gate related events */
void gateEventsListener(void) {
  uint8_t _gateState;
  for (uint8_t gateId = 0; gateId < Firmware->Device->configuration.noOfGates; gateId++) {
    if (Hardware->Gate[gateId]->event()) {
      _gateState = Hardware->Gate[gateId]->get();
      if (Hardware->GatesCurrentStates->state[gateId] != _gateState) {
        MqttAPI->publishGateState(gateId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishGateState(gateId);
#endif
        Hardware->GatesCurrentStates->state[gateId] = _gateState;
      }
    }
  }
}

#endif
