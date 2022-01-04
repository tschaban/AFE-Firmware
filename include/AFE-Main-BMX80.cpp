/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMEX80

/* ---------Headers ---------*/

void initializeBMX80Sensor(void);
void BMX80SensorEventsListener(void);

/* --------- Body -----------*/

void initializeBMX80Sensor(void) {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
#ifdef AFE_ESP32
      BMEX80Sensor[i].begin(i, &WirePort0, &WirePort1);
#else
      BMEX80Sensor[i].begin(i, &WirePort0);
#endif
    }
  }
}

void BMX80SensorEventsListener(void) {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i].listener();
      if (BMEX80Sensor[i].isReady()) {
        MqttAPI.publishBMx80SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI.publishBMx80SensorData(i);
#endif
      }
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80
