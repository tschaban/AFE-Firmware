void initRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      Relay[i].begin(i);
      Relay[i].setRelayAfterRestoringPower();
    } else {
      break;
    }
  }
}

/* Method checks if any relay should be automatically turned off */
void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
#ifndef SHELLY_1_DEVICE
        Led.on();
#endif
        Mqtt.publish(Relay[i].getMQTTTopic(), "state", "off");
        DomoticzPublishRelayState(i);
#ifndef SHELLY_1_DEVICE
        Led.off();
#endif
      }
    } else {
      break;
    }
  }
}
