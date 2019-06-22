/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#if defined(T5_CONFIG)

/* Method processes gate related events */
void mainGate() {
  if (Gate.event()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Led.on();
#endif
    if (lastPublishedGateStatus != Gate.get()) {
      MQTTPublishGateState();
      DomoticzPublishGateState();
      lastPublishedGateStatus = Gate.get();
    }
    for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
      if (Device.configuration.isContactron[i] &&
          lastPublishedContactronState[i] != Gate.Contactron[i].get()) {
        MQTTPublishContactronState(i);
        DomoticzPublishContactronState(i);
        lastPublishedContactronState[i] = Gate.Contactron[i].get();
      }
    }
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Led.off();
#endif
  }
}

#endif
