/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

void initRelay() {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
    Relay[i].begin(i);
#ifdef CONFIG_HARDWARE_RELAY
    // @TODO does not have to be set for Relay controlling a Gate
    Relay[i].setRelayAfterRestoringPower();
#endif
  }
}

#ifdef CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void relayEventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
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
  }
}
#endif
