/* Method initialize PIR Sensor */
void initPIR() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
    if (Device.configuration.isPIR[i]) {
      Pir[i].begin(i);
    }
  }
}

/* Code related to processing PIR events and Relay auto turn off */
void mainPIR() {

  for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
    if (Device.configuration.isPIR[i]) {

      /* Listener */
      Pir[i].listener();

      /* Code related to detecting motion */
      if (Pir[i].stateChanged()) {
        Led.on();
        MQTTPublishPIRState(i);
        if (Pir[i].configuration.relayId != 9 &&
            Pir[i].get() == PIR_OPEN) { // 9 is none
          Relay[Pir[i].configuration.relayId].setTimer(
              Pir[i].configuration.howLongKeepRelayOn);
          if (Pir[i].configuration.invertRelayState) {
            if (Relay[Pir[i].configuration.relayId].get() == RELAY_ON) {
              Relay[Pir[i].configuration.relayId].off(
                  Pir[i].configuration.invertRelayState);
              MQTTPublishRelayState(Pir[i].configuration.relayId);
            }
          } else {
            if (Relay[Pir[i].configuration.relayId].get() == RELAY_OFF) {
              Relay[Pir[i].configuration.relayId].on(
                  Pir[i].configuration.invertRelayState);
              MQTTPublishRelayState(Pir[i].configuration.relayId);
            }
          }
        }
        Led.off();
      }

      /* Relay autoTurnOff code */
      if (Relay[Pir[i].configuration.relayId].autoTurnOff(
              Pir[i].configuration.invertRelayState)) {
        Led.on();
        MQTTPublishRelayState(i);
        Relay[Pir[i].configuration.relayId].clearTimer();
        Led.off();
      }
    }
  }
}
