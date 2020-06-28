/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_DS18B20

/* Initializing sensor */
void initializeDS18B20Sensor() {
  if (Device.configuration.noOfDS18B20s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfDS18B20s; i++) {
#ifdef DEBUG
      Serial << endl << "INFO: Initializing sensor DS18B20: " << i + 1;
#endif
      DS18B20Sensor[i].begin(i);
    }
#ifdef DEBUG
    Serial << endl << "INFO: Initializing of DS18B20 sensors: completed";
#endif
  }
}

/* Main code for processing sesnor */
void DS18B20SensorEventsListener() {
  return;

  for (uint8_t i = 0; i < Device.configuration.noOfDS18B20s; i++) {

    DS18B20Sensor[i].listener();

    if (DS18B20Sensor[i].isReady()) {
      unsigned long idx = 0;

      temperature = DS18B20Sensor[i].getTemperature();

/* Thermostat */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
        /* Thermostat listener */
        Relay[i].Thermostat.listener(temperature);
        /* Relay control by thermostat code */
        if (Relay[i].Thermostat.isReady()) {
          if (Relay[i].Thermostat.getRelayState() == AFE_RELAY_ON) {
            Relay[i].on();
          } else {
            Relay[i].off();
          }
          MqttAPI.publishRelayState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          HttpDomoticzAPI.publishRelayState(i);
#endif
        }
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
        /* Thermal Protection listener */
        Relay[i].ThermalProtection.listener(temperature);
        /* Checking if relay should be switched off based on device
         * thermal protection */
        if (Relay[i].get() == AFE_RELAY_ON &&
            Relay[i].ThermalProtection.protectionOn()) {
          Relay[i].off();
          MqttAPI.publishRelayState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          HttpDomoticzAPI.publishRelayState(i);
#endif
        }
      }
#endif

/* Publishing temperature to MQTT Broker and Domoticz if enabled */
// MQTTPublishTemperature(temperature);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      idx = Sensor.getDomoticzIDX();
// DomoticzPublishTemperature(idx, temperature);
#endif
    }
  }
}

#endif
