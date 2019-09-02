/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMX80

void initializeBMx80Sensor() {}

void BMx80SensorEventsListener() {
  if (Device.configuration.noOfBMx80s > 0) {
    BMx80_DATA sensorDataBMx80;
    for (uint8_t i = 0; i < Device.configuration.noOfBMx80s; i++) {
      BMx80Sensor[i].listener();
      if (BMx80Sensor[i].isReady()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        sensorDataBMx80 = BMx80Sensor[i].get();
        MQTTPublishBMx80SensorData(sensorDataBMx80);
        DomoticzPublishBMx80SensorData(sensorDataBMx80);
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
