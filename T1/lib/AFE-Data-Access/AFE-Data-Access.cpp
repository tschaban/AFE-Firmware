/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;

  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));
  configuration.isLED[0] = Eeprom.read(366);
  configuration.isRelay[0] = Eeprom.read(396);
  configuration.isSwitch[0] = Eeprom.read(382);
  configuration.isSwitch[1] = Eeprom.read(389);
  configuration.isDS18B20 = Eeprom.read(369);
  configuration.httpAPI = Eeprom.read(25);
  configuration.mqttAPI = Eeprom.read(228);

  return configuration;
}

FIRMWARE AFEDataAccess::getFirmwareConfiguration() {
  FIRMWARE configuration;

  Eeprom.read(0, 7).toCharArray(configuration.version,
                                sizeof(configuration.version));
  Eeprom.read(28, 120).toCharArray(configuration.upgradeURL,
                                   sizeof(configuration.upgradeURL));
  configuration.type = Eeprom.readUInt8(7);
  configuration.autoUpgrade = Eeprom.readUInt8(27);
  return configuration;
}

NETWORK AFEDataAccess::getNetworkConfiguration() {
  NETWORK configuration;

  Eeprom.read(148, 32).toCharArray(configuration.ssid,
                                   sizeof(configuration.ssid));
  Eeprom.read(180, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));

  configuration.isDHCP = Eeprom.read(212);
  configuration.ip = Eeprom.readIP(213);
  configuration.gateway = Eeprom.readIP(217);
  configuration.subnet = Eeprom.readIP(221);
  configuration.noConnectionAttempts = Eeprom.readUInt8(225);
  configuration.waitTimeConnections = Eeprom.readUInt8(226);
  configuration.waitTimeSeries = Eeprom.readUInt8(227);
  return configuration;
}

MQTT AFEDataAccess::getMQTTConfiguration() {
  MQTT configuration;

  Eeprom.read(229, 32).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.ip = Eeprom.readIP(261);
  configuration.port = Eeprom.read(265, 5).toInt();
  Eeprom.read(270, 32).toCharArray(configuration.user,
                                   sizeof(configuration.user));
  Eeprom.read(302, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  Eeprom.read(334, 32).toCharArray(configuration.topic,
                                   sizeof(configuration.topic));

  return configuration;
}

LED AFEDataAccess::getLEDConfiguration(uint8_t id) {
  LED configuration;
  uint8_t nextLED = 0;
  configuration.gpio = Eeprom.readUInt8(367 + id * nextLED);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLED);

  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 40;

  char mqttTopic[49];
  configuration.gpio = Eeprom.readUInt8(397 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(399 + id * nextRelay, 5).toFloat();
  configuration.statePowerOn = Eeprom.readUInt8(404 + id * nextRelay);

  Eeprom.read(405 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(421 + id * nextRelay);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  configuration.showStatusUsingLED = Eeprom.read(422 + id * nextRelay);

  configuration.thermostat.turnOn =
      Eeprom.read(423 + id * nextRelay, 5).toFloat();
  configuration.thermostat.turnOff =
      Eeprom.read(428 + id * nextRelay, 5).toFloat();
  configuration.thermostat.turnOnAbove = Eeprom.read(433 + id * nextRelay);
  configuration.thermostat.turnOffAbove = Eeprom.read(434 + id * nextRelay);
  configuration.thermostat.enabled = isThermostatEnabled(id);

  configuration.thermalProtection =
      Eeprom.read(436 + id * nextRelay, 3).toInt();

  configuration.ledID = 0; // @TODO added for compatibility with the code
                           // introduced in T4 so the Relay can switch on
                           // assigned to it LED. To added for this version in
                           // future

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(383 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(384 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(385 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(388 + id * nextSwitch);

  return configuration;
}

DS18B20 AFEDataAccess::getDS18B20Configuration() {
  DS18B20 configuration;
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.correction = Eeprom.read(371, 5).toFloat();
  configuration.interval = Eeprom.read(376, 5).toInt();
  configuration.unit = Eeprom.readUInt8(381);
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  Eeprom.write(396, configuration.isRelay[0]);
  Eeprom.write(382, configuration.isSwitch[0]);
  Eeprom.write(389, configuration.isSwitch[1]);
  Eeprom.write(366, configuration.isLED[0]);
  Eeprom.write(369, configuration.isDS18B20);
  Eeprom.write(25, configuration.httpAPI);
  Eeprom.write(228, configuration.mqttAPI);
}

void AFEDataAccess::saveConfiguration(FIRMWARE configuration) {
  Eeprom.write(0, 7, configuration.version);
  Eeprom.writeUInt8(7, configuration.type);
  Eeprom.writeUInt8(27, configuration.autoUpgrade);
  Eeprom.write(28, 120, configuration.upgradeURL);
}

void AFEDataAccess::saveConfiguration(NETWORK configuration) {
  Eeprom.write(148, 32, configuration.ssid);
  Eeprom.write(180, 32, configuration.password);
  Eeprom.write(212, configuration.isDHCP);
  Eeprom.writeIP(213, configuration.ip);
  Eeprom.writeIP(217, configuration.gateway);
  Eeprom.writeIP(221, configuration.subnet);
  Eeprom.writeUInt8(225, configuration.noConnectionAttempts);
  Eeprom.writeUInt8(226, configuration.waitTimeConnections);
  Eeprom.writeUInt8(227, configuration.waitTimeSeries);
}

void AFEDataAccess::saveConfiguration(MQTT configuration) {
  Eeprom.write(229, 32, configuration.host);
  Eeprom.writeIP(261, configuration.ip);
  Eeprom.write(265, 5, (long)configuration.port);
  Eeprom.write(270, 32, configuration.user);
  Eeprom.write(302, 32, configuration.password);
  Eeprom.write(334, 32, configuration.topic);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
  uint8_t nextRelay = 40;

  Eeprom.writeUInt8(397 + id * nextRelay, configuration.gpio);
  Eeprom.write(399 + id * nextRelay, 5, configuration.timeToOff);
  Eeprom.writeUInt8(404 + id * nextRelay, configuration.statePowerOn);
  Eeprom.write(405 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(421 + id * nextRelay, configuration.stateMQTTConnected);

  Eeprom.write(422 + id * nextRelay, configuration.showStatusUsingLED);
  Eeprom.write(423 + id * nextRelay, 5, configuration.thermostat.turnOn);
  Eeprom.write(428 + id * nextRelay, 5, configuration.thermostat.turnOff);
  Eeprom.write(433 + id * nextRelay, configuration.thermostat.turnOnAbove);
  Eeprom.write(434 + id * nextRelay, configuration.thermostat.turnOffAbove);
  saveThermostatState(id, configuration.thermostat.enabled);
  Eeprom.write(436 + id * nextRelay, 3, configuration.thermalProtection);
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLED = 2;
  Eeprom.writeUInt8(367 + id * nextLED, configuration.gpio);
  Eeprom.write(368 + id * nextLED, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(383 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(384 + id * nextSwitch, configuration.type);
  Eeprom.write(385 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(388 + id * nextSwitch, configuration.functionality);
}

void AFEDataAccess::saveConfiguration(DS18B20 configuration) {
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.write(371, 5, (float)configuration.correction);
  Eeprom.write(376, 5, (long)configuration.interval);
  Eeprom.writeUInt8(381, configuration.unit);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  Eeprom.read(0, 7).toCharArray(version, sizeof(version));
  return *version;
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 40;
  return Eeprom.read(398 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 40;
  Eeprom.write(398 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

boolean AFEDataAccess::isThermostatEnabled(uint8_t id) {
  uint8_t nextRelay = 40;
  return Eeprom.read(435 + id * nextRelay);
}

void AFEDataAccess::saveThermostatState(uint8_t id, boolean state) {
  uint8_t nextRelay = 40;
  Eeprom.write(435 + id * nextRelay, state);
}
