/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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

/* It publishes temperature to Domotucz */
void DomoticzPublishTemperature(float temperature) {
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendTemperatureCommand(SensorDS18B20.getDomoticzIDX(),
                                    temperature);
  }
}
