/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_TLS2561

/* ---------Headers ---------*/

void initializeTLS2561Sensor(void);
void TLS2561SensorEventsListener(void);

/* --------- Body -----------*/

void initializeTLS2561Sensor(void) {
  if (Device.configuration.noOfTLS2561s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfTLS2561s; i++) {
#ifdef AFE_ESP32
      TLS2561Sensor[i].begin(i, &WirePort0, &WirePort1);
#else
      TLS2561Sensor[i].begin(i, WirePort0);
#endif
    }
  }
}

/* Main code for processing sesnor */
void TLS2561SensorEventsListener(void) {
  if (Device.configuration.noOfTLS2561s > 0) {
    /* Sensor: listener */
    for (uint8_t i = 0; i < Device.configuration.noOfTLS2561s; i++) {
      if (TLS2561Sensor[i].listener()) {
        MqttAPI.publishTLS2561SensorData(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        HttpDomoticzAPI.publishTLS2561SensorData(i);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED_DEVICE_LIGHT_EFFECT
        if (Device.configuration.effectDeviceLight) {
          if (TLS2561Sensor[i].data < 100) {
            CLedDeviceLight.on();
          } else if (TLS2561Sensor[i].data >= 100) {
            CLedDeviceLight.off();
          }
        }
#endif
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_TLS2561
