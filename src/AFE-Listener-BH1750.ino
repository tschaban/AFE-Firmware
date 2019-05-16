#ifdef CONFIG_HARDWARE_BH1750

/* Main code for processing sesnor */
void mainBH1750Sensor() {
  {
    if (Device.configuration.isBH1750) {
      /* Sensor: listener */
      BH1750Sensor.listener();
      if (BH1750Sensor.isReady()) {
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        float sensorData = BH1750Sensor.get();
        MQTTPublishLightLevel(sensorData);
        DomoticzPublishLightLevel(sensorData);
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    }
  }
}
#endif
