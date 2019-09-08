/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BMEX80

void initializeBMEX80Sensor() {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i].begin(i);
    }
  }
}

void BMEX80SensorEventsListener() {
  if (Device.configuration.noOfBMEX80s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBMEX80s; i++) {
      BMEX80Sensor[i].listener();
      if (BMEX80Sensor[i].isReady()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        MQTTPublishBMEX80SensorData(i);
        DomoticzPublishBMEX80SensorData(i);
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
