/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
  }
}

#if !defined(T5_CONFIG) // Not required for T5
/* It publishes relay state to Domotucz */
void DomoticzPublishRelayState(uint8_t id) {
  unsigned long idx = Relay[id].getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendSwitchCommand(idx, Relay[id].get() == RELAY_ON ? "On" : "Off");
  }
}
#endif

/* Temperature and Humiditity sensors */
#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)

/* It publishes temperature to Domotucz */
void DomoticzPublishTemperature(float temperature) {
#if defined(T1_CONFIG)
  unsigned long idx = Sensor.getDomoticzIDX();
#elif defined(T2_CONFIG) || defined(T5_CONFIG)
  unsigned long idx = Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE);
#endif

  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendTemperatureCommand(idx, temperature);
  }
}

#if defined(T2_CONFIG) || defined(T5_CONFIG)
/* It publishes humidity to Domoticz */
void DomoticzPublishHumidity(float humidity) {
  unsigned long idx = Sensor.getDomoticzIDX(IDX_TYPE_HUMIDITY);
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendHumidityCommand(idx, humidity);
  }
}

/* It publishes temperature and humidity to Domoticz */
void DomoticzPublishTemperatureAndHumidity(float temperature, float humidity) {
  unsigned long idx = Sensor.getDomoticzIDX(IDX_TYPE_TEMPERATURE_AND_HUMIDITY);
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendTemperatureAndHumidityCommand(idx, temperature, humidity);
  }
}
#endif

#endif

/* Pir */
#if defined(T3_CONFIG)
/* It publishes gate state to Domotucz */
void DomoticzPublishPirState(uint8_t id) {
  unsigned long idx = Pir[id].getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendPirCommand(idx, Pir[id].get() == PIR_OPEN ? "On" : "Off");
  }
}
#endif

/* Gate and Contactron */
#if defined(T5_CONFIG)
/* It publishes gate state to Domotucz */
void DomoticzPublishGateState() {
  unsigned long idx = Gate.getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendGateCommand(idx, Gate.get() == GATE_OPEN ? "On" : "Off");
  }
}

/* It publishes contactron state to Domotucz */
void DomoticzPublishContactronState(uint8_t id) {
  unsigned long idx = Gate.Contactron[id].getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendContactronCommand(
        idx, Gate.Contactron[id].get() == CONTACTRON_OPEN ? "On" : "Off");
  }
}
#endif
