#if defined(T6_CONFIG)

void mainBMx80Sensor() {
  if (Device.configuration.isBMx80) {
    BMx80Sensor.listener();
    if (BMx80Sensor.isReady()) {
      BMx80_DATA sensorDataBMx80;
      sensorDataBMx80 = BMx80Sensor.get();
      MQTTPublishBMx80SensorData(sensorDataBMx80);
      DomoticzPublishBMx80SensorData(sensorDataBMx80);
    }
  }
}

#endif
