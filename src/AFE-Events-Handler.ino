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
    /* Update relay status to Domoticz */
    if (Device.configuration.api.domoticz) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif

#ifdef CONFIG_HARDWARE_GATE
      for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
        DomoticzPublishGateState(i);
      }
#endif

#ifdef CONFIG_HARDWARE_CONTACTRON
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        DomoticzPublishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

#ifdef CONFIG_HARDWARE_RELAY
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
#ifdef CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
          DomoticzPublishRelayState(i);
#ifdef CONFIG_HARDWARE_GATE
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
      for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
        if (Device.configuration.isPIR[i]) {
          DomoticzPublishPirState(i);
        } else {
          break;
        }
      }
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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
#ifdef CONFIG_HARDWARE_GATE
      for (uint8_t i = 0; i < Device.configuration.noOfGates; i++) {
        MQTTPublishGateState(i);
      }
#endif

#ifdef CONFIG_HARDWARE_CONTACTRON
      for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
        MQTTPublishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i].get();
      }
#endif

/* Setting Relay state after connection to MQTT */
#ifdef CONFIG_HARDWARE_RELAY
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {

#ifdef CONFIG_HARDWARE_GATE
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
#ifdef CONFIG_HARDWARE_GATE
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

      /* Publishing state of Switch to MQTT */
      for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
        MQTTPublishSwitchState(i);
      }

/* Subscribing to MQTT ADC commands */
#ifdef CONFIG_HARDWARE_ADC_VCC
      if (Device.configuration.isAnalogInput) {
        Mqtt.subscribe(AnalogInput.getMQTTCommandTopic());
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
