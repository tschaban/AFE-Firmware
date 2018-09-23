/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing Relays */
void initRelay() {
  Relay[0].begin(0);
  Relay[0].setRelayAfterRestoringPower();
  Relay[0].setTimerUnitToSeconds(false);
}

/* Method checks if any relay should be automatically turned off */
void mainRelay() {
  if (Relay[0].autoTurnOff()) {
    Led.on();
    // MQTTPublishRelayState(0);
    Led.off();
  }
}
