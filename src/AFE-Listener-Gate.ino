/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#if defined(T5_CONFIG)

/* Method processes gate related events */
void mainGate() {
  for (uint8_t gateId = 1; gateId <= Device.configuration.noOfGates; gateId++) {
    if (Gate[gateId].event()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif
      if (lastPublishedGateStatus != Gate[gateId].get()) {
        MQTTPublishGateState(gateId);
        DomoticzPublishGateState(gateId);
        lastPublishedGateStatus = Gate[gateId].get();
      }
      for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
        if (Device.configuration.isContactron[i] &&
            lastPublishedContactronState[i] !=
                Gate[gateId].Contactron[i].get()) {
          MQTTPublishContactronState(i);
          DomoticzPublishContactronState(gateId, i);
          lastPublishedContactronState[i] = Gate[gateId].Contactron[i].get();
        }
      }
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.off();
#endif
    }
  }
}

#endif
