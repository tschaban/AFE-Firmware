/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;

  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));

  configuration.httpAPI = Eeprom.read(25);
  configuration.mqttAPI = Eeprom.read(228);
  configuration.domoticzAPI = Eeprom.read(800);

#if defined(T0_CONFIG)
  configuration.isLED[0] = Eeprom.read(366);
  configuration.isLED[1] = Eeprom.read(418);
  configuration.isSwitch[1] = Eeprom.read(402);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.isRelay[0] = Eeprom.read(369);
  configuration.isSwitch[0] = Eeprom.read(395);
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T4_CONFIG)

#if defined(T1_CONFIG)
  uint8_t index = 77;
#elif defined(T2_CONFIG)
  uint8_t index = 98;
#elif defined(T4_CONFIG)
  uint8_t index = 3;
#endif

  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    configuration.isLED[i] = Eeprom.read(366 + i * index);
  }
#endif

#ifdef T1_CONFIG
  index = 40;
  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
    configuration.isRelay[i] = Eeprom.read(396 + i * index);
  }

  index = 7;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(382 + i * index);
  }

  configuration.isDS18B20 = Eeprom.read(369);
#endif

#ifdef T2_CONFIG
  index = 0;
  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
    configuration.isRelay[i] = Eeprom.read(404 + i * index);
  }

  index = 7;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(390 + i * index);
  }

  configuration.isDHT = Eeprom.read(369);

#endif

#ifdef T4_CONFIG
  index = 27;
  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
    configuration.isRelay[i] = Eeprom.read(382 + i * index);
  }

  index = 8;
  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
    configuration.isSwitch[i] = Eeprom.read(490 + i * index);
  }
#endif

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

#ifndef T0_SHELLY_1_CONFIG
LED AFEDataAccess::getLEDConfiguration(uint8_t id) {
  LED configuration;

#if defined(T0_CONFIG)
  uint8_t nextLED = 52;
#elif defined(T1_CONFIG)
  uint8_t nextLED = 77;
#elif defined(T2_CONFIG)
  uint8_t nextLED = 98;
#elif defined(T4_CONFIG)
  uint8_t nextLED = 3;
#endif

  configuration.gpio = Eeprom.readUInt8(367 + id * nextLED);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLED);

  return configuration;
}
#endif

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;

#if defined(T4_CONFIG)
  uint8_t nextRelay = 27;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.gpio = Eeprom.readUInt8(370);
#elif defined(T1_CONFIG)
  configuration.gpio = Eeprom.readUInt8(397);
#elif defined(T2_CONFIG)
  configuration.gpio = Eeprom.readUInt8(405);
#elif defined(T4_CONFIG)
  configuration.gpio = Eeprom.readUInt8(383 + id * nextRelay);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.timeToOff = Eeprom.read(372, 5).toFloat();
#elif defined(T1_CONFIG)
  configuration.timeToOff = Eeprom.read(399, 5).toFloat();
#elif defined(T2_CONFIG)
  configuration.timeToOff = Eeprom.read(407, 5).toFloat();
#elif defined(T4_CONFIG)
  configuration.timeToOff = Eeprom.read(385 + id * nextRelay, 5).toFloat();
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.statePowerOn = Eeprom.readUInt8(377);
#elif defined(T1_CONFIG)
  configuration.statePowerOn = Eeprom.readUInt8(404);
#elif defined(T2_CONFIG)
  configuration.statePowerOn = Eeprom.readUInt8(412);
#elif defined(T4_CONFIG)
  configuration.statePowerOn = Eeprom.readUInt8(390 + id * nextRelay);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.read(378, 16).toCharArray(configuration.name,
                                   sizeof(configuration.name));
#elif defined(T1_CONFIG)
  Eeprom.read(405, 16).toCharArray(configuration.name,
                                   sizeof(configuration.name));
#elif defined(T2_CONFIG)
  Eeprom.read(413, 16).toCharArray(configuration.name,
                                   sizeof(configuration.name));
#elif defined(T4_CONFIG)
  Eeprom.read(391 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.stateMQTTConnected = Eeprom.readUInt8(394);
#elif defined(T1_CONFIG)
  configuration.stateMQTTConnected = Eeprom.readUInt8(421);
#elif defined(T2_CONFIG)
  configuration.stateMQTTConnected = Eeprom.readUInt8(429);
#elif defined(T4_CONFIG)
  configuration.stateMQTTConnected = Eeprom.readUInt8(407 + id * nextRelay);
#endif

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

#if defined(T0_CONFIG)
  configuration.ledID = Eeprom.readUInt8(421);
#elif defined(T1_CONFIG)
  configuration.ledID = Eeprom.readUInt8(442);
#elif defined(T2_CONFIG)
  configuration.ledID = Eeprom.readUInt8(463);
#elif defined(T4_CONFIG)
  configuration.ledID = Eeprom.readUInt8(531 + id);
#endif

  configuration.idx = Eeprom.read(930 + 6 * id, 6).toInt();

#if defined(T1_CONFIG)
  configuration.thermostat.turnOn = Eeprom.read(423, 5).toFloat();
  configuration.thermostat.turnOff = Eeprom.read(428, 5).toFloat();
  configuration.thermostat.turnOnAbove = Eeprom.read(433);
  configuration.thermostat.turnOffAbove = Eeprom.read(434);
  configuration.thermostat.enabled = isThermostatEnabled(id);
  configuration.thermalProtection = Eeprom.read(436, 3).toInt();
#endif

#if defined(T2_CONFIG)
  configuration.thermostat.turnOn = Eeprom.read(431, 5).toFloat();
  configuration.thermostat.turnOff = Eeprom.read(436, 5).toFloat();
  configuration.thermostat.turnOnAbove = Eeprom.read(441);
  configuration.thermostat.turnOffAbove = Eeprom.read(442);
  configuration.thermostat.enabled = isThermostatEnabled(id);

  configuration.humidistat.turnOn = Eeprom.read(444, 5).toFloat();
  configuration.humidistat.turnOff = Eeprom.read(449, 5).toFloat();
  configuration.humidistat.turnOnAbove = Eeprom.read(454);
  configuration.humidistat.turnOffAbove = Eeprom.read(455);
  configuration.humidistat.enabled = isHumidistatEnabled(id);
  configuration.thermalProtection = Eeprom.read(457, 3).toInt();
#endif

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;

#if defined(T0_CONFIG) || defined(T1_CONFIG) || defined(T0_SHELLY_1_CONFIG) || \
    defined(T2_CONFIG)
  uint8_t nextSwitch = 7;
#elif defined(T4_CONFIG)
  uint8_t nextSwitch = 8;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.gpio = Eeprom.readUInt8(396 + id * nextSwitch);
#elif defined(T1_CONFIG)
  configuration.gpio = Eeprom.readUInt8(383 + id * nextSwitch);
#elif defined(T2_CONFIG)
  configuration.gpio = Eeprom.readUInt8(391 + id * nextSwitch);
#elif defined(T4_CONFIG)
  configuration.gpio = Eeprom.readUInt8(491 + id * nextSwitch);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.type = Eeprom.readUInt8(397 + id * nextSwitch);
#elif defined(T1_CONFIG)
  configuration.type = Eeprom.readUInt8(384 + id * nextSwitch);
#elif defined(T2_CONFIG)
  configuration.type = Eeprom.readUInt8(392 + id * nextSwitch);
#elif defined(T4_CONFIG)
  configuration.type = Eeprom.readUInt8(492 + id * nextSwitch);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.sensitiveness = Eeprom.read(398 + id * nextSwitch, 3).toInt();
#elif defined(T1_CONFIG)
  configuration.sensitiveness = Eeprom.read(385 + id * nextSwitch, 3).toInt();
#elif defined(T2_CONFIG)
  configuration.sensitiveness = Eeprom.read(393 + id * nextSwitch, 3).toInt();
#elif defined(T4_CONFIG)
  configuration.sensitiveness = Eeprom.read(493 + id * nextSwitch, 3).toInt();
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.functionality = Eeprom.readUInt8(401 + id * nextSwitch);
#elif defined(T1_CONFIG)
  configuration.functionality = Eeprom.readUInt8(388 + id * nextSwitch);
#elif defined(T2_CONFIG)
  configuration.functionality = Eeprom.readUInt8(396 + id * nextSwitch);
#elif defined(T4_CONFIG)
  configuration.functionality = Eeprom.readUInt8(496 + id * nextSwitch);

#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  configuration.relayID =
      Eeprom.readUInt8(416 + id); /* It's in EEPROM one after the other one */
#elif defined(T1_CONFIG)
  configuration.relayID =
      Eeprom.readUInt8(440 + id); /* It's in EEPROM one after the other one */
#elif defined(T2_CONFIG)
  configuration.relayID =
      Eeprom.readUInt8(461 + id); /* It's in EEPROM one after the other one */
#elif defined(T4_CONFIG)
  configuration.relayID = Eeprom.readUInt8(497 + id * nextSwitch);
#endif

  return configuration;
}

#if defined(T1_CONFIG)
DS18B20 AFEDataAccess::getSensorConfiguration() {
  DS18B20 configuration;
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.correction = Eeprom.read(371, 5).toFloat();
  configuration.interval = Eeprom.read(376, 5).toInt();
  configuration.unit = Eeprom.readUInt8(381);
  configuration.sendOnlyChanges = Eeprom.read(446);
  configuration.idx = Eeprom.read(936, 6).toInt();
  return configuration;
}
#endif

#if defined(T2_CONFIG)
DH AFEDataAccess::getSensorConfiguration() {
  DH configuration;
  configuration.gpio = Eeprom.readUInt8(370);
  configuration.type = Eeprom.readUInt8(371);
  configuration.temperature.interval = Eeprom.read(372, 5).toInt();
  configuration.temperature.unit = Eeprom.readUInt8(377);
  configuration.temperature.correction = Eeprom.read(378, 4).toFloat();
  configuration.humidity.interval = Eeprom.read(382, 5).toInt();
  configuration.humidity.correction = Eeprom.read(387, 3).toFloat();
  configuration.temperatureIdx = Eeprom.read(936, 6).toInt();
  configuration.humidityIdx = Eeprom.read(942, 6).toInt();
  configuration.temperatureAndHumidityIdx = Eeprom.read(948, 6).toInt();
  configuration.sendOnlyChanges = Eeprom.read(467);
  configuration.publishHeatIndex = Eeprom.read(974);
  return configuration;
}
#endif

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  uint8_t index;
#if defined(T4_CONFIG)
  index = 27;
#endif

  for (uint8_t i = 0; i < sizeof(configuration.isRelay); i++) {
#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.write(369, configuration.isRelay[i]);
#elif defined(T1_CONFIG)
    Eeprom.write(396, configuration.isRelay[i]);
#elif defined(T2_CONFIG)
    Eeprom.write(404, configuration.isRelay[i]);
#elif defined(T4_CONFIG)
    Eeprom.write(382 + i * index, configuration.isRelay[i]);
#endif
  }

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T1_CONFIG) || \
    defined(T2_CONFIG)
  index = 7;
#elif defined(T4_CONFIG)
  index = 8;
#endif

  for (uint8_t i = 0; i < sizeof(configuration.isSwitch); i++) {
#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
    Eeprom.write(395 + i * index, configuration.isSwitch[i]);
#elif defined(T1_CONFIG)
    Eeprom.write(382 + i * index, configuration.isSwitch[i]);
#elif defined(T2_CONFIG)
    Eeprom.write(390 + i * index, configuration.isSwitch[i]);
#elif defined(T4_CONFIG)
    Eeprom.write(490 + i * index, configuration.isSwitch[i]);
#endif
  }

#ifndef T0_SHELLY_1_CONFIG

#if defined(T0_CONFIG)
  index = 52;
#elif defined(T1_CONFIG)
  index = 77;
#elif defined(T2_CONFIG)
  index = 98;
#elif defined(T4_CONFIG)
  index = 3;
#endif

  for (uint8_t i = 0; i < sizeof(configuration.isLED); i++) {
    Eeprom.write(366 + i * index, configuration.isLED[i]);
  }
#endif

  saveAPI(API_MQTT, configuration.mqttAPI);
  saveAPI(API_HTTP, configuration.httpAPI);
  saveAPI(API_DOMOTICZ, configuration.domoticzAPI);

#ifdef T1_CONFIG
  Eeprom.write(369, configuration.isDS18B20);
#endif

#ifdef T2_CONFIG
  Eeprom.write(369, configuration.isDHT);
#endif
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

#if defined(T4_CONFIG)
  uint8_t nextRelay = 27;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(370, configuration.gpio);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(397, configuration.gpio);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(405, configuration.gpio);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(383 + id * nextRelay, configuration.gpio);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.write(372, 5, configuration.timeToOff);
#elif defined(T1_CONFIG)
  Eeprom.write(399, 5, configuration.timeToOff);
#elif defined(T2_CONFIG)
  Eeprom.write(407, 5, configuration.timeToOff);
#elif defined(T4_CONFIG)
  Eeprom.write(385 + id * nextRelay, 5, configuration.timeToOff);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(377, configuration.statePowerOn);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(404, configuration.statePowerOn);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(412, configuration.statePowerOn);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(390 + id * nextRelay, configuration.statePowerOn);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.write(378, 16, configuration.name);
#elif defined(T1_CONFIG)
  Eeprom.write(405, 16, configuration.name);
#elif defined(T2_CONFIG)
  Eeprom.write(413, 16, configuration.name);
#elif defined(T4_CONFIG)
  Eeprom.write(391 + id * nextRelay, 16, configuration.name);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(394, configuration.stateMQTTConnected);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(421, configuration.stateMQTTConnected);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(429, configuration.stateMQTTConnected);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(407 + id * nextRelay, configuration.stateMQTTConnected);
#endif

#if defined(T0_CONFIG)
  Eeprom.writeUInt8(421 + id, configuration.ledID);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(442 + id, configuration.ledID);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(463 + id, configuration.ledID);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(531 + id, configuration.ledID);
#endif

  Eeprom.write(930 + 6 * id, 6, (long)configuration.idx);

#if defined(T1_CONFIG)
  Eeprom.write(436, 3, configuration.thermalProtection);
#elif defined(T2_CONFIG)
  Eeprom.write(457, 3, configuration.thermalProtection);
#endif
}

/* @TODO Unifiy below two */

#if defined(T1_CONFIG)
void AFEDataAccess::saveConfiguration(REGULATOR configuration,
                                      boolean thermostat) {
  Eeprom.write(423, 5, configuration.turnOn);
  Eeprom.write(428, 5, configuration.turnOff);
  Eeprom.write(433, configuration.turnOnAbove);
  Eeprom.write(434, configuration.turnOffAbove);
  saveThermostatState(0, configuration.enabled);
}
#endif

/* @TODO the code below should be changed if ID is not used */

#if defined(T2_CONFIG)
void AFEDataAccess::saveConfiguration(REGULATOR configuration,
                                      boolean thermostat) {

  /* id is actually not used. It' here for code compatibility with other
   * versions of AFE */
  uint8_t index;

  /* @TODO id is not used. 0 hardcoded. refactoring needed */

  if (thermostat) {
    index = 0;
    saveThermostatState(0, configuration.enabled);
  } else {
    index = 13;
    saveHumidistatState(0, configuration.enabled);
  }

  Eeprom.write(431 + index, 5, configuration.turnOn);
  Eeprom.write(436 + index, 5, configuration.turnOff);
  Eeprom.write(441 + index, configuration.turnOnAbove);
  Eeprom.write(442 + index, configuration.turnOffAbove);
}
#endif

#ifndef T0_SHELLY_1_CONFIG
void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {

#if defined(T0_CONFIG)
  uint8_t nextLED = 52;
#elif defined(T1_CONFIG)
  uint8_t nextLED = 77;
#elif defined(T2_CONFIG)
  uint8_t nextLED = 98;
#elif defined(T4_CONFIG)
  uint8_t nextLED = 3;
#endif

  Eeprom.writeUInt8(367 + id * nextLED, configuration.gpio);
  Eeprom.write(368 + id * nextLED, configuration.changeToOppositeValue);
}
#endif

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T1_CONFIG) || \
    defined(T2_CONFIG)
  uint8_t nextSwitch = 7;
#elif defined(T4_CONFIG)
  uint8_t nextSwitch = 8;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(396 + id * nextSwitch, configuration.gpio);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(383 + id * nextSwitch, configuration.gpio);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(391 + id * nextSwitch, configuration.gpio);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(491 + id * nextSwitch, configuration.gpio);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(397 + id * nextSwitch, configuration.type);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(384 + id * nextSwitch, configuration.type);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(392 + id * nextSwitch, configuration.type);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(492 + id * nextSwitch, configuration.type);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.write(398 + id * nextSwitch, 3, (long)configuration.sensitiveness);
#elif defined(T1_CONFIG)
  Eeprom.write(385 + id * nextSwitch, 3, (long)configuration.sensitiveness);
#elif defined(T2_CONFIG)
  Eeprom.write(393 + id * nextSwitch, 3, (long)configuration.sensitiveness);
#elif defined(T4_CONFIG)
  Eeprom.write(493 + id * nextSwitch, 3, (long)configuration.sensitiveness);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(401 + id * nextSwitch, configuration.functionality);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(388 + id * nextSwitch, configuration.functionality);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(396 + id * nextSwitch, configuration.functionality);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(496 + id * nextSwitch, configuration.functionality);
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(
      416 + id,
      configuration.relayID); /* It's in EEPROM one after the other one */
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(
      440 + id,
      configuration.relayID); /* It's in EEPROM one after the other one */
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(
      461 + id,
      configuration.relayID); /* It's in EEPROM one after the other one */
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(497 + id * nextSwitch, configuration.relayID);
#endif
}

#if defined(T1_CONFIG)
void AFEDataAccess::saveConfiguration(DS18B20 configuration) {
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.write(371, 5, (float)configuration.correction);
  Eeprom.write(376, 5, (long)configuration.interval);
  Eeprom.writeUInt8(381, configuration.unit);
  Eeprom.write(446, configuration.sendOnlyChanges);
  Eeprom.write(936, 6, (long)configuration.idx);
}
#endif

#if defined(T2_CONFIG)
void AFEDataAccess::saveConfiguration(DH configuration) {
  Eeprom.writeUInt8(370, configuration.gpio);
  Eeprom.writeUInt8(371, configuration.type);
  Eeprom.write(372, 5, (long)configuration.temperature.interval);
  Eeprom.writeUInt8(377, configuration.temperature.unit);
  Eeprom.write(378, 4, (float)configuration.temperature.correction);

  Eeprom.write(382, 5, (long)configuration.humidity.interval);
  Eeprom.write(387, 3, (float)configuration.humidity.correction);
  Eeprom.write(936, 6, (long)configuration.temperatureIdx);
  Eeprom.write(942, 6, (long)configuration.humidityIdx);
  Eeprom.write(948, 6, (long)configuration.temperatureAndHumidityIdx);

  Eeprom.write(467, configuration.sendOnlyChanges);
  Eeprom.write(974, configuration.publishHeatIndex);
}
#endif

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
#if defined(T4_CONFIG)
  uint8_t nextRelay = 27;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  return Eeprom.read(371);
#elif defined(T1_CONFIG)
  return Eeprom.read(398);
#elif defined(T2_CONFIG)
  return Eeprom.read(406);
#elif defined(T4_CONFIG)
  return Eeprom.read(384 + id * nextRelay);
#endif
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {

#if defined(T4_CONFIG)
  uint8_t nextRelay = 27;
#endif

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.write(371, state);
#elif defined(T1_CONFIG)
  Eeprom.write(398, state);
#elif defined(T2_CONFIG)
  Eeprom.write(406, state);
#elif defined(T4_CONFIG)
  Eeprom.write(384 + id * nextRelay, state);
#endif
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

#ifndef T0_SHELLY_1_CONFIG
uint8_t AFEDataAccess::getSystemLedID() {
#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  return Eeprom.readUInt8(415);
#elif defined(T1_CONFIG)
  return Eeprom.readUInt8(439);
#elif defined(T2_CONFIG)
  return Eeprom.readUInt8(460);
#elif defined(T4_CONFIG)
  return Eeprom.readUInt8(530);
#endif
}

void AFEDataAccess::saveSystemLedID(uint8_t id) {
#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG)
  Eeprom.writeUInt8(415, id);
#elif defined(T1_CONFIG)
  Eeprom.writeUInt8(439, id);
#elif defined(T2_CONFIG)
  Eeprom.writeUInt8(460, id);
#elif defined(T4_CONFIG)
  Eeprom.writeUInt8(530, id);
#endif
}
#endif

/* @TODO ID is not needed */
#if defined(T1_CONFIG) || defined(T2_CONFIG)

boolean AFEDataAccess::isThermostatEnabled(uint8_t id) {
#if defined(T1_CONFIG)
  return Eeprom.read(435);
#else
  return Eeprom.read(443);
#endif
}

void AFEDataAccess::saveThermostatState(uint8_t id, boolean state) {
#if defined(T1_CONFIG)
  Eeprom.write(435, state);
#else
  Eeprom.write(443, state);
#endif
}

#if defined(T2_CONFIG)
boolean AFEDataAccess::isHumidistatEnabled(uint8_t id) {
  return Eeprom.read(456);
}

void AFEDataAccess::saveHumidistatState(uint8_t id, boolean state) {
  Eeprom.write(456, state);
}
#endif

#endif

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
