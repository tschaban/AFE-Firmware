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

  sprintf(deviceConfiguration.name, "AFE-Device");

  deviceConfiguration.isLED[0] = true;
  deviceConfiguration.isLED[1] = false;
  deviceConfiguration.isLED[2] = false;
  deviceConfiguration.isLED[3] = false;
  deviceConfiguration.isLED[4] = false;

  deviceConfiguration.isSwitch[0] = true;
  deviceConfiguration.isSwitch[1] = false;
  deviceConfiguration.isSwitch[2] = false;
  deviceConfiguration.isSwitch[3] = false;
  deviceConfiguration.isSwitch[4] = false;

  deviceConfiguration.isRelay[0] = true;
  deviceConfiguration.isRelay[1] = false;
  deviceConfiguration.isRelay[2] = false;
  deviceConfiguration.isRelay[3] = false;

  deviceConfiguration.isPIR[0] = true;
  deviceConfiguration.isPIR[1] = false;
  deviceConfiguration.isPIR[2] = false;
  deviceConfiguration.isPIR[3] = false;

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
  Data->saveConfiguration(0, RelayConfiguration);

  sprintf(RelayConfiguration.name, "");
  Data->saveConfiguration(1, RelayConfiguration);
  Data->saveConfiguration(2, RelayConfiguration);
  Data->saveConfiguration(3, RelayConfiguration);

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  Data->saveConfiguration(0, SwitchConfiguration);
  Data->saveConfiguration(1, SwitchConfiguration);
  Data->saveConfiguration(2, SwitchConfiguration);
  Data->saveConfiguration(3, SwitchConfiguration);
  Data->saveConfiguration(4, SwitchConfiguration);

  LEDConfiguration.gpio = 13;
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(0, LEDConfiguration);

  PIRConfiguration.gpio = 6;
  sprintf(PIRConfiguration.name, "pir");
  PIRConfiguration.Led.gpio = 14;
  PIRConfiguration.relay.id = 0;
  PIRConfiguration.relay.howLongKeepItOpen = 10;
  PIRConfiguration.relay.invertedState = false;
  Data->saveConfiguration(0, PIRConfiguration);
  Data->saveConfiguration(1, PIRConfiguration);
  Data->saveConfiguration(2, PIRConfiguration);
  Data->saveConfiguration(3, PIRConfiguration);

  Data->saveDeviceMode(2);
  Data->saveRelayState(0, false);
  Data->saveLanguage(1);
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
