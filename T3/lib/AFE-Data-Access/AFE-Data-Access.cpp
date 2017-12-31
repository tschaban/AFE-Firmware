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
  configuration.isRelay[0] = Eeprom.read(369);
  configuration.isSwitch[0] = Eeprom.read(395);
  configuration.isSwitch[1] = Eeprom.read(402);
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

LED AFEDataAccess::getLEDConfiguration() {
  LED configuration;
  configuration.gpio = Eeprom.readUInt8(367);
  configuration.changeToOppositeValue = Eeprom.read(368);

  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 21;
  char mqttTopic[49];
  configuration.gpio = Eeprom.readUInt8(371 + id * nextRelay);

  configuration.statePowerOn = Eeprom.readUInt8(373 + id * nextRelay);

  Eeprom.read(374 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(390 + id * nextRelay);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(552 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(553 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(554 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(557 + id * nextSwitch);

  return configuration;
}

PIR AFEDataAccess::getPIRConfiguration(uint8_t id) {
  PIR configuration;
  uint8_t nextPIR = 27;
  configuration.gpio = Eeprom.readUInt8(525 + id * nextPIR);

  Eeprom.read(526 + id * nextPIR, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.Led.gpio = Eeprom.readUInt8(543 + id * nextPIR);
  configuration.relay.id = Eeprom.readUInt8(544 + id * nextPIR);
  configuration.relay.howLongKeepItOpen =
      Eeprom.read(545 + id * nextPIR, 5).toInt();
  configuration.relay.invertedState = Eeprom.read(550 + id * nextPIR);

  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  Eeprom.write(369, configuration.isRelay[0]);
  Eeprom.write(395, configuration.isSwitch[0]);
  Eeprom.write(402, configuration.isSwitch[1]);
  Eeprom.write(366, configuration.isLED[0]);
  Eeprom.write(25, configuration.httpAPI);
  Eeprom.write(228, configuration.mqttAPI);
  // Eeprom.write(413, configuration.isDS18B20);
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
  uint8_t nextRelay = 21;
  Eeprom.writeUInt8(371 + id * nextRelay, configuration.gpio);
  Eeprom.writeUInt8(373 + id * nextRelay, configuration.statePowerOn);
  Eeprom.write(374 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(390 + id * nextRelay, configuration.stateMQTTConnected);
}

void AFEDataAccess::saveConfiguration(LED configuration) {
  Eeprom.writeUInt8(367, configuration.gpio);
  Eeprom.write(368, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(552 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(553 + id * nextSwitch, configuration.type);
  Eeprom.write(554 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(557 + id * nextSwitch, configuration.functionality);
}

void AFEDataAccess::saveConfiguration(uint8_t id, PIR configuration) {
  uint8_t nextPIR = 27;
  Eeprom.writeUInt8(525 + id * nextPIR, configuration.gpio);
  Eeprom.write(526 + id * nextPIR, 16, configuration.name);
  Eeprom.writeUInt8(543 + id * nextPIR, configuration.Led.gpio);
  Eeprom.writeUInt8(544 + id * nextPIR, configuration.relay.id);
  Eeprom.write(545 + id * nextPIR, 5,
               (long)configuration.relay.howLongKeepItOpen);
  Eeprom.write(550 + id * nextPIR, configuration.relay.invertedState);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  Eeprom.read(0, 7).toCharArray(version, sizeof(version));
  return *version;
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 21;
  return Eeprom.read(372 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 21;
  Eeprom.write(372 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}
