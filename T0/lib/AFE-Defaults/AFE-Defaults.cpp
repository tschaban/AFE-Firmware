/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

  DEVICE deviceConfiguration;
  FIRMWARE firmwareConfiguration;
  NETWORK networkConfiguration;
  MQTT MQTTConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;

  sprintf(firmwareConfiguration.version, FIRMWARE_VERSION);
  firmwareConfiguration.type = FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  Data->saveConfiguration(firmwareConfiguration);

  sprintf(deviceConfiguration.name, "AFE-Device");
  deviceConfiguration.isLED[0] = true;
  deviceConfiguration.isLED[1] = false;
  deviceConfiguration.isRelay[0] = true;
  deviceConfiguration.isSwitch[0] = true;
  deviceConfiguration.isSwitch[1] = false;
  deviceConfiguration.mqttAPI = false;
  deviceConfiguration.domoticzAPI = false;
  deviceConfiguration.httpAPI = true;
  Data->saveConfiguration(deviceConfiguration);

  sprintf(networkConfiguration.ssid, "");
  sprintf(networkConfiguration.password, "");
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip = IPAddress(0, 0, 0, 0);
  networkConfiguration.gateway = IPAddress(0, 0, 0, 0);
  networkConfiguration.subnet = IPAddress(255, 255, 255, 0);
  networkConfiguration.noConnectionAttempts = 10;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 60;
  Data->saveConfiguration(networkConfiguration);

  sprintf(MQTTConfiguration.host, "");
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  sprintf(MQTTConfiguration.user, "");
  sprintf(MQTTConfiguration.password, "");
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");
  Data->saveConfiguration(MQTTConfiguration);

  RelayConfiguration.gpio = 12;
  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;
  sprintf(RelayConfiguration.name, "switch");
  RelayConfiguration.ledID = 0;
  RelayConfiguration.idx = 1;
  Data->saveConfiguration(0, RelayConfiguration);

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  SwitchConfiguration.relayID = 1;
  Data->saveConfiguration(0, SwitchConfiguration);

  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.type = 1;
  SwitchConfiguration.functionality = 1;
  Data->saveConfiguration(1, SwitchConfiguration);

  addDomoticzConfiguration();
  addLEDConfiguration(0, 13);
  addLEDConfiguration(1, 3);
  addDeviceID();

  Data->saveSystemLedID(1);
  Data->saveDeviceMode(2);
  Data->saveRelayState(0, false);
  Data->saveLanguage(1);
}

void AFEDefaults::addDomoticzConfiguration() {
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration.protocol = 0;
  sprintf(DomoticzConfiguration.host, "");
  sprintf(DomoticzConfiguration.user, "");
  sprintf(DomoticzConfiguration.password, "");
  DomoticzConfiguration.port = 8080;
  Data->saveConfiguration(DomoticzConfiguration);
}

void AFEDefaults::addLEDConfiguration(uint8_t id, uint8_t gpio) {
  LED LEDConfiguration;
  LEDConfiguration.gpio = gpio;
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(id, LEDConfiguration);
}

void AFEDefaults::addDeviceID() {
  char id[8];
  uint8_t range;
  for (uint8_t i = 0; i < sizeof(id); i++) {
    range = random(3);
    id[i] = char(range == 0 ? random(48, 57)
                            : range == 1 ? random(65, 90) : random(97, 122));
  }
  Data->saveDeviceID(String(id));
}
void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
