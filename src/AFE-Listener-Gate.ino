/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifdef CONFIG_HARDWARE_GATE

/* Method processes gate related events */
void gateEventsListener() {
  for (uint8_t gateId = 0; gateId < Device.configuration.noOfGates; gateId++) {
    if (Gate[gateId].event()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif
      if (lastPublishedGateStatus != Gate[gateId].get()) {
        MQTTPublishGateState(gateId);
        DomoticzPublishGateState(gateId);
        lastPublishedGateStatus = Gate[gateId].get();
      }
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.off();
#endif
    }
  }
}

#endif
