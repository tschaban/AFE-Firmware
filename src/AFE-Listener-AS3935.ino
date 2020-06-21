/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

/* Warning: for now it's hardcoded for one sensor only */

/* Initializing the sensor */
void initializeAS3935Sensor() {
  if (Device.configuration.noOfAS3935s > 0) {
    AS3935Sensor[0].begin(0);
  }
}

/* Main code for processing sensor */
void AS3935SensorEventsListener() {
  if (Device.configuration.noOfAS3935s > 0) {

    if (AS3935Sensor[0].listener()) {
      //MqttAPI.publishAS3935SensorData(0);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishAS3935SensorData(0);
#endif
    }
  }
}
#endif
