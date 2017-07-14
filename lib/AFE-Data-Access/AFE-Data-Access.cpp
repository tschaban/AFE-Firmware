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
        configuration.durationBetweenConnectionAttempts = Eeprom.readUInt8(226);
        configuration.durationBetweenNextConnectionAttemptsSeries = Eeprom.readUInt8(227);

        return configuration;
}

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

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
        RELAY configuration;

        configuration.present = Eeprom.read(365);
        configuration.gpio = Eeprom.readUInt8(366);
        configuration.state = Eeprom.read(367);
        configuration.timeToOff = Eeprom.read(368, 5).toFloat();
        configuration.statePowerOn =  Eeprom.readUInt8(373);
        Eeprom.read(374,16).toCharArray(configuration.name,sizeof(configuration.name));
        configuration.statePowerOn = Eeprom.readUInt8(390);

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
