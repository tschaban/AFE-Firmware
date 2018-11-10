

void initRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      Relay[i].begin(i);
#if !defined(T5_CONFIG)
      Relay[i].setRelayAfterRestoringPower();
#endif
    } else {
      break;
    }
  }
}

#if !defined(T5_CONFIG) /* Relay is not controlled in T5 firmware */
/* Method checks if any relay should be automatically turned off */
void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
#ifndef T0_SHELLY_1_CONFIG
        Led.on();
#endif
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(i);
#ifndef T0_SHELLY_1_CONFIG
        Led.off();
#endif
      }
    } else {
      break;
    }
  }
}
#endif
