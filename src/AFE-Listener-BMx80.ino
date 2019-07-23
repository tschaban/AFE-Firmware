/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef CONFIG_HARDWARE_BMX80

void mainBMx80Sensor() {
  if (Device.configuration.isBMx80) {
    BMx80Sensor.listener();
    if (BMx80Sensor.isReady()) {
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.on();
#endif
      BMx80_DATA sensorDataBMx80;
      sensorDataBMx80 = BMx80Sensor.get();
      MQTTPublishBMx80SensorData(sensorDataBMx80);
      DomoticzPublishBMx80SensorData(sensorDataBMx80);
#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
      Led.off();
#endif
    }
  }
}

#endif
