#if defined(CONFIG_HARDWARE_DS18B20) || defined(CONFIG_HARDWARE_DHXX)

/* Initializing sensor */
void initSensor() {
#ifdef CONFIG_HARDWARE_DS18B20
  if (Device.configuration.isDS18B20) {
    Sensor.begin();
  }
#endif
#ifdef CONFIG_HARDWARE_DHXX
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
#if defined(CONFIG_HARDWARE_DS18B20)
  if (Device.configuration.isDS18B20)
#elif defined(CONFIG_HARDWARE_DHXX)
  if (Device.configuration.isDHT)
#endif
  {
    /* Sensor: listener */
    Sensor.listener();
    if (Sensor.isReady()) {
      unsigned long idx = 0;

#ifdef CONFIG_HARDWARE_LED
      Led.on();
#endif
      temperature = Sensor.getTemperature();

/* Thermostat */
#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {

          /* Thermostat listener */
          Relay[i].Thermostat.listener(temperature);

          /* Relay control by thermostat code */
          if (Relay[i].Thermostat.isReady()) {
            if (Relay[i].Thermostat.getRelayState() == RELAY_ON) {
              Relay[i].on();
            } else {
              Relay[i].off();
            }
            MQTTPublishRelayState(i);
            DomoticzPublishRelayState(i);
          }
        }
      }
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
      for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
        if (Device.configuration.isRelay[i]) {

          /* Thermal Protection listener */
          Relay[i].ThermalProtection.listener(temperature);

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
#endif

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MQTTPublishTemperature(temperature);

#if defined(T1_CONFIG)
      idx = Sensor.getDomoticzIDX();
#elif (defined(T2_CONFIG) || defined(T5_CONFIG))
      idx = Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE);
#endif

      DomoticzPublishTemperature(idx, temperature);

#ifdef CONFIG_HARDWARE_DHXX
      humidity = Sensor.getHumidity();

/* Humidistat */
#if !defined(T5_CONFIG)
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
#endif

#ifdef CONFIG_HUMIDITY
      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MQTTPublishHumidity(humidity);
      idx = Sensor.getDomoticzIDX(IDX_TYPE_HUMIDITY);
      DomoticzPublishHumidity(idx, humidity);
#endif

#if (defined(CONFIG_TEMPERATURE) && defined(CONFIG_HUMIDITY))
      idx = Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE_AND_HUMIDITY);
      DomoticzPublishTemperatureAndHumidity(idx, temperature, humidity);
      if (Sensor.publishHeatIndex()) {
        MQTTPublishHeatIndex(Sensor.getHeatIndex());
      }
      if (Sensor.publishDewPoint()) {
        MQTTPublishDewPoint(Sensor.getDewPoint());
      }
#endif

#endif

#ifdef CONFIG_HARDWARE_LED
      Led.off();
#endif
    }
  }
}

#ifdef CONFIG_HARDWARE_DHXX
void dht_wrapper() { dht.isrCallback(); }
#endif

#endif
