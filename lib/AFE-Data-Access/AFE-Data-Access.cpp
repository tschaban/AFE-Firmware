#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

NETWORK AFEDataAccess::getNetworkConfiguration() {
  NETWORK configuration;

  Eeprom.read(148, 32).toCharArray(configuration.ssid,
                                   sizeof(configuration.ssid));
  Eeprom.read(180, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  Eeprom.read(9, 16).toCharArray(configuration.host,
                                 sizeof(configuration.host));
  configuration.isDHCP = Eeprom.read(212);
  configuration.ip = Eeprom.readIP(213);
  configuration.gateway = Eeprom.readIP(217);
  configuration.subnet = Eeprom.readIP(221);
  configuration.noConnectionAttempts = Eeprom.readUInt8(225);
  configuration.waitTimeConnections = Eeprom.readUInt8(226);
  configuration.waitTimeSeries = Eeprom.readUInt8(227);

  Serial << endl << "INFO: Requested : Network configuration";
  Serial << endl << "   - WiFi SSID : " << configuration.ssid;
  Serial << endl << "   - WiFi Password : " << configuration.password;
  Serial << endl << "   - Host name : " << configuration.host;
  Serial << endl << "   - LAN by DHCP? : " << configuration.isDHCP;
  Serial << endl << "   - IP : " << configuration.ip;
  Serial << endl << "   - Gateway : " << configuration.gateway;
  Serial << endl << "   - Subnet : " << configuration.subnet;
  Serial << endl
         << "   - Number of connection attempts : "
         << configuration.noConnectionAttempts;
  Serial << endl
         << "   - Duration between connecton attempts : "
         << configuration.waitTimeConnections;
  Serial << endl
         << "   - Duration between next connection attempts series : "
         << configuration.waitTimeSeries;

  return configuration;
}

/* @TODO Only for MQTT */
MQTT AFEDataAccess::getMQTTConfiguration() {
  MQTT configuration;

  Eeprom.read(228, 32).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.ip = Eeprom.readIP(260);
  configuration.port = Eeprom.read(264, 5).toInt();
  Eeprom.read(269, 32).toCharArray(configuration.user,
                                   sizeof(configuration.user));
  Eeprom.read(301, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  Eeprom.read(333, 32).toCharArray(configuration.topic,
                                   sizeof(configuration.topic));

  Serial << endl << "INFO: Requested : MQTT Broker";
  Serial << endl << "    - Host : " << configuration.host;
  Serial << endl << "    - IP : " << configuration.ip;
  Serial << endl << "    - Port : " << configuration.port;
  Serial << endl << "    - User : " << configuration.user;
  Serial << endl << "    - Password : " << configuration.password;
  Serial << endl << "    - Topic : " << configuration.topic;

  return configuration;
}

/* @TODO Only for Domoticz */
DOMOTICZ AFEDataAccess::getDomoticzConfiguration() {
  DOMOTICZ configuration;

  Eeprom.read(228, 32).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.ip = Eeprom.readIP(260);
  configuration.port = Eeprom.read(264, 5).toInt();
  Eeprom.read(269, 32).toCharArray(configuration.user,
                                   sizeof(configuration.user));
  Eeprom.read(301, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));

  Serial << endl << "INFO: Requested : Domoticz";
  Serial << endl << "    - Host : " << configuration.host;
  Serial << endl << "    - IP : " << configuration.ip;
  Serial << endl << "    - Port : " << configuration.port;
  Serial << endl << "    - User : " << configuration.user;
  Serial << endl << "    - Password : " << configuration.password;

  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 26;
  char mqttTopic[49];

  configuration.present = Eeprom.read(367 + id * nextRelay);
  configuration.gpio = Eeprom.readUInt8(368 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(370 + id * nextRelay, 5).toFloat();
  configuration.statePowerOn = Eeprom.readUInt8(375 + id * nextRelay);
  /* @TODO Only for MQTT */
  Eeprom.read(376 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(392 + id * nextRelay);

  Eeprom.read(333, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  /* @TODO Only for Domoticz */
  //  configuration.idx = Eeprom.read(376 + id * next, 5).toInt();
  //  configuration.publishToDomoticz = Eeprom.readUInt8(382 + id * next);

  Serial << endl << "INFO: Requested : Relay : ";
  Serial << endl << "    - Present : " << configuration.present;
  Serial << endl << "    - GPIO : " << configuration.gpio;
  Serial << endl << "    - Time to off : " << configuration.timeToOff;
  Serial << endl
         << "    - State after power on : " << configuration.statePowerOn;

  Serial << endl << "    - Relay name : " << configuration.name;
  Serial << endl
         << "    - Relay state after mqtt connected : "
         << configuration.stateMQTTConnected;
  Serial << endl << "    - Relay mqtt topic : " << configuration.mqttTopic;
  /* @TODO For Domoticz only
  Serial << endl << "    - Domoticz IDX : " << configuration.idx;
  Serial << endl
         << "    - Domoticz Publish state : "
         << configuration.publishToDomoticz;
  */

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 6;
  configuration.present = Eeprom.read(393 + id * nextSwitch);
  configuration.gpio = Eeprom.readUInt8(394 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(395 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(396 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(399 + id * nextSwitch);
  Serial << endl << "INFO: Requested : Switch : ";
  Serial << endl << "    - Present : " << configuration.present;
  Serial << endl << "    - GPIO : " << configuration.gpio;
  Serial << endl << "    - Type : " << configuration.type;
  Serial << endl << "    - Sensitiveness : " << configuration.sensitiveness;
  Serial << endl << "    - Functionality : " << configuration.functionality;
  return configuration;
}

DS18B20 AFEDataAccess::getDS18B20Configuration() {
  DS18B20 configuration;

  configuration.present = Eeprom.read(400);
  configuration.gpio = Eeprom.readUInt8(401);
  configuration.correction = Eeprom.read(402, 5).toFloat();
  configuration.interval = Eeprom.read(407, 5).toInt();
  configuration.unit = Eeprom.readUInt8(412);
  Serial << endl << "INFO: Requested : DS18B20 : ";
  Serial << endl << "    - Present : " << configuration.present;
  Serial << endl << "    - GPIO : " << configuration.gpio;
  Serial << endl << "    - Correction by : " << configuration.correction;
  Serial << endl << "    - Read interval : " << configuration.interval;
  Serial << endl << "    - Unit : " << configuration.unit;
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

  Serial << endl << "INFO: Requested : Firmware info";
  Serial << endl << " - Version : " << configuration.version;
  Serial << endl << " - Firmware type : " << configuration.type;
  Serial << endl << " - Auto update : " << configuration.autoUpgrade;
  Serial << endl << " - Update url : " << configuration.upgradeURL;

  return configuration;
}

LED AFEDataAccess::getLEDConfiguration() {
  LED configuration;
  configuration.present = Eeprom.read(365);
  configuration.gpio = Eeprom.readUInt8(366);

  Serial << endl << "INFO: Requested : LED : ";
  Serial << endl << "    - Present : " << configuration.present;
  Serial << endl << "    - gpio : " << configuration.gpio;

  return configuration;
}

void AFEDataAccess::saveConfiguration(NETWORK configuration) {
  Eeprom.write(148, 32, configuration.ssid);
  Eeprom.write(180, 32, configuration.password);
  Eeprom.write(9, 16, configuration.host);
  Eeprom.write(212, configuration.isDHCP);
  Eeprom.writeIP(213, configuration.ip);
  Eeprom.writeIP(217, configuration.gateway);
  Eeprom.writeIP(221, configuration.subnet);
  Eeprom.writeUInt8(225, configuration.noConnectionAttempts);
  Eeprom.writeUInt8(226, configuration.waitTimeConnections);
  Eeprom.writeUInt8(227, configuration.waitTimeSeries);
}

void AFEDataAccess::saveConfiguration(MQTT configuration) {
  Eeprom.write(228, 32, configuration.host);
  Eeprom.writeIP(260, configuration.ip);
  Eeprom.write(264, 5, (long)configuration.port);
  Eeprom.write(269, 32, configuration.user);
  Eeprom.write(301, 32, configuration.password);
  Eeprom.write(333, 32, configuration.topic);
}

void AFEDataAccess::saveConfiguration(DOMOTICZ configuration) {
  Eeprom.write(228, 32, configuration.host);
  Eeprom.writeIP(260, configuration.ip);
  Eeprom.write(264, 5, (long)configuration.port);
  Eeprom.write(269, 32, configuration.user);
  Eeprom.write(301, 32, configuration.password);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
  uint8_t nextRelay = 26;
  Eeprom.write(367 + id * nextRelay, configuration.present);
  Eeprom.writeUInt8(368 + id * nextRelay, configuration.gpio);
  Eeprom.write(370 + id * nextRelay, 5, configuration.timeToOff);
  Eeprom.writeUInt8(375 + id * nextRelay, configuration.statePowerOn);
  /* @TODO For MQTT only */
  Eeprom.write(376 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(392 + id * nextRelay, configuration.stateMQTTConnected);
  /* @TODO For Domoticz only */
  //  Eeprom.write(376 + id * nextRelay, 5, (long)configuration.idx);
  //  Eeprom.writeUInt8(382 + id * nextRelay, configuration.publishToDomoticz);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 6;
  Eeprom.write(393 + id * nextSwitch, configuration.present);
  Eeprom.writeUInt8(394 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(395 + id * nextSwitch, configuration.type);
  Eeprom.write(396 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(399 + id * nextSwitch, configuration.functionality);
}

void AFEDataAccess::saveConfiguration(DS18B20 configuration) {
  Eeprom.write(400, configuration.present);
  Eeprom.writeUInt8(401, configuration.gpio);
  Eeprom.write(402, 5, (float)configuration.correction);
  Eeprom.write(407, 5, (long)configuration.interval);
  Eeprom.writeUInt8(412, configuration.unit);
}

void AFEDataAccess::saveConfiguration(FIRMWARE configuration) {
  Eeprom.write(0, 7, configuration.version);
  Eeprom.writeUInt8(7, configuration.type);
  Eeprom.writeUInt8(27, configuration.autoUpgrade);
  Eeprom.write(28, 120, configuration.upgradeURL);
}

void AFEDataAccess::saveConfiguration(LED configuration) {
  Eeprom.write(365, configuration.present);
  Eeprom.writeUInt8(366, configuration.gpio);
}

const char AFEDataAccess::getVersion() {
  char version[7];
  sprintf(version, "1.0.0");
  return *version;
}

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 26;
  return Eeprom.read(369 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 26;
  Eeprom.write(369 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

boolean AFEDataAccess::isDebuggable() {
  return true; // Eeprom.read(25); @TODO enable debugger configuration
}

void AFEDataAccess::setDebuggable(boolean state) { Eeprom.write(25, state); }
