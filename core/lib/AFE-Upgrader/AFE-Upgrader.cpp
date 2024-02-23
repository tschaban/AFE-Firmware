/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Upgrader.h"

AFEUpgrader::AFEUpgrader(AFEFirmware *_Firmware) { Firmware = _Firmware; }

void AFEUpgrader::upgraded()
{
#ifdef DEBUG

  Firmware->Debugger->printInformation(F("Current firmware version: T"), F("UPGRADE"));
  Serial << Firmware->Configuration->Version->type << F("-")
         << Firmware->Configuration->Version->installed_version << F("-");

  switch (Firmware->Configuration->Version->api)
  {
  case AFE_FIRMWARE_API_STANDARD:

    Firmware->Debugger->printValue(F("Standard"));
    break;
  case AFE_FIRMWARE_API_DOMOTICZ:
    Firmware->Debugger->printValue(F("Domoticz"));
    break;
  case AFE_FIRMWARE_API_HOME_ASSISTANT:
    Firmware->Debugger->printValue(F("HomeAssistant"));
    break;
  default:
    Firmware->Debugger->printValue(F("Unknwon"));
    break;
  }

#endif

  if (strcmp(Firmware->Configuration->Version->installed_version,
             AFE_FIRMWARE_VERSION) == 0 &&
      Firmware->Configuration->Version->type == AFE_FIRMWARE_TYPE &&
      Firmware->Configuration->Version->api ==
#if defined(AFE_CONFIG_API_DOMOTICZ_ENABLED)
          AFE_FIRMWARE_API_DOMOTICZ
#elif defined(AFE_CONFIG_API_HOME_ASSISTANT_ENABLED)
          AFE_FIRMWARE_API_HOME_ASSISTANT
#else
          AFE_FIRMWARE_API_STANDARD
#endif
  )
  {
#ifdef DEBUG
    Firmware->Debugger->printInformation(F("up2date"), F("UPGRADE"));
#endif
  }
  else
  {
#ifdef DEBUG
    Firmware->Debugger->printInformation(F("Upgrading..."), F("UPGRADE"));
#endif

    /* Upgraded version from one T to other T */
    if (Firmware->Configuration->Version->type != AFE_FIRMWARE_TYPE)
    {
#ifdef DEBUG
      Firmware->Debugger->printInformation(F("Upgrading Firmware type"), F("UPGRADE"));

#endif
      upgradeFirmwarType();

      /* Upgrade from one version to other within the same T */
    }
    else if (strcmp(Firmware->Configuration->Version->installed_version,
                    AFE_FIRMWARE_VERSION) != 0)
    {
#ifdef DEBUG
      Firmware->Debugger->printInformation(F("Upgrading Firmware T"), F("UPGRADE"));
      Serial << AFE_FIRMWARE_TYPE
             << F(" from version: ")
             << Firmware->Configuration->Version->installed_version << F(" to ")
             << AFE_FIRMWARE_VERSION;
#endif
      updateFirmwareVersion();
    }

    /* Checking if in addition there has been API version change */
    if (Firmware->Configuration->Version->api != AFE_FIRMWARE_API)
    {
#ifdef DEBUG
      Firmware->Debugger->printInformation(F("Firmware API version upgraded"), F("UPGRADE"));
      Serial << F(" from version: ") << Firmware->Configuration->Version->api
             << F(" to ") << AFE_FIRMWARE_API;
#endif
      updateFirmwareAPIVersion();
    }

#ifdef DEBUG
    Firmware->Debugger->printInformation(F("Upgrade to version "), F("UPGRADE"));
    Serial << AFE_FIRMWARE_VERSION
           << F(" completed");
#endif
  }
}

void AFEUpgrader::upgradeFirmwarType()
{
  NETWORK networkConfiguration;
  MQTT mqttConfiguration;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  DOMOTICZ domoticzCofiguration;
#endif
  PRO_VERSION proConfiguration;
  PASSWORD passwordConiguration;
  uint8_t deviceState;

#ifdef DEBUG
  Firmware->Debugger->printInformation(F("Upgrading firmware type"), F("UPGRADE"));
  Firmware->Debugger->printInformation(F("Reading core configuration"), F("UPGRADE"));
#endif

  /* Reading current data */

  Firmware->API->Flash->getConfiguration(&networkConfiguration);
  Firmware->API->Flash->getConfiguration(&mqttConfiguration);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  Firmware->API->Flash->getConfiguration(&domoticzCofiguration);
#endif
  Firmware->API->Flash->getConfiguration(&proConfiguration);
  Firmware->API->Flash->getConfiguration(&passwordConiguration);
  deviceState = Firmware->API->Flash->getDeviceMode();

#ifdef DEBUG
  Firmware->Debugger->printInformation(F("Creating default configuration"), F("UPGRADE"));
#endif

  /* Setting the device from scratch */
  Firmware->API->Flash->setDefaultConfiguration();

#ifdef DEBUG
  Firmware->Debugger->printInformation(F("Restoring core configuration"), F("UPGRADE"));
#endif

  /* Restoring core configuration */
  Firmware->API->Flash->saveConfiguration(&networkConfiguration);
  Firmware->API->Flash->saveConfiguration(&mqttConfiguration);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  Firmware->API->Flash->saveConfiguration(&domoticzCofiguration);
#endif
  Firmware->API->Flash->saveConfiguration(&proConfiguration);
  Firmware->API->Flash->saveConfiguration(&passwordConiguration);
  Firmware->API->Flash->saveDeviceMode(deviceState);

  /* Used to show up message in the firmware HTTP site*/
  Firmware->Device->upgraded = AFE_UPGRADE_VERSION_TYPE;
  Firmware->API->Flash->getConfiguration(Firmware->Configuration->Version);
}

void AFEUpgrader::updateFirmwareVersion()
{

#ifndef AFE_ESP32

#ifdef T5_CONFIG
  if (strcmp(Firmware->Configuration->Version->installed_version, "2.0.0") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.0.1") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.2.0.B1") ==
          0)
  {
    upgradeToT5V220();
  }
#endif // T5_CONFIG

#ifdef T6_CONFIG
  if (strcmp(Firmware->Configuration->Version->installed_version, "2.0.0") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.1.0") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.2.0") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.2.1") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.2.2") ==
          0)
  {
    upgradeToT6V230();
  }

  if (strcmp(Firmware->Configuration->Version->installed_version, "2.3.0") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.3.1") ==
          0 ||
      strcmp(Firmware->Configuration->Version->installed_version, "2.3.1.E1") ==
          0)
  {
    upgradeToT6V250();
  }

#endif // T6_CONFIG

#endif // !ESP32

  Firmware->API->Flash->saveFirmwareVersion(AFE_FIRMWARE_VERSION);
  Firmware->Device->upgraded = AFE_UPGRADE_VERSION_TYPE;
  Firmware->API->Flash->getConfiguration(Firmware->Configuration->Version);
}

void AFEUpgrader::updateFirmwareAPIVersion()
{

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  /* Checking if there is Domoticz server configuration file */
  if (!Firmware->API->Flash->fileExist(AFE_FILE_DOMOTICZ_CONFIGURATION))
  {
    Firmware->API->Flash->createDomoticzConfigurationFile();
  }
#endif

  Firmware->API->Flash->saveFirmwareAPIVersion();
  Firmware->Device->upgraded = AFE_UPGRADE_VERSION_TYPE;
  Firmware->API->Flash->getConfiguration(Firmware->Configuration->Version);
}

#ifndef AFE_ESP32

#ifdef T5_CONFIG
void AFEUpgrader::upgradeToT5V220()
{

// It will do nothing for ESP8266 1MB - sensors are e
#if defined(AFE_CONFIG_HARDWARE_BMEX80) || defined(AFE_CONFIG_HARDWARE_BH1750)
  Firmware->API->Flash->createI2CConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  Firmware->API->Flash->createBMEX80SensorConfigurationFile();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  Firmware->API->Flash->createBH1750SensorConfigurationFile();
#endif
}

#endif // T0_CONFIG

#ifdef T6_CONFIG
void AFEUpgrader::upgradeToT6V230()
{
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  Firmware->API->Flash->createAnemometerSensorConfigurationFile();
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  Firmware->API->Flash->createRainmeterSensorConfigurationFile();
  Firmware->API->Flash->createRainmeterSensorDataConfigurationFile();
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  Firmware->API->Flash->createADCInputConfigurationFile();
#endif
}

void AFEUpgrader::upgradeToT6V250()
{
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  Firmware->API->Flash->createDS18B20SensorConfigurationFile();
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
  Firmware->API->Flash->createDHTSensorConfigurationFile();
#endif
}

#endif // T6_CONFIG

#endif // !ESP32