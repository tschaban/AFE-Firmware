/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_RELAY

/* ---------Headers ---------*/

void initializeRelay(void);

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
void relayEventsListener(void);
#endif

/* --------- Body -----------*/

void initializeRelay(void) {

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  Relay[i].addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
    Relay[i].begin(Data, i);
    // @TODO T5  does not have to be set for Relay controlling a Gate
    Relay[i].setRelayAfterRestoringPower();
  }
}

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void relayEventsListener(void) {
  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate listener is not needed. Skipping such
     * relays */
    if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      if (Relay[i].autoTurnOff()) {
        MqttAPI->publishRelayState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishRelayState(i);
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
