/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Method processes gate related events */
void mainGate() {
  if (Gate.event()) {
    Led.on();
    MQTTPublishGateState();
    Led.off();
  }
}
