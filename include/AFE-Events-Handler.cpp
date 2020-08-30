/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* ---------Headers ---------*/

void eventsListener(void);

/* --------- Body -----------*/

void eventsListener(void) {
  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {
#ifdef DEBUG
    Serial << endl << F("INFO: Connected to WiFi");
#endif

/* ################## HTTP DOMOTICZ ################### */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    /* Sendings hardware values to Domoticz */

    if (Device.configuration.api.domoticz) {

#ifdef DEBUG
      Serial << endl << F("INFO: Domoticz HTTP API boot actions triggering");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Led.on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
      Serial << endl << F("INFO: Sending current gate state to Domoticz");
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
        HttpDomoticzAPI.publishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
      Serial << endl
             << F("INFO: Sending current state of contactrons to Domoticz");
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        HttpDomoticzAPI.publishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
      Serial << endl << F("INFO: Sending current state of switches to Domoticz");
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
        HttpDomoticzAPI.publishSwitchState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
      Serial << endl << F("INFO: Sending current state of relays to Domoticz");
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
          HttpDomoticzAPI.publishRelayState(i);
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* Closing the condition for skipping relay if assigned to a gate */
        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("INFO: Excluding relay: ") << i
                 << F(" as it's assigned to a Gate: ") << Relay[i].gateId;
        }
#endif
#endif
      }
#endif

#if defined(T3_CONFIG)
#ifdef DEBUG
      Serial << endl << F("INFO: Sending current state of PIRs to Domoticz");
#endif
      for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
        if (Device.configuration.isPIR[i]) {
          //  DomoticzPublishPirState(i);
        } else {
          break;
        }
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Led.off();
#endif
    }
#endif /* AFE_CONFIG_API_DOMOTICZ_ENABLED  */

#ifdef DEBUG
    Serial << endl << F("INFO: Post WiFi Connection actions completed");
#endif

  } /* End of Network.eventConnected() */

  /* Checking if device has connected to MQTT Broker. If yes it subsribes to
   * MQTT Topics and sends device's items current states */
  if (Device.configuration.api.mqtt) {
    if (MqttAPI.Mqtt.eventConnected()) {
      MqttAPI.subscribe();
      MqttAPI.synchronize();
    }
  }
}
