/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

const char *AFEDefaults::getFirmwareVersion() { return "1.0rc1"; }
uint8_t AFEDefaults::getFirmwareType() { return 3; }
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

  sprintf(firmwareConfiguration.version, getFirmwareVersion());
  firmwareConfiguration.type = getFirmwareType();
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  Data->saveConfiguration(firmwareConfiguration);

  // Serial << endl << "firmwareConfiguration";
  sprintf(deviceConfiguration.name, "AFE-Device");

  deviceConfiguration.isLED[0] = true;
  for (uint8_t i = 1; i < 5; i++) {
    deviceConfiguration.isLED[i] = false;
  }

  for (uint8_t i = 0; i < 5; i++) {
    deviceConfiguration.isSwitch[i] = false;
  }

  for (uint8_t i = 0; i < 4; i++) {
    deviceConfiguration.isRelay[i] = false;
  }

  for (uint8_t i = 0; i < 4; i++) {
    deviceConfiguration.isPIR[0] = false;
  }

  deviceConfiguration.mqttAPI = false;
  deviceConfiguration.httpAPI = true;

  Data->saveConfiguration(deviceConfiguration);
  //  Serial << endl << "deviceConfiguration";

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
  //  Serial << endl << "MQTTConfiguration";

  RelayConfiguration.gpio = 12;
  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;
  sprintf(RelayConfiguration.name, "switch");
  for (uint8_t i = 0; i < 4; i++) {
    //  Serial << endl << "RelayConfiguration #";
    Data->saveConfiguration(i, RelayConfiguration);
    Data->saveRelayState(i, false);
    //  Serial << i;
  }

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  for (uint8_t i = 0; i < 5; i++) {
    //  Serial << endl << "SwitchConfiguration #";
    Data->saveConfiguration(i, SwitchConfiguration);
    //  Serial << i;
  }

  LEDConfiguration.gpio = 13;
  LEDConfiguration.changeToOppositeValue = false;
  for (uint8_t i = 0; i < 5; i++) {
    //  Serial << endl << "LEDConfiguration #";
    Data->saveConfiguration(i, LEDConfiguration);
    //  Serial << i;
  }

  PIRConfiguration.gpio = 6;
  sprintf(PIRConfiguration.name, "pir");
  PIRConfiguration.ledId = 0;
  PIRConfiguration.relayId = 0;
  PIRConfiguration.howLongKeepRelayOn = 10;
  PIRConfiguration.invertRelayState = false;
  for (uint8_t i = 0; i < 4; i++) {
    //  Serial << endl << "PIRConfiguration #";
    Data->saveConfiguration(i, PIRConfiguration);
    //  Serial << i;
  }

  Data->saveDeviceMode(2);
  //  Serial << endl << "saveDeviceMode";
  Data->saveLanguage(1);
  //  Serial << endl << "saveLanguage";
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
