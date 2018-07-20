void eventsListener() {
  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {

     /* Update relay status to Domoticz */
    if (Device.configuration.domoticzAPI) {
      Led.on();
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {
            DomoticzPublishRelayState(i);
        } else {
          break;
        }
      }
      Led.off();
    }
  } /* End of Network.eventConnected() */

}
