#if defined(T6_CONFIG)

void mainBME680Sensor() {
  if (Device.configuration.isBME680) {
    BME680Sensor.listener();
    if (BME680Sensor.isReady()) {
      BME680_DATA sensorDataBME680;
      sensorDataBME680 = BME680Sensor.get();
      MQTTPublishBME680SensorData(sensorDataBME680);
      DomoticzPublishBME680SensorData(sensorDataBME680);
    }
  }
}

#endif
