/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

/* ---------Headers ---------*/

void initializeContracton(void);
void contractonEventsListener(void);

/* --------- Body -----------*/


/* Method initialize contactrons */
void initializeContracton() {
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing contractons");
#endif
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].begin(i, &Device, &Data);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: Contactron: ") << i << F(" initialized");
#endif
  }
}

/* Method processes Contactron's related events */
void contractonEventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfContactrons; i++) {
    Contactron[i].listener();
    if (Contactron[i].changed()) {
#ifdef DEBUG
      Serial << endl << F("INFO: Contactron: ") << i << F(" has changed the state");
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE

#ifdef DEBUG
      Serial << F(", it's assigned to a Gate= ") << Contactron[i].gateId;
#endif

      if (Contactron[i].gateId != AFE_HARDWARE_ITEM_NOT_EXIST) {
        Gate[Contactron[i].gateId].triggerEvent();
      }
#endif

        MqttAPI->publishContactronState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishContactronState(i);
#endif
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_CONTACTRON
