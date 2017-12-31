/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

const char *AFEDefaults::getFirmwareVersion() { return "1.0.1"; }
uint8_t AFEDefaults::getFirmwareType() { return 2; }
void AFEDefaults::set() {

  AFEDataAccess *Data;

  DEVICE deviceConfiguration;
  FIRMWARE firmwareConfiguration;
  NETWORK networkConfiguration;
  MQTT MQTTConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;
  LED LEDConfiguration;
  DH DHTConfiguration;

  sprintf(firmwareConfiguration.version, getFirmwareVersion());
  firmwareConfiguration.type = getFirmwareType();
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  Data->saveConfiguration(firmwareConfiguration);

  sprintf(deviceConfiguration.name, "AFE-Device");
  deviceConfiguration.isLED[0] = true;
  deviceConfiguration.isRelay[0] = true;
  deviceConfiguration.isSwitch[0] = true;
  deviceConfiguration.isSwitch[1] = false;
  deviceConfiguration.isDHT = false;
  deviceConfiguration.mqttAPI = false;
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

  RelayConfiguration.thermostat.enabled = false;
  RelayConfiguration.thermostat.turnOn = 0;
  RelayConfiguration.thermostat.turnOnAbove = false;
  RelayConfiguration.thermostat.turnOff = 0;
  RelayConfiguration.thermostat.turnOffAbove = true;

  RelayConfiguration.humidistat.enabled = false;
  RelayConfiguration.humidistat.turnOn = 0;
  RelayConfiguration.humidistat.turnOnAbove = false;
  RelayConfiguration.humidistat.turnOff = 0;
  RelayConfiguration.humidistat.turnOffAbove = true;

  RelayConfiguration.thermalProtection = 0;

  Data->saveConfiguration(RelayConfiguration);

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  Data->saveConfiguration(0, SwitchConfiguration);

  SwitchConfiguration.gpio = 5;
  SwitchConfiguration.type = 1;
  SwitchConfiguration.functionality = 11;
  Data->saveConfiguration(1, SwitchConfiguration);

  LEDConfiguration.gpio = 13;
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(LEDConfiguration);

  DHTConfiguration.gpio = 14;
  DHTConfiguration.type = 1;
  DHTConfiguration.temperature.correction = 0;
  DHTConfiguration.temperature.interval = 60;
  DHTConfiguration.temperature.unit = 0;
  DHTConfiguration.humidity.correction = 0;
  DHTConfiguration.humidity.interval = 60;

  Data->saveConfiguration(DHTConfiguration);

  Data->saveDeviceMode(2);
  Data->saveRelayState(false);
  Data->saveLanguage(1);
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
