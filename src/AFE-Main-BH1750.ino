#if defined(T6_CONFIG)

/* Initializing sensor */

/* Main code for processing sesnor */
void mainBH1750Sensor() {
  {
    //  if (Device.configuration.isBH1750) {
    /* Sensor: listener */
    BH1750Sensor.listener();
    if (BH1750Sensor.isReady()) {
      float sensorData = BH1750Sensor.get();
      MQTTPublishLightLevel(sensorData);
      DomoticzPublishLightLevel(sensorData);
      //      }
    }
  }
}
#endif
