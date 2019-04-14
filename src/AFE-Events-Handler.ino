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

#if defined(T5_CONFIG)
      DomoticzPublishGateState();
      for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
        if (Device.configuration.isContactron[i]) {
          DomoticzPublishContactronState(i);
          lastPublishedContactronState[i] = Gate.Contactron[i].get();
        } else {
          break;
        }
      }
#else
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {
          DomoticzPublishRelayState(i);
        } else {
          break;
        }
      }

#if defined(T3_CONFIG)
      for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
        if (Device.configuration.isPIR[i]) {
          DomoticzPublishPirState(i);
        } else {
          break;
        }
      }
#endif

#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.off();
#endif
    }
  } /* End of Network.eventConnected() */

  if (Device.configuration.api.mqtt) {
    if (Mqtt.eventConnected()) {
#ifdef DEBUG
      Serial << endl
             << "Connected to MQTT Server: triggering post connection updates";
#endif

/* Publishing mesages after connection to MQTT Broker has been established */
#if defined(T5_CONFIG)
      MQTTPublishGateState();
      for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
        if (Device.configuration.isContactron[i]) {
          MQTTPublishContactronState(i);
          lastPublishedContactronState[i] = Gate.Contactron[i].get();
        } else {
          break;
        }
      }
#else
      Mqtt.publishTopic("lwt", "connected");

      /* Setting Relay state after connection to MQTT */
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {
          if (!Relay[i].setRelayAfterRestoringMQTTConnection()) {
            /* Requesting state from MQTT Broker / service */
            Mqtt.publishTopic(Relay[i].getMQTTTopic(), "defaultState", "get");
          } else {
            /* Updating relay state after setting default value after MQTT
             * connected */
            MQTTPublishRelayState(i);
          }
        } else {
          break;
        }
      }

#if defined(T3_CONFIG)
      for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
        if (Device.configuration.isPIR[i]) {
          MQTTPublishPIRState(i);
        } else {
          break;
        }
      }
#endif

#endif
    }
  }
}
