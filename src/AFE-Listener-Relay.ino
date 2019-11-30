/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

void initializeRelay() {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
    Relay[i].begin(i);
#ifdef AFE_CONFIG_HARDWARE_RELAY
    // @TODO does not have to be set for Relay controlling a Gate
    Relay[i].setRelayAfterRestoringPower();
#endif
  }
}

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void relayEventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate listener is not needed. Skipping such
     * relays */
    if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      if (Relay[i].autoTurnOff()) {
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(i);
      }
#ifdef AFE_CONFIG_HARDWARE_GATE
      /* Closing the condition for skipping relay if assigned to a gate */
    }
#endif
  }
}
#endif
