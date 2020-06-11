/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_SWITCH

/* Initializing Switches */
void initializeSwitch() {
  for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
    Switch[i].begin(i, &Device);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: Switch(es) initialized");
#endif
}

/* Method processes Switch related events */
void processSwitchEvents() {
  if (Device.getMode() == AFE_MODE_NORMAL) {
    for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed() &&
          Switch[i].configuration.functionality !=
              AFE_SWITCH_FUNCTIONALITY_NONE &&
          Switch[i].configuration.relayID != AFE_HARDWARE_ITEM_NOT_EXIST &&
          Switch[i].configuration.relayID + 1 <=
              Device.configuration.noOfRelays) {

#ifdef DEBUG
        Serial << endl
               << F("INFO: Switch pressed with assigned Relay: ")
               << Switch[i].configuration.relayID;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* The code here is only appilcable for a Switch that controlls a Gate
         */
        if (Relay[Switch[i].configuration.relayID].gateId !=
            AFE_HARDWARE_ITEM_NOT_EXIST) {
          if (Relay[Switch[i].configuration.relayID].gateId <=
              Device.configuration.noOfGates) {

#ifdef DEBUG
            Serial << endl
                   << F("INFO: Relay is assigned to a gate: ")
                   << Relay[Switch[i].configuration.relayID].gateId;
#endif
            Gate[Relay[Switch[i].configuration.relayID].gateId].toggle();
          }
        } else {
#endif

          Relay[Switch[i].configuration.relayID].toggle();
          MqttAPI.publishRelayState(Switch[i].configuration.relayID);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          HttpDomoticzAPI.publishRelayState(Switch[i].configuration.relayID);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
          /* The code here is only appilcable for a Switch that controlls a Gate
           */
        }
#endif
      }

      if (Switch[i].isPressed(true)) {
        MqttAPI.publishSwitchState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishSwitchState(i);
#endif
      }
    }
  }
}

void switchEventsListener() {

  /* Listens for switch events */
  for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
    Switch[i].listener();

    /* One of the Multifunction switches pressed for 10 seconds */
    if (Switch[i].configuration.functionality ==
        AFE_SWITCH_FUNCTIONALITY_MULTI) {
      if (Switch[i].is10s()) {
        Device.getMode() == AFE_MODE_NORMAL
            ? Device.reboot(AFE_MODE_ACCESS_POINT)
            : Device.reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is5s()) {
        Device.getMode() == AFE_MODE_NORMAL
            ? Device.reboot(AFE_MODE_CONFIGURATION)
            : Device.reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is30s()) {
        Device.setDevice();
        Device.reboot(AFE_MODE_ACCESS_POINT);
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_SWITCH