/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

  AFEDataAccess *Data;

  DEVICE deviceConfiguration;
  FIRMWARE firmwareConfiguration;
  NETWORK networkConfiguration;
  MQTT MQTTConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;
  LED LEDConfiguration;
  PIR PIRConfiguration;

  sprintf(firmwareConfiguration.version, FIRMWARE_VERSION);
  firmwareConfiguration.type = FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  Data->saveConfiguration(firmwareConfiguration);

  // Serial << endl << "firmwareConfiguration";
  sprintf(deviceConfiguration.name, "AFE-Device");

  deviceConfiguration.isLED[0] = true;
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isLED); i++) {
    deviceConfiguration.isLED[i] = false;
  }

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    deviceConfiguration.isSwitch[i] = false;
  }

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    deviceConfiguration.isRelay[i] = false;
  }

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isPIR); i++) {
    deviceConfiguration.isPIR[i] = false;
  }

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
  //  Serial << endl << "networkConfiguration";

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
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Data->saveConfiguration(i, RelayConfiguration);
    Data->saveRelayState(i, false);
  }

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    Data->saveConfiguration(i, SwitchConfiguration);
  }

  LEDConfiguration.gpio = 13;
  LEDConfiguration.changeToOppositeValue = false;
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isLED); i++) {
    Data->saveConfiguration(i, LEDConfiguration);
  }

  PIRConfiguration.gpio = 6;
  sprintf(PIRConfiguration.name, "pir");
  PIRConfiguration.ledId = 0;
  PIRConfiguration.relayId = 0;
  PIRConfiguration.howLongKeepRelayOn = 10;
  PIRConfiguration.invertRelayState = false;
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isPIR); i++) {
    Data->saveConfiguration(i, PIRConfiguration);
  }

  Data->saveDeviceMode(2);
  Data->saveLanguage(1);
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
