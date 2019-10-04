/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_AS3935

void initializeAS3935Sensor() {
  if (Device.configuration.noOfAS3935s > 0) {
    
      BH1750Sensor[i].begin(i);
    
  }
}

/* Main code for processing sesnor */
void AS3935SensorEventsListener() {
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
