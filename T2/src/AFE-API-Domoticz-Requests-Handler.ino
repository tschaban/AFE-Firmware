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
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendSwitchCommand(Relay[id].getDomoticzIDX(),
                               Relay[id].get() == RELAY_ON ? "On" : "Off");
  }
}

/* It publishes temperature to Domoticz */
void DomoticzPublishTemperature(float temperature) {
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendTemperatureCommand(
        SensorDHT.getDomoticzIDX(IDX_TYPE_TEMPERATURE), temperature);
  }
}

/* It publishes humidity to Domoticz */
void DomoticzPublishHumidity(float humidity) {
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendHumidityCommand(SensorDHT.getDomoticzIDX(IDX_TYPE_HUMIDITY),
                                 humidity);
  }
}

/* It publishes temperature and humidity to Domoticz */
void DomoticzPublishHumidity(float temperature, float humidity) {
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendTemperatureAndHumidityCommand(
        SensorDHT.getDomoticzIDX(IDX_TYPE_TEMPERATURE_AND_HUMIDITY),
        temperature, humidity);
  }
}
