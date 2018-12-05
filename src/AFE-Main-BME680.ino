#if defined(T6_CONFIG)

void mainBME680Sensor() {

  BME680Sensor.listener();
  if (BME680Sensor.isReady()) {
    BME680_DATA sensorDataBME680;
    sensorDataBME680 = BME680Sensor.getLatestData();

    MQTTPublishBME680SensorData(sensorDataBME680);
  }
}

#endif
