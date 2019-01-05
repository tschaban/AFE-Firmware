#ifdef CONFIG_HARDWARE_HPMA115S0

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
#ifdef CONFIG_HARDWARE_LED
        Led.on();
#endif
        HPMA115S0_DATA sensorData;
        sensorData = ParticleSensor.get();
        MQTTPublishParticleSensorData(sensorData);
        DomoticzPublishParticleSensorData(sensorData);
#ifdef CONFIG_HARDWARE_LED
        Led.off();
#endif
      }
    }
  }
}
#endif
