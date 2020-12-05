/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMEX80

/* ---------Headers ---------*/

void initializeBMX80Sensor(void);
void BMX80SensorEventsListener(void);

/* --------- Body -----------*/

void initializeBMX80Sensor(void) {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i].begin(i);
    }
  }
}

void BMX80SensorEventsListener(void) {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i].listener();
      if (BMEX80Sensor[i].isReady()) {
        MqttAPI.publishBMx80SensorData(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishBMx80SensorData(i);
#endif
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80
