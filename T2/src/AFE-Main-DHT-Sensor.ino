/* Initializing sensor: DHT */
void initDHTSensor() {
  if (Device.configuration.isDHT) {
    SensorDHT.begin();
  }
}

/* Main code for processing DH sesnor */
void mainDHTSensor() {
  if (Device.configuration.isDHT) {

    /* Sensor: DHT listener */
    SensorDHT.listener();

    if (SensorDHT.temperatureSensorReady()) {
      Led.on();
      temperature = SensorDHT.getLatestTemperature();

      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {

          /* Thermostat listener */
          Relay[i].Thermostat.listener(temperature);

          /* Thermal Protection listener */
          Relay[i].ThermalProtection.listener(temperature);

          /* Relay control by thermostat code */
          if (Relay[i].Thermostat.isReady()) {
            if (Relay[i].Thermostat.getRelayState() == RELAY_ON &&
                !Relay[i].ThermalProtection.protectionOn()) {
              Relay[i].on();
            } else {
              Relay[i].off();
            }
            MQTTPublishRelayState(i);
            DomoticzPublishRelayState(i);
          }

          /* Checking if relay should be switched off based on device thermal
           * protection */
          if (Relay[i].get() == RELAY_ON &&
              Relay[i].ThermalProtection.protectionOn()) {
            Relay[i].off();
            MQTTPublishRelayState(i);
            DomoticzPublishRelayState(i);
          }
        }
      }

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MQTTPublishTemperature(temperature);
      DomoticzPublishTemperature(temperature);

      Led.off();
    }

    /* Humidity sensor related code */
    if (SensorDHT.humiditySensorReady()) {
      Led.on();
      humidity = SensorDHT.getLatestHumidity();

      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {

          /* Humiditstat listener */
          Relay[i].Humidistat.listener(humidity);

          /* Relay control by thermostat code */
          if (Relay[i].Humidistat.isReady()) {
            if (Relay[i].Humidistat.getRelayState() == RELAY_ON) {
              Relay[i].on();
            } else {
              Relay[i].off();
            }
            MQTTPublishRelayState(i);
            DomoticzPublishRelayState(i);
          }
        }
      }

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MQTTPublishHumidity(humidity);
      DomoticzPublishHumidity(humidity);

      Led.off();
    }
  }
}
