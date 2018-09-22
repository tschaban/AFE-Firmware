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
  CONTACTRON ContactronConfiguration;
  GATE GateConfiguration;
  DH DHTConfiguration;
  LED LEDConfiguration;

  sprintf(firmwareConfiguration.version, FIRMWARE_VERSION);
  firmwareConfiguration.type = FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  firmwareConfiguration.upgradeURL[0] = '\0';

  Data->saveConfiguration(firmwareConfiguration);

  sprintf(deviceConfiguration.name, "AFE-Device");
  deviceConfiguration.isLED[0] = true;

  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isLED); i++) {
    deviceConfiguration.isLED[i] = false;
  }

  deviceConfiguration.isSwitch[0] = true;
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isSwitch); i++) {
    deviceConfiguration.isSwitch[i] = false;
  }

  deviceConfiguration.isRelay[0] = true;

  deviceConfiguration.isContactron[0] = true;
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isContactron); i++) {
    deviceConfiguration.isContactron[i] = false;
  }

  deviceConfiguration.isDHT = false;
  deviceConfiguration.mqttAPI = false;
  deviceConfiguration.domoticzAPI = false;
  deviceConfiguration.httpAPI = true;

  Data->saveConfiguration(deviceConfiguration);

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
  //  Serial << endl << "networkConfiguration";

  MQTTConfiguration.host[0] = '\0';
  MQTTConfiguration.ip = IPAddress(0, 0, 0, 0);
  MQTTConfiguration.user[0] = '\0';
  MQTTConfiguration.password[0] = '\0';
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.topic, "/device/");
  Data->saveConfiguration(MQTTConfiguration);

  RelayConfiguration.timeToOff = 200;
  RelayConfiguration.gpio = 12;
  Data->saveConfiguration(0, RelayConfiguration);

  SwitchConfiguration.type = 0;
  SwitchConfiguration.sensitiveness = 50;

  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.functionality = 0;
  SwitchConfiguration.relayID = 1;
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isSwitch); i++) {
    Data->saveConfiguration(i, SwitchConfiguration);
  }

  LEDConfiguration.gpio = 16; // System
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(0, LEDConfiguration);
  LEDConfiguration.gpio = 14; // S1
  Data->saveConfiguration(1, LEDConfiguration);
  LEDConfiguration.gpio = 13; // S1
  Data->saveConfiguration(2, LEDConfiguration);

  ContactronConfiguration.outputDefaultState = CONTACTRON_NO;
  ContactronConfiguration.bouncing = 200;
  ContactronConfiguration.gpio = 4;
  ContactronConfiguration.ledID = 2;
  ContactronConfiguration.idx = 0;
  sprintf(ContactronConfiguration.name, "C1");
  Data->saveConfiguration(0, ContactronConfiguration);
  ContactronConfiguration.gpio = 5;
  ContactronConfiguration.ledID = 3;
  sprintf(ContactronConfiguration.name, "C2");
  Data->saveConfiguration(1, ContactronConfiguration);

  DHTConfiguration.gpio = 15;
  DHTConfiguration.type = 1;
  DHTConfiguration.sendOnlyChanges = true;
  DHTConfiguration.temperature.correction = 0;
  DHTConfiguration.temperature.interval = 60;
  DHTConfiguration.temperature.unit = 0;
  DHTConfiguration.sendOnlyChanges = true;
  DHTConfiguration.publishHeatIndex = false;
  DHTConfiguration.humidity.correction = 0;
  DHTConfiguration.humidity.interval = 60;
  DHTConfiguration.temperatureIdx = 0;
  DHTConfiguration.humidityIdx = 0;
  DHTConfiguration.temperatureAndHumidityIdx = 0;

  Data->saveConfiguration(DHTConfiguration);

  GateConfiguration.state[0] = GATE_OPEN;
  GateConfiguration.state[1] = GATE_PARTIALLY_OPEN;
  GateConfiguration.state[2] = GATE_PARTIALLY_OPEN;
  GateConfiguration.state[3] = GATE_CLOSED;
  GateConfiguration.idx = 0;

  Data->saveConfiguration(GateConfiguration);
  Data->saveSystemLedID(1);
  Data->saveDeviceMode(2);
  Data->saveGateState(0);
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

void AFEDefaults::addLEDConfiguration(uint8_t id, uint8_t gpio) {
  LED LEDConfiguration;
  LEDConfiguration.gpio = gpio;
  LEDConfiguration.changeToOppositeValue = false;
  Data->saveConfiguration(id, LEDConfiguration);
}

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
