/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef CONFIG_HARDWARE_CONTACTRON

/* Method initialize contactrons */
void initializeContractons() {
#ifdef DEBUG
  Serial << endl << "Initializing contractons";
#endif
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].begin(i, &Device, &Data);
#ifdef DEBUG
    Serial << endl << " - Contactron: " << i << " initialized";
#endif
  }
}

/* Method processes Contactron's related events */
void contractonEventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].listener();
    if (Contactron[i].changed()) {
#ifdef DEBUG
      Serial << endl << " - Contactron: " << i << " has changed the state";
#endif

#ifdef CONFIG_HARDWARE_GATE

#ifdef DEBUG
      Serial << ", it's assigned to a Gate= " << Contactron[i].gateId;
#endif

      if (Contactron[i].gateId != AFE_HARDWARE_ITEM_NOT_EXIST) {
        Gate[Contactron[i].gateId].triggerEvent();
      }
#endif

      MQTTPublishContactronState(i);
      DomoticzPublishContactronState(i);
    }
  }
}

#endif
