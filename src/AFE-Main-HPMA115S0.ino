#if defined(T6_CONFIG)

/* Initializing sensor */
void initHPMA115S0Sensor() { ParticleSensor.begin(); }

/* Main code for processing sesnor */
void mainHPMA115S0Sensor() {
  {
    /* Sensor: listener */
    ParticleSensor.listener();
    if (ParticleSensor.isReady()) {
      uint16_t pm25, pm10;
      ParticleSensor.get(&pm25, &pm10);
      MQTTPublishParticleSensorState(pm25, HPMA115S0_TYPE_PM25);
      MQTTPublishParticleSensorState(pm10, HPMA115S0_TYPE_PM10);
      DomoticzPublishParticleSensorState(pm25, HPMA115S0_TYPE_PM25);
      delay(10);
      DomoticzPublishParticleSensorState(pm10, HPMA115S0_TYPE_PM10);
    }
  }
}
#endif
