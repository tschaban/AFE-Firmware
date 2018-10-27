#if defined(T1_CONFIG) || defined(T2_CONFIG)
/* Initializing sensor */
void initSensor() {
#if defined(T1_CONFIG)
  if (Device.configuration.isDS18B20)
#elif defined(T2_CONFIG)
  if (Device.configuration.isDHT)
#endif
  {
    Sensor.begin();
  }
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
#if defined(T1_CONFIG)
    if (Sensor.isReady())
#elif defined(T2_CONFIG)
    if (Sensor.temperatureSensorReady())
#endif
    {
      Led.on();
      temperature = Sensor.getLatest();

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

#ifdef T2_CONFIG
      if (Sensor.publishHeatIndex()) {
        MQTTPublishHeatIndex(Sensor.getHeatIndex());
      }
#endif

      DomoticzPublishTemperature(temperature);

#ifdef T2_CONFIG
      delay(10);
      DomoticzPublishTemperatureAndHumidity(temperature, humidity);
#endif

      Led.off();
    }

#ifdef T2_CONFIG
    /* Humidity sensor related code */
    if (Sensor.humiditySensorReady()) {
      Led.on();
      humidity = Sensor.getLatestHumidity();

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
      if (Sensor.publishHeatIndex()) {
        MQTTPublishHeatIndex(Sensor.getHeatIndex());
      }
      DomoticzPublishHumidity(humidity);
      delay(10);
      DomoticzPublishTemperatureAndHumidity(temperature, humidity);
      Led.off();
    }
#endif
  }
}
#endif
