/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef CONFIG_HARDWARE_GATE

/* Method processes gate related events */
void gateEventsListener() {
  uint8_t _gateState;
  for (uint8_t gateId = 0; gateId < Device.configuration.noOfGates; gateId++) {
    if (Gate[gateId].event()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif
      _gateState = Gate[gateId].get();
      if (GatesCurrentStates.state[gateId] != _gateState) {
        MQTTPublishGateState(gateId);
        DomoticzPublishGateState(gateId);
        GatesCurrentStates.state[gateId] = _gateState;
      }
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.off();
#endif
    }
  }
}

#endif
