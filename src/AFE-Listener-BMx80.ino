/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMX80

void initializeBMx80Sensor() {}

void BMx80SensorEventsListener() {
  if (Device.configuration.noOfBMx80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMx80s; i++) {
      BMx80Sensor[i].listener();
      if (BMx80Sensor[i].isReady()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        MQTTPublishBMx80SensorData(i);
        DomoticzPublishBMx80SensorData(i);
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
