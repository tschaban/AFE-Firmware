/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0


/* ---------Headers ---------*/

void initializeHPMA115S0Sensor(void);
void HPMA115S0SensorEventsListener(void);

/* --------- Body -----------*/

/* Initializing sensor */
void initializeHPMA115S0Sensor(void) {
  if (Device.configuration.noOfHPMA115S0s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfHPMA115S0s; i++) {
      HPMA115S0Sensor[i].begin(i);
    }
  }
}

/* Main code for processing sesnor */
void HPMA115S0SensorEventsListener(void) {
  if (Device.configuration.noOfHPMA115S0s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfHPMA115S0s; i++) {
      HPMA115S0Sensor[i].listener();
      if (HPMA115S0Sensor[i].isReady()) {
        MqttAPI.publishHPMA115S0SensorData(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishHPMA115S0SensorData(i);
#endif
      }
    }
  }
}
#endif
