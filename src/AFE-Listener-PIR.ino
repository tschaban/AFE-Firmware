#if defined(T3_CONFIG)
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

#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif

#ifdef DEBUG
        Serial << endl
               << "PIR: " << (Pir[i].motionDetected() ? " Yes " : " No ");
#endif

        MQTTPublishPIRState(i);
        DomoticzPublishPirState(i);

        if (Pir[i].configuration.relayId != 9 && Pir[i].motionDetected()) {
          Relay[Pir[i].configuration.relayId].setTimer(
              Pir[i].configuration.howLongKeepRelayOn);
          if (Pir[i].configuration.invertRelayState) {
            if (Relay[Pir[i].configuration.relayId].get() == RELAY_ON) {
              Relay[Pir[i].configuration.relayId].off(
                  Pir[i].configuration.invertRelayState);
              MQTTPublishRelayState(Pir[i].configuration.relayId);
              DomoticzPublishRelayState(Pir[i].configuration.relayId);
            }
          } else {
            if (Relay[Pir[i].configuration.relayId].get() == RELAY_OFF) {
              Relay[Pir[i].configuration.relayId].on(
                  Pir[i].configuration.invertRelayState);
              MQTTPublishRelayState(Pir[i].configuration.relayId);
              DomoticzPublishRelayState(Pir[i].configuration.relayId);
            }
          }
        }

#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }

      /* Relay autoTurnOff code */
      if (Relay[Pir[i].configuration.relayId].autoTurnOff(
              Pir[i].configuration.invertRelayState)) {
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(Pir[i].configuration.relayId);
        Relay[Pir[i].configuration.relayId].clearTimer();
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    }
  }
}
#endif