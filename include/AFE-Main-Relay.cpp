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

#ifdef AFE_CONFIG_HARDWARE_GATE
    Relay[i].begin(Data, Device, i);
#else
    Relay[i].begin(Data, i);
#endif

/* Setting relay state after restoring power is not required when the relay is
 * assigned to a gate*/
#ifdef AFE_CONFIG_HARDWARE_GATE
    if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
      Relay[i].setRelayAfterRestoringPower();

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
    } else {
      Serial << endl
             << F("INFO: RELAY: Restoring relay state is not required for a "
                  "relay assigned to a gate");
#endif
    }
#endif
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Relay initialized");
#endif
}

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Method checks if any relay should be automatically turned off */
void relayEventsListener(void) {
  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
    /* For the Relay assigned to a gate listener is not needed. Skipping such
     * relays. Auto turn-off is called out in the gate event listener */
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
