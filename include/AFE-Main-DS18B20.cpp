/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_DS18B20

/* ---------Headers ---------*/

void initializeDS18B20Sensor(void);
void DS18B20SensorEventsListener(void);

/* --------- Body -----------*/

/* Initializing sensor */
void initializeDS18B20Sensor(void) {
  if (Device.configuration.noOfDS18B20s > 0) {
    for (uint8_t i = 0; i < Device.configuration.noOfDS18B20s; i++) {
      DS18B20Sensor[i].begin(&Data, i);
    }
  }
}

/* Main code for processing sesnor */
void DS18B20SensorEventsListener(void) {

  for (uint8_t i = 0; i < Device.configuration.noOfDS18B20s; i++) {

    if (DS18B20Sensor[i].listener()) {

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION)
      float temperature = DS18B20Sensor[i].getTemperature();
#endif

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
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
        }
      }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMOSTAT

/* Thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
      for (uint8_t i = 0; i < Device.configuration.noOfRelays; i++) {
        /* Thermal Protection listener */
        if (Relay[i].get() == AFE_RELAY_ON &&
            Relay[i].ThermalProtection.listener(temperature)) {
          Relay[i].off();
          MqttAPI.publishRelayState(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
          HttpDomoticzAPI.publishRelayState(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
        }
      }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MqttAPI.publishDS18B20SensorData(i);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
      HttpDomoticzAPI.publishDS18B20SensorData(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    }
  }
}

#endif
