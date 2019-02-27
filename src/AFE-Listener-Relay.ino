

void initRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      Relay[i].begin(i);
#ifdef CONFIG_FUNCTIONALITY_RELAY
      Relay[i].setRelayAfterRestoringPower();
#endif
    } else {
      break;
    }
  }
}

#ifdef CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(i);
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    } else {
      break;
    }
  }
}
#endif
