

void initRelay() {
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
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
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(i);
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    } else {
      break;
    }
  }
}
#endif
