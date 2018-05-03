/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;

  uint8_t index = 77;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    configuration.isLED[i] = Eeprom.read(366 + i * index);
  }

  index = 40;
  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
    configuration.isRelay[i] = Eeprom.read(396 + i * index);
  }

  index = 7;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(382 + i * index);
  }

  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));

  configuration.isDHT = Eeprom.read(369);
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
  uint8_t nextLED = 77;
  configuration.gpio = Eeprom.readUInt8(367 + id * nextLED);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLED);

  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 40;

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

  configuration.ledID = Eeprom.readUInt8(442 + id);

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
  configuration.humidistat.enabled = isHumidistatEnabled(id);

  configuration.thermalProtection =
      Eeprom.read(457 + id * nextRelay, 3).toInt();

  configuration.idx = Eeprom.read(930 + id, 6).toInt();

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(391 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(392 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(393 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(396 + id * nextSwitch);
  configuration.relayID = Eeprom.readUInt8(440 + id);
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
  configuration.temperatureIdx = Eeprom.read(936, 6).toInt();
  configuration.humidityIdx = Eeprom.read(936, 6).toInt();
  configuration.temperatureAndHumidityIdx = Eeprom.read(936, 6).toInt();
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);

  uint8_t index = 40;
  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
    Eeprom.write(396 + i * index, configuration.isRelay[i]);
  }

  index = 7;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    Eeprom.write(382 + i * index, configuration.isSwitch[i]);
  }

  index = 77;
  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    Eeprom.write(366 + i * index, configuration.isLED[i]);
  }

  Eeprom.write(369, configuration.isDHT);
  saveAPI(API_MQTT, configuration.mqttAPI);
  saveAPI(API_HTTP, configuration.httpAPI);
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
  uint8_t nextRelay = 53;

  Eeprom.writeUInt8(397 + id * nextRelay, configuration.gpio);
  Eeprom.write(399 + id * nextRelay, 5, configuration.timeToOff);
  Eeprom.writeUInt8(404 + id * nextRelay, configuration.statePowerOn);
  Eeprom.write(405 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(421 + id * nextRelay, configuration.stateMQTTConnected);
  Eeprom.writeUInt8(442 + id, configuration.ledID);
  Eeprom.write(457, 3, configuration.thermalProtection);
  Eeprom.write(930 + id, 6, (long)configuration.idx);
}

void AFEDataAccess::saveConfiguration(REGULATOR configuration,
                                      boolean thermostat) {

  uint8_t id;

  if (thermostat) {
    id = 0;
    saveThermostatState(configuration.enabled);
  } else {
    id = 13;
    saveHumidistatState(configuration.enabled);
  }

  Eeprom.write(431 + id, 5, configuration.turnOn);
  Eeprom.write(436 + id, 5, configuration.turnOff);
  Eeprom.write(441 + id, configuration.turnOnAbove);
  Eeprom.write(442 + id, configuration.turnOffAbove);
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLED = 77;
  Eeprom.writeUInt8(367 + id * nextLED, configuration.gpio);
  Eeprom.write(368 + id * nextLED, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(391 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(392 + id * nextSwitch, configuration.type);
  Eeprom.write(393 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(396 + id * nextSwitch, configuration.functionality);
  Eeprom.writeUInt8(440 + id, configuration.relayID);
}

void AFEDataAccess::saveConfiguration(DH configuration) {
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.writeUInt8(371, configuration.type);
  Eeprom.write(372, 5, (long)configuration.temperature.interval);
  Eeprom.writeUInt8(377, configuration.temperature.unit);
  Eeprom.write(378, 4, (float)configuration.temperature.correction);
  Eeprom.write(382, 5, (long)configuration.humidity.interval);
  Eeprom.write(387, 3, (float)configuration.humidity.correction);
  Eeprom.write(936, 6, (long)configuration.temperatureIdx);
  Eeprom.write(936, 6, (long)configuration.humidityIdx);
  Eeprom.write(936, 6, (long)configuration.temperatureAndHumidityIdx);
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

boolean AFEDataAccess::isHumidistatEnabled(uint8_t id) {
  uint8_t nextRelay = 40;
  return Eeprom.read(435 + id * nextRelay);
}

void AFEDataAccess::saveHumidistatState(uint8_t id, boolean state) {
  uint8_t nextRelay = 40;
  Eeprom.write(435 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getSystemLedID() { return Eeprom.readUInt8(439); }

void AFEDataAccess::saveSystemLedID(uint8_t id) { Eeprom.writeUInt8(439, id); }

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
