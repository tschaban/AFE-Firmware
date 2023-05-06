/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

/* ---------Headers ---------*/

void contractonEventsListener(void);

/* --------- Body -----------*/

/* Method processes Contactron's related events */
void contractonEventsListener() {
  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfContactrons; i++) {
    Hardware->Contactron[i]->listener();
    if (Hardware->Contactron[i]->changed()) {
#ifdef DEBUG
      Serial << endl << F("INFO: Contactron: ") << i << F(" has changed the state");
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE

#ifdef DEBUG
      Serial << F(", it's assigned to a Gate= ") << Hardware->Contactron[i]->gateId;
#endif

      if (Hardware->Contactron[i]->gateId != AFE_HARDWARE_ITEM_NOT_EXIST) {
        Hardware->Gate[Hardware->Contactron[i]->gateId]->triggerEvent();
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
