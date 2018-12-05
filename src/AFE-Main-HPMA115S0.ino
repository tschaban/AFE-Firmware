#if defined(T6_CONFIG)

/* Initializing sensor */
void initHPMA115S0Sensor() {
  if (Device.configuration.isHPMA115S0) {
    ParticleSensor.begin();
  }
}

/* Main code for processing sesnor */
void mainHPMA115S0Sensor() {
  {
    if (Device.configuration.isHPMA115S0) {
      /* Sensor: listener */
      ParticleSensor.listener();
      if (ParticleSensor.isReady()) {
        uint16_t pm25, pm10;
        ParticleSensor.get(&pm25, &pm10);
        MQTTPublishParticleSensorData(pm25, HPMA115S0_TYPE_PM25);
        MQTTPublishParticleSensorData(pm10, HPMA115S0_TYPE_PM10);
        DomoticzPublishParticleSensorData(pm25, HPMA115S0_TYPE_PM25);
        delay(10);
        DomoticzPublishParticleSensorData(pm10, HPMA115S0_TYPE_PM10);
      }
    }
  }
}
#endif
