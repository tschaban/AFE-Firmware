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
  configuration.isLED[1] = Eeprom.read(369);
  configuration.isLED[2] = Eeprom.read(372);
  configuration.isLED[3] = Eeprom.read(375);
  configuration.isLED[4] = Eeprom.read(378);

  configuration.isRelay[0] = Eeprom.read(381);
  configuration.isRelay[1] = Eeprom.read(402);
  configuration.isRelay[2] = Eeprom.read(423);
  configuration.isRelay[3] = Eeprom.read(444);

  configuration.isSwitch[0] = Eeprom.read(470);
  configuration.isSwitch[1] = Eeprom.read(477);
  configuration.isSwitch[2] = Eeprom.read(484);
  configuration.isSwitch[3] = Eeprom.read(491);
  configuration.isSwitch[4] = Eeprom.read(498);

  configuration.isPIR[0] = Eeprom.read(505);
  configuration.isPIR[1] = Eeprom.read(532);
  configuration.isPIR[2] = Eeprom.read(559);
  configuration.isPIR[3] = Eeprom.read(586);

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
  uint8_t nextLed = 3;
  configuration.gpio = Eeprom.readUInt8(367 + id * nextLed);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLed);
  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 21;
  char mqttTopic[49];
  configuration.gpio = Eeprom.readUInt8(382 + id * nextRelay);

  configuration.statePowerOn = Eeprom.readUInt8(384 + id * nextRelay);

  Eeprom.read(385 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(401 + id * nextRelay);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(471 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(472 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(473 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(476 + id * nextSwitch);

  return configuration;
}

PIR AFEDataAccess::getPIRConfiguration(uint8_t id) {
  PIR configuration;
  MQTT configurationMQTT;
  uint8_t nextPIR = 27;
  char mqttTopic[49];
  configuration.gpio = Eeprom.readUInt8(506 + id * nextPIR);

  Eeprom.read(507 + id * nextPIR, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.ledId = Eeprom.readUInt8(524 + id * nextPIR);
  configuration.relayId = Eeprom.readUInt8(525 + id * nextPIR);
  configuration.howLongKeepRelayOn = Eeprom.read(526 + id * nextPIR, 5).toInt();
  configuration.invertRelayState = Eeprom.read(531 + id * nextPIR);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  Eeprom.write(381, configuration.isRelay[0]);
  Eeprom.write(402, configuration.isRelay[1]);
  Eeprom.write(423, configuration.isRelay[2]);
  Eeprom.write(444, configuration.isRelay[3]);

  Eeprom.write(470, configuration.isSwitch[0]);
  Eeprom.write(477, configuration.isSwitch[1]);
  Eeprom.write(484, configuration.isSwitch[2]);
  Eeprom.write(491, configuration.isSwitch[3]);
  Eeprom.write(498, configuration.isSwitch[4]);

  Eeprom.write(366, configuration.isLED[0]);
  Eeprom.write(369, configuration.isLED[1]);
  Eeprom.write(372, configuration.isLED[2]);
  Eeprom.write(375, configuration.isLED[3]);
  Eeprom.write(378, configuration.isLED[4]);

  Eeprom.write(505, configuration.isPIR[0]);
  Eeprom.write(532, configuration.isPIR[1]);
  Eeprom.write(559, configuration.isPIR[2]);
  Eeprom.write(586, configuration.isPIR[3]);

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
  uint8_t nextRelay = 21;
  Eeprom.writeUInt8(382 + id * nextRelay, configuration.gpio);
  Eeprom.writeUInt8(384 + id * nextRelay, configuration.statePowerOn);
  Eeprom.write(385 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(401 + id * nextRelay, configuration.stateMQTTConnected);
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLed = 3;
  Eeprom.writeUInt8(367 + id * nextLed, configuration.gpio);
  Eeprom.write(368 + id * nextLed, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(471 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(472 + id * nextSwitch, configuration.type);
  Eeprom.write(473 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(476 + id * nextSwitch, configuration.functionality);
}

void AFEDataAccess::saveConfiguration(uint8_t id, PIR configuration) {
  uint8_t nextPIR = 27;
  Eeprom.writeUInt8(506 + id * nextPIR, configuration.gpio);
  Eeprom.write(507 + id * nextPIR, 16, configuration.name);
  Eeprom.writeUInt8(524 + id * nextPIR, configuration.ledId);
  Eeprom.writeUInt8(525 + id * nextPIR, configuration.relayId);
  Eeprom.write(526 + id * nextPIR, 5, (long)configuration.howLongKeepRelayOn);
  Eeprom.write(531 + id * nextPIR, configuration.invertRelayState);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  Eeprom.read(0, 7).toCharArray(version, sizeof(version));
  return *version;
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 21;
  return Eeprom.read(383 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 21;
  Eeprom.write(383 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}
