/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;

  uint8_t index = 3;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    configuration.isLED[i] = Eeprom.read(366 + i * index);
  }

  index = 8;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(398 + i * index);
  }
  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));

  index = 24;
  for (uint8_t i = 0; i < sizeof(configuration.isContactron); i++) {
    configuration.isContactron[i] = Eeprom.read(414 + i * index);
  }

  configuration.isDHT = Eeprom.read(376);
  configuration.httpAPI = Eeprom.read(25);
  configuration.mqttAPI = Eeprom.read(228);
  configuration.domoticzAPI = Eeprom.read(800);

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

DOMOTICZ AFEDataAccess::getDomoticzConfiguration() {
  DOMOTICZ configuration;

  configuration.protocol = Eeprom.readUInt8(801);
  Eeprom.read(802, 40).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.port = Eeprom.read(842, 5).toInt();
  Eeprom.read(847, 32).toCharArray(configuration.user, 32);
  Eeprom.read(879, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
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
  uint8_t nextRelay = 5;

  configuration.gpio = Eeprom.readUInt8(462 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(463 + id * nextRelay, 4).toInt();

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 8;
  configuration.gpio = Eeprom.readUInt8(399 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(400 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(401 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(404 + id * nextSwitch);
  configuration.relayID = Eeprom.readUInt8(405 + id * nextSwitch);
  return configuration;
}

CONTACTRON AFEDataAccess::getContactronConfiguration(uint8_t id) {
  CONTACTRON configuration;
  uint8_t nextContactron = 24;
  MQTT configurationMQTT;

  configuration.gpio = Eeprom.readUInt8(415 + id * nextContactron);
  configuration.outputDefaultState =
      Eeprom.readUInt8(416 + id * nextContactron);
  configuration.ledID = Eeprom.readUInt8(417 + id * nextContactron);
  configuration.bouncing = Eeprom.read(418 + id * nextContactron, 4).toInt();

  Eeprom.read(422 + id * nextContactron, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  return configuration;
}

DH AFEDataAccess::getDHTConfiguration() {
  DH configuration;
  configuration.gpio = Eeprom.readUInt8(377);
  configuration.type = Eeprom.readUInt8(378);
  configuration.temperature.interval = Eeprom.read(379, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(384);
  configuration.temperature.correction = Eeprom.read(385, 4).toFloat();
  configuration.humidity.interval = Eeprom.read(389, 5).toInt();
  configuration.humidity.correction = Eeprom.read(394, 3).toFloat();
  configuration.sendOnlyChanges = Eeprom.read(397);
  return configuration;
}

GATE AFEDataAccess::getGateConfiguration() {
  GATE configuration;
  for (uint8_t i = 0; i < sizeof(configuration.state); i++) {
    configuration.state[i] = Eeprom.readUInt8(467 + i);
  }
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);

  uint8_t index = 8;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    Eeprom.write(398 + i * index, configuration.isSwitch[i]);
  }

  index = 3;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    Eeprom.write(366 + i * index, configuration.isLED[i]);
  }

  index = 24;
  for (uint8_t i = 0; i < sizeof(configuration.isContactron); i++) {
    Eeprom.write(414 + i * index, configuration.isContactron[i]);
  }

  Eeprom.write(376, configuration.isDHT);

  saveAPI(API_HTTP, configuration.httpAPI);
  saveAPI(API_MQTT, configuration.mqttAPI);
  saveAPI(API_DOMOTICZ, configuration.domoticzAPI);
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

void AFEDataAccess::saveConfiguration(DOMOTICZ configuration) {
  Eeprom.writeUInt8(801, configuration.protocol);
  Eeprom.write(802, 40, configuration.host);
  Eeprom.write(842, 5, (long)configuration.port);
  Eeprom.write(847, 32, configuration.user);
  Eeprom.write(879, 32, configuration.password);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
  uint8_t nextRelay = 5;
  Eeprom.writeUInt8(462 + id * nextRelay, configuration.gpio);
  Eeprom.write(463 + id * nextRelay, 4, long(configuration.timeToOff));
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLed = 3;

  Eeprom.writeUInt8(367 + id * nextLed, configuration.gpio);
  Eeprom.write(368 + id * nextLed, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 8;
  Eeprom.writeUInt8(399 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(400 + id * nextSwitch, configuration.type);
  Eeprom.write(401 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(404 + id * nextSwitch, configuration.functionality);
  Eeprom.writeUInt8(405 + id * nextSwitch, configuration.relayID);
}

void AFEDataAccess::saveConfiguration(uint8_t id, CONTACTRON configuration) {
  uint8_t nextContactron = 24;
  Eeprom.writeUInt8(415 + id * nextContactron, configuration.gpio);
  Eeprom.writeUInt8(416 + id * nextContactron,
                    configuration.outputDefaultState);
  Eeprom.writeUInt8(417 + id * nextContactron, configuration.ledID);
  Eeprom.write(418 + id * nextContactron, 4, (long)configuration.bouncing);
  Eeprom.write(422 + id * nextContactron, 16, configuration.name);
}

void AFEDataAccess::saveConfiguration(DH configuration) {
  Eeprom.writeUInt8(377, configuration.gpio);
  Eeprom.writeUInt8(378, configuration.type);
  Eeprom.write(379, 5, (long)configuration.temperature.interval);
  Eeprom.writeUInt8(384, configuration.temperature.unit);
  Eeprom.write(385, 4, (float)configuration.temperature.correction);
  Eeprom.write(389, 5, (long)configuration.humidity.interval);
  Eeprom.write(394, 3, (float)configuration.humidity.correction);
  Eeprom.write(397, configuration.sendOnlyChanges);
}

void AFEDataAccess::saveConfiguration(GATE configuration) {
  for (uint8_t i = 0; i < sizeof(configuration.state); i++) {
    Eeprom.writeUInt8(467 + i, configuration.state[i]);
  }
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

uint8_t AFEDataAccess::getSystemLedID() { return Eeprom.readUInt8(530); }

void AFEDataAccess::saveSystemLedID(uint8_t id) { Eeprom.writeUInt8(530, id); }

boolean AFEDataAccess::getRelayState(uint8_t id) { return false; }

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

const String AFEDataAccess::getDeviceID() { return Eeprom.read(1000, 8); }

void AFEDataAccess::saveDeviceID(String id) { Eeprom.write(1000, 8, id); }

void AFEDataAccess::saveAPI(uint8_t apiID, boolean state) {
  if (apiID == API_HTTP) {
    Eeprom.write(25, state);
  } else if (apiID == API_MQTT) {
    Eeprom.write(228, state);
  } else if (apiID == API_DOMOTICZ) {
    Eeprom.write(800, state);
    if (state) {
      Eeprom.write(25, true);
    }
  }
}
