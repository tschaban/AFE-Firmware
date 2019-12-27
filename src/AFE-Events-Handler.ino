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
        DomoticzPublishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
Serial << endl << "Sending current state of contactrons to Domoticz";
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        DomoticzPublishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
Serial << endl << "Sending current state of switches to Domoticz";
#endif
      for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
        if (Switch[i].getDomoticzIDX() > 0) {
          Domoticz.sendSwitchCommand(Switch[i].getDomoticzIDX(),
                                     Switch[i].getPhisicalState() ? "On"
                                                                  : "Off");
        }
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
          DomoticzPublishRelayState(i);
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
          DomoticzPublishPirState(i);
        } else {
          break;
        }
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Led.off();
#endif
    }
  } /* End of Network.eventConnected() */

  /* ## MQTT ## */

  if (Device.configuration.api.mqtt) {
    if (Mqtt.eventConnected()) {
#ifdef DEBUG
      Serial << endl
             << "Connected to MQTT Server: triggering post connection updates";
#endif

      Mqtt.publishTopic(Mqtt.getLWTTopic(), "connected");

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
          Mqtt.subscribe(Relay[i].getMQTTCommandTopic());

          if (!Relay[i].setRelayAfterRestoringMQTTConnection()) {
            /* Requesting state from MQTT Broker / service */
            Mqtt.publishTopic(Relay[i].getMQTTStateTopic(), "get");
          } else {
            /* Updating relay state after setting default value after MQTT
             * connected */
            MQTTPublishRelayState(i);
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
        MQTTPublishSwitchState(i);
      }
#endif

/* Subscribing to MQTT ADC commands */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      if (Device.configuration.isAnalogInput) {
        Mqtt.subscribe(AnalogInput.mqttCommandTopic);
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
