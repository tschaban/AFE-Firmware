/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

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
      if (PN532Sensor[i].readTag()) {
        MqttAPI.publishPN532SensorData(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishPN532SensorData();
#endif
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
