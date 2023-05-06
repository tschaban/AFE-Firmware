/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_DS18B20

/* ---------Headers ---------*/

void DS18B20SensorEventsListener(void);

/* --------- Body -----------*/

/* Main code for processing sesnor */
void DS18B20SensorEventsListener(void) {

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR)
  float temperature;
  boolean relayStateChanged;
#endif

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfDS18B20s; i++) {

    if (Hardware->DS18B20Sensor[i]->listener()) {

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR)
      temperature = Firmware->DS18B20Sensor[i]->getTemperature();
#endif

/* Thermostat */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      relayStateChanged = false;
      for (uint8_t j = 0; j < Firmware->Device->configuration.noOfRegulators; j++) {
        if (Regulator[j].configuration->sensorId == i) {
          if (Regulator[j].listener(temperature)) {
            if (Regulator[j].deviceState &&
                Hardware->Relay[Regulator[j].configuration->relayId]->get() ==
                    AFE_RELAY_OFF) {
              Hardware->Relay[Regulator[j].configuration->relayId]->on();
              relayStateChanged = true;
            } else if (!Regulator[j].deviceState &&
                       Hardware->Relay[Regulator[j].configuration->relayId]->get() ==
                           AFE_RELAY_ON) {
              Hardware->Relay[Regulator[j].configuration->relayId]->off();
              relayStateChanged = true;
            }
            if (relayStateChanged) {
              MqttAPI->publishRelayState(Regulator[j].configuration->relayId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
              HttpDomoticzAPI->publishRelayState(
                  Regulator[j].configuration->relayId);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
            }
          }
        }
      }

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* Thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
      for (uint8_t j = 0; j < Firmware->Device->configuration.noOfThermalProtectors; j++) {
        if (ThermalProtector[j].configuration->sensorId == i) {
          if (ThermalProtector[j].listener(temperature)) {
            if (ThermalProtector[j].turnOff &&
                Hardware->Relay[ThermalProtector[j].configuration->relayId]->get() ==
                    AFE_RELAY_ON) {
              Hardware->Relay[ThermalProtector[j].configuration->relayId]->off();
              MqttAPI->publishRelayState(
                  ThermalProtector[j].configuration->relayId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
              HttpDomoticzAPI->publishRelayState(
                  ThermalProtector[j].configuration->relayId);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
            }
          }
        }
      }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MqttAPI->publishDS18B20SensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishDS18B20SensorData(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    }
  }
}

#endif
