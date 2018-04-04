/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

/* Initializing Domoticz API */
void DomoticzInit() {
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
  }
}

void DomoticzPublishRelayState(uint8_t id) {
  if (Device.configuration.domoticzAPI) {
    Domoticz.sendSwitchCommand(Relay[id].getDomoticzIDX(),
                               Relay[id].get() == RELAY_ON ? "On" : "Off");
  }
}
