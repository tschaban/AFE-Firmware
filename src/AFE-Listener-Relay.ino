/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_RELAY

void initializeRelay() {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
    Relay[i].begin(i);
    // @TODO does not have to be set for Relay controlling a Gate
    Relay[i].setRelayAfterRestoringPower();
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
#if defined(AFE_CONFIG_API_MQTT_ENABLED) || defined(AFE_CONFIG_API_DOMOTICZ_ENABLED)
        MqttAPI.publishRelayState(i);
#endif
      }
#ifdef AFE_CONFIG_HARDWARE_GATE
      /* Closing the condition for skipping relay if assigned to a gate */
    }
#endif
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF


#endif // AFE_CONFIG_HARDWARE_RELAY
