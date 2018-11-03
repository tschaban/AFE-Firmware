/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Method processes gate related events */
void mainGate() {

  if (Gate.event()) {
    Led.on();
    MQTTPublishGateState();
    DomoticzPublishGateState();
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i]) {
        MQTTPublishContactronState(i);
        DomoticzPublishContactronState(i);
      }
    }
    Led.off();
  }
}
