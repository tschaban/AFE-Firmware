/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Web-Server.h"

AFEWebServer::AFEWebServer() {}

#if defined(AFE_CONFIG_HARDWARE_LED) && !defined(AFE_CONFIG_HARDWARE_I2C)
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI,
                         AFELED *_Led) {
  SystemLED = _Led;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#elif defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI,
                         AFELED *_Led, TwoWire *_WirePort0,
                         TwoWire *_WirePort1) {
  SystemLED = _Led;
  WirePort0 = _WirePort0;
  WirePort1 = _WirePort1;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#else
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI,
                         AFELED *_Led, TwoWire *_WirePort0) {
  SystemLED = _Led;
  WirePort0 = _WirePort0;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#endif // AFE_ESP32
#elif !defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI,
                         TwoWire *_WirePort0, TwoWire *_WirePort1) {
  WirePort0 = _WirePort0;
  WirePort1 = _WirePort1;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#else
void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI,
                         TwoWire *_WirePort0) {
  WirePort0 = _WirePort0;
  begin(_Data, _Device, _FirmwarePro, _RestAPI);
}
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_LED  AFE_CONFIG_HARDWARE_I2C

void AFEWebServer::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                         AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI) {
  server.begin(80);
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
  Site.begin(_Data, _Device, _FirmwarePro, _RestAPI, WirePort0, WirePort1);
#else
  Site.begin(_Data, _Device, _FirmwarePro, _RestAPI, WirePort0);
#endif //  AFE_ESP32
#else
  Site.begin(_Data, _Device, _FirmwarePro, _RestAPI);
#endif // AFE_CONFIG_HARDWARE_I2C
  Data = _Data;
  Device = _Device;
  RestAPI = _RestAPI;
  FirmwarePro = _FirmwarePro;
}

String AFEWebServer::generateSite(AFE_SITE_PARAMETERS *siteConfig,
                                  String &page) {

  /*
    if (siteConfig->twoColumns) {
      Site.generateMenu(page, siteConfig->rebootTime);
    } else {
      Site.generateEmptyMenu(page, siteConfig->rebootTime);
    }
  */
  if (siteConfig->form) {
    page.concat("<form  method=\"post\" action=\"/?c=");
    page.concat(AFE_SERVER_CMD_SAVE);
    page.concat("&o=");
    page.concat(siteConfig->ID);
    if (siteConfig->deviceID >= 0) {
      page.concat("&i=");
      page.concat(siteConfig->deviceID);
    }
    page.concat("\">");
  }

  switch (siteConfig->ID) {
  case AFE_CONFIG_SITE_INDEX:
    Site.siteIndex(page, siteConfig->deviceID == -1 ? true : false);
    break;
  case AFE_CONFIG_SITE_INDEX_MONITOR:
    Site.siteFirmware(page, true);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME:
    Site.siteNetwork(page);
    break;
  case AFE_CONFIG_SITE_FIRST_TIME_CONNECTING:
    Site.siteConnecting(page);
    break;
  case AFE_CONFIG_SITE_DEVICE:
    Site.siteDevice(page);
    break;
  case AFE_CONFIG_SITE_NETWORK:
    Site.siteNetwork(page);
    break;
  case AFE_CONFIG_SITE_MQTT:
    Site.siteMQTTBroker(page);
    break;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  case AFE_CONFIG_SITE_DOMOTICZ:
    Site.siteDomoticzServer(page);
    break;
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  case AFE_CONFIG_SITE_HOME_ASSISTANT_INTEGRATION:
    Site.siteHomeAssistantDiscoveryConfiguration(page);
    break;
#endif

  case AFE_CONFIG_SITE_PASSWORD:
    Site.sitePassword(page);
    break;
  case AFE_CONFIG_SITE_PRO_VERSION:
    Site.siteProKey(page);
    break;
  case AFE_CONFIG_SITE_EXIT:
    Site.siteExit(page, siteConfig->rebootMode);
    break;
  case AFE_CONFIG_SITE_RESET:
    Site.siteReset(page);
    break;
  case AFE_CONFIG_SITE_POST_RESET:
    Site.sitePostReset(page);
    break;
  case AFE_CONFIG_SITE_FIRMWARE:
    Site.siteFirmware(page);
    break;

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  case AFE_CONFIG_SITE_UPGRADE:
    Site.siteUpgrade(page);
    break;
  case AFE_CONFIG_SITE_POST_UPGRADE:
    Site.sitePostUpgrade(page, upgradeSuccess);
    break;
  case AFE_CONFIG_SITE_WAN_UPGRADE:
    Site.siteWANUpgrade(page, F(L_UPGRADE_IN_PROGRESS));
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
  case AFE_CONFIG_SITE_RELAY:
    Site.siteRelay(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  case AFE_CONFIG_SITE_SWITCH:
    Site.siteSwitch(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  case AFE_CONFIG_SITE_ANALOG_INPUT:
#ifdef AFE_ESP32
    Site.siteADCInput(page, siteConfig->deviceID);
#else  // ESP8266
    Site.siteADCInput(page);
#endif // AFE_ESP32
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  case AFE_CONFIG_SITE_CONTACTRON:
    Site.siteContactron(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
  case AFE_CONFIG_SITE_GATE:
    Site.siteGate(page, siteConfig->deviceID);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_UART
  case AFE_CONFIG_SITE_UART:
    Site.siteUARTBUS(page);
    break;
#endif
#ifdef AFE_CONFIG_HARDWARE_I2C
  case AFE_CONFIG_SITE_I2C:
#ifdef AFE_ESP32
    Site.siteI2CBUS(page, siteConfig->deviceID);
#else
    Site.siteI2CBUS(page);
#endif
    break;
#endif // AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  case AFE_CONFIG_SITE_BMEX80:
    Site.siteBMEX80Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_BMEX80
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  case AFE_CONFIG_SITE_HPMA115S0:
    Site.siteHPMA115S0Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_HPMA115S0
#ifdef AFE_CONFIG_HARDWARE_BH1750
  case AFE_CONFIG_SITE_BH1750:
    Site.siteBH1750Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_BH1750
#ifdef AFE_CONFIG_HARDWARE_AS3935
  case AFE_CONFIG_SITE_AS3935:
    Site.siteAS3935Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_AS3935
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  case AFE_CONFIG_SITE_DS18B20:
    Site.siteDS18B20Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_DS18B20
#ifdef AFE_CONFIG_HARDWARE_DHT
  case AFE_CONFIG_SITE_DHT:
    Site.siteDHTSensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_DHT
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  case AFE_CONFIG_SITE_ANEMOMETER_SENSOR:
    Site.siteAnemometerSensor(page);
    break;
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  case AFE_CONFIG_SITE_RAINMETER_SENSOR:
    Site.siteRainmeterSensor(page);
    break;
#endif // AFE_CONFIG_HARDWARE_RAINMETER
#ifdef AFE_CONFIG_HARDWARE_LED
  case AFE_CONFIG_SITE_LED:
    Site.siteLED(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_SYSTEM_LED:
    Site.siteSystemLED(page);
    break;
#endif // AFE_CONFIG_HARDWARE_LED
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  case AFE_CONFIG_SITE_REGULATOR:
    Site.siteRegulator(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  case AFE_CONFIG_SITE_THERMAL_PROTECTOR:
    Site.siteThermalProtector(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  case AFE_CONFIG_SITE_BINARY_SENSOR:
    Site.siteBinarySensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  case AFE_CONFIG_SITE_PN532_SENSOR:
    Site.sitePN532Sensor(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_PN532_SENSOR_ADMIN:
    Site.sitePN532SensorAdmin(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_MIFARE_CARDS:
    Site.siteMiFareCard(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
#ifdef AFE_CONFIG_HARDWARE_CLED
  case AFE_CONFIG_SITE_CLED:
    Site.siteCLED(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_CLED_EFFECT_BLINKING:
    Site.siteCLEDEffectBlinking(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_CLED_EFFECT_WAVE:
    Site.siteCLEDEffectWave(page, siteConfig->deviceID);
    break;
  case AFE_CONFIG_SITE_CLED_EFFECT_FADE_IN_OUT:
    Site.siteCLEDEffectFadeInOut(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_CLED
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  case AFE_CONFIG_SITE_TSL2561:
    Site.siteTSL2561Sensor(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_TSL2561
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  case AFE_CONFIG_SITE_MCP23XXX:
    Site.siteMCP23XXX(page, siteConfig->deviceID);
    break;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
  }

  if (siteConfig->form) {
    if (siteConfig->formButton) {
      page.concat("<input type=\"submit\" class=\"b bs\" value=\"");
      page.concat(L_SAVE);
      page.concat("\">");
    }
    page.concat("</form>");
  }

  return page;
}

boolean AFEWebServer::generate(boolean upload) {
  boolean _ret = true;

  if (getOptionName()) {
#ifdef DEBUG
    Serial << endl << F("INFO: HTTP Server: API Request");
#endif
    _ret = false;
  } else {

    /* Reading lastest Device configuration */
    if (_refreshConfiguration) {
      _refreshConfiguration = false;
      Device->begin();
    }

    /* Requested: site paramters */
    AFE_SITE_PARAMETERS siteConfig;
    siteConfig.ID = getSiteID();
    uint8_t command = getCommand();
    siteConfig.deviceID = getID();
    siteConfig.option = getOption();

    if (!upload) {
      /* Setting page refresh time if automatic logout is set */
      if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
           Device->getMode() == AFE_MODE_ACCESS_POINT) &&
          Device->configuration.timeToAutoLogOff > 0) {

        /* Setting the time to auto-close the configuration panel */
        siteConfig.rebootTime =
            Device->configuration.timeToAutoLogOff * 60 +
            10; // adds additional 10sec for a reboot to be finished
#ifdef DEBUG
        Serial << endl
               << F("INFO: SITE: Setting auto-logout to ")
               << siteConfig.rebootTime << F("seconds");
#endif
      }

      if (command == AFE_SERVER_CMD_SAVE) {
        if (siteConfig.ID == AFE_CONFIG_SITE_FIRST_TIME) {
          NETWORK configuration;
          get(configuration);
          siteConfig.twoColumns = false;
          siteConfig.reboot = true;
          siteConfig.rebootMode = AFE_MODE_CONFIGURATION;
          siteConfig.form = false;
          siteConfig.ID = AFE_CONFIG_SITE_FIRST_TIME_CONNECTING;
          Data->saveConfiguration(&configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_DEVICE) {
          DEVICE configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_NETWORK) {
          NETWORK configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_PASSWORD) {
          PASSWORD configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_RESET) {
          siteConfig.ID = AFE_CONFIG_SITE_POST_RESET;
          siteConfig.reboot = true;
          siteConfig.rebootMode = AFE_MODE_FIRST_TIME_LAUNCH;
          siteConfig.rebootTime = AFE_SITE_REBOOT_POST_UPGRADE;
          siteConfig.form = false;
          siteConfig.twoColumns = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_PRO_VERSION) {
          PRO_VERSION configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          sprintf(RestAPI->Pro.serial, "%s", configuration.serial);
          sprintf(FirmwarePro->Pro.serial, "%s", configuration.serial);
          FirmwarePro->validate();
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_MQTT) {
          MQTT configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        else if (siteConfig.ID == AFE_CONFIG_SITE_DOMOTICZ) {
          DOMOTICZ configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
        else if (siteConfig.ID == AFE_CONFIG_SITE_HOME_ASSISTANT_INTEGRATION) {
          HOME_ASSISTANT_CONFIG configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
        else if (siteConfig.ID == AFE_CONFIG_SITE_ANALOG_INPUT) {
          ADCINPUT configuration;
          get(configuration);
#ifdef AFE_ESP32
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
#else
          Data->saveConfiguration(&configuration);
#endif // AFE_ESP
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
        else if (siteConfig.ID == AFE_CONFIG_SITE_LED) {
          LED configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_SYSTEM_LED) {
          Data->saveSystemLedID(getSystemLEDData());
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
        else if (siteConfig.ID == AFE_CONFIG_SITE_RELAY) {
          RELAY configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
        else if (siteConfig.ID == AFE_CONFIG_SITE_SWITCH) {
          SWITCH configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
        else if (siteConfig.ID == AFE_CONFIG_SITE_CONTACTRON) {
          CONTACTRON configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          // @TODO T5 why this doesn't work here? => configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
        else if (siteConfig.ID == AFE_CONFIG_SITE_GATE) {
          GATE configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
        else if (siteConfig.ID == AFE_CONFIG_SITE_HPMA115S0) {
          HPMA115S0 configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
        else if (siteConfig.ID == AFE_CONFIG_SITE_BMEX80) {
          BMEX80 configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
        else if (siteConfig.ID == AFE_CONFIG_SITE_BH1750) {
          BH1750_CONFIG configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
        else if (siteConfig.ID == AFE_CONFIG_SITE_AS3935) {
          AS3935 configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
        else if (siteConfig.ID == AFE_CONFIG_SITE_ANEMOMETER_SENSOR) {
          ANEMOMETER configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
        else if (siteConfig.ID == AFE_CONFIG_SITE_RAINMETER_SENSOR) {
          RAINMETER configuration;
          get(configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_DS18B20
        else if (siteConfig.ID == AFE_CONFIG_SITE_DS18B20) {
          DS18B20 ds18B20Configuration;
          get(ds18B20Configuration);
          Data->saveConfiguration(siteConfig.deviceID, &ds18B20Configuration);
          ds18B20Configuration = {0};
        }
#endif
#ifdef AFE_CONFIG_HARDWARE_DHT
        else if (siteConfig.ID == AFE_CONFIG_SITE_DHT) {
          DHT dhtConfiguration;
          get(dhtConfiguration);
          Data->saveConfiguration(siteConfig.deviceID, &dhtConfiguration);
          dhtConfiguration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_DHT
#ifdef AFE_CONFIG_HARDWARE_UART
        else if (siteConfig.ID == AFE_CONFIG_SITE_UART) {
          SERIALPORT configuration;
          getSerialPortData(&configuration);
          Data->saveConfiguration(&configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_UART
#ifdef AFE_CONFIG_HARDWARE_I2C
        else if (siteConfig.ID == AFE_CONFIG_SITE_I2C) {
          I2CPORT configuration;
          get(configuration);
#ifdef AFE_ESP32
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
#else
          Data->saveConfiguration(&configuration);
#endif
          configuration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
        else if (siteConfig.ID == AFE_CONFIG_SITE_REGULATOR) {
          REGULATOR configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
        else if (siteConfig.ID == AFE_CONFIG_SITE_THERMAL_PROTECTOR) {
          THERMAL_PROTECTOR configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
        else if (siteConfig.ID == AFE_CONFIG_SITE_BINARY_SENSOR) {
          BINARY_SENSOR configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
        else if (siteConfig.ID == AFE_CONFIG_SITE_PN532_SENSOR) {
          PN532_SENSOR configuration;
          get(configuration);
          Data->saveConfiguration(0, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_MIFARE_CARDS) {
          MIFARE_CARD configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_PN532_SENSOR_ADMIN) {
          processMiFareCard();
        }
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
#ifdef AFE_CONFIG_HARDWARE_CLED
        else if (siteConfig.ID == AFE_CONFIG_SITE_CLED) {
          CLED configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_CLED_EFFECT_BLINKING) {
          CLED_EFFECT_BLINKING configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_CLED_EFFECT_WAVE) {
          CLED_EFFECT_WAVE configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        } else if (siteConfig.ID == AFE_CONFIG_SITE_CLED_EFFECT_FADE_IN_OUT) {
          CLED_EFFECT_FADE_INOUT configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }

#endif // AFE_CONFIG_HARDWARE_CLED
#ifdef AFE_CONFIG_HARDWARE_TSL2561
        else if (siteConfig.ID == AFE_CONFIG_SITE_TSL2561) {
          TSL2561 configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_TSL2561
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
        else if (siteConfig.ID == AFE_CONFIG_SITE_MCP23XXX) {
          MCP23XXX configuration;
          get(configuration);
          Data->saveConfiguration(siteConfig.deviceID, &configuration);
          configuration = {0};
        }
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

      } else if (command == AFE_SERVER_CMD_NONE) {
        if (siteConfig.ID == AFE_CONFIG_SITE_INDEX) {
          siteConfig.form = false;
          siteConfig.twoColumns = false;
          if (siteConfig.deviceID > AFE_MODE_NORMAL) {
            boolean authorize = true;
            PASSWORD accessControl;
            Data->getConfiguration(&accessControl);
            if (accessControl.protect) {
              PASSWORD data;
              get(data);
              if (strcmp(accessControl.password, data.password) != 0) {
                authorize = false;
              }
            }
            if (authorize) {
              siteConfig.rebootMode = siteConfig.deviceID;
              siteConfig.ID = AFE_CONFIG_SITE_EXIT;
              siteConfig.reboot = true;
              siteConfig.rebootTime = AFE_SITE_REBOOT;
            }
          }
        } else if (siteConfig.ID == AFE_CONFIG_SITE_INDEX_MONITOR) {
          siteConfig.form = false;
          siteConfig.twoColumns = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_EXIT) {
          siteConfig.reboot = true;
          siteConfig.rebootMode = AFE_MODE_NORMAL;
          siteConfig.rebootTime = AFE_SITE_REBOOT;
          siteConfig.form = false;
          siteConfig.twoColumns = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_FIRMWARE) {
          siteConfig.form = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_FIRST_TIME) {
          siteConfig.twoColumns = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_RESET) {
          siteConfig.formButton = false;
#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
        } else if (siteConfig.ID == AFE_CONFIG_SITE_UPGRADE) {
          siteConfig.form = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_WAN_UPGRADE) {
          siteConfig.form = false;
          siteConfig.rebootTime = AFE_SITE_REBOOT_POST_WAN_UPGRADE;
          siteConfig.twoColumns = false;
        } else if (siteConfig.ID == AFE_CONFIG_SITE_POST_UPGRADE) {
          if (!upload) {
            siteConfig.form = false;
            siteConfig.twoColumns = false;
            siteConfig.rebootTime = AFE_SITE_REBOOT_POST_UPGRADE;
            siteConfig.reboot = true;
            siteConfig.rebootMode = Device->getMode();
          }
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE
        }
      }
    }

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
    if (upload) {
      upgradeSuccess = upgradOTAFile();
    } else {
#endif // #ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE

#ifdef DEBUG
      Serial << endl << F("INFO: Building HTTP site response ");
#ifndef AFE_ESP32
      Serial << endl
             << F("INFO: RAM: ") << system_get_free_heap_size() / 1024
             << F("kB to build the site");
#endif // !ESP32
#endif

      String page;
/* page.reserve(AFE_MAX_PAGE_SIZE);
*/
      server.sendHeader("Cache-Control", "no-cache");
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);

      if (siteConfig.twoColumns) {
        Site.generateMenu(page, siteConfig.rebootTime);
      } else {
        Site.generateEmptyMenu(page, siteConfig.rebootTime);
      }
      Site.setAttributes(&page);
      server.send(200, "text/html", page);

#if defined(DEBUG) && !defined(ESP32)
      Serial << endl
             << F("INFO: RAM: ")
             << system_get_free_heap_size() / 1024 << F("kB: header + menu generated");
#endif

      page = "";

      generateSite(&siteConfig, page);
      Site.setAttributes(&page);
      server.sendContent(page);

#if defined(DEBUG) && !defined(ESP32)
      Serial << endl
             << F("INFO: RAM: ")
             << system_get_free_heap_size() / 1024 << F("kB: content generated");
#endif


      page = "";
      Site.generateFooter(page, (Device->getMode() == AFE_MODE_NORMAL ||
                                 Device->getMode() == AFE_MODE_CONFIGURATION)
                                    ? true
                                    : false);
      Site.setAttributes(&page);
      server.sendContent(page);

#if defined(DEBUG) && !defined(ESP32)
      Serial << endl
             << F("INFO: RAM: ")
             << system_get_free_heap_size() / 1024 << F("kB: footer generated");
#endif


      server.sendContent("");
      page = "";

#if defined(DEBUG) && !defined(ESP32)
      Serial << endl
             << F("INFO: RAM: ")
             << system_get_free_heap_size() / 1024 << F("kB: site generated and published");
#endif

  if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
       Device->getMode() == AFE_MODE_ACCESS_POINT) &&
      Device->configuration.timeToAutoLogOff > 0) {
    howLongInConfigMode = millis();
  }


#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
      if (siteConfig.ID == AFE_CONFIG_SITE_WAN_UPGRADE) {
        if (upgradeOTAWAN(getOTAFirmwareId())) {
          siteConfig.reboot = true;
          siteConfig.rebootMode = AFE_MODE_CONFIGURATION;
        }
      }
    }
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

    /* Rebooting device */
    if (siteConfig.reboot) {
#ifdef AFE_CONFIG_HARDWARE_LED
      SystemLED->on();
#endif
      Device->reboot(siteConfig.rebootMode);
    }
  }
  return _ret;
}

/* Methods related to the url request */

boolean AFEWebServer::getOptionName() {
  /* Recived HTTP API Command */
  if (server.hasArg("command")) {
    /* Constructing command */
    server.arg(F("command"))
        .toCharArray(httpCommand.command, sizeof(httpCommand.command));
    if (server.arg(F("device"))) {
      server.arg(F("device"))
          .toCharArray(httpCommand.device, sizeof(httpCommand.device));
    } else {
      memset(httpCommand.device, 0, sizeof httpCommand.device);
    }
    if (server.arg(F("name"))) {
      server.arg(F("name")).toCharArray(httpCommand.name,
                                        sizeof(httpCommand.name));
    } else {
      memset(httpCommand.name, 0, sizeof httpCommand.name);
    }

    if (server.arg(F("source"))) {
      server.arg(F("source"))
          .toCharArray(httpCommand.source, sizeof(httpCommand.source));
    } else {
      memset(httpCommand.source, 0, sizeof httpCommand.source);
    }
    receivedHTTPCommand = true;
  }

  return receivedHTTPCommand;
}

uint8_t AFEWebServer::getSiteID() {

  if (Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
    return AFE_CONFIG_SITE_FIRST_TIME;
  } else {
    return Device->getMode() == AFE_MODE_NORMAL
               ? server.arg(F("o")).toInt() == AFE_CONFIG_SITE_INDEX_MONITOR
                     ? AFE_CONFIG_SITE_INDEX_MONITOR
                     : AFE_CONFIG_SITE_INDEX
               : server.arg(F("o")).toInt() > 0 ? server.arg(F("o")).toInt()
                                                : AFE_CONFIG_SITE_DEVICE;
  }
}

uint8_t AFEWebServer::getCommand() {
  if (server.hasArg(F("c"))) {
    return server.arg(F("c")).toInt();
  } else {
    return AFE_SERVER_CMD_NONE;
  }
}

uint8_t AFEWebServer::getID() {
  if (server.hasArg(F("i"))) {
    return server.arg(F("i")).toInt();
  } else {
    return -1;
  }
}

uint8_t AFEWebServer::getOption() {
  if (server.hasArg(F("opt"))) {
    return server.arg(F("opt")).toInt();
  } else {
    return AFE_HARDWARE_ITEM_NOT_EXIST;
  }
}

/* Server methods */

HTTPCOMMAND AFEWebServer::getHTTPCommand() {
  receivedHTTPCommand = false;
  return httpCommand;
}

void AFEWebServer::listener() {
  server.handleClient();
  /* Code for automatic logoff from the config panel */
  if ((Device->getMode() == AFE_MODE_CONFIGURATION ||
       Device->getMode() == AFE_MODE_ACCESS_POINT) &&
      Device->configuration.timeToAutoLogOff > 0) {
    if (Device->configuration.timeToAutoLogOff * 60000 + howLongInConfigMode <
        millis()) {
#ifdef DEBUG
      Serial << endl
             << endl
             << F("INFO: SITE: Automatic logout from the config panel after : ")
             << Device->configuration.timeToAutoLogOff
             << F("min. of idle time");
#endif
      Device->reboot(AFE_MODE_NORMAL);
    }
  }
}

boolean AFEWebServer::httpAPIlistener() { return receivedHTTPCommand; }

void AFEWebServer::sendJSON(const String &json) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: HTTP Server: Pubishing reply") << endl
         << F("-----") << endl
         << json << endl
         << F("-----");
#endif

  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Content-Length", String(json.length()));
  server.setContentLength(json.length());
  server.send(200, "application/json", json);
}

#ifndef AFE_ESP32 /* ESP82xx */
void AFEWebServer::handle(const char *uri,
                          ESP8266WebServer::THandlerFunction handler) {
  server.on(uri, handler);
}

void AFEWebServer::handleFirmwareUpgrade(
    const char *uri, ESP8266WebServer::THandlerFunction handlerUpgrade,
    ESP8266WebServer::THandlerFunction handlerUpload) {
  server.on(uri, HTTP_POST, handlerUpgrade, handlerUpload);
}

void AFEWebServer::onNotFound(ESP8266WebServer::THandlerFunction fn) {
  server.onNotFound(fn);
}
#else /* ESP32 */
void AFEWebServer::handle(const char *uri,
                          WebServer::THandlerFunction handler) {
  server.on(uri, handler);
}

void AFEWebServer::handleFirmwareUpgrade(
    const char *uri, WebServer::THandlerFunction handlerUpgrade,
    WebServer::THandlerFunction handlerUpload) {
  server.on(uri, HTTP_POST, handlerUpgrade, handlerUpload);
}

void AFEWebServer::onNotFound(WebServer::THandlerFunction fn) {
  server.onNotFound(fn);
}
#endif

/* Upgrade methods */

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE

String AFEWebServer::getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

boolean AFEWebServer::upgradeOTAWAN(uint16_t firmwareId) {
  boolean _success = true;
  WiFiClient WirelessClient;
  long contentLength = 0;
  char firmwareFileName[AFE_FIRMARE_FILE_NAME_LENGTH];
  char _message[164]; //  calcuated based on L_UPGRADE_SUCCESS_MESSAGE and
                      //  L_UPGRADE_NOT_FULL_LOADED

// firmwareId = 601;

#ifdef AFE_CONFIG_HARDWARE_LED
  SystemLED->on();
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: UPGRADE WAN: Connecting to: api.smartnydom.pl");
#endif
  if (WirelessClient.connect("api.smartnydom.pl", 80)) {

#ifdef DEBUG
    Serial << F("... connected") << endl
           << F("INFO: UPGRADE WAN: downloading the firmware ID: ")
           << firmwareId;
#endif

    WirelessClient.print(
        String("GET /download/") + firmwareId +
        " HTTP/1.1\r\nHost: api.smartnydom.pl\r\nCache-Control: " +
        "no-cache\r\nConnection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (WirelessClient.available() == 0) {
      if (millis() - timeout > 5000) {
#ifdef DEBUG
        Serial << endl << F("ERROR: UPGRADE WAN: ") << F(L_UPGRADE_TIMEOUT);
#endif
        WirelessClient.stop();
        _success = false;
        Data->saveWelecomeMessage(L_UPGRADE_TIMEOUT);
      }
    }

    while (_success && WirelessClient.available()) {
      String line = WirelessClient.readStringUntil('\n');
      line.trim();
      if (!line.length()) {
        break;
      }

      /* Expecting reply from the server with HTTP = 200 */
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
#ifdef DEBUG
          Serial << endl
                 << F("ERROR: UPGRADE WAN: Got a NONE 200 status code from "
                      "server");
#endif
          Data->saveWelecomeMessage(L_UPGRADE_SERVER_NONE_200);
          _success = false;
          break;
        }
      }

      if (_success) {
        /* Expeting firmware files size > 0 */
        if (line.startsWith("content-length: ")) {
          contentLength =
              atol(getHeaderValue(line, "content-length: ").c_str());
#ifdef DEBUG
          Serial << endl
                 << F("INFO: UPGRADE WAN: Got ") << (contentLength / 1024)
                 << F("kB from server");
#endif
          if (contentLength == 0) {
            _success = false;
            Data->saveWelecomeMessage(L_UPGRADE_FIRMWARE_SIZE_0);
            break;
          }
        }
      } // Success of content-length

      if (_success) {
        if (line.startsWith("content-type: ")) {
          if (getHeaderValue(line, "content-type: ") !=
              "application/octet-stream") {
            _success = false;
            Data->saveWelecomeMessage(L_UPGRADE_WRONG_CONTENT_TYPE);
            break;
          }
        }
      } // Success of content-type

      if (line.startsWith("content-disposition: attachment; filename=")) {
        getHeaderValue(line, "content-disposition: attachment; filename=")
            .toCharArray(firmwareFileName, AFE_FIRMARE_FILE_NAME_LENGTH);
#ifdef DEBUG
        Serial << endl
               << F("INFO: UPGRADE WAN: Firmware file name: ")
               << firmwareFileName;
#endif
      }
    }

  } else {
#ifdef DEBUG
    Serial << endl
           << F("ERROR: UPGRADE WAN: ")
           << F(L_UPGRADE_CANNOT_CONNECT_TO_SERVER);
#endif
    Data->saveWelecomeMessage(L_UPGRADE_CANNOT_CONNECT_TO_SERVER);
    _success = false;
  }

  if (_success) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: UPGRADE WAN: Upgrade started. Device might be quite "
                "for a while");
#endif
    if (Update.begin(contentLength)) {
      size_t written = Update.writeStream(WirelessClient);

      if (written != contentLength) {
        _success = false;
        sprintf(_message, L_UPGRADE_NOT_FULL_LOADED, written / 1024,
                (uint16_t)(contentLength / 1024));
        Data->saveWelecomeMessage(_message);
#ifdef DEBUG
        Serial << endl << F("ERROR: UPGRADE WAN: ") << _message;
      } else {
        Serial << endl
               << F("INFO: UPGRADE WAN: Written : ") << (written / 1024)
               << F("kB successfully");
#endif
      }

      if (Update.end()) {
#ifdef DEBUG
        Serial << endl << F("INFO: UPGRADE WAN: Upgrade done!");
#endif
        if (Update.isFinished()) {
          sprintf(_message, L_UPGRADE_SUCCESS_MESSAGE, firmwareFileName,
                  (uint16_t)(contentLength / 1024));
          Data->saveWelecomeMessage(_message);
#ifdef DEBUG
          Serial << endl
                 << F("INFO: UPGRADE WAN: Update successfully completed");
#endif
        } else {
#ifdef DEBUG
          Serial << endl
                 << F("ERROR: UPGRADE WAN: Update not finished. Something went "
                      "wrong");
#endif
          _success = false;
        }
      } else {
        sprintf(_message, L_UPGRADE_SOMETHING_WRONG, Update.getError());
#ifdef DEBUG
        Serial << endl << F("ERROR: UPGRADE WAN: ") << _message;
#endif
        Data->saveWelecomeMessage(_message);
        _success = false;
      }
    } else {
#ifdef DEBUG
      Serial << endl << F("ERROR: UPGRADE WAN: ") << F(L_UPGRADE_NO_SPACE);
#endif
      Data->saveWelecomeMessage(L_UPGRADE_NO_SPACE);
      WirelessClient.flush();
      _success = false;
    }
  }
#ifdef AFE_CONFIG_HARDWARE_LED
  SystemLED->off();
#endif

  return _success;
}

boolean AFEWebServer::upgradOTAFile(void) {
  HTTPUpload &upload = server.upload();
  String _updaterError;
  boolean _success = false;
  if (upload.status == UPLOAD_FILE_START) {
#ifndef AFE_ESP32
    WiFiUDP::stopAll();
#endif

#ifdef DEBUG
    Serial << endl
           << F("INFO: UPGRADE: Firmware file name: ")
           << upload.filename.c_str();
#endif

#ifdef AFE_ESP32
    uint32_t maxSketchSpace = UPDATE_SIZE_UNKNOWN;
#else  // ESP8266
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#endif // AFE_ESP32

#ifdef DEBUG
    Serial << endl
           << F("INFO: UPGRADE: Current Firmware size: ")
           << (ESP.getSketchSize() / 1024) << F("Kb") << endl
           << F("INFO: UPGRADE: Free space size: ")
           << (ESP.getFreeSketchSpace() / 1024) << F("Kb") << endl
           << F("INFO: UPGRADE: Max free space size for this hardware: ")
           << (maxSketchSpace / 1024) << F("Kb") << endl
#ifdef AFE_ESP32
           << F("INFO: UPGRADE: Max size: ")
           << (UPDATE_SIZE_UNKNOWN / 1024 / 1024) << F("KB") << endl
#endif // ESP32
           << F("INFO: UPGRADE: ");
#endif

    if (!Update.begin(maxSketchSpace)) {
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
  } else if (upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()) {
#ifdef AFE_CONFIG_HARDWARE_LED
    SystemLED->toggle();
#endif
#ifdef DEBUG
    // Serial << endl << (upload.totalSize / 1024) << F(" kB");
    Serial << F(">");
#endif

    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
#ifdef DEBUG
      Serial << endl;
      Update.printError(Serial);
#endif
    }
  } else if (upload.status == UPLOAD_FILE_END && !_updaterError.length()) {
    if (Update.end(true)) { // true to set the size to the current
      // progress
      _success = true;
#ifdef DEBUG
      Serial << endl
             << F("INFO: UPGRADE: Success. Firmware size: ") << upload.totalSize
             << endl
             << F("INFO: UPGRADE: Rebooting...");
#endif
    }
#ifdef DEBUG
    else {
      Update.printError(Serial);
    }
#endif
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    Update.end();
#ifdef DEBUG
    Serial << endl << F("ERROR: UPGRADE: Update was aborted");
#endif
  }
  // yield(); // @TODO removed with T7
  return _success;
}

#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

/* Reading Server data */

void AFEWebServer::get(DEVICE &data) {

  _refreshConfiguration =
      true; // it will cause that device configuration will be refeshed

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.api.http = server.arg(F("h")).length() > 0 ? true : false;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.api.domoticz = server.arg(F("m")).length() > 0
                          ? (server.arg(F("m")).toInt() == 1 ? true : false)
                          : false;
  data.api.mqtt = server.arg(F("m")).length() > 0
                      ? (server.arg(F("m")).toInt() == 2 ? true : false)
                      : false;
#else
  data.api.mqtt = server.arg(F("m")).length() > 0 ? true : false;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  data.noOfLEDs = server.arg(F("l")).length() > 0
                      ? server.arg(F("l")).toInt()
                      : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  data.noOfContactrons =
      server.arg(F("co")).length() > 0 ? server.arg(F("co")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  data.noOfGates =
      server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  data.noOfRelays = server.arg(F("r")).length() > 0
                        ? server.arg(F("r")).toInt()
                        : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS;
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  data.noOfSwitches = server.arg(F("s")).length() > 0
                          ? server.arg(F("s")).toInt()
                          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES;
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  data.noOfDS18B20s = server.arg(F("ds")).length() > 0
                          ? server.arg(F("ds")).toInt()
                          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20;
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  data.noOfDHTs = server.arg(F("dh")).length() > 0
                      ? server.arg(F("dh")).toInt()
                      : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT;
#endif

#if defined(T3_CONFIG)
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    data.isPIR[i] = server.arg(F("p")).toInt() > i ? true : false;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  data.noOfHPMA115S0s =
      server.arg(F("hp")).length() > 0 ? server.arg(F("hp")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  data.noOfBMEX80s =
      server.arg(F("b6")).length() > 0 ? server.arg(F("b6")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  data.noOfBH1750s =
      server.arg(F("bh")).length() > 0 ? server.arg(F("bh")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  data.noOfAS3935s =
      server.arg(F("a3")).length() > 0 ? server.arg(F("a3")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  data.noOfAnemometerSensors =
      server.arg(F("w")).length() > 0 ? server.arg(F("w")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  data.noOfRainmeterSensors =
      server.arg(F("d")).length() > 0 ? server.arg(F("d")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  data.noOfRegulators =
      server.arg(F("re")).length() > 0 ? server.arg(F("re")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  data.noOfThermalProtectors =
      server.arg(F("tp")).length() > 0 ? server.arg(F("tp")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  data.noOfAnalogInputs =
      server.arg(F("ad")).length() > 0 ? server.arg(F("ad")).toInt() : 0;
#else
  data.isAnalogInput = server.arg(F("ad")).length() > 0 ? true : false;
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  data.noOfBinarySensors =
      server.arg(F("b")).length() > 0 ? server.arg(F("b")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  data.noOfPN532Sensors =
      server.arg(F("ck")).length() > 0
          ? server.arg(F("ck")).toInt()
          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_PN532_SENSORS;
  data.noOfMiFareCards =
      server.arg(F("f")).length() > 0
          ? server.arg(F("f")).toInt()
          : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MIFARE_CARDS;

#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  data.noOfCLEDs = server.arg(F("cl")).length() > 0
                       ? server.arg(F("cl")).toInt()
                       : AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CLED_STRIPS;
#endif

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  data.noOfI2Cs =
      server.arg(F("ii")).length() > 0 ? server.arg(F("ii")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  data.noOfTSL2561s =
      server.arg(F("tl")).length() > 0 ? server.arg(F("tl")).toInt() : 0;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  data.noOfMCP23xxx =
      server.arg(F("e")).length() > 0 ? server.arg(F("e")).toInt() : 0;
#endif

  data.timeToAutoLogOff =
      server.arg(F("al")).length() > 0 ? AFE_AUTOLOGOFF_DEFAULT_TIME : 0;
}

void AFEWebServer::get(NETWORK &data) {

  if (server.arg(F("s")).length() > 0) {
    server.arg(F("s")).toCharArray(data.ssid, sizeof(data.ssid));
  } else {
    data.ssid[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("sb")).length() > 0) {
    server.arg(F("sb")).toCharArray(data.ssidBackup, sizeof(data.ssidBackup));
  } else {
    data.ssidBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("p")).length() > 0) {
    server.arg(F("p")).toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("pb")).length() > 0) {
    server.arg(F("pb")).toCharArray(data.passwordBackup,
                                    sizeof(data.passwordBackup));
  } else {
    data.passwordBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i1")).length() > 0) {
    server.arg(F("i1")).toCharArray(data.ip, sizeof(data.ip));
  } else {
    data.ip[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i2")).length() > 0) {
    server.arg(F("i2")).toCharArray(data.gateway, sizeof(data.gateway));
  } else {
    data.gateway[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i3")).length() > 0) {
    server.arg(F("i3")).toCharArray(data.subnet, sizeof(data.subnet));
  } else {
    data.subnet[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i1b")).length() > 0) {
    server.arg(F("i1b")).toCharArray(data.ipBackup, sizeof(data.ipBackup));
  } else {
    data.ipBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i2b")).length() > 0) {
    server.arg(F("i2b")).toCharArray(data.gatewayBackup,
                                     sizeof(data.gatewayBackup));
  } else {
    data.gatewayBackup[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("i3b")).length() > 0) {
    server.arg(F("i3b")).toCharArray(data.subnetBackup,
                                     sizeof(data.subnetBackup));
  } else {
    data.subnetBackup[0] = AFE_EMPTY_STRING;
  }

  data.noConnectionAttempts =
      server.arg(F("na")).length() > 0
          ? server.arg(F("na")).toInt()
          : AFE_CONFIG_NETWORK_DEFAULT_CONNECTION_ATTEMPTS;

  data.waitTimeConnections = server.arg(F("wc")).length() > 0
                                 ? server.arg(F("wc")).toInt()
                                 : AFE_CONFIG_NETWORK_DEFAULT_WAIT_TIME;

  data.waitTimeSeries = server.arg(F("ws")).length() > 0
                            ? server.arg(F("ws")).toInt()
                            : AFE_CONFIG_NETWORK_DEFAULT_WAIT_SERIES;

  data.noFailuresToSwitchNetwork =
      server.arg(F("fs")).length() > 0
          ? server.arg(F("fs")).toInt()
          : AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER;

  data.isDHCP = server.arg(F("d")).length() > 0 ? true : false;
  data.isDHCPBackup = server.arg(F("db")).length() > 0 ? true : false;

#if !defined(ESP32)
  data.radioMode = server.arg(F("r")).length() > 0
                       ? server.arg(F("r")).toInt()
                       : AFE_CONFIG_NETWORK_DEFAULT_RADIO_MODE;
  data.outputPower = server.arg(F("y")).length() > 0
                         ? server.arg(F("y")).toFloat()
                         : AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER;
#endif
}

void AFEWebServer::get(MQTT &data) {
  if (server.arg(F("h")).length() > 0) {
    server.arg(F("h")).toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("a")).length() > 0) {
    server.arg(F("a")).toCharArray(data.ip, sizeof(data.ip));

  } else {
    data.ip[0] = AFE_EMPTY_STRING;
  }

  data.port = server.arg(F("p")).length() > 0 ? server.arg(F("p")).toInt()
                                              : AFE_CONFIG_MQTT_DEFAULT_PORT;

  if (server.arg(F("u")).length() > 0) {
    server.arg(F("u")).toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("s")).length() > 0) {
    server.arg(F("s")).toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.lwt.idx =
      server.arg(F("x")).length() > 0 ? server.arg(F("x")).toInt() : 0;
#else
  if (server.arg(F("t0")).length() > 0) {
    server.arg(F("t0")).toCharArray(data.lwt.topic, sizeof(data.lwt.topic));
  } else {
    data.lwt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  data.retainLWT = server.arg(F("rl")).length() > 0 ? true : false;
  data.retainAll = server.arg(F("ra")).length() > 0 ? true : false;

  data.qos = server.arg(F("q")).length() > 0 ? server.arg(F("q")).toInt()
                                             : AFE_CONFIG_MQTT_DEFAULT_QOS;
}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEWebServer::get(DOMOTICZ &data) {

  if (server.arg(F("t")).length() > 0) {
    data.protocol = server.arg(F("t")).toInt();
  }

  if (server.arg(F("h")).length() > 0) {
    server.arg(F("h")).toCharArray(data.host, sizeof(data.host));
  } else {
    data.host[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("p")).length() > 0) {
    data.port = server.arg(F("p")).toInt();
  }

  if (server.arg(F("u")).length() > 0) {
    server.arg(F("u")).toCharArray(data.user, sizeof(data.user));
  } else {
    data.user[0] = AFE_EMPTY_STRING;
  }
  if (server.arg(F("s")).length() > 0) {
    server.arg(F("s")).toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }
}
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
void AFEWebServer::get(HOME_ASSISTANT_CONFIG &data) {

  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.discovery.topic,
                                   sizeof(data.discovery.topic));
  } else {
    data.discovery.topic[0] = AFE_EMPTY_STRING;
  }
  data.addingComponents = server.arg(F("a")).length() > 0 ? true : false;
  data.removeingComponents = server.arg(F("r")).length() > 0 ? true : false;
  data.retainConfiguration = server.arg(F("m")).length() > 0 ? true : false;
}
#endif // AFE_FIRMWARE_API

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEWebServer::get(RELAY &data) {

  data.gpio = server.arg(F("g")).length() ? server.arg(F("g")).toInt() : 0;

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  data.timeToOff =
      server.arg(F("ot")).length() > 0 ? server.arg(F("ot")).toFloat() : 0;
#endif

  data.state.powerOn =
      server.arg(F("pr")).length() > 0 ? server.arg(F("pr")).toInt() : 0;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.state.MQTTConnected =
      server.arg(F("mc")).length() > 0 ? server.arg(F("mc")).toInt() : 0;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  data.ledID = server.arg(F("l")).length() > 0 ? server.arg(F("l")).toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.triggerSignal = server.arg(F("ts")).length() > 0
                           ? server.arg(F("ts")).toInt()
                           : AFE_CONFIG_HARDWARE_RELAY_DEFAULT_SIGNAL_TRIGGER;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  data.mcp23017.gpio = server.arg(F("mg")).length() > 0
                           ? server.arg(F("mg")).toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.id = server.arg(F("a")).length() > 0
                         ? server.arg(F("a")).toInt()
                         : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEWebServer::get(SWITCH &data) {
  data.type = server.arg(F("m")).length() > 0 ? server.arg(F("m")).toInt()
                                              : AFE_SWITCH_TYPE_MONO;

  data.sensitiveness = server.arg(F("s")).length() > 0
                           ? server.arg(F("s")).toInt()
                           : AFE_HARDWARE_SWITCH_DEFAULT_BOUNCING;

  data.functionality = server.arg(F("f")).length() > 0
                           ? server.arg(F("f")).toInt()
                           : AFE_SWITCH_FUNCTIONALITY_NONE;

  data.gpio = server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : 0;
#ifdef AFE_CONFIG_HARDWARE_RELAY
  data.relayID = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx =
      server.arg(F("x")).length() > 0 ? server.arg(F("x")).toInt() : 0;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  data.mcp23017.gpio = server.arg(F("mg")).length() > 0
                           ? server.arg(F("mg")).toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.id = server.arg(F("a")).length() > 0
                         ? server.arg(F("a")).toInt()
                         : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

void AFEWebServer::get(PASSWORD &data) {
  if (server.arg(F("p")).length() > 0) {
    server.arg(F("p")).toCharArray(data.password, sizeof(data.password));
  } else {
    data.password[0] = AFE_EMPTY_STRING;
  }

  data.protect = server.arg(F("r")).length() > 0 ? true : false;
}

void AFEWebServer::get(PRO_VERSION &data) {
  if (server.arg(F("k")).length() > 0) {
    server.arg(F("k")).toCharArray(data.serial, sizeof(data.serial));
  } else {
    data.serial[0] = AFE_EMPTY_STRING;
  }

  data.valid = false;
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEWebServer::get(REGULATOR &data) {
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.enabled = server.arg(F("e")).length() > 0 ? true : false;
  data.turnOn =
      server.arg(F("on")).length() > 0 ? server.arg(F("on")).toFloat() : 0;
  data.turnOff =
      server.arg(F("off")).length() > 0 ? server.arg(F("off")).toFloat() : 0;
  data.turnOnAbove =
      server.arg(F("ta")).length() > 0 && server.arg(F("ta")).toInt() == 1
          ? true
          : false;
  data.turnOffAbove =
      server.arg(F("tb")).length() > 0 && server.arg(F("tb")).toInt() == 1
          ? true
          : false;
  data.relayId = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;
  data.sensorId = server.arg(F("s")).length() > 0 ? server.arg(F("s")).toInt()
                                                  : AFE_HARDWARE_ITEM_NOT_EXIST;
/* Hardcoded 0 for DS18B20, 1 for DHT */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  data.sensorHardware =
      server.arg(F("h")).length() > 0 ? server.arg(F("h")).toInt() : 0;
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  data.sensorHardware =
      server.arg(F("h")).length() > 0 ? server.arg(F("h")).toInt() : 1;

  data.controllingParameter = server.arg(F("cp")).length() > 0
                                  ? server.arg(F("cp")).toInt()
                                  : AFE_HARDWARE_ITEM_NOT_EXIST;

#endif // AFE_CONFIG_HARDWARE_DHT

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEWebServer::get(THERMAL_PROTECTOR &data) {
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.enabled = server.arg(F("e")).length() > 0 ? true : false;
  data.temperature =
      server.arg(F("m")).length() > 0
          ? server.arg(F("m")).toFloat()
          : AFE_FUNCTIONALITY_THERMAL_PROTECTOR_DEFAULT_TEMPERATURE;
  data.relayId = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;
  data.sensorId = server.arg(F("s")).length() > 0 ? server.arg(F("s")).toInt()
                                                  : AFE_HARDWARE_ITEM_NOT_EXIST;
  /* Hardcoded 0 for DS18B20 */
  data.sensorHardware =
      server.arg(F("h")).length() > 0 ? server.arg(F("h")).toInt() : 0;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEWebServer::get(CONTACTRON &data) {
  data.type = server.arg(F("y")).length() > 0
                  ? server.arg(F("y")).toInt()
                  : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE;

#ifdef AFE_CONFIG_HARDWARE_LED
  data.ledID = server.arg("l").length() > 0 ? server.arg("l").toInt()
                                            : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.bouncing = server.arg(F("b")).length() > 0
                      ? server.arg(F("b")).toInt()
                      : AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING;

  data.gpio = server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : 0;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEWebServer::get(GATE &data) {

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.relayId = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.contactron.id[0] = server.arg(F("c1")).length() > 0
                              ? server.arg(F("c1")).toInt()
                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.contactron.id[1] = server.arg(F("c2")).length() > 0
                              ? server.arg(F("c2")).toInt()
                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  for (uint8_t i = 0; i < sizeof(data.states.state); i++) {
    data.states.state[i] = server.arg("s" + String(i)).length() > 0
                               ? server.arg("s" + String(i)).toInt()
                               : AFE_GATE_UNKNOWN;
  }
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticzControl.idx = server.arg(F("z")).length() > 0
                                 ? server.arg(F("z")).toInt()
                                 : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_GATE

#if defined(T3_CONFIG)
void AFEWebServer::getPIRData(uint8_t id, PIR *data) {

  if (server.arg(F("g" + String(id)).length() > 0) {
    data.gpio = server.arg(F("g" + String(id)).toInt();
  }

  if (server.arg(F("n" + String(id)).length() > 0) {
    server.arg(F("n" + String(id)).toCharArray(data->name, sizeof(data->name));
  }

  if (server.arg(F("l" + String(id)).length() > 0) {
    data->ledId = server.arg(F("l" + String(id)).toInt();
  }

  if (server.arg(F("r" + String(id)).length() > 0) {
    data->relayId = server.arg(F("r" + String(id)).toInt();
  }

  if (server.arg(F("d" + String(id)).length() > 0) {
    data->howLongKeepRelayOn = server.arg(F("d" + String(id)).toInt();
  }

  server.arg(F("i" + String(id)).length() > 0 ? data->invertRelayState = true
                                            : data->invertRelayState = false;

  if (server.arg(F("o" + String(id)).length() > 0) {
    data->type = server.arg(F("o" + String(id)).toInt();
  }

  if (server.arg(F("x" + String(id)).length() > 0) {
    data->idx = server.arg(F("x" + String(id)).toInt();
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWebServer::get(LED &data) {
  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_CONFIG_HARDWARE_LED_0_DEFAULT_GPIO;

  data.changeToOppositeValue = server.arg(F("w")).length() > 0 ? true : false;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  data.mcp23017.gpio = server.arg(F("mg")).length() > 0
                           ? server.arg(F("mg")).toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.id = server.arg(F("a")).length() > 0
                         ? server.arg(F("a")).toInt()
                         : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
}

uint8_t AFEWebServer::getSystemLEDData() {
  return server.arg(F("l")).length() > 0 ? server.arg(F("l")).toInt()
                                         : AFE_HARDWARE_ITEM_NOT_EXIST;
}
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEWebServer::get(DS18B20 &data) {
  AFESensorDS18B20 _Sensor;
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  if (server.arg(F("a")).length() > 0) {
    char address[17];
    server.arg(F("a")).toCharArray(address, 17);
    _Sensor.addressToInt(address, data.address);
  } else {
    _Sensor.addressNULL(data.address);
  }
  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_GPIO;

  data.correction =
      server.arg(F("k")).length() > 0
          ? server.arg(F("k")).toFloat()
          : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_TEMPERATURE_CORRECTION;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_INTERVAL;

  data.unit = server.arg(F("u")).length() > 0 ? server.arg(F("u")).toInt()
                                              : AFE_TEMPERATURE_UNIT_CELSIUS;

  data.sendOnlyChanges = server.arg(F("s")).length() > 0 ? true : false;

  data.resolution = server.arg(F("r")).length() > 0
                        ? server.arg(F("r")).toInt()
                        : AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_RESOLUTION;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("x")).length() > 0
                          ? server.arg(F("x")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}

#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEWebServer::get(DHT &data) {

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_CONFIG_HARDWARE_DHT_DEFAULT_GPIO;

  data.type = server.arg(F("t")).length() > 0 ? server.arg(F("t")).toInt()
                                              : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_DHT_DEFAULT_INTERVAL;

  data.temperature.correction =
      server.arg(F("tc")).length() > 0
          ? server.arg(F("tc")).toFloat()
          : AFE_CONFIG_HARDWARE_DHT_DEFAULT_TEMPERATURE_CORRECTION;

  data.temperature.unit = server.arg(F("tu")).length() > 0
                              ? server.arg(F("tu")).toInt()
                              : AFE_TEMPERATURE_UNIT_CELSIUS;

  data.humidity.correction =
      server.arg(F("hc")).length() > 0
          ? server.arg(F("hc")).toFloat()
          : AFE_CONFIG_HARDWARE_DHT_DEFAULT_HUMIDITY_CORRECTION;

  data.humidity.unit = server.arg(F("hu")).length() > 0
                           ? server.arg(F("hu")).toInt()
                           : AFE_HUMIDITY_UNIT;

  data.sendOnlyChanges = server.arg(F("s")).length() > 0 ? true : false;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.temperature.idx = server.arg(F("i1")).length() > 0
                                      ? server.arg(F("i1")).toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.humidity.idx = server.arg(F("i2")).length() > 0
                                   ? server.arg(F("i2")).toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.dewPoint.idx = server.arg(F("i3")).length() > 0
                                   ? server.arg(F("i3")).toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.heatIndex.idx = server.arg(F("i4")).length() > 0
                                    ? server.arg(F("i4")).toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperatureHumidity.idx = server.arg(F("i5")).length() > 0
                                              ? server.arg(F("i5")).toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.perception.idx = server.arg(F("i6")).length() > 0
                                     ? server.arg(F("i6")).toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.comfort.idx = server.arg(F("i7")).length() > 0
                                  ? server.arg(F("i7")).toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.absoluteHumidity.idx = server.arg(F("i8")).length() > 0
                                           ? server.arg(F("i8")).toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;

#else
  if (server.arg(F("m")).length() > 0) {
    server.arg(F("m")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_UART
void AFEWebServer::getSerialPortData(SERIALPORT *data) {

  data->RXD = server.arg(F("r")).length() > 0
                  ? server.arg(F("r")).toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD;
  data->TXD = server.arg(F("t")).length() > 0
                  ? server.arg(F("t")).toInt()
                  : AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD;
}
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFEWebServer::get(I2CPORT &data) {
  data.SDA = server.arg(F("a")).length() > 0
                 ? server.arg(F("a")).toInt()
                 : AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SDA;
  data.SCL = server.arg(F("l")).length() > 0
                 ? server.arg(F("l")).toInt()
                 : AFE_CONFIG_HARDWARE_I2C_0_DEFAULT_SCL;
#ifdef AFE_ESP32
  data.frequency = server.arg(F("f")).length() > 0
                       ? server.arg(F("f")).toInt()
                       : AFE_CONFIG_HARDWARE_I2C_DEFAULT_FREQUENCY;
#endif
}
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEWebServer::get(HPMA115S0 &data) {
  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL;

  data.timeToMeasure =
      server.arg(F("m")).length() > 0
          ? server.arg(F("m")).toInt()
          : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE;

  data.whoPM10Norm = server.arg(F("n1")).length() > 0
                         ? server.arg(F("n1")).toFloat()
                         : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM10;

  data.whoPM25Norm = server.arg(F("n2")).length() > 0
                         ? server.arg(F("n2")).toFloat()
                         : AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_WHO_NORM_PM25;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.pm25.idx = server.arg(F("x2")).length() > 0
                               ? server.arg(F("x2")).toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.pm10.idx = server.arg(F("x1")).length() > 0
                               ? server.arg(F("x1")).toInt()
                               : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.whoPM10Norm.idx = server.arg(F("x3")).length() > 0
                                      ? server.arg(F("x3")).toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.whoPM25Norm.idx = server.arg(F("x4")).length() > 0
                                      ? server.arg(F("x4")).toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
};
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEWebServer::get(BMEX80 &data) {
  data.type = server.arg(F("b")).length() > 0 ? server.arg(F("b")).toInt()
                                              : AFE_BMX_UNKNOWN_SENSOR;

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  data.wirePortId = server.arg(F("wr")).length() > 0
                        ? server.arg(F("wr")).toInt()
                        : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.i2cAddress =
      server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt() : 0;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL;
#ifndef AFE_ESP32
  data.resolution = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                    : BMP085_ULTRAHIGHRES;
#endif // AFE_ESP32
  data.seaLevelPressure = server.arg(F("s")).length() > 0
                              ? server.arg(F("s")).toInt()
                              : AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE;

  data.altitude =
      server.arg(F("hi")).length() > 0 ? server.arg(F("hi")).toInt() : 0;

  data.temperature.unit = server.arg(F("tu")).length() > 0
                              ? server.arg(F("tu")).toInt()
                              : AFE_TEMPERATURE_UNIT_CELSIUS;

  data.temperature.correction =
      server.arg(F("tc")).length() > 0 ? server.arg(F("tc")).toFloat() : 0;

  data.humidity.correction =
      server.arg(F("hc")).length() > 0 ? server.arg(F("hc")).toFloat() : 0;

  data.pressure.unit = server.arg(F("pu")).length() > 0
                           ? server.arg(F("pu")).toInt()
                           : AFE_PRESSURE_UNIT_HPA;
  data.pressure.correction =
      server.arg(F("pc")).length() > 0 ? server.arg(F("pc")).toFloat() : 0;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.temperatureHumidityPressure.idx =
      server.arg(F("i0")).length() > 0 ? server.arg(F("i0")).toInt()
                                       : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperature.idx = server.arg(F("i1")).length() > 0
                                      ? server.arg(F("i1")).toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.humidity.idx = server.arg(F("i2")).length() > 0
                                   ? server.arg(F("i2")).toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.dewPoint.idx = server.arg(F("i3")).length() > 0
                                   ? server.arg(F("i3")).toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.heatIndex.idx = server.arg(F("i4")).length() > 0
                                    ? server.arg(F("i4")).toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.pressure.idx = server.arg(F("i5")).length() > 0
                                   ? server.arg(F("i5")).toInt()
                                   : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.relativePressure.idx = server.arg(F("i6")).length() > 0
                                           ? server.arg(F("i6")).toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.iaq.idx = server.arg(F("i7")).length() > 0
                              ? server.arg(F("i7")).toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.staticIaq.idx = server.arg(F("i8")).length() > 0
                                    ? server.arg(F("i8")).toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.co2Equivalent.idx = server.arg(F("i9")).length() > 0
                                        ? server.arg(F("i9")).toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.breathVocEquivalent.idx = server.arg(F("i10")).length() > 0
                                              ? server.arg(F("i10")).toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.gasResistance.idx = server.arg(F("i11")).length() > 0
                                        ? server.arg(F("i11")).toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.temperatureHumidity.idx = server.arg(F("i12")).length() > 0
                                              ? server.arg(F("i12")).toInt()
                                              : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.perception.idx = server.arg(F("i13")).length() > 0
                                     ? server.arg(F("i13")).toInt()
                                     : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.comfort.idx = server.arg(F("i14")).length() > 0
                                  ? server.arg(F("i14")).toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.absoluteHumidity.idx = server.arg(F("i15")).length() > 0
                                           ? server.arg(F("i15")).toInt()
                                           : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEWebServer::get(BH1750_CONFIG &data) {

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  data.wirePortId = server.arg(F("wr")).length() > 0
                        ? server.arg(F("wr")).toInt()
                        : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.i2cAddress =
      server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt() : 0;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL;

  data.mode = server.arg(F("m")).length() > 0
                  ? server.arg(F("m")).toInt()
                  : AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx = server.arg(F("d")).length() > 0
                          ? server.arg(F("d")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEWebServer::get(AS3935 &data) {

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.i2cAddress = server.arg(F("a")).length() > 0
                        ? server.arg(F("a")).toInt()
                        : AFE_CONFIG_HARDWARE_I2C_DEFAULT_NON_EXIST_ADDRESS;

  data.irqGPIO =
      server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : 0;

  data.setNoiseFloorAutomatically =
      server.arg(F("f")).length() > 0 ? true : false;

  data.noiseFloor = server.arg(F("nf")).length() > 0
                        ? server.arg(F("nf")).toInt()
                        : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_NOISE_FLOOR;

  data.minimumNumberOfLightningSpikes =
      server.arg(F("m")).length() > 0
          ? server.arg(F("m")).toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_MINIMUM_NO_OF_SPIKES;

  data.watchdogThreshold =
      server.arg(F("e")).length() > 0
          ? server.arg(F("e")).toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_WATCHDOG_THRESHOLD;

  data.spikesRejectionLevel =
      server.arg(F("s")).length() > 0
          ? server.arg(F("s")).toInt()
          : AFE_CONFIG_HARDWARE_AS3935_DEFAULT_SPIKES_REJECTION_LEVEL;

  data.indoor =
      server.arg(F("w")).length() > 0 && server.arg(F("w")).toInt() == 1
          ? true
          : false;

  data.unit = server.arg(F("u")).length() > 0 ? server.arg(F("u")).toInt()
                                              : AFE_DISTANCE_KM;
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx =
      server.arg(F("d")).length() > 0 ? server.arg(F("d")).toInt() : 0;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEWebServer::get(ANEMOMETER &data) {
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_HARDWARE_ANEMOMETER_DEFAULT_GPIO;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_HARDWARE_ANEMOMETER_DEFAULT_INTERVAL;

  data.sensitiveness = server.arg(F("s")).length() > 0
                           ? server.arg(F("s")).toInt()
                           : AFE_HARDWARE_ANEMOMETER_DEFAULT_BOUNCING;

  data.impulseDistance = server.arg(F("l")).length() > 0
                             ? server.arg(F("l")).toFloat()
                             : AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE;

  data.impulseDistanceUnit =
      server.arg(F("u")).length() > 0
          ? server.arg(F("u")).toInt()
          : AFE_HARDWARE_ANEMOMETER_DEFAULT_IMPULSE_DISTANCE_UNIT;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx =
      server.arg(F("x")).length() > 0 ? server.arg(F("x")).toInt() : 0;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEWebServer::get(ADCINPUT &data) {
  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_GPIO;

  data.interval = server.arg(F("v")).length() > 0
                      ? server.arg(F("v")).toInt()
                      : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_INTERVAL;

#ifdef AFE_ESP32
  if (server.arg(F("l")).length() > 0) {
    server.arg(F("l")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
#endif

  data.numberOfSamples =
      server.arg(F("n")).length() > 0
          ? server.arg(F("n")).toInt()
          : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_NUMBER_OF_SAMPLES;

  data.maxVCC = server.arg(F("m")).length() > 0
                    ? server.arg(F("m")).toFloat()
                    : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_MAX_VCC;

  data.divider.Ra =
      server.arg(F("ra")).length() > 0 ? server.arg(F("ra")).toFloat() : 0;

  data.divider.Rb =
      server.arg(F("rb")).length() > 0 ? server.arg(F("rb")).toFloat() : 0;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.raw = server.arg(F("x0")).length() > 0
                          ? server.arg(F("x0")).toInt()
                          : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.percent = server.arg(F("x1")).length() > 0
                              ? server.arg(F("x1")).toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.voltage = server.arg(F("x2")).length() > 0
                              ? server.arg(F("x2")).toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;
  data.domoticz.voltageCalculated = server.arg(F("x3")).length() > 0
                                        ? server.arg(F("x3")).toInt()
                                        : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  data.battery.minVoltage =
      server.arg(F("lv")).length() > 0
          ? server.arg(F("lv")).toFloat()
          : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_BATTER_MIN_V;

  data.battery.maxVoltage =
      server.arg(F("hv")).length() > 0
          ? server.arg(F("hv")).toFloat()
          : AFE_CONFIG_HARDWARE_ANALOG_INPUT_DEFAULT_BATTER_MAX_V;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.battery.domoticz.idx = server.arg(F("x")).length() > 0
                                  ? server.arg(F("x")).toInt()
                                  : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("bt")).length() > 0) {
    server.arg(F("bt")).toCharArray(data.battery.mqtt.topic,
                                    sizeof(data.battery.mqtt.topic));
  } else {
    data.battery.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
}
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEWebServer::get(RAINMETER &data) {

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.gpio = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_HARDWARE_RAINMETER_DEFAULT_GPIO;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_HARDWARE_RAINMETER_DEFAULT_INTERVAL;

  data.sensitiveness = server.arg(F("s")).length() > 0
                           ? server.arg(F("s")).toInt()
                           : AFE_HARDWARE_RAINMETER_DEFAULT_BOUNCING;

  data.resolution = server.arg(F("r")).length() > 0
                        ? server.arg(F("r")).toFloat()
                        : (float)AFE_HARDWARE_RAINMETER_DEFAULT_RESOLUTION;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx =
      server.arg(F("x")).length() > 0 ? server.arg(F("x")).toInt() : 0;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEWebServer::get(BINARY_SENSOR &data) {
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.bouncing = server.arg(F("b")).length() > 0
                      ? server.arg(F("b")).toInt()
                      : AFE_HARDWARE_BINARY_SENSOR_DEFAULT_BOUNCING;

  data.revertSignal = server.arg(F("rs")).length() > 0 ? true : false;
  data.internalPullUp = server.arg(F("pr")).length() > 0 ? true : false;
  data.sendAsSwitch = server.arg(F("ss")).length() > 0 ? true : false;

  data.gpio = server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : 0;

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  data.mcp23017.gpio = server.arg(F("mg")).length() > 0
                           ? server.arg(F("mg")).toInt()
                           : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.mcp23017.id = server.arg(F("a")).length() > 0
                         ? server.arg(F("a")).toInt()
                         : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.idx =
      server.arg(F("x")).length() > 0 ? server.arg(F("x")).toInt() : 0;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
uint16_t AFEWebServer::getOTAFirmwareId() {
  return server.arg(F("f")).length() > 0 ? server.arg(F("f")).toInt() : 0;
}
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
void AFEWebServer::get(PN532_SENSOR &data) {
  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

  data.tx = server.arg(F("tx")).length() > 0 ? server.arg(F("tx")).toInt()
                                             : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.rx = server.arg(F("rx")).length() > 0 ? server.arg(F("rx")).toInt()
                                             : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.requestProcessingTime =
      server.arg(F("f")).length() > 0
          ? server.arg(F("f")).toInt()
          : AFE_HARDWARE_PN532_DEFUALT_REQUEST_PROCESSING_TIME;

  data.timeout = server.arg(F("w")).length() > 0
                     ? server.arg(F("w")).toInt()
                     : AFE_HARDWARE_PN532_DEFUALT_TIMEOUT;

  data.listenerTimeout = server.arg(F("b")).length() > 0
                             ? server.arg(F("b")).toInt()
                             : AFE_HARDWARE_PN532_DEFUALT_LISTENER_TIMEOUT;

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  data.wirePortId = server.arg(F("wr")).length() > 0
                        ? server.arg(F("wr")).toInt()
                        : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.i2cAddress =
      server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt() : 0;

  data.interface = server.arg(F("d")).length() > 0
                       ? server.arg(F("d")).toInt()
                       : AFE_HARDWARE_PN532_DEFAULT_INTERFACE;

#ifdef AFE_CONFIG_HARDWARE_LED
  data.ledID = server.arg(F("l")).length() > 0 ? server.arg(F("l")).toInt()
                                               : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}

void AFEWebServer::processMiFareCard() {
  AFESensorPN532 PN532Sensor;
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
  PN532Sensor.begin(0, Data, Device, WirePort0, WirePort1);
#else
  PN532Sensor.begin(0, Data, Device, WirePort0);
#endif // AFE_ESP32
#else
  PN532Sensor.begin(0, Data, Device);
#endif // AFE_CONFIG_HARDWARE_I2C

  char tag[AFE_HARDWARE_PN532_BLOCK_SIZE];
  char label[3];

  for (uint8_t i = 0;
       i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
    sprintf(label, "t%d", i);
    if (server.arg(label).length() > 0) {
      server.arg(label).toCharArray(tag, AFE_HARDWARE_PN532_BLOCK_SIZE + 1);
    } else {
      tag[0] = AFE_EMPTY_STRING;
    }

#ifdef DEBUG
    Serial << endl
           << F("INFO: PN532: Writing to block: "
           << AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i << F(" : ") << tag;
#endif
    PN532Sensor.writeBlock(AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i, tag);
  }

  for (uint8_t i = 0;
       i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
    sprintf(label, "t%d", i + 4);
    if (server.arg(label).length() > 0) {
      server.arg(label).toCharArray(tag, AFE_HARDWARE_PN532_BLOCK_SIZE + 1);
    } else {
      tag[0] = AFE_EMPTY_STRING;
    }

#ifdef DEBUG
    Serial << endl
           << F("INFO: PN532: Writing to block: "
           << AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i << F(" : ") << tag;
#endif
    PN532Sensor.writeBlock(AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i, tag);
  }
}

void AFEWebServer::get(MIFARE_CARD &data) {
  if (server.arg(F("m")).length() > 0) {
    server.arg(F("m")).toCharArray(data.cardId, sizeof(data.cardId));
  } else {
    data.cardId[0] = AFE_EMPTY_STRING;
  }

  data.action = server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt()
                                                : AFE_HARDWARE_ITEM_NOT_EXIST;

  data.sendAsSwitch = server.arg(F("s")).length() > 0
                          ? server.arg(F("s")).toInt()
                          : AFE_HARDWARE_MIFARE_CARD_DEFAULT_SEND_AS;

  data.howLongKeepState =
      server.arg(F("h")).length() > 0
          ? server.arg(F("h")).toInt()
          : AFE_HARDWARE_MIFARE_CARD_DEFAULT_HOW_LONG_KEEP_STATE;

  data.relayId = server.arg(F("r")).length() > 0 ? server.arg(F("r")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  char label[3];

  for (uint8_t i = 0; i < AFE_HARDWARE_PN532_TAG_SIZE; i++) {
    sprintf(label, "x%d", i);
    data.domoticz[i].idx =
        server.arg(label).length() > 0 ? server.arg(label).toInt() : 0;
  }

#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEWebServer::get(CLED &data) {
  data.gpio =
      server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt() : AFE_NONE;

  data.ledNumbers = server.arg(F("l")).length() > 0
                        ? server.arg(F("l")).toInt()
                        : AFE_CONFIG_HARDWARE_CLED_MAX_NUMBER_OF_LED;

  data.on.color.red = server.arg(F("or")).length() > 0
                          ? server.arg(F("or")).toInt()
                          : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.green = server.arg(F("og")).length() > 0
                            ? server.arg(F("og")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.blue = server.arg(F("ob")).length() > 0
                           ? server.arg(F("ob")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.off.color.red = server.arg(F("fr")).length() > 0
                           ? server.arg(F("fr")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.green = server.arg(F("fg")).length() > 0
                             ? server.arg(F("fg")).toInt()
                             : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.blue = server.arg(F("fb")).length() > 0
                            ? server.arg(F("fb")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.on.brightness = server.arg(F("ol")).length() > 0
                           ? server.arg(F("ol")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_BRIGHTNESS;

  data.off.brightness = server.arg(F("fl")).length() > 0
                            ? server.arg(F("fl")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_BRIGHTNESS;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.cled.idx = server.arg(F("cd")).length() > 0 ? server.arg(F("cd")).toInt()
                                                   : AFE_DOMOTICZ_DEFAULT_IDX;
  data.effect.idx = server.arg(F("ed")).length() > 0
                        ? server.arg(F("ed")).toInt()
                        : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("ct")).length() > 0) {
    server.arg(F("ct")).toCharArray(data.cled.topic, sizeof(data.cled.topic));
  } else {
    data.cled.topic[0] = AFE_EMPTY_STRING;
  }
  if (server.arg(F("et")).length() > 0) {
    server.arg(F("et")).toCharArray(data.effect.topic,
                                    sizeof(data.effect.topic));
  } else {
    data.effect.topic[0] = AFE_EMPTY_STRING;
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  data.brightnessConversion =
      server.arg(F("bc")).length() > 0
          ? server.arg(F("bc")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_DEFAULT_BRIGHTNESS_CONVERSION;
#endif

#endif
}

void AFEWebServer::get(CLED_EFFECT_BLINKING &data) {

  data.on.color.red = server.arg(F("or")).length() > 0
                          ? server.arg(F("or")).toInt()
                          : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.green = server.arg(F("og")).length() > 0
                            ? server.arg(F("og")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.blue = server.arg(F("ob")).length() > 0
                           ? server.arg(F("ob")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.off.color.red = server.arg(F("fr")).length() > 0
                           ? server.arg(F("fr")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.green = server.arg(F("fg")).length() > 0
                             ? server.arg(F("fg")).toInt()
                             : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.blue = server.arg(F("fb")).length() > 0
                            ? server.arg(F("fb")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.on.brightness =
      server.arg(F("ol")).length() > 0
          ? server.arg(F("ol")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_BRIGHTNESS;

  data.off.brightness =
      server.arg(F("fl")).length() > 0
          ? server.arg(F("fl")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_BRIGHTNESS;

  data.onTimeout =
      server.arg(F("ot")).length() > 0
          ? server.arg(F("ot")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_ON_TIMER;

  data.offTimeout =
      server.arg(F("ft")).length() > 0
          ? server.arg(F("ft")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING_DEFAULT_OFF_TIMER;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}

void AFEWebServer::get(CLED_EFFECT_WAVE &data) {
  data.on.color.red = server.arg(F("or")).length() > 0
                          ? server.arg(F("or")).toInt()
                          : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.green = server.arg(F("og")).length() > 0
                            ? server.arg(F("og")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.on.color.blue = server.arg(F("ob")).length() > 0
                           ? server.arg(F("ob")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.off.color.red = server.arg(F("fr")).length() > 0
                           ? server.arg(F("fr")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.green = server.arg(F("fg")).length() > 0
                             ? server.arg(F("fg")).toInt()
                             : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.off.color.blue = server.arg(F("fb")).length() > 0
                            ? server.arg(F("fb")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_OFF_COLOR;

  data.on.brightness =
      server.arg(F("b")).length() > 0
          ? server.arg(F("b")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_BRIGHTNESS;

  data.timeout =
      server.arg(F("z")).length() > 0
          ? server.arg(F("z")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE_DEFAULT_WAVE_TIMEOUT;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}

void AFEWebServer::get(CLED_EFFECT_FADE_INOUT &data) {
  data.in.color.red = server.arg(F("or")).length() > 0
                          ? server.arg(F("or")).toInt()
                          : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.in.color.green = server.arg(F("og")).length() > 0
                            ? server.arg(F("og")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.in.color.blue = server.arg(F("ob")).length() > 0
                           ? server.arg(F("ob")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_DEFAULT_ON_COLOR;

  data.in.brightness =
      server.arg(F("ol")).length() > 0
          ? server.arg(F("ol")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_IN_BRIGHTNESS;

  data.out.brightness =
      server.arg(F("fl")).length() > 0
          ? server.arg(F("fl")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_OUT_BRIGHTNESS;

  data.timeout =
      server.arg(F("z")).length() > 0
          ? server.arg(F("z")).toInt()
          : AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT_DEFAULT_FADE_TIMEOUT;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}

#ifdef AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
void AFEWebServer::get(CLED &CLEDData, CLED_BACKLIGHT &CLEDBacklightData) {
  CLEDData.gpio = server.arg(F("g")).length() > 0 ? server.arg(F("g")).toInt()
                                                  : AFE_HARDWARE_ITEM_NOT_EXIST;

  CLEDData.colorOrder = server.arg(F("o")).length() > 0
                            ? server.arg(F("o")).toInt()
                            : AFE_CONFIG_HARDWARE_CLED_COLORS_ORDER;

  CLEDData.chipset =
      server.arg(F("m")).length() > 0 ? server.arg(F("m")).toInt() : 0;

  CLEDData.ledNumber = server.arg(F("l")).length() > 0
                           ? server.arg(F("l")).toInt()
                           : AFE_CONFIG_HARDWARE_CLED_8_LEDS;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  CLEDData.domoticz.idx = server.arg(F("d")).length() > 0
                              ? server.arg(F("d")).toInt()
                              : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(CLEDData.mqtt.topic,
                                   sizeof(CLEDData.mqtt.topic));
  } else {
    CLEDData.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  char _label[3];

  CLEDBacklightData.lightSensorId = server.arg(F("s")).length() > 0
                                        ? server.arg(F("s")).toInt()
                                        : AFE_HARDWARE_ITEM_NOT_EXIST;

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_BACKLIGHT_LEVELS;
       i++) {
    sprintf(_label, "b%d", i);
    CLEDBacklightData.config[i].brightness =
        server.arg(_label).length() > 0 ? server.arg(_label).toInt() : 0;

    sprintf(_label, "l%d", i);
    CLEDBacklightData.config[i].luxLevel =
        server.arg(_label).length() > 0 ? server.arg(_label).toInt() : 0;

    sprintf(_label, "k%d", i);
    CLEDBacklightData.config[i].color =
        server.arg(_label).length() > 0 ? server.arg(_label).toInt() : 0;
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED_LIGHT_CONTROLLED_EFFECT
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEWebServer::get(TSL2561 &data) {

#if defined(AFE_CONFIG_HARDWARE_I2C) && defined(AFE_ESP32)
  data.wirePortId = server.arg(F("wr")).length() > 0
                        ? server.arg(F("wr")).toInt()
                        : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.i2cAddress =
      server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt() : 0;

  data.interval = server.arg(F("f")).length() > 0
                      ? server.arg(F("f")).toInt()
                      : AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_INTERVAL;

  data.sensitiveness = server.arg(F("s")).length() > 0
                           ? server.arg(F("s")).toInt()
                           : AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_SENSITIVENESS;

  data.gain = server.arg(F("g")).length() > 0
                  ? server.arg(F("g")).toInt()
                  : AFE_CONFIG_HARDWARE_TSL2561_DEFAULT_GAIN;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  data.domoticz.ir.idx = server.arg(F("d3")).length() > 0
                             ? server.arg(F("d3")).toInt()
                             : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.illuminance.idx = server.arg(F("d1")).length() > 0
                                      ? server.arg(F("d1")).toInt()
                                      : AFE_DOMOTICZ_DEFAULT_IDX;

  data.domoticz.broadband.idx = server.arg(F("d2")).length() > 0
                                    ? server.arg(F("d2")).toInt()
                                    : AFE_DOMOTICZ_DEFAULT_IDX;
#else
  if (server.arg(F("t")).length() > 0) {
    server.arg(F("t")).toCharArray(data.mqtt.topic, sizeof(data.mqtt.topic));
  } else {
    data.mqtt.topic[0] = AFE_EMPTY_STRING;
  }
#endif

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
void AFEWebServer::get(MCP23XXX &data) {

#if defined(AFE_ESP32)
  data.wirePortId = server.arg(F("wr")).length() > 0
                        ? server.arg(F("wr")).toInt()
                        : AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  data.address = server.arg(F("a")).length() > 0 ? server.arg(F("a")).toInt()
                                                 : AFE_HARDWARE_ITEM_NOT_EXIST;

  if (server.arg(F("n")).length() > 0) {
    server.arg(F("n")).toCharArray(data.name, sizeof(data.name));
  } else {
    data.name[0] = AFE_EMPTY_STRING;
  }
}
#endif
