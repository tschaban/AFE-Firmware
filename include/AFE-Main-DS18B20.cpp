/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_DS18B20

/* ---------Headers ---------*/

void DS18B20SensorEventsListener(void);

/* --------- Body -----------*/

/* Main code for processing sesnor */
void DS18B20SensorEventsListener(void) {

//#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||    it was before 3.8.0 the why?
 #if defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) || defined(AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR)
  float temperature;
  boolean relayStateChanged;
#endif

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfDS18B20s; i++) {

    if (Hardware->DS18B20Sensor[i]->listener()) {

// #if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||   it was before 3.8.0 the why?
#if defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) || defined(AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR)
      temperature = Hardware->DS18B20Sensor[i]->getTemperature();
#endif

/* Thermostat */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      relayStateChanged = false;
      for (uint8_t j = 0; j < Firmware->Device->configuration.noOfRegulators; j++) {
        if (Hardware->Regulator[j]->configuration->sensorId == i) {
          if (Hardware->Regulator[j]->listener(temperature)) {
            if (Hardware->Regulator[j]->deviceState &&
                Hardware->Relay[Hardware->Regulator[j]->configuration->relayId]->get() ==
                    AFE_RELAY_OFF) {
              Hardware->Relay[Hardware->Regulator[j]->configuration->relayId]->on();
              relayStateChanged = true;
            } else if (!Hardware->Regulator[j]->deviceState &&
                       Hardware->Relay[Hardware->Regulator[j]->configuration->relayId]->get() ==
                           AFE_RELAY_ON) {
              Hardware->Relay[Hardware->Regulator[j]->configuration->relayId]->off();
              relayStateChanged = true;
            }
            if (relayStateChanged) {
              MqttAPI->publishRelayState(Hardware->Regulator[j]->configuration->relayId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
              HttpDomoticzAPI->publishRelayState(
                  Hardware->Regulator[j]->configuration->relayId);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
            }
          }
        }
      }

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* Thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
      for (uint8_t j = 0; j < Firmware->Device->configuration.noOfThermalProtectors; j++) {
        if (Hardware->ThermalProtector[j]->configuration->sensorId == i) {
          if (Hardware->ThermalProtector[j]->listener(temperature)) {
            if (Hardware->ThermalProtector[j]->turnOff &&
                Hardware->Relay[Hardware->ThermalProtector[j]->configuration->relayId]->get() ==
                    AFE_RELAY_ON) {
              Hardware->Relay[Hardware->ThermalProtector[j]->configuration->relayId]->off();
              MqttAPI->publishRelayState(
                  Hardware->ThermalProtector[j]->configuration->relayId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
              HttpDomoticzAPI->publishRelayState(
                  Hardware->ThermalProtector[j]->configuration->relayId);
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
