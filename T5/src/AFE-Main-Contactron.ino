/* Initializing Contactrons */
void initContactron() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Contactron[i].begin(i);
    } else {
      break;
    }
  }
}

/* Method processes Contactrons related events */
void mainContactron() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      /* One of the contactrons state was changed */
      if (Contactron[i].changed()) {
        Led.on();
        MQTTPublishContactronState(i); // MQTT Listener library
        Led.off();
      }
    } else {
      break;
    }
  }
}

/* Listens for contactrons events */
void mainContactronListener() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Contactron[i].listener();
    } else {
      break;
    }
  }
}
