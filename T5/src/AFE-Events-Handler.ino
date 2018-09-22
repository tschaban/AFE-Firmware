void eventsListener() {
  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {

    /* Update relay status to Domoticz */
    if (Device.configuration.domoticzAPI) {
      Led.on();
      /* Update Gate state to Domoticz */
      DomoticzPublishGateState();
      /* Update Contactrons state to Domoticz */
      for (uint8_t i = 0; i < sizeof(Device.configuration.isContactron); i++) {
        if (Device.configuration.isContactron[i]) {
          DomoticzPublishContactronState(i);
        }
      }
      Led.off();
    }
  } /* End of Network.eventConnected() */
}
