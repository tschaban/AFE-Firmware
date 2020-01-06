/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
void eventsListener() {

  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {
#ifdef DEBUG
    Serial << endl
           << endl
           << "#### CONNECTED TO THE WIFI ####" << endl
           << endl
           << "Events listener: triggered";
#endif

/* ################## HTTP DOMOTICZ ################### */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    /* Sendings hardware values to Domoticz */
    if (Device.configuration.api.httpDomoticz) {
#ifdef AFE_CONFIG_HARDWARE_LED
      Led.on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
      Serial << endl << "Sending current gate state to Domoticz";
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
        //    DomoticzPublishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
      Serial << endl << "Sending current state of contactrons to Domoticz";
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        //    DomoticzPublishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
      Serial << endl << "Sending current state of switches to Domoticz";
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
        HttpDomoticzAPI.publishSwitchState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
      Serial << endl << "Sending current state of relays to Domoticz";
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
                 << "Excluding relay: " << i
                 << " as it's assigned to a Gate: " << Relay[i].gateId;
        }
#endif
#endif
      }
#endif

#if defined(T3_CONFIG)
#ifdef DEBUG
      Serial << endl << "Sending current state of PIRs to Domoticz";
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
  }    /* End of Network.eventConnected() */

  /* ################## MQTT ################### */

  if (Device.configuration.api.mqtt) {
    if (MqttAPI.Mqtt.eventConnected()) {
#ifdef DEBUG
      Serial << endl
             << "Connected to MQTT Server: triggering post connection updates";
#endif
// @TODO 2.1.0
// Mqtt.publishTopic(Mqtt.getLWTTopic(), "connected");

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      MqttAPI.Mqtt.subscribe(AFE_CONFIG_API_DOMOTICZ_TOPIC_OUT);
#endif

/* Publishing mesages after connection to MQTT Broker has been established */
#ifdef AFE_CONFIG_HARDWARE_GATE
      for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
        Mqtt.subscribe(Gate[i].getMQTTCommandTopic());
        MQTTPublishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        Mqtt.subscribe(Contactron[i].getMQTTCommandTopic());
        MQTTPublishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

/* Setting Relay state after connection to MQTT */
#ifdef AFE_CONFIG_HARDWARE_RELAY
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif

          /* Subscribing to MQTT Topic for Relay*/

          // @TODO 2.1.0
          //    Mqtt.subscribe(Relay[i].getMQTTCommandTopic());

          if (!Relay[i].setRelayAfterRestoringMQTTConnection()) {
            /* Requesting state from MQTT Broker / service */
            // @TODO 2.1.0
            // Mqtt.publishTopic(Relay[i].getMQTTStateTopic(), "get");
          } else {
            /* Updating relay state after setting default value after MQTT
             * connected */
            MqttAPI.publishRelayState(i);
          }
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* Closing the condition for skipping relay if assigned to a gate */
        }
#ifdef DEBUG
        else {
          Serial << endl
                 << "Excluding relay: " << i
                 << " as it's assigned to a Gate: " << Relay[i].gateId;
        }
#endif
#endif
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
      /* Publishing state of Switch to MQTT */
      for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
        MqttAPI.publishSwitchState(i);
      }
#endif

/* Subscribing to MQTT ADC commands */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      if (Device.configuration.isAnalogInput) {
        // @TODO 2.1.0
        // Mqtt.subscribe(AnalogInput.mqttCommandTopic);
      }
#endif

#if defined(T3_CONFIG)
      for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
        if (Device.configuration.isPIR[i]) {
          MQTTPublishPIRState(i);
        } else {
          break;
        }
      }
#endif
    }
  }
}
