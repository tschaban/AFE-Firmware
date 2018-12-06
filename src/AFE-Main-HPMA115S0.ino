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
        HPMA115S0_DATA sensorData;
        sensorData = ParticleSensor.get();
        MQTTPublishParticleSensorData(sensorData);
        DomoticzPublishParticleSensorData(sensorData);
      }
    }
  }
}
#endif
