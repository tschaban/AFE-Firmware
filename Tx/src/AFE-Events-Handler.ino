void eventsListener() {
  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {
#ifdef DEBUG
    Serial << endl << "Events listener: triggered";
#endif
    /* Update relay status to Domoticz */
    if (Device.configuration.domoticzAPI) {
#ifndef T0_SHELLY_1_CONFIG
      Led.on();
#endif

#if defined(T5_CONFIG)
      DomoticzPublishGateState();
      for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
        if (Device.configuration.isContactron[i]) {
          DomoticzPublishContactronState(i);
        }
      }
#else
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {
          DomoticzPublishRelayState(i);
          lastPublishedContactronState[i] = Gate.Contactron[i].get();
        } else {
          break;
        }
      }
#endif

#ifndef T0_SHELLY_1_CONFIG
      Led.off();
#endif
    }
  } /* End of Network.eventConnected() */
}
