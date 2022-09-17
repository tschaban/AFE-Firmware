/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_SWITCH

/* ---------Headers ---------*/

void initializeSwitch(void);
void processSwitchEvents(void);
void switchEventsListener(void);

/* --------- Body -----------*/

/* Initializing Switches */
void initializeSwitch(void) {
  for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
    Switch[i].addMCP23017Reference(MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#ifdef AFE_CONFIG_HARDWARE_LED
    Switch[i].begin(i, Data, Led);
#else
    Switch[i].begin(i, Data);
#endif
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Switch(es) initialized");
#endif
}

/**
 * @brief Method processes Switch related events
 *
 */
void processSwitchEvents(void) {

  if (Device->getMode() == AFE_MODE_NORMAL) {
    for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
      /**
       * @brief One of the switches has been shortly pressed and there is
       * assigned functionaity to it
       *
       */

      if (Switch[i].isPressed() &&
          Switch[i].configuration->functionality !=
              AFE_SWITCH_FUNCTIONALITY_NONE) {

#ifdef AFE_CONFIG_HARDWARE_RELAY
        if (Switch[i].configuration->relayID != AFE_HARDWARE_ITEM_NOT_EXIST &&
            Switch[i].configuration->relayID + 1 <=
                Device->configuration.noOfRelays) {

#ifdef DEBUG
          Serial << endl
                 << F("INFO: Switch pressed with assigned Relay: ")
                 << Switch[i].configuration->relayID;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
          /**
           * @brief The code here is only appilcable for a Switch that controlls
           * a
           * Gate
           *
           */
          if (Relay[Switch[i].configuration.relayID].gateId !=
              AFE_HARDWARE_ITEM_NOT_EXIST) {
            if (Relay[Switch[i].configuration.relayID].gateId <=
                Device->configuration.noOfGates) {

#ifdef DEBUG
              Serial << endl
                     << F("INFO: Relay is assigned to a gate: ")
                     << Relay[Switch[i].configuration.relayID].gateId;
#endif
              Gate[Relay[Switch[i].configuration.relayID].gateId].toggle();
            }
          } else {
#endif

            Relay[Switch[i].configuration->relayID].toggle();
            MqttAPI->publishRelayState(Switch[i].configuration->relayID);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
            HttpDomoticzAPI->publishRelayState(
                Switch[i].configuration->relayID);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
            /**
             * @brief The code here is only appilcable for a Switch that
             * controlls
             * a Gate
             *
             */
          }
#endif
        }
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
        /**
         * @brief Processing switch short press assigned to RGB LED
         *
         */
        if (Switch[i].configuration->rgbLedID != AFE_HARDWARE_ITEM_NOT_EXIST &&
            Switch[i].configuration->rgbLedID + 1 <=
                Device->configuration.noOfCLEDs) {

#ifdef DEBUG
          Serial << endl
                 << F("INFO: Switch pressed with assigned RGB LED: ")
                 << Switch[i].configuration->rgbLedID;
#endif

          CLEDStrip->toggle(Switch[i].configuration->rgbLedID, true);
          MqttAPI->publishCLEDState(Switch[i].configuration->rgbLedID);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
/**
 * @brief HTTP API for Domoticz is nos supported in T7 version for RGB LED
 *

HttpDomoticzAPI->publishCLEDState(Switch[i].configuration->rgbLedID);
   */
#endif
        }

#endif // AFE_CONFIG_HARDWARE_CLED

      } // Switch is pressed

      if (Switch[i].isPressed(true)) {
        MqttAPI->publishSwitchState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishSwitchState(i);
#endif
      }
    }
  }
}

void switchEventsListener(void) {

  /**
   * @brief Listens for switch events
   *
   */
  for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
    Switch[i].listener();

    /**
     * @brief One of the Multifunction switches pressed for 10 seconds
     *
     */
    if (Switch[i].configuration->functionality ==
        AFE_SWITCH_FUNCTIONALITY_MULTI) {
      if (Switch[i].is10s()) {
        Device->getMode() == AFE_MODE_NORMAL
            ? Device->reboot(AFE_MODE_ACCESS_POINT)
            : Device->reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is5s()) {
        Device->getMode() == AFE_MODE_NORMAL
            ? Device->reboot(AFE_MODE_CONFIGURATION)
            : Device->reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is30s()) {
        Device->setDevice();
        Device->reboot(AFE_MODE_ACCESS_POINT);
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_SWITCH