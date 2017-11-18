#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

  AFEDataAccess *Data;

  NETWORK networkConfiguration;
  FIRMWARE firmwareConfiguration;
  MQTT MQTTConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;
  LED LEDConfiguration;
  // @TODO DOMOTICZ DomoticzConfiguration;
  // @TODO DS18B20 DS18B20Configuration;

  sprintf(firmwareConfiguration.version, "1.0.2");
  firmwareConfiguration.type = 0;
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  sprintf(networkConfiguration.ssid, "");
  sprintf(networkConfiguration.password, "");
  sprintf(networkConfiguration.host, "AFE-Device");
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip = IPAddress(0, 0, 0, 0);
  networkConfiguration.gateway = IPAddress(0, 0, 0, 0);
  networkConfiguration.subnet = IPAddress(255, 255, 255, 0);
  networkConfiguration.noConnectionAttempts = 10;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 60;

  sprintf(MQTTConfiguration.host, "");
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  sprintf(MQTTConfiguration.user, "");
  sprintf(MQTTConfiguration.password, "");
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");

  RelayConfiguration.present = true;
  RelayConfiguration.gpio = 12;
  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;
  sprintf(RelayConfiguration.name, "switch");
  /* @TODO DOMOTICZ
  RelayConfiguration.idx = 0;
  RelayConfiguration.publishToDomoticz = false;
  */

  Serial << endl << "INFO: Setting defaults: switch";

  SwitchConfiguration.present = true;
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;

  LEDConfiguration.present = true;
  LEDConfiguration.gpio = 13;

  /* @TODO DS18B20
    Serial << endl << "INFO: Setting defaults: ds18b20";
    DS18B20Configuration.present = false;
    DS18B20Configuration.gpio = 0;
    DS18B20Configuration.correction = 0;
    DS18B20Configuration.interval = 60;
    DS18B20Confi/guration.unit = 0;
  */

  /* @TODO DOMOTICZ
  Serial << endl << "INFO: Setting defaults: domoticz";
  sprintf(DomoticzConfiguration.host, "");
  DomoticzConfiguration.ip = IPAddress(0, 0, 0, 0);
  sprintf(DomoticzConfiguration.user, "");
  sprintf(DomoticzConfiguration.password, "");
  DomoticzConfiguration.port = 8080;
  */

  /* Saving data */
  Data->saveConfiguration(networkConfiguration);
  Data->saveConfiguration(firmwareConfiguration);
  Data->saveConfiguration(MQTTConfiguration);

  Data->saveConfiguration(LEDConfiguration);

  Data->saveConfiguration(0, RelayConfiguration);
  Data->saveConfiguration(0, SwitchConfiguration);
  // @TODO DS18B20 Data->saveConfiguration(DS18B20Configuration);
  // @TODO DOMOTICZ Data->saveConfiguration(DomoticzConfiguration);

  Data->saveDeviceMode(2);
  Data->saveRelayState(0, false);
  Data->saveLanguage(1);
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
