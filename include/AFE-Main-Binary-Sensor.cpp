/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR

/* ---------Headers ---------*/

void initializeBinarySensor(void);
void binarySensorEventsListener(void);

/* --------- Body -----------*/

/* Initializing Binary Sensor */
void initializeBinarySensor(void) {
  for (uint8_t i = 0; i < Device.configuration.noOfBinarySensors; i++) {

#ifdef AFE_CONFIG_HARDWARE_MCP23017
    BinarySensor[i].addMCP23017Reference(&MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23017
    BinarySensor[i].begin(i, &Data);
  }
#ifdef DEBUG
  Serial << endl << F("INFO: Binary sensors initialized");
#endif
}

void binarySensorEventsListener(void) {
  /* Listens for sensor events */
  for (uint8_t i = 0; i < Device.configuration.noOfBinarySensors; i++) {
    if (BinarySensor[i].listener()) {
      MqttAPI.publishBinarySensorState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishBinarySensorState(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR