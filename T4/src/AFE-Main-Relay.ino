void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
        Led.on();
        Mqtt.publish(Relay[i].getMQTTTopic(), "state", "off");
        Led.off();
      }
    } else {
      break;
    }
  }
}
