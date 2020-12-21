/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_RELAY

/* ---------Headers ---------*/

void initializeRelay(void);

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
void relayEventsListener(void);
#endif

/* --------- Body -----------*/

void initializeRelay(void) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  uint8_t MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
    Relay[i].initialize(&Data, i);
#ifdef AFE_CONFIG_HARDWARE_MCP23017
    if (Relay[i].configuration.mcp23017.address !=
        AFE_HARDWARE_ITEM_NOT_EXIST) {
      MCP23017Id =
          MCP23017Broker.getId(Relay[i].configuration.mcp23017.address);
      if (MCP23017Id != AFE_HARDWARE_ITEM_NOT_EXIST) {
        Relay[i].addMCP23017Reference(&MCP23017Broker.MCP[MCP23017Id]);
      }
    }
#endif // AFE_CONFIG_HARDWARE_MCP23017
    Relay[i].begin();
    // @TODO does not have to be set for Relay controlling a Gate
    Relay[i].setRelayAfterRestoringPower();
  }
}

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void relayEventsListener(void) {
  for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate listener is not needed. Skipping such
     * relays */
    if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      if (Relay[i].autoTurnOff()) {
        MqttAPI.publishRelayState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishRelayState(i);
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
