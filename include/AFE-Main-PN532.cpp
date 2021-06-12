/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_PN532

/* ---------Headers ---------*/

void initializePN532Sensor(void);
void PN532EventsListener(void);

/* --------- Body -----------*/

/* Method initialize contactrons */
void initializePN532Sensor() {
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Initializing PN532");
#endif
  for (uint8_t i = 0; i < Device.configuration.noOfPN532Sensors; i++) {
    PN532Sensor[i].begin(i, &Data);
#ifdef DEBUG
    Serial << endl << F("INFO: BOOT: PN532: ") << i << F(" initialized");
#endif
  }
}

/* Method processes Contactron's related events */
void PN532EventsListener() {
  for (uint8_t i = 0; i < Device.configuration.noOfPN532Sensors; i++) {
    if (PN532Sensor[i].listener()) {
      char json[100];
      PN532Sensor[i].getJSON(json);
      Serial << endl << json;
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_PN532
