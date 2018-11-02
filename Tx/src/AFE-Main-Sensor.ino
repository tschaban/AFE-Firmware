#if defined(T1_CONFIG) || defined(T2_CONFIG)

/* Initializing sensor */
void initSensor() {
#if defined(T1_CONFIG)
  if (Device.configuration.isDS18B20) {
    Sensor.begin();
  }
#elif defined(T2_CONFIG)
  if (Device.configuration.isDHT) {
    AFEDataAccess Data;
    DH configuration = Data.getSensorConfiguration();

    dht.begin(configuration.gpio,
              (uint8_t)configuration.type == 1
                  ? DHT11
                  : configuration.type == 2 ? DHT21 : DHT22,
              dht_wrapper);
  }
#endif
}

/* Main code for processing sesnor */
void mainSensor() {
#if defined(T1_CONFIG)
  if (Device.configuration.isDS18B20)
#elif defined(T2_CONFIG)
  if (Device.configuration.isDHT)
#endif
  {
    /* Sensor: listener */
    Sensor.listener();
    if (Sensor.isReady()) {
      Led.on();
      temperature = Sensor.getTemperature();
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

          /* Checking if relay should be switched off based on device
           * thermal protection */
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

#ifdef T2_CONFIG
      humidity = Sensor.getHumidity();

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
      delay(10);
      DomoticzPublishTemperatureAndHumidity(temperature, humidity);
      if (Sensor.publishHeatIndex()) {
        MQTTPublishHeatIndex(Sensor.getHeatIndex());
      }
      if (Sensor.publishDewPoint()) {
        MQTTPublishDewPoint(Sensor.getDewPoint());
      }
#endif
      Led.off();
    }
  }
}

#if defined(T2_CONFIG)
void dht_wrapper() { dht.isrCallback(); }
#endif

#endif
