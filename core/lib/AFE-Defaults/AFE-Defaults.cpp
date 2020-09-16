/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Defaults.h"

AFEDefaults::AFEDefaults() {}

void AFEDefaults::set() {

/* Turning devicve LED on */
#ifdef AFE_CONFIG_HARDWARE_LED
#ifdef DEBUG
  Serial << endl
         << F("Turning on system LED on GPIO ")
         << AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;
#endif
  pinMode(AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO, OUTPUT);
  digitalWrite(AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO, LOW);
#endif

  if (Data->formatFileSystem()) {
    Data->createDeviceUIDFile();
    Data->createDeviceConfigurationFile();
    Data->createFirmwareConfigurationFile();
    Data->createNetworkConfigurationFile();
    Data->createMQTTConfigurationFile();
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    Data->createDomoticzConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
    Data->createRelayConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
    Data->createSwitchConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
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

#ifdef AFE_CONFIG_HARDWARE_DHT
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

#ifdef AFE_CONFIG_HARDWARE_I2C
    Data->createI2CConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
    Data->createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
    Data->createBH1750SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
    Data->createAS3935SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
    Data->createAnemometerSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
    Data->createRainmeterSensorConfigurationFile();
    Data->createRainmeterSensorDataConfigurationFile();
#endif

/* DS18B20 presence */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
    Data->createDS18B20SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    Data->createRegulatorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
    Data->createThermalProtectionConfigurationFile();
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
    Serial << endl << F("ERROR: Formating failed");
  }
#endif
}
