/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#ifdef AFE_CONFIG_HARDWARE_DHT

/* ---------Headers ---------*/

void DHTSensorEventsListener(void);

/* --------- Body -----------*/


/* Main code for processing sesnor */
void DHTSensorEventsListener(void) {

#if defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT) ||                            \
    defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR)
  boolean relayStateChanged;
#endif

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfDHTs; i++) {

    if (Hardware->DHTSensor[i]->listener()) {

/* Thermostat */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
      relayStateChanged = false;
      for (uint8_t j = 0; j < Firmware->Device->configuration.noOfRegulators; j++) {
        if (Regulator[j].configuration->sensorId == i) {

          float _value = 0;

          switch (Regulator[j].configuration->controllingParameter) {
          case AFE_FUNCTIONALITY_REGULATOR_CP_TEMPERATURE:
            _value = Firmware->DHTSensor[i]->currentTemperature;
            break;
          case AFE_FUNCTIONALITY_REGULATOR_CP_HEAT_INDEX:
            _value = Firmware->DHTSensor[i]->heatIndex(
                Hardware->DHTSensor[i]->currentTemperature, Hardware->DHTSensor[i]->currentHumidity,
                Hardware->DHTSensor[i]->configuration->temperature.unit ==
                    AFE_TEMPERATURE_UNIT_FAHRENHEIT);
            break;
          case AFE_FUNCTIONALITY_REGULATOR_CP_HUMIDITY:
            _value = Hardware->DHTSensor[i]->currentHumidity;
            break;
          case AFE_FUNCTIONALITY_REGULATOR_CP_ABSOLOUTE_HUMIDITY:
            _value = Hardware->DHTSensor[i]->absoluteHumidity(
                Hardware->DHTSensor[i]->currentTemperature, Hardware->DHTSensor[i]->currentHumidity,
                Hardware->DHTSensor[i]->configuration->temperature.unit ==
                    AFE_TEMPERATURE_UNIT_FAHRENHEIT);
            break;
          case AFE_FUNCTIONALITY_REGULATOR_CP_DEW_POINT:
            _value = Hardware->DHTSensor[i]->dewPoint(
                Hardware->DHTSensor[i]->currentTemperature, Hardware->DHTSensor[i]->currentHumidity,
                Hardware->DHTSensor[i]->configuration->temperature.unit ==
                    AFE_TEMPERATURE_UNIT_FAHRENHEIT);
            break;
          default:
            _value = Hardware->DHTSensor[i]->currentTemperature;
            break;
          }

          if (Regulator[j].listener(_value)) {
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
        if (ThermalProtector[j].configuration->enabled) {
          if (ThermalProtector[j].configuration->sensorId == i) {
            if (ThermalProtector[j].listener(Firmware->Hardware[i]->currentTemperature)) {
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
      }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

      /* Publishing temperature to MQTT Broker and Domoticz if enabled */
      MqttAPI->publishDHTSensorData(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      HttpDomoticzAPI->publishDHTSensorData(i);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
    }
  }
}

#endif
