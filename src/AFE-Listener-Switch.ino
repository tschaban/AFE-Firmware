/* Initializing Switches */
void initSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      Switch[i].begin(i, &Device);
    } else {
      break;
    }
  }
}

/* Method processes Switch related events */
void mainSwitch() {
  if (Device.getMode() == MODE_NORMAL) {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
      if (Device.configuration.isSwitch[i]) {
        /* One of the switches has been shortly pressed */
        if (Switch[i].isPressed() &&
            Switch[i].getFunctionality() != SWITCH_FUNCTIONALITY_NONE &&
            Switch[i].getControlledRelayID() > 0) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          Led.on();
#endif

#ifdef CONFIG_FUNCTIONALITY_GATE
          Gate.toggle();
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY
          Relay[Switch[i].getControlledRelayID() - 1].toggle();
          MQTTPublishRelayState(Switch[i].getControlledRelayID() - 1);
          DomoticzPublishRelayState(Switch[i].getControlledRelayID() - 1);
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
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

      } else {
        break;
      }
    }
  }
}

void mainSwitchListener() {

  /* Listens for switch events */
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {

      Switch[i].listener();

      /* One of the Multifunction switches pressed for 10 seconds */
      if (Switch[i].getFunctionality() == SWITCH_FUNCTIONALITY_MULTI) {
        if (Switch[i].is10s()) {
          Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                          : Device.reboot(MODE_NORMAL);
        } else if (Switch[i].is5s()) {
          Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                          : Device.reboot(MODE_NORMAL);
        } else if (Switch[i].is30s()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          Led.on();
#endif
          Device.setDevice();
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          Led.off();
#endif
          Device.reboot(MODE_ACCESS_POINT);
        }
      }
    } else {
      break;
    }
  }
}
