/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_BH1750

void initializeBH1750Sensor() {
  if (Device.configuration.noOfBH1750s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfBH1750s; i++) {
      BH1750Sensor[i].begin(i);
    }
  }
}

/* Main code for processing sesnor */
void BH1750SensorEventsListener() {
    if (Device.configuration.noOfBH1750s > 0) {
      /* Sensor: listener */
      for (uint8_t i = 0; i < Device.configuration.noOfBH1750s; i++) {
        BH1750Sensor[i].listener();
        if (BH1750Sensor[i].isReady()) {
          MQTTPublishBH1750SensorData(i);
          DomoticzPublishLightLevel(i);
        }
      }
    }
}
#endif
