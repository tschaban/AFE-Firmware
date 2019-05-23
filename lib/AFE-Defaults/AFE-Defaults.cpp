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
  DOMOTICZ DomoticzConfiguration;
  RELAY RelayConfiguration;
  SWITCH SwitchConfiguration;
  PASSWORD PasswordConfiguration;
  PRO_VERSION ProConfiguration;

#if defined(CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                                 \
    CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  LED LEDConfiguration;
#endif

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
  REGULATOR RegulatorConfiguration;
#endif

#ifdef CONFIG_HARDWARE_DS18B20
  DS18B20 SensorConfiguration;
#endif

#ifdef CONFIG_HARDWARE_DHXX
  DH SensorConfiguration;
#endif

#if defined(T3_CONFIG)
  PIR PIRConfiguration;
#endif

#if defined(T5_CONFIG)
  CONTACTRON ContactronConfiguration;
  GATE GateConfiguration;
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  HPMA115S0 SensorHPMA115S0Configuration;
#endif

#ifdef CONFIG_HARDWARE_UART
  SERIALPORT SerialPortConfiguration;
#endif

#ifdef CONFIG_HARDWARE_BMX80
  BMx80 SensorBMx80Configuration;
#endif

#ifdef CONFIG_HARDWARE_BH1750
  BH1750 SensorBH1750Configuration;
#endif

  if (SPIFFS.format()) {
#ifdef DEBUG
    Serial << endl << "- File system formated";
  } else {
    Serial << endl << "- File system NOT formated";
#endif
  }

  /* Setting device mode to Access Point */
  Data->saveDeviceMode(MODE_NETWORK_NOT_SET);

  sprintf(firmwareConfiguration.version, FIRMWARE_VERSION);
  firmwareConfiguration.type = FIRMWARE_TYPE;
  firmwareConfiguration.autoUpgrade = 0;
  firmwareConfiguration.upgradeURL[0] = '\0';

  Data->saveConfiguration(firmwareConfiguration);

  /* Setting password */
  PasswordConfiguration.protect = false;
  PasswordConfiguration.password[0] = '\0';
  Data->saveConfiguration(PasswordConfiguration);

  ProConfiguration.serial[0] = '\0';
  ProConfiguration.valid = false;
  Data->saveConfiguration(ProConfiguration);

  /* Device UID */
  byte m[6];
  char uid[18];
  WiFi.macAddress(m);
  sprintf(uid, "%X%x%X%x-%X%x%X%x", m[0], m[5], m[1], m[4], m[2], m[3], m[3],
          m[2]);
  Data->saveDeviceUID(uid);

  sprintf(deviceConfiguration.name, "AFE-Device");

  /* APIs */
  deviceConfiguration.api.mqtt = false;
  deviceConfiguration.api.domoticz = false;
  deviceConfiguration.api.http = true;

/* Relay presence */
#ifdef CONFIG_FUNCTIONALITY_RELAY
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
    deviceConfiguration.isRelay[i] = false;
  }
#endif

  /* Switch presence */
  deviceConfiguration.isSwitch[0] = true;
  for (uint8_t i = 1; i < CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++) {
    deviceConfiguration.isSwitch[i] = false;
  }

/* LEDs presence */
#if defined(CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                                 \
    CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  deviceConfiguration.isLED[0] = true;
  for (uint8_t i = 1; i < CONFIG_HARDWARE_NUMBER_OF_LEDS; i++) {
    deviceConfiguration.isLED[i] = false;
  }
#endif

/* DS18B20 presence */
#ifdef CONFIG_HARDWARE_DS18B20
  deviceConfiguration.isDS18B20 = false;
#endif

/* DHxx presence */
#ifdef CONFIG_HARDWARE_DHXX
  deviceConfiguration.isDHT = false;
#endif

/* PIR */
#if defined(T3_CONFIG)
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isPIR); i++) {
    deviceConfiguration.isPIR[i] = false;
  }
#endif

/* Contactron */
#if defined(T5_CONFIG)
  for (uint8_t i = 1; i < sizeof(deviceConfiguration.isContactron); i++) {
    deviceConfiguration.isContactron[i] = false;
  }
#endif

/* HPMA115S0 Sesnor */
#ifdef CONFIG_HARDWARE_HPMA115S0
  deviceConfiguration.isHPMA115S0 = false;
#endif

#ifdef CONFIG_HARDWARE_BMX80
  deviceConfiguration.isBMx80 = false;
#endif

#ifdef CONFIG_HARDWARE_BH1750
  deviceConfiguration.isBH1750 = false;
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  deviceConfiguration.isAnalogInput = false;
#endif

  Data->saveConfiguration(deviceConfiguration);

  /* Network default config */
  networkConfiguration.ssid[0] = '\0';
  networkConfiguration.password[0] = '\0';
  networkConfiguration.isDHCP = true;
  networkConfiguration.ip[0] = '\0';
  networkConfiguration.gateway[0] = '\0';
  networkConfiguration.subnet[0] = '\0';
  networkConfiguration.noConnectionAttempts = 30;
  networkConfiguration.waitTimeConnections = 1;
  networkConfiguration.waitTimeSeries = 20;
  Data->saveConfiguration(networkConfiguration);

  /* MQTT Default config */
  MQTTConfiguration.host[0] = '\0';
  MQTTConfiguration.ip[0] = '\0';
  MQTTConfiguration.user[0] = '\0';
  MQTTConfiguration.password[0] = '\0';
  MQTTConfiguration.port = 1883;
  sprintf(MQTTConfiguration.mqtt.topic, "device");
  Data->saveConfiguration(MQTTConfiguration);

  /* Domoticz config */

  DomoticzConfiguration.protocol = 0;
  DomoticzConfiguration.host[0] = '\0';
  DomoticzConfiguration.user[0] = '\0';
  DomoticzConfiguration.password[0] = '\0';
  DomoticzConfiguration.port = 8080;
  Data->saveConfiguration(DomoticzConfiguration);

/* Relay config */
#ifdef T0_SHELLY_1_CONFIG
  RelayConfiguration.gpio = 4;
#else
  RelayConfiguration.gpio = 12;
#endif

#ifdef CONFIG_FUNCTIONALITY_GATE
  RelayConfiguration.timeToOff = 200;
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  RelayConfiguration.timeToOff = 0;
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY

  RelayConfiguration.state.powerOn = 3;
  RelayConfiguration.state.MQTTConnected = 0;
  RelayConfiguration.ledID = 0;
  RelayConfiguration.domoticz.idx = 0;

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  RelayConfiguration.thermalProtection = 0;
#endif

  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_RELAYS; i++) {
    sprintf(RelayConfiguration.name, "relay%d", i + 1);
    sprintf(RelayConfiguration.mqtt.topic, "relay/%d", i + 1);
    Data->saveRelayState(i, false);
    Data->saveConfiguration(i, RelayConfiguration);
  }

#endif

/* Regulator config */
#if defined(T1_CONFIG) || defined(T2_CONFIG)
  RegulatorConfiguration.enabled = false;
  RegulatorConfiguration.turnOn = 0;
  RegulatorConfiguration.turnOnAbove = false;
  RegulatorConfiguration.turnOff = 0;
  RegulatorConfiguration.turnOffAbove = true;
#endif

/* Thermostat configuration */
#if defined(CONFIG_FUNCTIONALITY_REGULATOR) &&                                 \
    defined(CONFIG_FUNCTIONALITY_THERMOSTAT)
  Data->saveConfiguration(RegulatorConfiguration, THERMOSTAT_REGULATOR);
#endif

/* Hunidistat confiuration */
#if defined(CONFIG_FUNCTIONALITY_REGULATOR) &&                                 \
    defined(CONFIG_FUNCTIONALITY_HUMIDISTAT)
  Data->saveConfiguration(RegulatorConfiguration, HUMIDISTAT_REGULATOR);
#endif

  /* Switch config */
  SwitchConfiguration.type = SWITCH_TYPE_MONO;
  SwitchConfiguration.functionality = SWITCH_FUNCTIONALITY_MULTI;
  SwitchConfiguration.sensitiveness = 50;
  SwitchConfiguration.domoticz.idx = 0;
  SwitchConfiguration.mqtt.topic[0] = '\0';

#ifdef T0_SHELLY_1_CONFIG
  SwitchConfiguration.gpio = 5;
  SwitchConfiguration.relayID = 1;
#else
  SwitchConfiguration.gpio = 0;
  SwitchConfiguration.relayID = 0;
#endif

  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_SWITCHES; i++) {
    Data->saveConfiguration(i, SwitchConfiguration);
  }

#if defined(CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                                 \
    CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  LEDConfiguration.gpio = 13;
  LEDConfiguration.changeToOppositeValue = false;
  for (uint8_t i = 0; i < CONFIG_HARDWARE_NUMBER_OF_LEDS; i++) {
    Data->saveConfiguration(i, LEDConfiguration);
  }
#endif

/* DS18B20 or DHTxx Sensor configuration */
#if defined(CONFIG_HARDWARE_DHXX) || defined(CONFIG_HARDWARE_DS18B20)
  SensorConfiguration.gpio = 14;
  SensorConfiguration.sendOnlyChanges = true;
  SensorConfiguration.interval = 60;
#ifdef CONFIG_HARDWARE_DS18B20
  SensorConfiguration.correction = 0;
  SensorConfiguration.unit = 0;
  SensorConfiguration.idx = 0;
#else
  SensorConfiguration.gpio = 2;
  SensorConfiguration.type = 1;
  SensorConfiguration.temperature.correction = 0;
  SensorConfiguration.humidity.correction = 0;
  SensorConfiguration.temperature.unit = 0;
  SensorConfiguration.publishHeatIndex = false;
  SensorConfiguration.publishDewPoint = false;
  SensorConfiguration.temperatureIdx = 0;
  SensorConfiguration.humidityIdx = 0;
  SensorConfiguration.temperatureAndHumidityIdx = 0;
#endif

  Data->saveConfiguration(SensorConfiguration);
#endif

#if defined(T3_CONFIG)
  PIRConfiguration.gpio = 4;
  PIRConfiguration.howLongKeepRelayOn = 10;
  PIRConfiguration.invertRelayState = false;
  PIRConfiguration.bouncing = 0;
  PIRConfiguration.idx = 0;
  PIRConfiguration.outputDefaultState = PIR_NO;
  PIRConfiguration.ledId = 0;
  for (uint8_t i = 0; i < sizeof(deviceConfiguration.isPIR); i++) {
    sprintf(PIRConfiguration.name, "pir%d", i + 1);
    PIRConfiguration.relayId = i + 1;

    Data->saveConfiguration(i, PIRConfiguration);
  }
#endif

/* T5: Contactron and Gate configuration */
#if defined(T5_CONFIG)
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

  GateConfiguration.state[0] = GATE_OPEN;
  GateConfiguration.state[1] = GATE_PARTIALLY_OPEN;
  GateConfiguration.state[2] = GATE_PARTIALLY_OPEN;
  GateConfiguration.state[3] = GATE_CLOSED;
  GateConfiguration.idx = 0;

  Data->saveConfiguration(GateConfiguration);
  Data->saveGateState(0);

#endif

/* T6 HPMA115S0 Sensor */
#ifdef CONFIG_HARDWARE_HPMA115S0
  SensorHPMA115S0Configuration.interval = 60;
  SensorHPMA115S0Configuration.timeToMeasure = 0;
  SensorHPMA115S0Configuration.idx.pm10 = 0;
  SensorHPMA115S0Configuration.idx.pm25 = 0;
  Data->saveConfiguration(SensorHPMA115S0Configuration);
#endif

#ifdef CONFIG_HARDWARE_UART
  SerialPortConfiguration.RXD = 12;
  SerialPortConfiguration.TXD = 14;
  Data->saveConfiguration(SerialPortConfiguration);
#endif

#ifdef CONFIG_HARDWARE_BMX80
  SensorBMx80Configuration.interval = 60;
  SensorBMx80Configuration.i2cAddress = 0;
  SensorBMx80Configuration.idx.temperatureHumidityPressure = 0;
  SensorBMx80Configuration.idx.gasResistance = 0;
  SensorBMx80Configuration.idx.temperature = 0;
  SensorBMx80Configuration.idx.humidity = 0;
  SensorBMx80Configuration.idx.pressure = 0;
  Data->saveConfiguration(SensorBMx80Configuration);
#endif

#ifdef CONFIG_HARDWARE_BH1750
  SensorBH1750Configuration.interval = 60;
  SensorBH1750Configuration.i2cAddress = 0;
  SensorBH1750Configuration.idx = 0;
  SensorBH1750Configuration.mode = 0;
  Data->saveConfiguration(SensorBH1750Configuration);
#endif

#if defined(CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                                 \
    CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  Data->saveSystemLedID(1);
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  ADCINPUT AnalogInputConfiguration;
  AnalogInputConfiguration.gpio = 17;
  AnalogInputConfiguration.interval = 60;
  AnalogInputConfiguration.numberOfSamples = 1;
  AnalogInputConfiguration.maxVCC = 1;
  sprintf(AnalogInputConfiguration.mqtt.topic, "analog");
  AnalogInputConfiguration.domoticz.raw = 0;
  AnalogInputConfiguration.domoticz.voltage = 0;
  AnalogInputConfiguration.domoticz.percent = 0;
  AnalogInputConfiguration.domoticz.voltageCalculated = 0;
  AnalogInputConfiguration.divider.Ra = 0;
  AnalogInputConfiguration.divider.Ra = 0;
  Data->saveConfiguration(AnalogInputConfiguration);
#endif
}
