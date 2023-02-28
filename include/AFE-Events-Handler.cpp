/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

/* ---------Headers ---------*/
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

void eventsListener(void) {
  /* Event: connected to WiFi*/
  if (Network->eventConnected()) {

    /* Actions to run only on Normal mode */
 // if (Device->getMode() == AFE_MODE_NORMAL) {

    _RestAPI->checkAccessToWAN();

  } /* End of Network->eventConnected() */
}

#endif /* Domoticz  */