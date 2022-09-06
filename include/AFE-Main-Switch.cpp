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

/* Method processes Switch related events */
void processSwitchEvents(void) {
  #ifdef AFE_CONFIG_HARDWARE_RELAY
  if (Device->getMode() == AFE_MODE_NORMAL) {
    for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed() &&
          Switch[i].configuration->functionality !=
              AFE_SWITCH_FUNCTIONALITY_NONE &&
          Switch[i].configuration->relayID != AFE_HARDWARE_ITEM_NOT_EXIST &&
          Switch[i].configuration->relayID + 1 <=
              Device->configuration.noOfRelays) {

#ifdef DEBUG
        Serial << endl
               << F("INFO: Switch pressed with assigned Relay: ")
               << Switch[i].configuration->relayID;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* The code here is only appilcable for a Switch that controlls a Gate
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
          HttpDomoticzAPI->publishRelayState(Switch[i].configuration->relayID);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
          /* The code here is only appilcable for a Switch that controlls a Gate
           */
        }
#endif
      }

      if (Switch[i].isPressed(true)) {
        MqttAPI->publishSwitchState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishSwitchState(i);
#endif
      }
    }
  }
  #endif // AFE_CONFIG_HARDWARE_RELAY
}

void switchEventsListener(void) {

  /* Listens for switch events */
  for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
    Switch[i].listener();

    /* One of the Multifunction switches pressed for 10 seconds */
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