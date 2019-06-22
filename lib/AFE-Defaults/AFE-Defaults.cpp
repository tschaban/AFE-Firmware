/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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

#if defined(CONFIG_HARDWARE_NUMBER_OF_LEDS) &&                                 \
    CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
    Data->createLEDConfigurationFile();
    Data->createSystemLedIDConfigurationFile();
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
    Data->createADCInputConfigurationFile();
#endif

    Data->createProVersionConfigurationFile();
    Data->createPasswordConfigurationFile();

    /* Setting device mode to Access Point */
    Data->saveDeviceMode(MODE_NETWORK_NOT_SET);

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

#ifdef CONFIG_FUNCTIONALITY_GATE
    Data->createContractonConfigurationFile();
    Data->createGateConfigurationFile();
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

  }
#ifdef DEBUG
  else {
    Serial << endl << "ERROR: Formating failed";
  }
#endif
}
