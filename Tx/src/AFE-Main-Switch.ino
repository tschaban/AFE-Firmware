/* Initializing Switches */
void initSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      Switch[i].begin(i);
    } else {
      break;
    }
  }
}

/* Method processes Switch related events */
void mainSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed() && Switch[i].getControlledRelayID() > 0) {
#ifndef T0_SHELLY_1_CONFIG
        Led.on();
#endif

#if defined(T5_CONFIG) /* Control either gate or a relay */
        Gate.toggle();
#else
        Relay[Switch[i].getControlledRelayID() - 1].toggle();
        MQTTPublishRelayState(Switch[i].getControlledRelayID() - 1);
        DomoticzPublishRelayState(Switch[i].getControlledRelayID() - 1);
#endif

#ifndef T0_SHELLY_1_CONFIG
        Led.off();
#endif
      }
    } else {
      break;
    }
  }
}

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
        } else if (Switch[i].is30s()) {
#ifndef T0_SHELLY_1_CONFIG
          Led.on();
#endif
          Device.setDevice();
#ifndef T0_SHELLY_1_CONFIG
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
