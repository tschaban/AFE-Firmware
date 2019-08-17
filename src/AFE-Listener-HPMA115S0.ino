/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0

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
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.on();
#endif
        HPMA115S0_DATA sensorData;
        sensorData = ParticleSensor.get();
        MQTTPublishParticleSensorData(sensorData);
        DomoticzPublishParticleSensorData(sensorData);
#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        Led.off();
#endif
      }
    }
  }
}
#endif
