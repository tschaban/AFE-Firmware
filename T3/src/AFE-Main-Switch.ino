/* Initializing Switches */
void initSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      Switch[i].begin(i);
    }
  }
}

/* Method processes Switch related events */
void mainSwitch() {

  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed() && Switch[i].getFunctionality() != 0) {
        Led.on();
        Relay[Switch[i].getFunctionality() - 11].toggle();
        MQTTPublishRelayState(Switch[i].getFunctionality() -
                              11); // MQTT Listener library
        Led.off();
      }
    }
  }
}

/* Methods listens for switch related events */
void mainSwitchListener() {

  /* Listens for switch events */
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      Switch[i].listener();

      /* One of the Multifunction switches pressed for 10 seconds */
      if (Switch[i].getFunctionality() == SWITCH_MULTI) {
        if (Switch[i].is10s()) {
          Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                          : Device.reboot(MODE_NORMAL);
        } else if (Switch[i].is5s()) {
          Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                          : Device.reboot(MODE_NORMAL);
        }
      }
    }
  }
}
