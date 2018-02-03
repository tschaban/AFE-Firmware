/* Initializing sensor: DHT */
void initDHTSensor() {
  if (Device.configuration.isDHT) {
    SensorDHT.begin();
  }
}

void mainDHTSensor() {
  /* Sensor: DS18B20 related code */
  if (Device.configuration.isDHT) {

    /* Sensor: DS18B20 listener */
    SensorDHT.listener();

    /* Temperature sensor related code */
    if (SensorDHT.temperatureSensorReady()) {
      Led.on();
      temperature = SensorDHT.getLatestTemperature();

      /* Publishing temperature to MQTT Broker if enabled */
      Mqtt.publish("temperature", temperature);
      Led.off();
    }

    /* Humidity sensor related code */
    if (SensorDHT.humiditySensorReady()) {
      Led.on();
      humidity = SensorDHT.getLatestHumidity();

      /* Publishing humidity to MQTT Broker if enabled */
      Mqtt.publish("humidity", humidity);
      Led.off();
    }
  }
}
