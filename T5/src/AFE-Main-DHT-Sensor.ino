/* Initializing sensor: DHT */
void initDHTSensor() {
  if (Device.configuration.isDHT) {
    SensorDHT.begin();
  }
}

/* Main code for processing DH sesnor */
void mainDHTSensor() {
  /* Sensor: DTxx related code */
  if (Device.configuration.isDHT) {

    /* Sensor: DTxx listener */
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
