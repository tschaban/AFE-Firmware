/* Initializing Switches */
void initContactron() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Contactron[i].begin(i);
    } else {
      break;
    }
  }
}
