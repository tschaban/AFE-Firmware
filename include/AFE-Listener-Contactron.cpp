/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

/* Method initialize contactrons */
void initializeContractons() {
#ifdef DEBUG
  Serial << endl << F("INFO: Initializing contractons";
#endif
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].begin(i, &Device, &Data);
#ifdef DEBUG
    Serial << endl << F("INFO: Contactron: ") << i << F(" initialized";
#endif
  }
}

/* Method processes Contactron's related events */
void contractonEventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].listener();
    if (Contactron[i].changed()) {
#ifdef DEBUG
      Serial << endl << F("INFO: Contactron: ") << i << F(" has changed the state";
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE

#ifdef DEBUG
      Serial << F(", it's assigned to a Gate= ") << Contactron[i].gateId;
#endif

      if (Contactron[i].gateId != AFE_HARDWARE_ITEM_NOT_EXIST) {
        Gate[Contactron[i].gateId].triggerEvent();
      }
#endif

        MqttAPI.publishContactronState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishContactronState(i);
#endif
    }
  }
}

#endif
