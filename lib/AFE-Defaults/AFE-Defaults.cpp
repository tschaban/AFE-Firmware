#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

const char *AFEDefaults::getFirmwareVersion() { return "1.0rc3"; }
uint8_t AFEDefaults::getFirmwareType() { return 0; }
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

  sprintf(firmwareConfiguration.version, getFirmwareVersion());
  firmwareConfiguration.type = getFirmwareType();
  firmwareConfiguration.autoUpgrade = 0;
  sprintf(firmwareConfiguration.upgradeURL, "");

  Data->saveConfiguration(firmwareConfiguration);

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

  Data->saveConfiguration(networkConfiguration);

  sprintf(MQTTConfiguration.host, "");
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  sprintf(MQTTConfiguration.user, "");
  sprintf(MQTTConfiguration.password, "");
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");

  Data->saveConfiguration(MQTTConfiguration);

  RelayConfiguration.present = true;
  RelayConfiguration.gpio = 12;
  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;
  sprintf(RelayConfiguration.name, "switch");

  Data->saveConfiguration(0, RelayConfiguration);

  /* @TODO DOMOTICZ
  RelayConfiguration.idx = 0;
  RelayConfiguration.publishToDomoticz = false;
  */

  SwitchConfiguration.present = true;
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  Data->saveConfiguration(0, SwitchConfiguration);

  SwitchConfiguration.present = false;
  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.type = 1;
  SwitchConfiguration.functionality = 11;
  Data->saveConfiguration(1, SwitchConfiguration);

  LEDConfiguration.present = true;
  LEDConfiguration.gpio = 13;
  Data->saveConfiguration(LEDConfiguration);

  /* @TODO DS18B20
    Serial << endl << "INFO: Setting defaults: ds18b20";
    DS18B20Configuration.present = false;
    DS18B20Configuration.gpio = 0;
    DS18B20Configuration.correction = 0;
    DS18B20Configuration.interval = 60;
    DS18B20Confi/guration.unit = 0;

    DS18B20 Data->saveConfiguration(DS18B20Configuration);
  */

  /* @TODO DOMOTICZ
  Serial << endl << "INFO: Setting defaults: domoticz";
  sprintf(DomoticzConfiguration.host, "");
  DomoticzConfiguration.ip = IPAddress(0, 0, 0, 0);
  sprintf(DomoticzConfiguration.user, "");
  sprintf(DomoticzConfiguration.password, "");
  DomoticzConfiguration.port = 8080;

Data->saveConfiguration(DomoticzConfiguration);
  
  */
  Data->saveHTTPAPI(true);
  Data->saveDeviceMode(2);
  Data->saveRelayState(0, false);
  Data->saveLanguage(1);
}

void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
