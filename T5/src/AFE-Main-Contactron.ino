/* Initializing Switches */
void initSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
    if (Device.configuration.isContactron[i]) {
      Switch[i].begin(i);
    } else {
      break;
    }
  }
}
