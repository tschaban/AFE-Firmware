/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

/* ---------Headers ---------*/
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

void eventsListener(void) {
  /* Event: connected to WiFi*/
  if (Network->eventConnected()) {

    /* Actions to run only on Normal mode */
    if (Device->getMode() == AFE_MODE_NORMAL) {

      /* Sendings hardware values to Domoticz */

      if (Device->configuration.api.domoticz) {

#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Domoticz HTTP API boot actions triggering");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
        Led->on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current gate state to Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfGates; i++) {
          HttpDomoticzAPI->publishGateState(i);
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current state of contactrons to "
                    "Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfContactrons; i++) {
          HttpDomoticzAPI->publishContactronState(i);
          lastPublishedContactronState[i] = Contactron[i].get();
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
        Serial
            << endl
            << F("INFO: EVENTS: Sending current state of switches to Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
          HttpDomoticzAPI->publishSwitchState(i);
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current state of binary sensors to "
                    "Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfBinarySensors; i++) {
          HttpDomoticzAPI->publishBinarySensorState(i);
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current state of RGB LEDs "
                    "Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfCLEDs; i++) {
          //     HttpDomoticzAPI->publishCLEDState(i);
          //     HttpDomoticzAPI->publishCLEDEffectState(i);
        }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current state of regulator to "
                    "Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfRegulators; i++) {
          HttpDomoticzAPI->publishRegulatorState(i);
        }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef DEBUG
        Serial << endl
               << F("INFO: EVENTS: Sending current state of regulator to "
                    "Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfThermalProtectors;
             i++) {
          HttpDomoticzAPI->publishThermalProtectorState(i);
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
        Serial
            << endl
            << F("INFO: EVENTS: Sending current state of relays to Domoticz");
#endif
        for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* For the Relay assigned to a gate code below is not needed for
           * execution
           */
          if (Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
            HttpDomoticzAPI->publishRelayState(i);
#ifdef AFE_CONFIG_HARDWARE_GATE
            /* Closing the condition for skipping relay if assigned to a gate */
          }
#ifdef DEBUG
          else {
            Serial << endl
                   << F("INFO: EVENTS: Excluding relay: ") << i
                   << F(" as it's assigned to a Gate: ") << Relay[i].gateId;
          }
#endif
#endif
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
        Led->off();
#endif
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Post WiFi Connection actions completed");
#endif

    } // if (Device->getMode() == AFE_MODE_NORMAL) {

    _RestAPI->checkAccessToWAN();

  } /* End of Network->eventConnected() */
}

#endif /* Domoticz  */