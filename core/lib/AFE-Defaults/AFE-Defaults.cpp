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
    Data->createProVersionConfigurationFile();
    Data->createPasswordConfigurationFile();
    /* Setting device mode to Access Point */
    Data->saveDeviceMode(AFE_MODE_NETWORK_NOT_SET);

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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
    Data->createAnemometerSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
    Data->createRainmeterSensorConfigurationFile();
    Data->createRainmeterSensorDataConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
    Data->createDS18B20SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    Data->createRegulatorConfigurationFile();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    Data->createThermalProtectorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
    Data->createDHTSensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
    Data->createBinarySensorConfigurationFile();
#endif

  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: Formating failed");
  }
#endif
}
