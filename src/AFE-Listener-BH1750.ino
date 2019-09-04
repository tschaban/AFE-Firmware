/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BH1750

void initializeBH1750Sensor() {}

/* Main code for processing sesnor */
void BH1750SensorEventsListener() {
  {
    if (Device.configuration.noOfBH1750s > 0) {
      /* Sensor: listener */
      for (uint8_t i = 0; i < Device.configuration.noOfBH1750s; i++) {
        BH1750Sensor[i].listener();
        if (BH1750Sensor[i].isReady()) {
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          Led.on();
#endif
          MQTTPublishBH1750SensorData(i);
          DomoticzPublishLightLevel(i);
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          Led.off();
#endif
        }
      }
    }
  }
}
#endif
