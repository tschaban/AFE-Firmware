#ifdef CONFIG_HARDWARE_BH1750

/* Main code for processing sesnor */
void mainBH1750Sensor() {
  {
    if (Device.configuration.isBH1750) {
      /* Sensor: listener */
      BH1750Sensor.listener();
      if (BH1750Sensor.isReady()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        float sensorData = BH1750Sensor.get();
        MQTTPublishLightLevel(sensorData);
        DomoticzPublishLightLevel(sensorData);
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
