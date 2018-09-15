/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
  }
}

/* It publishes relay state to Domotucz */
void DomoticzPublishRelayState(uint8_t id) {
  unsigned long idx = Relay[id].getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendSwitchCommand(idx, Relay[id].get() == RELAY_ON ? "On" : "Off");
  }
}

/* It publishes gate state to Domotucz */
void DomoticzPublishGateState() {
  unsigned long idx = Gate.getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendGateCommand(idx, Gate.get() == RELAY_ON ? "On" : "Off");
  }
}

/* It publishes contactron state to Domotucz */
void DomoticzPublishContactronState(uint8_t id) {
  unsigned long idx = Gate.Contactron[id].getDomoticzIDX();
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendContactronCommand(idx, Gate.Contactron[id].get() == RELAY_ON ? "On" : "Off");
  }
}

/* It publishes temperature to Domoticz */
void DomoticzPublishTemperature(float temperature) {
  unsigned long idx = SensorDHT.getDomoticzIDX(IDX_TYPE_TEMPERATURE);
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendTemperatureCommand(idx, temperature);
  }
}

/* It publishes humidity to Domoticz */
void DomoticzPublishHumidity(float humidity) {
  unsigned long idx = SensorDHT.getDomoticzIDX(IDX_TYPE_HUMIDITY);
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendHumidityCommand(idx, humidity);
  }
}

/* It publishes temperature and humidity to Domoticz */
void DomoticzPublishTemperatureAndHumidity(float temperature, float humidity) {
  unsigned long idx =
      SensorDHT.getDomoticzIDX(IDX_TYPE_TEMPERATURE_AND_HUMIDITY);
  if (Device.configuration.domoticzAPI && idx > 0) {
    Domoticz.sendTemperatureAndHumidityCommand(idx, temperature, humidity);
  }
}
