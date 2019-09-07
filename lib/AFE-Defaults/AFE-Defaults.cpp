/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

  if (Data->formatFileSystem()) {
    Data->createDeviceUIDFile();
    Data->createDeviceConfigurationFile();
    Data->createFirmwareConfigurationFile();
    Data->createNetworkConfigurationFile();
    Data->createMQTTConfigurationFile();
    Data->createDomoticzConfigurationFile();
    Data->createRelayConfigurationFile();
    Data->createSwitchConfigurationFile();

#if defined(AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                             \
    AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Data->createLEDConfigurationFile();
    Data->createSystemLedIDConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
    Data->createADCInputConfigurationFile();
#endif

    Data->createProVersionConfigurationFile();
    Data->createPasswordConfigurationFile();

    /* Setting device mode to Access Point */
    Data->saveDeviceMode(AFE_MODE_NETWORK_NOT_SET);

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    REGULATOR RegulatorConfiguration;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    DS18B20 SensorConfiguration;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
    DH SensorConfiguration;
#endif

#if defined(T3_CONFIG)
    PIR PIRConfiguration;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
    Data->createContractonConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    Data->createGateConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
    Data->createHPMA115S0SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
    Data->createSerialConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    Data->createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    Data->createBH1750SensorConfigurationFile();
#endif

/* DS18B20 presence */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
    deviceConfiguration.isDS18B20 = false;
#endif

/* DHxx presence */
#ifdef AFE_CONFIG_HARDWARE_DHXX
    deviceConfiguration.isDHT = false;
#endif

/* PIR */
#if defined(T3_CONFIG)
    for (uint8_t i = 1; i < sizeof(deviceConfiguration.isPIR); i++) {
      deviceConfiguration.isPIR[i] = false;
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
#if defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) &&                             \
    defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT)
    Data->saveConfiguration(RegulatorConfiguration, THERMOSTAT_REGULATOR);
#endif

/* Hunidistat confiuration */
#if defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) &&                             \
    defined(AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT)
    Data->saveConfiguration(RegulatorConfiguration, HUMIDISTAT_REGULATOR);
#endif

/* DS18B20 or DHTxx Sensor configuration */
#if defined(AFE_CONFIG_HARDWARE_DHXX) || defined(AFE_CONFIG_HARDWARE_DS18B20)
    SensorConfiguration.gpio = 14;
    SensorConfiguration.sendOnlyChanges = true;
    SensorConfiguration.interval = 60;
#ifdef AFE_CONFIG_HARDWARE_DS18B20
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
    PIRConfiguration.type = PIR_NO;
    PIRConfiguration.ledId = 0;
    for (uint8_t i = 0; i < sizeof(deviceConfiguration.isPIR); i++) {
      sprintf(PIRConfiguration.name, "pir%d", i + 1);
      PIRConfiguration.relayId = i + 1;

      Data->saveConfiguration(i, PIRConfiguration);
    }
#endif

  }
#ifdef DEBUG
  else {
    Serial << endl << "ERROR: Formating failed";
  }
#endif
}
