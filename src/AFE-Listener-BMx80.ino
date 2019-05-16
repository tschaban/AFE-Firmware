#ifdef CONFIG_HARDWARE_BMX80

void mainBMx80Sensor() {
  if (Device.configuration.isBMx80) {
    BMx80Sensor.listener();
    if (BMx80Sensor.isReady()) {
#ifdef CONFIG_HARDWARE_LED
      Led.on();
#endif
      BMx80_DATA sensorDataBMx80;
      sensorDataBMx80 = BMx80Sensor.get();
      MQTTPublishBMx80SensorData(sensorDataBMx80);
      DomoticzPublishBMx80SensorData(sensorDataBMx80);
#ifdef CONFIG_HARDWARE_LED
      Led.off();
#endif
    }
  }
}

#endif
