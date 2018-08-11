/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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
  firmwareConfiguration.upgradeURL[0] = '\0';

  Data->saveConfiguration(firmwareConfiguration);

  sprintf(deviceConfiguration.name, "AFE-Device");
  deviceConfiguration.isLED[0] = true;

  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isLED); i++) {
    deviceConfiguration.isLED[i] = false;
  }
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    deviceConfiguration.isRelay[i] = true;
  }

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch) - 1; i++) {
    deviceConfiguration.isSwitch[i] = true;
  }

  deviceConfiguration.isSwitch[sizeof(deviceConfiguration.isSwitch) - 1] =
      false;

  deviceConfiguration.mqttAPI = false;
  deviceConfiguration.domoticzAPI = false;
  deviceConfiguration.httpAPI = true;

  Data->saveConfiguration(deviceConfiguration);

  networkConfiguration.ssid[0] = '\0';
  networkConfiguration.password[0] = '\0';
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip = IPAddress(0, 0, 0, 0);
  networkConfiguration.gateway = IPAddress(0, 0, 0, 0);
  networkConfiguration.subnet = IPAddress(255, 255, 255, 0);
  networkConfiguration.noConnectionAttempts = 10;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 60;

  Data->saveConfiguration(networkConfiguration);

  MQTTConfiguration.host[0] = '\0';
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  MQTTConfiguration.user[0] = '\0';
  MQTTConfiguration.password[0] = '\0';
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");
  Data->saveConfiguration(MQTTConfiguration);

  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;
  RelayConfiguration.ledID = 0;
  RelayConfiguration.idx = 0;
  RelayConfiguration.gpio = 12;
  sprintf(RelayConfiguration.name, "switch1");

  Data->saveConfiguration(0, RelayConfiguration);

  RelayConfiguration.gpio = 5;
  sprintf(RelayConfiguration.name, "switch2");
  Data->saveConfiguration(1, RelayConfiguration);

  RelayConfiguration.gpio = 4;
  sprintf(RelayConfiguration.name, "switch3");
  Data->saveConfiguration(2, RelayConfiguration);

  RelayConfiguration.gpio = 15;
  sprintf(RelayConfiguration.name, "switch4");
  Data->saveConfiguration(3, RelayConfiguration);

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Data->saveRelayState(i, false);
  }

  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.functionality = 0;
  SwitchConfiguration.relayID = 1;
  Data->saveConfiguration(0, SwitchConfiguration);

  SwitchConfiguration.gpio = 9;
  SwitchConfiguration.functionality = 1;
  SwitchConfiguration.relayID = 2;
  Data->saveConfiguration(1, SwitchConfiguration);

  SwitchConfiguration.gpio = 10;
  SwitchConfiguration.relayID = 3;
  Data->saveConfiguration(2, SwitchConfiguration);

  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.relayID = 4;
  Data->saveConfiguration(3, SwitchConfiguration);

  addDomoticzConfiguration();
  addLEDConfiguration(0, 13);
  addLEDConfiguration(1, 3);
  addDeviceID();

  Data->saveSystemLedID(1);

  Data->saveDeviceMode(2);
  Data->saveLanguage(1);
}
void AFEDefaults::addDomoticzConfiguration() {
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration.protocol = 0;
  DomoticzConfiguration.host[0] = '\0';
  DomoticzConfiguration.user[0] = '\0';
  DomoticzConfiguration.password[0] = '\0';
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
