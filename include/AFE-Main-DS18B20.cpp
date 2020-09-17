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

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR)
  float temperature;
  boolean relayStateChanged;
#endif

  for (uint8_t i = 0; i < Device.configuration.noOfDS18B20s; i++) {

    if (DS18B20Sensor[i].listener()) {

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR)
      temperature = DS18B20Sensor[i].getTemperature();
#endif

/* Thermostat */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      relayStateChanged = false;
      for (uint8_t j = 0; j < Device.configuration.noOfRegulators; j++) {
        if (Regulator[j].configuration.sensorId == i) {
          if (Regulator[j].listener(temperature)) {
            if (Regulator[j].deviceState &&
                Relay[Regulator[j].configuration.relayId].get() ==
                    AFE_RELAY_OFF) {
              Relay[Regulator[j].configuration.relayId].on();
              relayStateChanged = true;
            } else if (!Regulator[j].deviceState &&
                       Relay[Regulator[j].configuration.relayId].get() ==
                           AFE_RELAY_ON) {
              Relay[Regulator[j].configuration.relayId].off();
              relayStateChanged = true;
            }
            if (relayStateChanged) {
              MqttAPI.publishRelayState(Regulator[j].configuration.relayId);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
              HttpDomoticzAPI.publishRelayState(
                  Regulator[j].configuration.relayId);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
            }
          }
        }
      }

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* Thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
      for (uint8_t j = 0; j < Device.configuration.noOfThermalProtectors; j++) {
        if (ThermalProtector[j].configuration.sensorId == i) {
          if (ThermalProtector[j].listener(temperature)) {
            if (ThermalProtector[j].turnOff &&
                Relay[ThermalProtector[j].configuration.relayId].get() ==
                    AFE_RELAY_ON) {
              Relay[ThermalProtector[j].configuration.relayId].off();
              MqttAPI.publishRelayState(
                  ThermalProtector[j].configuration.relayId);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
              HttpDomoticzAPI.publishRelayState(
                  ThermalProtector[j].configuration.relayId);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
            }
          }
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
