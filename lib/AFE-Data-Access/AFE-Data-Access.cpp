#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {
}

NETWORK AFEDataAccess::getNetworkConfiguration() {
        NETWORK configuration;

        Eeprom.read(148,32).toCharArray(configuration.ssid,sizeof(configuration.ssid));
        Eeprom.read(180,32).toCharArray(configuration.password,sizeof(configuration.password));
        Eeprom.read(9,16).toCharArray(configuration.host,sizeof(configuration.host));
        configuration.dhcp = Eeprom.read(212);
        configuration.ip = Eeprom.readIP(213);
        configuration.gateway = Eeprom.readIP(217);
        configuration.subnet = Eeprom.readIP(221);
        configuration.noConnectionAttempts = Eeprom.readUInt8(225);
        configuration.waitTimeConnections = Eeprom.readUInt8(226);
        configuration.waitTimeSeries = Eeprom.readUInt8(227);

        return configuration;
}

/* @TODO Only for MQTT */
MQTT AFEDataAccess::getMQTTConfiguration() {
        MQTT configuration;

        Eeprom.read(228,32).toCharArray(configuration.host,sizeof(configuration.host));
        configuration.ip = Eeprom.readIP(260);
        configuration.port  = Eeprom.read(264, 5).toInt();
        Eeprom.read(269,32).toCharArray(configuration.user,sizeof(configuration.user));
        Eeprom.read(301,32).toCharArray(configuration.password,sizeof(configuration.password));
        Eeprom.read(333,32).toCharArray(configuration.topic,sizeof(configuration.topic));

        return configuration;
}

/* @TODO Only for Domoticz */
DOMOTICZ AFEDataAccess::getDomoticzConfiguration() {
        DOMOTICZ configuration;

        Eeprom.read(228,32).toCharArray(configuration.host,sizeof(configuration.host));
        configuration.ip = Eeprom.readIP(260);
        configuration.port  = Eeprom.read(264, 5).toInt();
        Eeprom.read(269,32).toCharArray(configuration.user,sizeof(configuration.user));
        Eeprom.read(301,32).toCharArray(configuration.password,sizeof(configuration.password));

        return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
        RELAY configuration;

        configuration.present = Eeprom.read(365);
        configuration.gpio = Eeprom.readUInt8(366);
        configuration.timeToOff = Eeprom.read(368, 5).toFloat();
        configuration.statePowerOn =  Eeprom.readUInt8(373);
        /* @TODO Only for MQTT */
        Eeprom.read(374,16).toCharArray(configuration.name,sizeof(configuration.name));
        configuration.stateMQTTConnected = Eeprom.readUInt8(390);
        /* @TODO Only for Domoticz */
        configuration.idx = Eeprom.read(374,4).toInt();
        configuration.publishToDomoticz = Eeprom.readUInt8(378);

        return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
        SWITCH configuration;

        configuration.present = Eeprom.read(391);
        configuration.gpio = Eeprom.readUInt8(392);
        configuration.type = Eeprom.readUInt8(393);
        configuration.sensitiveness = Eeprom.readUInt8(394);
        configuration.functionality =  Eeprom.readUInt8(395);

        return configuration;
}

DS18B20 AFEDataAccess::getDS18B20Configuration() {
        DS18B20 configuration;

        configuration.present = Eeprom.read(396);
        configuration.gpio = Eeprom.readUInt8(397);
        configuration.correction = Eeprom.read(398, 5).toFloat();
        configuration.interval = Eeprom.read(403, 5).toInt();
        configuration.unit = Eeprom.readUInt8(408);

        return configuration;
}

FIRMWARE AFEDataAccess::getFirmwareConfiguration() {
        FIRMWARE configuration;

        Eeprom.read(0,7).toCharArray(configuration.version,sizeof(configuration.version));
        Eeprom.read(28,120).toCharArray(configuration.upgradeURL,sizeof(configuration.upgradeURL));
        configuration.type = Eeprom.readUInt8(7);
        configuration.autoUpgrade = Eeprom.readUInt8(27);

        return configuration;
}

void AFEDataAccess::saveConfiguration(NETWORK configuration) {
        Eeprom.write(148,32,configuration.ssid);
        Eeprom.write(180,32,configuration.password);
        Eeprom.write(9,16,configuration.host);
        Eeprom.write(212,configuration.dhcp);
        Eeprom.writeIP(213,configuration.ip);
        Eeprom.writeIP(217,configuration.gateway);
        Eeprom.writeIP(221,configuration.subnet);
        Eeprom.writeUInt8(225,configuration.noConnectionAttempts);
        Eeprom.writeUInt8(226,configuration.waitTimeConnections);
        Eeprom.writeUInt8(227,configuration.waitTimeSeries);
}

void AFEDataAccess::saveConfiguration(MQTT configuration) {
        Eeprom.write(228,32,configuration.host);
        Eeprom.writeIP(260,configuration.ip);
        Eeprom.write(264,5,(long)configuration.port);
        Eeprom.write(269,32,configuration.user);
        Eeprom.write(301,32,configuration.password);
        Eeprom.write(333,32,configuration.topic);
}

void AFEDataAccess::saveConfiguration(DOMOTICZ configuration) {
        Eeprom.write(228,32,configuration.host);
        Eeprom.writeIP(260,configuration.ip);
        Eeprom.write(264,5,(long)configuration.port);
        Eeprom.write(269,32,configuration.user);
        Eeprom.write(301,32,configuration.password);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
        Eeprom.write(365,configuration.present);
        Eeprom.writeUInt8(366,configuration.gpio);
        Eeprom.write(368,5,configuration.timeToOff);
        Eeprom.writeUInt8(373,configuration.statePowerOn);
        /* @TODO For MQTT only */
        Eeprom.write(374,16,configuration.name);
        Eeprom.writeUInt8(390,configuration.stateMQTTConnected);
        /* @TODO For Domoticz only */
        Eeprom.write(374,4,(long)configuration.idx);
        Eeprom.writeUInt8(378,configuration.publishToDomoticz);

}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
        Eeprom.write(391,configuration.present);
        Eeprom.writeUInt8(392,configuration.gpio);
        Eeprom.writeUInt8(393,configuration.type);
        Eeprom.writeUInt8(394,configuration.sensitiveness);
        Eeprom.writeUInt8(395,configuration.functionality);
}

void AFEDataAccess::saveConfiguration(DS18B20 configuration) {
        Eeprom.write(396,configuration.present);
        Eeprom.writeUInt8(397,configuration.gpio);
        Eeprom.write(398,5,(float)configuration.correction);
        Eeprom.write(403,5,(long)configuration.interval);
        Eeprom.writeUInt8(408,configuration.unit);
}

void AFEDataAccess::saveConfiguration(FIRMWARE configuration) {
        Eeprom.write(0,7,configuration.version);
        Eeprom.writeUInt8(7,configuration.type);
        Eeprom.writeUInt8(27,configuration.autoUpgrade);
        Eeprom.write(28,120,configuration.upgradeURL);
}

const char AFEDataAccess::getVersion() {
        char version[7];
        sprintf(version,"1.0.0");
        return *version;
}

boolean AFEDataAccess::getRelayState(uint8_t id) {
        return Eeprom.read(367);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
        Eeprom.write(367,state);
}

uint8_t AFEDataAccess::getDeviceMode() {
        return Eeprom.readUInt8(26);
}

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
        Eeprom.writeUInt8(26,mode);
}

uint8_t AFEDataAccess::getLanguage() {
        return Eeprom.readUInt8(8);
}

void AFEDataAccess::saveLanguage(uint8_t language) {
        Eeprom.writeUInt8(8,language);
}

boolean AFEDataAccess::isDebuggable() {
        return Eeprom.read(25);
}

void AFEDataAccess::setDebuggable(boolean state) {
        Eeprom.write(25,state);
}
