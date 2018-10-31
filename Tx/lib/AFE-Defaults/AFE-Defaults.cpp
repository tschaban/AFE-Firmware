/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

  DEVICE deviceConfiguration;
  FIRMWARE firmwareConfiguration;
  NETWORK networkConfiguration;
  MQTT MQTTConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  REGULATOR RegulatorConfiguration;
#endif

#if defined(T1_CONFIG)
  DS18B20 SensorConfiguration;
#endif

#if defined(T2_CONFIG)
  DH SensorConfiguration;
#endif

  sprintf(firmwareConfiguration.version, FIRMWARE_VERSION);
  firmwareConfiguration.type = FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  firmwareConfiguration.upgradeURL[0] = '\0';

  Data->saveConfiguration(firmwareConfiguration);

  sprintf(deviceConfiguration.name, "AFE-Device");

  /* APIs */
  deviceConfiguration.mqttAPI = false;
  deviceConfiguration.domoticzAPI = false;
  deviceConfiguration.httpAPI = true;

  /* Relay presence */
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    deviceConfiguration.isRelay[i] = false;
  }

  /* Switch presence */
  deviceConfiguration.isSwitch[0] = true;
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isSwitch); i++) {
    deviceConfiguration.isSwitch[i] = false;
  }

#ifndef T0_SHELLY_1_CONFIG
  /* LED */
  deviceConfiguration.isLED[0] = true;
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isLED); i++) {
    deviceConfiguration.isLED[i] = false;
  }
#endif

#ifdef T1_CONFIG
  deviceConfiguration.isDS18B20 = false;
#endif

#ifdef T2_CONFIG
  deviceConfiguration.isDHT = false;
#endif

  Data->saveConfiguration(deviceConfiguration);

  /* Network default config */
  networkConfiguration.ssid[0] = '\0';
  networkConfiguration.password[0] = '\0';
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip = IPAddress(0, 0, 0, 0);
  networkConfiguration.gateway = IPAddress(0, 0, 0, 0);
  networkConfiguration.subnet = IPAddress(255, 255, 255, 0);
  networkConfiguration.noConnectionAttempts = 10;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 60;
  Data->saveConfiguration(networkConfiguration);

  /* MQTT Default config */
  MQTTConfiguration.host[0] = '\0';
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  MQTTConfiguration.user[0] = '\0';
  MQTTConfiguration.password[0] = '\0';
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");
  Data->saveConfiguration(MQTTConfiguration);

  /* Domoticz config */
  addDomoticzConfiguration();

/* Relay config */
#ifdef T0_SHELLY_1_CONFIG
  RelayConfiguration.gpio = 4;
#else
  RelayConfiguration.gpio = 12;
#endif

  RelayConfiguration.timeToOff = 0;
  RelayConfiguration.statePowerOn = 3;
  RelayConfiguration.stateMQTTConnected = 0;

#if defined(T0_CONFIG) || defined(T0_SHELLY_1_CONFIG) || defined(T1_CONFIG) || \
    defined(T2_CONFIG)
  sprintf(RelayConfiguration.name, "switch");
#elif defined(T4_CONFIG)
  sprintf(RelayConfiguration.name, "switch1");
#endif

  RelayConfiguration.ledID = 0;
  RelayConfiguration.idx = 0;

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  RelayConfiguration.thermalProtection = 0;
#endif

  Data->saveConfiguration(0, RelayConfiguration);

#if defined(T4_CONFIG)
  RelayConfiguration.gpio = 5;
  sprintf(RelayConfiguration.name, "switch2");
  Data->saveConfiguration(1, RelayConfiguration);

  RelayConfiguration.gpio = 4;
  sprintf(RelayConfiguration.name, "switch3");
  Data->saveConfiguration(2, RelayConfiguration);

  RelayConfiguration.gpio = 15;
  sprintf(RelayConfiguration.name, "switch4");
  Data->saveConfiguration(3, RelayConfiguration);
#endif

  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isRelay); i++) {
    Data->saveRelayState(i, false);
  }

/* Regulator config */
#if defined(T1_CONFIG) || defined(T2_CONFIG)
  RegulatorConfiguration.enabled = false;
  RegulatorConfiguration.turnOn = 0;
  RegulatorConfiguration.turnOnAbove = false;
  RegulatorConfiguration.turnOff = 0;
  RegulatorConfiguration.turnOffAbove = true;
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  Data->saveConfiguration(RegulatorConfiguration, THERMOSTAT_REGULATOR);
#endif

#if defined(T2_CONFIG)
  Data->saveConfiguration(RegulatorConfiguration, HUMIDISTAT_REGULATOR);
#endif

/* Switch config */
#ifdef T0_SHELLY_1_CONFIG
  SwitchConfiguration.gpio = 5;
  SwitchConfiguration.type = 1;
#else
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.type = 0;
#endif

  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.functionality = 0;
  SwitchConfiguration.relayID = 1;
  Data->saveConfiguration(0, SwitchConfiguration);

#if defined(T0_CONFIG) || defined(T1_CONFIG) || defined(T2_CONFIG)
  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.type = 1;
#elif defined(T4_CONFIG)
  SwitchConfiguration.gpio = 9;
  SwitchConfiguration.relayID = 2;
#endif

#if defined(T0_CONFIG) || defined(T2_CONFIG) || defined(T4_CONFIG)
  SwitchConfiguration.functionality = 1;
  Data->saveConfiguration(1, SwitchConfiguration);
#endif

#if defined(T4_CONFIG)
  SwitchConfiguration.gpio = 10;
  SwitchConfiguration.relayID = 3;
  Data->saveConfiguration(2, SwitchConfiguration);
#endif

#if defined(T4_CONFIG)
  SwitchConfiguration.gpio = 14;
  SwitchConfiguration.relayID = 4;
  Data->saveConfiguration(3, SwitchConfiguration);
#endif

#ifndef T0_SHELLY_1_CONFIG
  addLEDConfiguration(0, 13);
  addLEDConfiguration(1, 3);
#endif

  addDeviceID();

/* DS18B20 or DHTxx Sensor configuration */
#if defined(T1_CONFIG) || defined(T2_CONFIG)
  SensorConfiguration.gpio = 14;
  SensorConfiguration.sendOnlyChanges = true;
#if defined(T1_CONFIG)
  SensorConfiguration.correction = 0;
  SensorConfiguration.interval = 60;
  SensorConfiguration.unit = 0;
  SensorConfiguration.idx = 0;
#elif defined(T2_CONFIG)
  SensorConfiguration.type = 1;
  SensorConfiguration.temperature.correction = 0;
  SensorConfiguration.temperature.interval = 60;
  SensorConfiguration.temperature.unit = 0;
  SensorConfiguration.publishHeatIndex = false;
  SensorConfiguration.humidity.correction = 0;
  SensorConfiguration.humidity.interval = 60;
  SensorConfiguration.temperatureIdx = 0;
  SensorConfiguration.humidityIdx = 0;
  SensorConfiguration.temperatureAndHumidityIdx = 0;
#endif

  Data->saveConfiguration(SensorConfiguration);
#endif

#ifndef T0_SHELLY_1_CONFIG
  Data->saveSystemLedID(1);
#endif

  Data->saveDeviceMode(2);
  Data->saveLanguage(1);
}

void AFEDefaults::addDomoticzConfiguration() {
  DOMOTICZ DomoticzConfiguration;
  DomoticzConfiguration.protocol = 0;
  DomoticzConfiguration.host[0] = '\0';
  DomoticzConfiguration.user[0] = '\0';
  DomoticzConfiguration.password[0] = '\0';
  DomoticzConfiguration.port = 8080;
  Data->saveConfiguration(DomoticzConfiguration);
}

#ifndef T0_SHELLY_1_CONFIG
void AFEDefaults::addLEDConfiguration(uint8_t id, uint8_t gpio) {
  LED LEDConfiguration;
  LEDConfiguration.gpio = gpio;
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(id, LEDConfiguration);
}
#endif

void AFEDefaults::addDeviceID() {
  char id[8];
  uint8_t range;
  for (uint8_t i = 0; i < sizeof(id); i++) {
    range = random(3);
    id[i] = char(range == 0 ? random(48, 57)
                            : range == 1 ? random(65, 90) : random(97, 122));
  }
  Data->saveDeviceID(String(id));
}
void AFEDefaults::eraseConfiguration() { Eeprom.erase(); }
