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
  configuration.isRelay[0] = Eeprom.read(404);
  configuration.isSwitch[0] = Eeprom.read(390);
  configuration.isSwitch[1] = Eeprom.read(397);
  configuration.isDHT = Eeprom.read(369);
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
  uint8_t nextRelay = 53;

  char mqttTopic[49];
  configuration.gpio = Eeprom.readUInt8(405 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(407 + id * nextRelay, 5).toFloat();
  configuration.statePowerOn = Eeprom.readUInt8(412 + id * nextRelay);

  Eeprom.read(413 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(429 + id * nextRelay);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  configuration.showStatusUsingLED = Eeprom.read(430 + id * nextRelay);

  configuration.thermostat.turnOn =
      Eeprom.read(431 + id * nextRelay, 5).toFloat();
  configuration.thermostat.turnOff =
      Eeprom.read(436 + id * nextRelay, 5).toFloat();
  configuration.thermostat.turnOnAbove = Eeprom.read(441 + id * nextRelay);
  configuration.thermostat.turnOffAbove = Eeprom.read(442 + id * nextRelay);
  configuration.thermostat.enabled = isThermostatEnabled(id);

  configuration.humidistat.turnOn =
      Eeprom.read(444 + id * nextRelay, 5).toFloat();
  configuration.humidistat.turnOff =
      Eeprom.read(449 + id * nextRelay, 5).toFloat();
  configuration.humidistat.turnOnAbove = Eeprom.read(454 + id * nextRelay);
  configuration.humidistat.turnOffAbove = Eeprom.read(455 + id * nextRelay);
  configuration.humidistat.enabled = isThermostatEnabled(id);

  configuration.thermalProtection =
      Eeprom.read(457 + id * nextRelay, 3).toInt();

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(391 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(392 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(393 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(396 + id * nextSwitch);

  return configuration;
}

DH AFEDataAccess::getDHTConfiguration() {
  DH configuration;
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.type = Eeprom.readUInt8(371);
  configuration.temperature.interval = Eeprom.read(372, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(377);
  configuration.temperature.correction = Eeprom.read(378, 4).toFloat();
  configuration.humidity.interval = Eeprom.read(382, 5).toInt();
  configuration.humidity.correction = Eeprom.read(387, 3).toFloat();
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  Eeprom.write(404, configuration.isRelay[0]);
  Eeprom.write(390, configuration.isSwitch[0]);
  Eeprom.write(397, configuration.isSwitch[1]);
  Eeprom.write(366, configuration.isLED[0]);
  Eeprom.write(369, configuration.isDHT);
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
  uint8_t nextRelay = 53;

  Eeprom.writeUInt8(405 + id * nextRelay, configuration.gpio);
  Eeprom.write(407 + id * nextRelay, 5, configuration.timeToOff);
  Eeprom.writeUInt8(412 + id * nextRelay, configuration.statePowerOn);
  Eeprom.write(413 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(429 + id * nextRelay, configuration.stateMQTTConnected);

  Eeprom.write(430 + id * nextRelay, configuration.showStatusUsingLED);

  Eeprom.write(431 + id * nextRelay, 5, configuration.thermostat.turnOn);
  Eeprom.write(436 + id * nextRelay, 5, configuration.thermostat.turnOff);
  Eeprom.write(441 + id * nextRelay, configuration.thermostat.turnOnAbove);
  Eeprom.write(442 + id * nextRelay, configuration.thermostat.turnOffAbove);
  saveThermostatState(id, configuration.thermostat.enabled);

  Eeprom.write(444 + id * nextRelay, 5, configuration.humidistat.turnOn);
  Eeprom.write(449 + id * nextRelay, 5, configuration.humidistat.turnOff);
  Eeprom.write(454 + id * nextRelay, configuration.humidistat.turnOnAbove);
  Eeprom.write(455 + id * nextRelay, configuration.humidistat.turnOffAbove);
  saveHumidistatState(id, configuration.humidistat.enabled);

  Eeprom.write(457 + id * nextRelay, 3, configuration.thermalProtection);
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLED = 2;
  Eeprom.writeUInt8(367 + id * nextLED, configuration.gpio);
  Eeprom.write(368 + id * nextLED, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(391 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(392 + id * nextSwitch, configuration.type);
  Eeprom.write(393 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(396 + id * nextSwitch, configuration.functionality);
}

void AFEDataAccess::saveConfiguration(DH configuration) {
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.writeUInt8(371, configuration.type);
  Eeprom.write(372, 5, (long)configuration.temperature.interval);
  Eeprom.writeUInt8(377, configuration.temperature.unit);
  Eeprom.write(378, 4, (float)configuration.temperature.correction);
  Eeprom.write(382, 5, (long)configuration.humidity.interval);
  Eeprom.write(387, 3, (float)configuration.humidity.correction);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  Eeprom.read(0, 7).toCharArray(version, sizeof(version));
  return *version;
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 53;
  return Eeprom.read(406 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 53;
  Eeprom.write(406 + id * nextRelay, state);
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
  uint8_t nextRelay = 53;
  return Eeprom.read(443 + id * nextRelay);
}

void AFEDataAccess::saveThermostatState(uint8_t id, boolean state) {
  uint8_t nextRelay = 53;
  Eeprom.write(443 + id * nextRelay, state);
}

boolean AFEDataAccess::isHumidistatEnabled(uint8_t id) {
  uint8_t nextRelay = 53;
  return Eeprom.read(456 + id * nextRelay);
}

void AFEDataAccess::saveHumidistatState(uint8_t id, boolean state) {
  uint8_t nextRelay = 53;
  Eeprom.write(456 + id * nextRelay, state);
}
