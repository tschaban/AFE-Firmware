/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* Initializing Switches */
void initializeSwitch() {
  for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
    Switch[i].begin(i, &Device);
  }
}

/* Method processes Switch related events */
void processSwitchEvents() {
  if (Device.getMode() == AFE_MODE_NORMAL) {
    for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed() &&
          Switch[i].getFunctionality() != AFE_SWITCH_FUNCTIONALITY_NONE &&
          Switch[i].getControlledRelayID() != AFE_HARDWARE_ITEM_NOT_EXIST &&
          Switch[i].getControlledRelayID() + 1 <=
              Device.configuration.noOfRelays) {

#ifdef DEBUG
        Serial << endl
               << "Switch pressed with assigned Relay: "
               << Switch[i].getControlledRelayID();
#endif

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* The code here is only appilcable for a Switch that controlls a Gate
         */
        if (Relay[Switch[i].getControlledRelayID()].gateId !=
            AFE_HARDWARE_ITEM_NOT_EXIST) {
          if (Relay[Switch[i].getControlledRelayID()].gateId <=
              Device.configuration.noOfGates) {

#ifdef DEBUG
            Serial << endl
                   << "- Relay is assigned to a gate: "
                   << Relay[Switch[i].getControlledRelayID()].gateId;
#endif

            Gate[Relay[Switch[i].getControlledRelayID()].gateId].toggle();
          }
        } else {
#endif

          Relay[Switch[i].getControlledRelayID()].toggle();
          MQTTPublishRelayState(Switch[i].getControlledRelayID());
          DomoticzPublishRelayState(Switch[i].getControlledRelayID());

#ifdef AFE_CONFIG_HARDWARE_GATE
          /* The code here is only appilcable for a Switch that controlls a Gate
           */
        }
#endif

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }

      if (Switch[i].isPressed(true)) {
        if (Device.configuration.api.mqtt) {
          MQTTPublishSwitchState(i);
        }

        if (Device.configuration.api.domoticz) {
          if (Switch[i].getDomoticzIDX() > 0) {
            Domoticz.sendSwitchCommand(Switch[i].getDomoticzIDX(),
                                       Switch[i].getPhisicalState() ? "On"
                                                                    : "Off");
          }
        }
      }
    }
  }
}

void switchEventsListener() {

  /* Listens for switch events */
  for (uint8_t i = 0; i < Device.configuration.noOfSwitches; i++) {
    Switch[i].listener();

    /* One of the Multifunction switches pressed for 10 seconds */
    if (Switch[i].getFunctionality() == AFE_SWITCH_FUNCTIONALITY_MULTI) {
      if (Switch[i].is10s()) {
        Device.getMode() == AFE_MODE_NORMAL
            ? Device.reboot(AFE_MODE_ACCESS_POINT)
            : Device.reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is5s()) {
        Device.getMode() == AFE_MODE_NORMAL
            ? Device.reboot(AFE_MODE_CONFIGURATION)
            : Device.reboot(AFE_MODE_NORMAL);
      } else if (Switch[i].is30s()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        Device.setDevice();
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
        Device.reboot(AFE_MODE_ACCESS_POINT);
      }
    }
  }
}
