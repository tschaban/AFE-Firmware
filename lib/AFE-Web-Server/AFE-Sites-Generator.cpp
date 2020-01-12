/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() {}

void AFESitesGenerator::begin(AFEDevice *_Device, AFEFirmwarePro *_Firmware) {
  firmware = Data.getFirmwareConfiguration();
  Device = _Device;
  Firmware = _Firmware;
  Data.getDeviceUID().toCharArray(deviceID, sizeof(deviceID) + 1);
}

const String AFESitesGenerator::generateHeader(uint8_t redirect) {

  String page = "<!doctype html><html lang=\"";
  page += L_LANGUAGE;
  page += "\"><head><meta charset=\"utf-8\">";

  if (redirect > 0) {
    page += "<meta http-equiv=\"refresh\" content=\"";
    page += String(redirect);
    page += "; url=/\">";
  }

  page += "<title>AFE Firmware ";
  page += firmware.version;
  page += " [T";
  page += firmware.type;
  page += "]</title><style>";
  page += AFE_CSS;
  page += "</style></head><body>";

  if (Device->getMode() == AFE_MODE_CONFIGURATION ||
      Device->getMode() == AFE_MODE_NORMAL) {
    page += "<img src=\"http://api.smartnydom.pl/logo/T";
    page += firmware.type;
    page += "/";
    page += firmware.version;
    page += "/";
    page += deviceID;
    page += "/";
    page += AFE_DEVICE_TYPE_ID;
    page += "\" style=\"width: 100%;display: block\" alt=\"AFE Firmware\">";
  } else {
    page += "<h3 class=\"la\">AFE Firmware: ";
    page += AFE_DEVICE_TYPE_NAME;
    page += "</h3>";
  }

  page += "<div id=\"c\">";

  return page;
}

const String AFESitesGenerator::generateOneColumnLayout(uint8_t redirect) {
  String page = generateHeader(redirect);
  page += "<div id=\"r\">";
  return page;
}

const String AFESitesGenerator::generateTwoColumnsLayout(uint8_t redirect) {
  String page = generateHeader(redirect);
  page += "<div id=\"l\">";
  if (Device->getMode() == AFE_MODE_ACCESS_POINT) {
    page += "<h3 class=\"ltit\">AFE FIRMWARE</h3>";
  }
  page += "<h4>";
  page += L_FIRMWARE_NAME;
  page += "</h4><ul class=\"lst\">";

  Device->begin();
  page += "<li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_DEVICE;
  page += "\">";
  page += L_DEVICE;
  page += "</a></li><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_NETWORK;
  page += "\">";
  page += L_NETWORK;
  page += "</a></li>";

  if (

      Device->configuration.api.mqtt) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_MQTT;
    page += "\">";
    page += L_MQTT_BROKER;
    page += "</a></li>";
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_DOMOTICZ;
    page += "\">";
    page += L_DOMOTICZ_SERVER;
    page += "</a></li>";
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  if (Device->configuration.noOfLEDs > 0) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_LED;
    page += "\">";
    page += L_LEDS;
    page += "</a></li>";
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (Device->configuration.noOfGates > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_GATE_CONFIGURATION;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < Device->configuration.noOfGates; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_GATE;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_GATE;
      page += ": ";
      page += i + 1;
      page += "</a></li>";
    }
  }
#endif

  /* Relay */

  if (Device->configuration.noOfRelays > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_RELAYS_CONFIGURATION;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_RELAY;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_RELAY;
      page += ": ";
      page += i + 1;
      page += "</a></li>";

/* Thermostat */
#if defined(AFE_CONFIG_HARDWARE_DS18B20) &&                                    \
    defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT)
      if (Device->configuration.isDS18B20) {
        page += addThermostateMenuItem();
      }
#endif

/* Humidistat */
#if defined(AFE_CONFIG_HARDWARE_DHXX) &&                                       \
    defined(AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT)
      if (Device->configuration.isDHT) {
        page += addThermostateMenuItem();
        page += addHumidistatMenuItem();
      }
#endif
    }
  }

  if (Device->configuration.noOfSwitches > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_BUTTONS_SWITCHES;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < Device->configuration.noOfSwitches; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_SWITCH;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_SWITCH;
      page += ": ";
      page += i + 1;
      page += "</a></li>";
    }
  }

/* Pir */
#if defined(T3_CONFIG)
  itemPresent = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    if (Device->configuration.isPIR[i]) {
      itemPresent++;
    } else {
      break;
    }
  }
  if (itemPresent > 0) {
    page += "<li class=\"itm\"><a><i>Konfiguracja czujników ruchu "
            "(PIR)</i></a></li>";
    for (uint8_t i = 0; i < 4; i++) {
      if (Device->configuration.isPIR[i]) {
        page += "<li class=\"itm\"><a href=\"\\?o=pir";
        page += i;
        page += "\">&#8227; Czujnik: ";
        page += i + 1;
        page += "</a></li>";
      }
    }
  }
#endif

/* Contactrons and Gate */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

  if (Device->configuration.noOfContactrons > 0) {
    page += "<li class=\"itm\"><a><i>";
    page += L_MAGNETIC_SENSORS;
    page += "</i></a></li>";
    for (uint8_t i = 0; i < Device->configuration.noOfContactrons; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_CONTACTRON;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_SENSOR;
      page += ": ";
      page += i + 1;
      page += "</a></li>";
    }
  }

#endif

/* Sensor DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  if (Device->configuration.isDS18B20) {
    page += "<li class=\"itm\"><a href=\"\\?o=ds18b20\">";
    page += language == 0 ? "Czujnik temperatury" : "Temperature sensor";
    page += "</a></li>";
  }
#endif

/* Sensor DHxx */
#ifdef AFE_CONFIG_HARDWARE_DHXX
  if (Device->configuration.isDHT) {
    page += "<li class=\"itm\"><a href=\"\\?o=DHT\">";
    page += language == 0 ? "Czujnik DHT" : "DHT sensor";
    page += "</a></li>";
  }
#endif

/* UART */
#ifdef AFE_CONFIG_HARDWARE_UART

/* Don't show it if HPMA115SO sensor is not added to the device */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  if (Device->configuration.noOfHPMA115S0s > 0) {
#endif

    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_UART;
    page += "\">UART</a></li>";

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  }
#endif

#endif

/* I2C */
#ifdef AFE_CONFIG_HARDWARE_I2C
/* Don't show it if I2C sensor is not added to the device, this is check for AFE
 * T6 only*/
#ifdef T6_CONFIG
  if (Device->configuration.noOfBMEX80s > 0 ||
      Device->configuration.noOfBH1750s > 0 ||
      Device->configuration.noOfAS3935s > 0) {
#endif

    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_I2C;
    page += "\">I2C</a></li>";

#ifdef T6_CONFIG
  }
#endif

#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  /* This is hardcoded for one sensor */
  if (Device->configuration.noOfHPMA115S0s > 0) {
    page += "<li class=\"itm\"><a href=\"\\?i=0&o=";
    page += AFE_CONFIG_SITE_HPMA115S0;
    page += "\">";
    page += L_PARTICLE_SENSOR;
    page += "</a></li>";
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  if (Device->configuration.noOfBMEX80s > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_BMEX80_SENSORS;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < Device->configuration.noOfBMEX80s; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_BMEX80;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_SENSOR;
      page += ": ";
      page += i + 1;
      page += "</a></li>";
    }
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  if (Device->configuration.noOfBH1750s > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_BH1750_SENSORS;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < Device->configuration.noOfBH1750s; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=";
      page += AFE_CONFIG_SITE_BH1750;
      page += "&i=";
      page += i;
      page += "\">&#8227; ";
      page += L_SENSOR;
      page += ": ";
      page += i + 1;
      page += "</a></li>";
    }
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  if (Device->configuration.noOfAS3935s > 0 && Firmware->Pro.valid) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_AS3935;
    page += "\">";
    page += L_AS3935_SENSOR;
    page += "</a></li>";
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (Device->configuration.isAnalogInput && Firmware->Pro.valid) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_ANALOG_INPUT;
    page += "\">";
    page += L_ANALOG_INPUT;
    page += "</a></li>";
  }
#endif

  page += "<br><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_PASSWORD;
  page += "\">";
  page += L_SET_PASSWORD;
  page += "</a></li>";

  /* Language, Upgrade, Exit */
  page += "<br></li><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_UPGRADE;
  page += "\">";
  page += L_FIRMWARE_UPGRADE;
  page += "</a></li><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_RESET;
  page += "\">";
  page += L_RESET_DEVICE;
  page += "</a></li><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_PRO_VERSION;
  page += "\">";
  page += L_PRO_VERSION;
  page += "</a></li><br><br><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_EXIT;
  page += "\">";
  page += L_FINISH_CONFIGURATION;

  /* Information section */
  page += "</a></li></ul></div><div id=\"r\">";

  return page;
}

String AFESitesGenerator::addDeviceConfiguration() {
  DEVICE configuration = Device->configuration;
  String page = "";

  if (Device->upgraded != AFE_UPGRADE_NONE) {
    page += "<h4 class=\"bc\" style=\"padding:5px;\">";
    switch (Device->upgraded) {
    case AFE_UPGRADE_VERSION:
      page += L_UPGRADED_TO_NEW_VERSION;
      break;
    case AFE_UPGRADE_VERSION_TYPE:
      page += L_UPGRADED_TO_NEW_VERSION_TYPE;
      break;
    }
    page += "</h4>";
    Device->upgraded = AFE_UPGRADE_NONE;
  }

  String body = "<fieldset>";
  body += addItem("text", "n", L_DEVICE_NAME, configuration.name, "16");
  body += "</fieldset>";
  page += addConfigurationBlock(L_DEVICE, L_DEVICE_SECTION_INFO, body);
  body = "<fieldset>";

/* LED */
#ifdef AFE_CONFIG_HARDWARE_LED

  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS,
                                    Device->configuration.noOfLEDs, "l",
                                    L_NUMBER_OF_LEDS);

#endif

/* Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS,
                                    Device->configuration.noOfContactrons, "co",
                                    L_NUMBER_OF_MAGNETIC_SENSORS);

#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Relay */
  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS,
                                    Device->configuration.noOfRelays, "r",
                                    L_NUMBER_OF_RELAYS);
#endif

  /* Switch */

  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES,
                                    Device->configuration.noOfSwitches, "s",
                                    L_NUMBER_OF_SWITCHES);

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isDS18B20 ? " checked=\"checked\">" : ">";
  body += " DS18B20";
  body += "</label></div>";
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  body += "<div class=\"cc\"><label><input name =\"dh\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isDHT ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " DHT";
  body += "</label></div>";
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0,
                                    Device->configuration.noOfHPMA115S0s, "hp",
                                    L_NUMBER_OF_HPMA115S0_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750,
                                    Device->configuration.noOfBH1750s, "bh",
                                    L_NUMBER_OF_BH1750_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80,
                                    Device->configuration.noOfBMEX80s, "b6",
                                    L_NUMBER_OF_BMEX80_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  if (Firmware->Pro.valid) {
    body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935,
                                      Device->configuration.noOfAS3935s, "a3",
                                      L_NUMBER_OF_AS3935_SENSORS);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (Firmware->Pro.valid) {
    body += "<div class=\"cc\"><label><input name =\"ad\" type=\"checkbox\" "
            "value=\"1\"";
    body += configuration.isAnalogInput ? " checked=\"checked\">" : ">";
    body += L_DO_MEASURE_ADC;
    body += "</label></div>";
  }

#endif

#if defined(T3_CONFIG)
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isPIR); i++) {
    if (Device->configuration.isPIR[i]) {
      itemsNumber++;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device->configuration.isPIR), itemsNumber, "p",
      language == 0 ? "Ilość czujników PIR" : "Number of PIRs");
#endif

  body += "</fieldset>";

  page += addConfigurationBlock(L_HARDWARE_CONFIGURATION,
                                L_HARDWARE_CONFIGURATION_INFO, body);

#ifdef AFE_CONFIG_HARDWARE_GATE
  body = "<fieldset>";

  body += generateHardwareItemsList(AFE_CONFIG_HARDWARE_NUMBER_OF_GATES,
                                    Device->configuration.noOfGates, "g",
                                    L_NUMBER_OF_CONTROLLED_GATES);

  body += "</fieldset>";

  page += addConfigurationBlock(L_CONTROLLED_GATES, "", body);
#endif

  body = "<fieldset>";

  body += "<div class=\"cc\"><label><input name=\"h\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.api.http ? " checked=\"checked\"" : "";
  body += ">HTTP API</label></div><div class=\"cc\"><label>";

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  body += "<input name=\"m\" type=\"radio\" "
          "value=\"1\"";
  body += configuration.api.domoticz ? " checked=\"checked\"" : "";
  body += ">Domoticz HTTP API</label></div>";
  body += "<div class=\"cc\"><label><input name=\"m\" "
          "type=\"radio\" value=\"2\"";
  body += configuration.api.mqtt ? " checked=\"checked\"" : "";
  body += ">Domoticz MQTT API";
#else
  body += "<input name=\"m\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.api.mqtt ? " checked=\"checked\"" : "";
  body += ">MQTT API</label></div><div class=\"cc\">";
#endif

  body += "</label></div></fieldset>";

  page += addConfigurationBlock(L_DEVICE_CONTROLLING, L_DEVICE_CONTROLLING_INFO,
                                body);

  return page;
}

String AFESitesGenerator::addConnectingSite() {
  NETWORK configuration = Data.getNetworkConfiguration();
  String body = "<p>";
  body += L_DEVICE_CONNECTS;
  body += ": <strong>";
  body += configuration.ssid;
  body += "</strong> [";
  body += configuration.password;
  body += "]</p><ul><li>";
  body += L_CONNECT_TO;
  body += ": ";
  body += configuration.ssid;
  body += "</li><li>";
  body += L_SEARCH_FOR_IP_ADDRESS;
  body += ": ";
  body += WiFi.macAddress();
  body += "</li><li>";
  body += L_FINISH_NETWORK_CONFIGURATION;
  body += "</li></ul>";
  return body;
}

String AFESitesGenerator::addNetworkConfiguration() {

  NETWORK configuration;
  configuration = Data.getNetworkConfiguration();

  String body = "<fieldset>";
  char _ssid[sizeof(configuration.ssid)];
  body += "<div class=\"cf\"><label>";
  body += L_SSID;
  body += "</label><select name=\"s\">";
#ifdef DEBUG
  Serial << endl << "Searching for WiFi networks: ";
#endif
  int numberOfNetworks = WiFi.scanNetworks();

#ifdef DEBUG
  Serial << endl << " - found: " << numberOfNetworks;
#endif

  for (int i = 0; i < numberOfNetworks; i++) {

#ifdef DEBUG
    Serial << endl << " - " << WiFi.SSID(i);
#endif

    body += "<option value=\"";
    body += WiFi.SSID(i);
    WiFi.SSID(i).toCharArray(_ssid, sizeof(_ssid));
    body += "\"";
    body +=
        strcmp(_ssid, configuration.ssid) == 0 ? " selected=\"selected\"" : "";
    body += ">";
    body += WiFi.SSID(i);
    body += "</option>";
  }

  body += "</select><input type=\"submit\" class =\"b bc\" value=\"";
  body += L_REFRESH;
  body += "\" formaction=\"/?o=";
  body += AFE_CONFIG_SITE_NETWORK;
  body += "&c=0\"></div>";
  body += addItem("password", "p", L_PASSWORD, configuration.password, "32");

  char _ip[18];
  WiFi.macAddress().toCharArray(_ip, 18);

  body += addItem("text", "m", "MAC", _ip, "?", "?", "?", "?", "?", true);

  body += "</fieldset>";

  String page = addConfigurationBlock(L_NETWORK_CONFIGURATION,
                                      L_NETWORK_CONFIGURATION_INFO, body);

  body = "<fieldset><div class=\"cc\"><label><input name=\"d\" "
         "type=\"checkbox\" value=\"1\"";
  body += (configuration.isDHCP ? " checked=\"checked\"" : "");
  body += "> ";
  body += L_DHCP_ENABLED;
  body += "?</label></div>";

  body += addItem("text", "i1", L_IP_ADDRESS, configuration.ip);
  body += addItem("text", "i2", L_GATEWAY, configuration.gateway);
  body += addItem("text", "i3", L_SUBNET, configuration.subnet);

  body += "</fieldset>";

  page += addConfigurationBlock(L_DEVICE_IP, L_DEVICE_IP_INFO, body);

  char _int[4];
  sprintf(_int, "%d", configuration.noConnectionAttempts);

  body = "<fieldset>";
  body += addItem("number", "na", L_NUMBER_OF_CONNECTIONS, _int, "?", "1",
                  "255", "1");

  sprintf(_int, "%d", configuration.waitTimeConnections);
  body += addItem("number", "wc", L_TIME_BETWEEN_CONNECTIONS, _int, "?", "1",
                  "255", "1", L_SECONDS);

  sprintf(_int, "%d", configuration.waitTimeSeries);
  body += addItem("number", "ws", L_SLEEP_TIME, _int, "?", "1", "255", "1",
                  L_SECONDS);

  body += "</fieldset>";

  page += addConfigurationBlock(L_ADVANCED, "", body);

  return page;
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {
  MQTT configuration;
  configuration = Data.getMQTTConfiguration();

  String body = "<fieldset>";

  body += addItem("text", "h", "Hostname", configuration.host, "32");
  body += addItem("text", "i", L_IP_ADDRESS, configuration.ip);

  char _number[6];
  sprintf(_number, "%d", configuration.port);
  body += addItem("number", "p", "Port", _number, "?", "0", "65535", "1");

  body += addItem("text", "u", L_USERNAME, configuration.user, "32");
  body += addItem("password", "s", L_PASSWORD, configuration.password, "32");

  body += "</fieldset>";

  String page =
      addConfigurationBlock("MQTT Broker", L_MQTT_CONFIGURATION_INFO, body);

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  body = "<fieldset>";
  body += addItem("text", "t0", L_MQTT_TOPIC, configuration.lwt.topic, "64");
  body += "</fieldset>";
  page += addConfigurationBlock(L_MQTT_TOPIC_LWT, L_MQTT_TOPIC_EMPTY, body);
#endif

  return page;
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
String AFESitesGenerator::addDomoticzServerConfiguration() {
  DOMOTICZ configuration;
  configuration = Data.getDomoticzConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += L_PROTOCOL;
  body += "</label><select name=\"t\"><option value=\"0\"";
  body += configuration.protocol == 0 ? " selected=\"selected\"" : "";
  body += ">http://</option><option value=\"1\"";
  body += configuration.protocol == 1 ? " selected=\"selected\"" : "";
  body += ">https://</option></select></div>";

  body += addItem("text", "h", "Hostname/IP", configuration.host, "40");

  char _number[6];
  sprintf(_number, "%d", configuration.port);
  body += addItem("number", "p", "Port", _number, "?", "0", "65535", "1");
  body += addItem("text", "u", L_USERNAME, configuration.user, "32");
  body += addItem("password", "s", L_PASSWORD, configuration.password, "32");

  body += "</fieldset>";

  return addConfigurationBlock(L_DOMOTICZ_CONFIGURATION,
                               L_DOMOTICZ_CONFIGURATION_INFO, body);
}
#endif

String AFESitesGenerator::addPasswordConfigurationSite() {
  PASSWORD configuration = Data.getPasswordConfiguration();

  String body = "<fieldset><div class=\"cc\"><label><input name=\"r\" "
                "type=\"checkbox\" value=\"1\"";
  body += (configuration.protect ? " checked=\"checked\"" : "");
  body += "> ";
  body += L_PROTECT_BY_PASSWORD;
  body += "?</label></div>";

  if (configuration.protect) {
    body += addItem("text", "p", L_PASSWORD, configuration.password, "8");
  }
  body += "</fieldset>";

  return addConfigurationBlock(L_SET_PASSWORD_TO_PANEL, "", body);
}

#ifdef AFE_CONFIG_HARDWARE_LED
String AFESitesGenerator::addLEDConfiguration(uint8_t id) {
  LED configuration;
  configuration = Data.getLEDConfiguration(id);

  String body = "<fieldset>";
  char field[13];
  sprintf(field, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(field, configuration.gpio);
  body += "<label style=\"width: 300px;\"><input name=\"o";
  body += id;
  body += "\" type=\"checkbox\" value=\"1\"";
  body += configuration.changeToOppositeValue ? " checked=\"checked\"" : "";
  body += ">";
  body += L_CHANGE_LED_INDICATION;
  body += "</label></div></fieldset>";

  return addConfigurationBlock("LED #" + String(id + 1), "", body);
}

String AFESitesGenerator::addSystemLEDConfiguration() {
  uint8_t configuration = Data.getSystemLedID();

  String body = "<fieldset>";

  body += generateHardwareList(Device->configuration.noOfLEDs, configuration,
                               "s", "LED", 0, AFE_HARDWARE_ITEM_NOT_EXIST);

  body += "</fieldset>";

  return addConfigurationBlock(L_SYSTEM_LED, L_SYSTEM_LED_INFO, body);
}
#endif

String AFESitesGenerator::addRelayConfiguration(uint8_t id) {
  RELAY configuration = Data.getRelayConfiguration(id);

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE gateConfiguration;
  boolean isGateRelay = false;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES; i++) {
    gateConfiguration = Data.getGateConfiguration(i);

    if (gateConfiguration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST &&
        gateConfiguration.relayId == id) {
      isGateRelay = true;
      break;
    }
  }
#endif

  String body = "<fieldset>";

  body += generateConfigParameter_GPIO("g", configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif

    body += addItem("text", "n", L_NAME, configuration.name, "16");

    body += "<p class=\"cm\">";
    body += L_DEFAULT_VALUES;
    body += "</p><div class=\"cf\"><label>";
    body += L_DEFAULT_POWER_RESTORED;
    body += "</label><select name=\"pr\"><option value=\"0\"";
    body += (configuration.state.powerOn == 0 ? " selected=\"selected\"" : "");
    body += ">";
    body += L_NO_ACTION;
    body += "</option><option value=\"1\"";
    body += (configuration.state.powerOn == 1 ? " selected=\"selected\"" : "");
    body += ">";
    body += L_OFF;
    body += "</option><option value=\"2\"";
    body += (configuration.state.powerOn == 2 ? " selected=\"selected\"" : "");
    body += ">";
    body += L_ON;
    body += "</option><option value=\"3\"";
    body += (configuration.state.powerOn == 3 ? " selected=\"selected\"" : "");
    body += ">";
    body += L_LAST_KNOWN_STATE;
    body += "</option><option value=\"4\"";
    body += (configuration.state.powerOn == 4 ? " selected=\"selected\"" : "");
    body += ">";
    body += L_OPPOSITE_TO_LAST_KNOWN_STATE;
    body += "</option></select></div>";

    if (

        Device->configuration.api.mqtt) {

      body += "<div class=\"cf\">";
      body += "<label>";
      body += L_DEFAULT_MQTT_CONNECTED;
      body += "</label>";
      body += "<select  name=\"mc\">";
      body += "<option value=\"0\"";
      body += (configuration.state.MQTTConnected == 0 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_NO_ACTION;
      body += "</option>";
      body += "<option value=\"1\"";
      body += (configuration.state.MQTTConnected == 1 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_OFF;
      body += "</option>";
      body += "<option value=\"2\"";
      body += (configuration.state.MQTTConnected == 2 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_ON;
      body += "</option>";
      body += "<option value=\"3\"";
      body += (configuration.state.MQTTConnected == 3 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_LAST_KNOWN_STATE;
      body += "</option>";
      body += "<option value=\"4\"";
      body += (configuration.state.MQTTConnected == 4 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_OPPOSITE_TO_LAST_KNOWN_STATE;
      body += "</option>";

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      body += "<option value=\"5\"";
      body += (configuration.state.MQTTConnected == 5 ? " selected=\"selected\""
                                                      : "");
      body += ">";
      body += L_DEFAULT_GET_FROM_MQTT;
      body += "</option>";
#endif
      body += "</select>";
      body += "</div>";
    }

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif
    body += "<br><p class=\"cm\">";
    body += L_AUTOMATIC_SWITCHING_OFF;
    body += "</p>";
#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

  char _number[9];
  dtostrf(configuration.timeToOff, 1, 1, _number);

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (isGateRelay) {
    body += addItem("number", "ot", L_IMPULSE_DURATION, _number, "?", "1",
                    "99999", "1", L_MILISECONDS);
  } else {
    body += addItem("number", "ot", L_SWITCH_OFF_AFTER, _number, "?", "0",
                    "86400", "0.1", L_SECONDS);
  }
#else // Not a GATE
  body += addItem("number", "ot", L_SWITCH_OFF_AFTER, _number, "?", "0",
                  "86400", "0.1", L_SECONDS);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  if (Device->isDS18B20)
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
    if (Device->isDHT)
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
    {

      body += "<br><p class=\"cm\">";
      body += L_THERMAL_PROTECTION;
      body += "</p>";

      body += "<div class=\"cf\">";
      body += "<label>";
      body += L_SWITCH_OFF_ABOVE;
      body += "</label>";
      body += "<input name=\"tp" + String(id) +
              "\" type=\"number\" step=\"1\" min=\"-67\" max=\"259\"  value=\"";
      body += configuration.thermalProtection;
      body += "\">";
      body += "<span class=\"hint\">";
      body += L_RANGE;
      body += ": -55C : +125C (-67F : +259F). ";
      body += L_NO_ACTION_IF_0;
      body += "</span></div>";
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_HARDWARE_LED

    body += "<br><p class=\"cm\">";
    body += L_SELECT_LED_4_RELAY;
    body += "</p>";

    body += generateHardwareList(Device->configuration.noOfLEDs,
                                 configuration.ledID, "l", "LED", 0,
                                 AFE_HARDWARE_ITEM_NOT_EXIST);

#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    /* LED Exclusion for a relay assigned to ta gate */
  }
#endif

  body += "</fieldset>";

  char title[23];
  sprintf(title, "%s #%d", L_RELAY, id + 1);

  String page = addConfigurationBlock(title, "", body);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device->configuration.api.domoticz ||
        Device->configuration.api.mqtt) {

      body = "<fieldset>";
      char _idx[7];
      sprintf(_idx, "%d", configuration.domoticz.idx);
      body += addItem("number", "x", "IDX", _idx, "?", "0", "999999", "1");

      body += "</fieldset>";

      page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
    }
#else
    if (Device->configuration.api.mqtt) {
      body = "<fieldset>";
      body +=
          addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
      body += "</fieldset>";
      page +=
          addConfigurationBlock(L_RELAY_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY, body);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif
  return page;
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

String AFESitesGenerator::addRegulatorConfiguration(uint8_t type) {
  RELAY configuration = Data.getRelayConfiguration(0);

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
  if (type == THERMOSTAT_REGULATOR)
#endif
  {
    String body = generateTwoValueController(configuration.thermostat,
                                             THERMOSTAT_REGULATOR);

    return addConfigurationBlock(language == 0 ? "Termostat" : "Thermostat",
                                 language == 0
                                     ? "Termostat kontroluje przekaźnik w "
                                       "zależności od wartości temperatury"
                                     : "Thermostat controlls the relay "
                                       "depending on temperature value",
                                 body);

  }
#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
  else {
    String body = generateTwoValueController(configuration.humidistat,
                                             HUMIDISTAT_REGULATOR);

    return addConfigurationBlock(
        language == 0 ? "Regulator wilgotności" : "Humidistat",
        language == 0
            ? "Regulator wilgotności kontroluje przekaźnik w "
              "zależności od wartości wilgotności"
            : "Humidistat controlls the relay depending on humidity value",
        body);
  }
#endif
}
#endif

String AFESitesGenerator::addSwitchConfiguration(uint8_t id) {

  SWITCH configuration;
  configuration = Data.getSwitchConfiguration(id);

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE gateConfiguration;
#endif

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO("g", configuration.gpio);
  body += "</div><div class=\"cf\"><label>";
  body += L_FUNCTIONALITY;
  body += "</label><select name=\"f\"><option value=\"";
  body += AFE_SWITCH_FUNCTIONALITY_NONE;
  body += "\"";
  body += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_NONE
               ? " selected=\"selected\""
               : "");
  body += ">";
  body += L_NONE;
  body += "</option><option value=\"";
  body += AFE_SWITCH_FUNCTIONALITY_MULTI;
  body += "\"";
  body += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_MULTI
               ? " selected=\"selected\""
               : "");
  body += ">";
  body += L_SYSTEM_BUTTON;
  body += "</option>";

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_GATE)
  body += "<option value=\"";
  body += AFE_SWITCH_FUNCTIONALITY_RELAY;
  body += "\"";
  body += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_RELAY
               ? " selected=\"selected\""
               : "");
  body += ">";
  body += L_CONTROL_RELAY;
  body += "</option></select></div><div class=\"cf\"><label>";
  body += L_RELAY_CONTROLLED_BY_SWITCH;
  body += "</label><select name=\"r\"><option value=\"";
  body += AFE_HARDWARE_ITEM_NOT_EXIST;
  body += "\"";
  body += configuration.relayID == AFE_HARDWARE_ITEM_NOT_EXIST
              ? " selected=\"selected\""
              : "";
  body += ">";
  body += L_NONE;
  body += "</option>";

#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t relayIsForGate;
#endif

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    body += "<option value=\"";
    body += i;
    body += "\"";
    body += configuration.relayID == i ? " selected=\"selected\"" : "";
    body += ">";
#ifdef AFE_CONFIG_HARDWARE_GATE
    relayIsForGate = false;
    for (uint8_t j = 0; j < Device->configuration.noOfGates; j++) {
      gateConfiguration = Data.getGateConfiguration(j);
      if (i == gateConfiguration.relayId) {
        body += L_GATE;
        body += ": ";
        body += gateConfiguration.name;
        relayIsForGate = true;
        break;
      }
    }
    if (!relayIsForGate) {
      body += L_RELAY;
      body += ": ";
      body += i + 1;
    }
#else
    body += L_RELAY;
    body += ": ";
    body += i + 1;
#endif
    body += "</option>";
  }
  body += "</select></div>";

#endif

  body += "<div class=\"cf\"><label>";
  body += L_TYPE;
  body += "</label><select name=\"m\"><option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += L_MONOSTABLE;
  body += "</option><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += L_BISTABLE;
  body += "</option></select></div><br><p class=\"cm\">";
  body += L__SENSITIVENESS_HINT;
  body += "</p>";

  char _number[4];
  sprintf(_number, "%d", configuration.sensitiveness);

  body += addItem("number", "s", L_SENSITIVENESS, _number, "?", "0", "999", "1",
                  L_MILISECONDS);
  body += "</fieldset>";

  char title[23];
  sprintf(title, "%s #%d", L_SWITCH_BUTTON, id + 1);

  String page = addConfigurationBlock(title, "", body);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz ||
      Device->configuration.api.mqtt) {
    body = "<fieldset>";

    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    body += addItem("number", "x", "IDX", _idx, "?", "0", "999999", "1");

    body += "</fieldset>";

    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }
#else
  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page +=
        addConfigurationBlock(L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY, body);
  }
#endif

  return page;
}

#ifdef AFE_CONFIG_HARDWARE_DS18B20
String AFESitesGenerator::addDS18B20Configuration() {

  DS18B20 configuration = Data.getSensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label>";
  body += "<input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span>";
  body += "</div>";

  body += "<div class=\"cc\"><label><input name=\"o\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0
              ? ">Wysyłać dane tylko, gdy wartość temperatury zmieni się"
              : ">Send data only if value of temperature has changed";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Korekta wartości o" : "Temperature correction";
  body += "</label><input name=\"c\" type=\"number\" min=\"-9.99\" "
          "max=\"9.99\" "
          "step=\"0.01\" "
          "value=\"";
  body += configuration.correction;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "stopni. Zakres" : "degrees. Range";
  body += ": -9.99 - +9.99</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Jednostka" : "Unit";
  body += "</label><select name=\"u\"><option value=\"0\"";
  body += (configuration.unit == 0 ? " selected=\"selected\">" : ">");
  body += language == 0 ? "Celsjusz" : "Celsius";
  body += "</option>option value=\"1\"";
  body += (configuration.unit == 1 ? " selected=\"selected\"" : "");
  body += ">Fahrenheit</option></select></div></fieldset>";

  String page =
      addConfigurationBlock(language == 0 ? "Czujnik temperatury DS18B20"
                                          : "DS18B20 temperature sensor",
                            "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";
    body += "<div class=\"cf\">";
    body += "<label>IDX</label>";
    body += "<input name=\"x\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }
  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
String AFESitesGenerator::addDHTConfiguration() {

  DH configuration = Data.getSensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\"><label>Typ";
  body += language == 1 ? "e" : "";
  body += "</label><select name=\"t\"><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">DH11</option><option value=\"2\"";
  body += (configuration.type == 2 ? " selected=\"selected\"" : "");
  body += ">DH21</option><option value=\"3\"";
  body += (configuration.type == 3 ? " selected=\"selected\"" : "");
  body += ">DH22</option></select></div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label><input name=\"i\" min=\"10\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 10 do 86400sek"
                        : "seconds. Range: 10 to 86400sec";
  body += " (24h)</span></div><div class=\"cc\"><label><input name=\"o\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body += language == 0
              ? ">Wysyłać dane tylko, gdy wartość temperatury lub wilgotności "
                "zmieni się"
              : ">Send data only if value of temperature or humidity has "
                "changed";
  body += "</label></div>";

  if (device.api.mqtt) {
    body += "<div class=\"cc\"><label><input name=\"p\" type=\"checkbox\" "
            "value=\"1\"";
    body += configuration.publishHeatIndex ? " checked=\"checked\"" : "";
    body += language == 0 ? ">Wysyłać temperaturę odczuwalną"
                          : ">Publish Heat Index";
    body += "?</label></div><div class=\"cc\"><label><input name=\"j\" "
            "type=\"checkbox\" value=\"1\"";
    body += configuration.publishDewPoint ? " checked=\"checked\"" : "";
    body += language == 0 ? ">Wysyłać punkt rosy" : ">Publish Dew Point";
    body += "?</label></div>";
  }

  body += "<br><p class=\"cm\">Temperatur";
  body += language == 0 ? "y" : "e";
  body += "</p>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Korekta wartości o" : "Value to correct";
  body += "</label><input name=\"c\" type=\"number\" min=\"-9.99\" "
          "max=\"9.99\" "
          "step=\"0.01\" value=\"";
  body += configuration.temperature.correction;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "stopni. Zakres" : "degrees. Range";
  body += ": -9.99 - +9.99</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Jednostka" : "Unit";
  body += "</label><select  name=\"u\"><option value=\"0\"";
  body +=
      (configuration.temperature.unit == 0 ? " selected=\"selected\">" : ">");
  body += language == 0 ? "Celsjusz" : "Celsius";
  body += "</option><option value=\"1\"";
  body += (configuration.temperature.unit == 1 ? " selected=\"selected\"" : "");
  body += ">Fahrenheit</option></select></div><br><p class=\"cm\">";
  body += language == 0 ? "Wilgotnośc" : "Humidity";
  body += "</p><div class=\"cf\"><label>";
  body += language == 0 ? "Korekta wartości o" : "Value to correct";
  body += "</label><input name=\"d\" type=\"number\" min=\"-99.9\" "
          "max=\"99.9\" "
          "step=\"0.1\" value=\"";
  body += configuration.humidity.correction;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  body += ": -99.9 - +99.9</span></div>";

  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik temperatury i wilgotności DHT"
                    : "DHT temperature and humidity sensor",
      "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label> ";
    body +=
        language == 0 ? "IDX czujnika temperatury" : "Temperature sensor IDX";
    body += " </label>";
    body += "<input name=\"xt\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika wilgotności" : "Humidity sensor IDX";
    body += "</label><input name=\"xh\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.humidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika temperatury i wilgotności"
                          : "Temperature and humidity sensor IDX";
    body += "</label><input name=\"xth\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureAndHumidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
String AFESitesGenerator::addThermostateMenuItem() {
  String page = "<li class=\"itm\"><a href=\"\\?o=thermostat\">&#8227; ";
  page += language == 0 ? "Termostat" : "Thermostat";
  page += "</a></li>";
  return page;
}
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
String AFESitesGenerator::addHumidistatMenuItem() {
  String page = "<li class=\"itm\"><a href=\"\\?o=humidistat\">&#8227; ";
  page += language == 0 ? "Regulator wilgotności" : "Humidistat";
  page += "</a></li>";
  return page;
}
#endif

#if defined(T3_CONFIG)
String AFESitesGenerator::addPIRConfiguration(uint8_t id) {
  PIR configuration = Data.getPIRConfiguration(id);
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  char field[13];
  sprintf(field, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(field, configuration.gpio);
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "</label>";
  body += "<input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\">";
  body += "<span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Typ" : "Type";
  body += "</label><select name=\"o" + String(id) + "\"><option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NO" : "NO";
  body += "</option><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NC" : "NC";
  body += "</option></select></div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "LED" : "LED";
  body += "</label>";
  body += "<select name=\"l" + String(id) + "\">";
  body += "<option value=\"9\" ";
  body += (configuration.ledId == 9 ? "selected=\"selected\"" : "");
  body += ">Brak</option>";
  for (uint8_t i = 0; i < 5; i++) {
    if (Device->configuration.isLED[i]) {
      body += "<option value=\"";
      body += i;
      body += "\" ";
      body += (configuration.ledId == i ? "selected=\"selected\"" : "");
      body += ">";
      body += i + 1;
      body += "</option>";
    }
  }
  body += "</select>";
  body += "<span class=\"hint\">";
  body += language == 0 ? "Dioda LED sygnalizuje wykryty ruch przez czujnik"
                        : "LED indicates motion detected by the sensor";
  body += "</span>";
  body += "</div>";
  body += "<br><p class=\"cm\">";
  body += language == 0
              ? "Czujnik PIR może bezpośrednio sterować jednym przekaźnikiem. "
                "Poniżej możesz wybrać którym oraz ustawić dodatkowe "
                "parametry sterowania"
              : "Motion detection sensor can control a relay assigned to it. "
                "Below you can choose a one and set additional parameters";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wybierz przekaźnik" : "Select relay";
  body += "</label>";
  body += "<select name=\"r" + String(id) + "\">";
  body += "<option value=\"9\" ";
  body += (configuration.relayId == 9 ? "selected=\"selected\"" : "");
  body += ">Brak</option>";
  for (uint8_t i = 0; i < 4; i++) {
    if (Device->configuration.isRelay[i]) {
      body += "<option value=\"";
      body += i;
      body += "\" ";
      body += (configuration.relayId == i ? "selected=\"selected\"" : "");
      body += ">";
      body += i + 1;
      body += "</option>";
    }
  }
  body += "</select>";
  body += "</div>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Czas uruchomienia" : "How long active";
  body += "</label>";
  body += "<input name=\"d" + String(id) +
          "\" type=\"number\" max=\"86400\" min=\"0.01\" step=\"0.01\" "
          "value=\"";
  body += configuration.howLongKeepRelayOn;
  body += "\">";
  body += "<span class=\"hint\">0.01 - 86400 (24h) se";
  body += language == 0 ? "kund" : "conds";
  body += "</span>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"i";
  body += id;
  body += "\" type=\"checkbox\" value=\"1\"";
  body += configuration.invertRelayState ? " checked=\"checked\"" : "";
  body += ">";
  body += language == 0 ? "Wyłącz przekaźnik, gdy PIR wykryje ruch"
                        : "Switch off relay if PIR detects move";

  body += "</label></div></fieldset>";
  char title[25];
  language == 0 ? sprintf(title, "Czujnik ruchu (PIR) #%d", id + 1)
                : sprintf(title, "Motion detection sesnor (PIR) #%d", id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset><div class=\"cf\"><label>IDX</label><input name=\"x";
    body += id;
    body += "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\"><span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span></div></fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
String AFESitesGenerator::addContactronConfiguration(uint8_t id) {
  CONTACTRON configuration = Data.getContactronConfiguration(id);
  // DEVICE deviceConfiguration = Device->configuration;

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO("g", configuration.gpio);
  body += "</div>";

  body += addItem("text", "n", L_NAME, configuration.name, "16");

  body += "<div class=\"cf\"><label>";
  body += L_TYPE;
  body += "</label><select name=\"y\"><option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">NO</option><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">NC</option></select></div>";

  body += generateHardwareList(
      Device->configuration.noOfLEDs, configuration.ledID, "l",
      L_LED_ASSIGNED_TO_SENSOR, 0, AFE_HARDWARE_ITEM_NOT_EXIST);

  body += "<br><p class=\"cm\">";
  body += L_SET_CONTACTRON_SENSITIVENESS;
  body += "</p>";

  char _number[5];
  sprintf(_number, "%d", configuration.bouncing);

  body += addItem("number", "b", L_SENSITIVENESS, _number, "?", "0", "2000",
                  "1", L_MILISECONDS);

  char title[23];
  sprintf(title, "%s #%d", L_MAGNETIC_SENSOR, id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";

    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    body += addItem("number", "x", "IDX", _idx, "?", "0", "999999", "1");

    body += "</fieldset>";

    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }

  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page += addConfigurationBlock(L_CONTACTRON_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY,
                                  body);
  }

  body += "</fieldset>";

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
String AFESitesGenerator::addGateConfiguration(uint8_t id) {
  GATE gateConfiguration = Data.getGateConfiguration(id);
  CONTACTRON contactronConfiguration[2];

  String body = "<fieldset>";
  body += addItem("text", "n", L_NAME, gateConfiguration.name, "16");

  body += generateHardwareList(
      Device->configuration.noOfRelays, gateConfiguration.relayId, "r",
      L_RELAY_ID_CONTROLLING_GATE, 0, AFE_HARDWARE_ITEM_NOT_EXIST);

  if (Device->configuration.noOfContactrons > 0) {
    body +=
        generateHardwareList(Device->configuration.noOfContactrons,
                             gateConfiguration.contactron.id[0], "c1",
                             L_MAGNETIC_SENSOR, 0, AFE_HARDWARE_ITEM_NOT_EXIST);
  }

  /* If there is more than a one contactron connected, add option to assigne
   * it to the gate */

  if (Device->configuration.noOfContactrons > 1) {
    body +=
        generateHardwareList(Device->configuration.noOfContactrons,
                             gateConfiguration.contactron.id[1], "c2",
                             L_MAGNETIC_SENSOR, 0, AFE_HARDWARE_ITEM_NOT_EXIST);
  }

  body += "</fieldset>";

  String page = addConfigurationBlock(L_GATE_CONFIGURATION, "", body);

  /* Add section of Gate states configuration is there is at least one
   * contactron connected to the device and assigned to the Gate */
  if (Device->configuration.noOfContactrons > 0) {

    uint8_t numberOfContractons = 0;
    if (gateConfiguration.contactron.id[1] != AFE_HARDWARE_ITEM_NOT_EXIST) {
      numberOfContractons = 2;
    } else if (gateConfiguration.contactron.id[0] !=
               AFE_HARDWARE_ITEM_NOT_EXIST) {
      numberOfContractons = 1;
    }

#ifdef DEBUG
    Serial << endl
           << "Number of contactros set for the gate: " << numberOfContractons;
#endif

    if (numberOfContractons > 0) {

      for (uint8_t i = 0; i < numberOfContractons; i++) {
        contactronConfiguration[i] =
            Data.getContactronConfiguration(gateConfiguration.contactron.id[i]);
      }

      body = "<fieldset>";

      body += "<p class=\"cm\">";
      body += L_IF_MAGNETIC_SENSOR;
      body += ": <strong>";
      body += contactronConfiguration[0].name;

      if (numberOfContractons == 2) {
        body += "</strong> ";
        body += L_AND_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[1].name;
        body += "</strong> ";
        body += L_ARE_OPEN;
      } else {
        body += "</strong> ";
        body += L_IS_OPEN;
      }
      body += " ";
      body += L_THEN;
      body += ":</p>";
      body += generateGateStatesList(0, gateConfiguration.states.state[0]);
      if (numberOfContractons == 2) {
        body += "<br><br><p class=\"cm\">";
        body += L_IF_MAGNETIC_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[0].name;
        body += "</strong> ";
        body += L_IS_OPEN;
        body += " ";
        body += L_AND_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[1].name;
        body += "</strong> ";
        body += L_IS_CLOSED;
        body += " ";
        body += L_THEN;
        body += ":";
        body += "</p>";
        body += generateGateStatesList(1, gateConfiguration.states.state[1]);

        body += "<br><br><p class=\"cm\">";
        body += L_IF_MAGNETIC_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[0].name;
        body += "</strong> ";
        body += L_IS_CLOSED;
        body += " ";
        body += L_AND_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[1].name;
        body += "</strong> ";
        body += L_IS_OPEN;
        body += " ";
        body += L_THEN;
        body += ":";
        body += "</p>";

        body += generateGateStatesList(2, gateConfiguration.states.state[2]);
      }

      body += "<br><br><p class=\"cm\">";

      body += L_IF_MAGNETIC_SENSOR;
      body += ": <strong>";
      body += contactronConfiguration[0].name;
      if (numberOfContractons == 2) {
        body += "</strong> ";
        body += L_AND_SENSOR;
        body += ": <strong>";
        body += contactronConfiguration[1].name;
        body += "</strong> ";
        body += L_ARE_CLOSED;
      } else {
        body += "</strong> ";
        body += L_IS_CLOSED;
      }
      body += " ";
      body += L_THEN;
      body += ":</p>";
      body += generateGateStatesList(3, gateConfiguration.states.state[3]);

      page += addConfigurationBlock(L_GATES_STATES_CONFIGURATION, "", body);
    }
  }

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";

    char _idx[7];
    sprintf(_idx, "%d", gateConfiguration.domoticz.idx);
    body += addItem("number", "x", "IDX", _idx, "?", "0", "999999", "1");

    body += "</fieldset>";

    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }

  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body +=
        addItem("text", "t", L_MQTT_TOPIC, gateConfiguration.mqtt.topic, "64");
    body += "</fieldset>";
    page += addConfigurationBlock(L_GATE_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY, body);
  }

  page += "</fieldset>";

  return page;
}

const String AFESitesGenerator::generateGateStatesList(uint8_t id, byte state) {

  String body = "<div class=\"cf\"><label>";
  body += L_SET_GATE_STATE;
  body += "</label><select name=\"s" + String(id) + "\"><option value=\"";
  body += AFE_GATE_OPEN;
  body += "\"";
  body += (state == AFE_GATE_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += L_OPENED;
  body += "</option><option value=\"";
  body += AFE_GATE_PARTIALLY_OPEN;
  body += "\"";
  body += (state == AFE_GATE_PARTIALLY_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += L_PARTIALLY_OPENED;
  body += "</option><option value=\"";
  body += AFE_GATE_CLOSED;
  body += "\"";
  body += (state == AFE_GATE_CLOSED ? " selected=\"selected\"" : "");
  body += ">";
  body += L_CLOSED;
  body += "</option><option value=\"";
  body += AFE_GATE_UNKNOWN;
  body += "\"";
  body += (state == AFE_GATE_UNKNOWN ? " selected=\"selected\"" : "");
  body += ">";
  body += L_UNKNOWN_STATE;
  body += "</option></select></div>";

  return body;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
String AFESitesGenerator::addHPMA115S0Configuration(uint8_t id) {
  HPMA115S0 configuration = Data.getHPMA115S0SensorConfiguration(id);

  String body = "<fieldset>";

  body += addItem("text", "n", L_NAME, configuration.name, "16");

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  body += addItem("number", "f", L_MEASURMENTS_INTERVAL, _number, "?", "5",
                  "86400", "f", L_SECONDS);

  body += "<br><br>";
  body += "<p class=\"cm\">";
  body += L_SENSOR_POST_SLEEP_INTERVAL;
  body += "</p>";

  sprintf(_number, "%d", configuration.timeToMeasure);
  body += addItem("number", "m", L_MEASURE_AFTER, _number, "?", "0", "999", "1",
                  L_SECONDS);

  body += "</fieldset>";

  String page = addConfigurationBlock(L_PARTICLE_SENSOR, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";

    sprintf(_number, "%d", configuration.domoticz.pm25.idx);
    body +=
        addItem("number", "x2", "IDX PM2.5", _number, "?", "0", "999999", "1");

    sprintf(_number, "%d", configuration.domoticz.pm10.idx);
    body +=
        addItem("number", "x1", "IDX PM10", _number, "?", "0", "999999", "1");

    body += "</fieldset>";

    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }

  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page +=
        addConfigurationBlock(L_MQTT_TOPIC_HPMA115S0, L_MQTT_TOPIC_EMPTY, body);
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
String AFESitesGenerator::addBMEX80Configuration(uint8_t id) {
  BMEX80 configuration = Data.getBMEX80SensorConfiguration(id);
  char _number[7];
  String page;
  String body = "<fieldset>";

  body += addDeviceI2CAddressSelection(configuration.i2cAddress);

  body += "<div class=\"cf\"><label>";
  body += L_BMEX80_SENSOR_TYPE;
  body += "</label><select name=\"b\"><option value=\"";
  body += AFE_BMX_UNKNOWN_SENSOR;
  body += "\"";
  body +=
      (configuration.type == AFE_BMX_UNKNOWN_SENSOR ? " selected=\"selected\""
                                                    : "");
  body += ">";
  body += L_NONE;
  body += "</option><option value=\"";
  body += AFE_BMP180_SENSOR;
  body += "\"";
  body +=
      (configuration.type == AFE_BMP180_SENSOR ? " selected=\"selected\"" : "");
  body += ">BMx085/BMx180</option><option value=\"";
  body += AFE_BME280_SENSOR;
  body += "\"";
  body +=
      (configuration.type == AFE_BME280_SENSOR ? " selected=\"selected\"" : "");
  body += ">BMx280</option><option value=\"";
  body += AFE_BME680_SENSOR;
  body += "\"";
  body +=
      (configuration.type == AFE_BME680_SENSOR ? " selected=\"selected\"" : "");
  body += ">BMx680</option></select></div>";

  body += "<input type=\"submit\" class=\"b bw\" value=\"";
  body += L_REFRESH_SETTINGS_FOR_BMEX80_SENSOR;
  body += "\"><br><br>";

  body += addItem("text", "n", L_NAME, configuration.name, "16");

  sprintf(_number, "%d", configuration.interval);
  body += addItem("number", "f", L_MEASURMENTS_INTERVAL, _number, "?", "5",
                  "86400", "1", L_SECONDS);

  body += "</fieldset>";

  page = addConfigurationBlock(L_BMEX80_SENSOR, "", body);

  if (configuration.type != AFE_BMX_UNKNOWN_SENSOR) {

    /* Sensor's units */
    body = "<fieldset>";

    body += "<div class=\"cf\"><label>";
    body += L_TEMPERATURE;
    body += "</label><select name=\"tu\"><option value=\"";
    body += AFE_TEMPERATURE_UNIT_CELSIUS;
    body += "\"";
    body += (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                 ? " selected=\"selected\""
                 : "");
    body += ">C</option><option value=\"";
    body += AFE_TEMPERATURE_UNIT_FAHRENHEIT;
    body += "\"";
    body += (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
                 ? " selected=\"selected\""
                 : "");
    body += ">F</option></select></div>";

    body += "</fieldset>";
    page += addConfigurationBlock(L_UNITS, "", body);

    /* Corrections of sensor values */
    body = "<fieldset>";
    sprintf(_number, "%.3f", configuration.temperature.correction);
    body += addItem("number", "tc", L_TEMPERATURE, _number, "?", "-99.999",
                    "99.999", "0.001");

    if (configuration.type != AFE_BMP180_SENSOR) {
      sprintf(_number, "%.3f", configuration.humidity.correction);
      body += addItem("number", "hc", L_HUMIDITY, _number, "?", "-99.999",
                      "99.999", "0.001");
    }

    sprintf(_number, "%.3f", configuration.pressure.correction);
    body += addItem("number", "pc", L_PRESSURE, _number, "?", "-999.999",
                    "999.999", "0.001");

    sprintf(_number, "%d", configuration.altitude);
    body += addItem("number", "hi", L_ALTITIDE, _number, "?", "-431", "8850",
                    "1", L_METERS);

    body += "</fieldset>";
    page += addConfigurationBlock(L_CORRECTIONS, "", body);

    if (Device->configuration.api.domoticz) {
      body = "<fieldset>";

      sprintf(_number, "%d", configuration.domoticz.temperature.idx);
      body += addItem("number", "i1", L_IDX_TEMPERATURE, _number, "?", "0",
                      "999999", "1");

      if (configuration.type != AFE_BMP180_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.humidity.idx);
        body += addItem("number", "i2", L_IDX_HUMIDITY, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.dewPoint.idx);
        body += addItem("number", "i3", L_IDX_DEW_POINT, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.heatIndex.idx);
        body += addItem("number", "i4", L_IDX_HEAT_INDEX, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d",
                configuration.domoticz.temperatureHumidityPressure.idx);
        body += addItem("number", "i0", L_IDX_TEMP_HUM_BAR, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.temperatureHumidity.idx);
        body += addItem("number", "i12", L_IDX_TEMP_HUM, _number, "?", "0",
                        "999999", "1");
      }

      sprintf(_number, "%d", configuration.domoticz.pressure.idx);
      body += addItem("number", "i5", L_IDX_PRESSURE, _number, "?", "0",
                      "999999", "1");

      sprintf(_number, "%d", configuration.domoticz.relativePressure.idx);
      body += addItem("number", "i6", L_IDX_RELATIVE_PRESSURE, _number, "?",
                      "0", "999999", "1");

      if (configuration.type == AFE_BME680_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.iaq.idx);
        body += addItem("number", "i7", L_IDX_IQA, _number, "?", "0", "999999",
                        "1");

        sprintf(_number, "%d", configuration.domoticz.staticIaq.idx);
        body += addItem("number", "i8", L_IDX_STATIC_IAQ, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.co2Equivalent.idx);
        body += addItem("number", "i9", L_IDX_CO2_EQUVALENT, _number, "?", "0",
                        "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.breathVocEquivalent.idx);
        body += addItem("number", "i10", L_IDX_BVOC_EQUIVALENT, _number, "?",
                        "0", "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.gasResistance.idx);
        body += addItem("number", "i11", L_IDX_GAS_SENSOR, _number, "?", "0",
                        "999999", "1");
      }
      body += "</fieldset>";
      page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
    }

    if (Device->configuration.api.mqtt) {
      body = "<fieldset>";
      body +=
          addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
      body += "</fieldset>";
      page +=
          addConfigurationBlock(L_MQTT_TOPIC_BMEX80, L_MQTT_TOPIC_EMPTY, body);
    }
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
String AFESitesGenerator::addBH1750Configuration(uint8_t id) {

  BH1750 configuration = Data.getBH1750SensorConfiguration(id);

  String body = "<fieldset>";

  body += addDeviceI2CAddressSelection(configuration.i2cAddress);

  body += addItem("text", "n", L_NAME, configuration.name, "16");

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  body += addItem("number", "f", L_MEASURMENTS_INTERVAL, _number, "?", "5",
                  "86400", "1", L_SECONDS);

  sprintf(_number, "%d", configuration.mode);
  body += addItem("number", "m", L_SENSITIVENESS, _number, "?", "?", "?", "?",
                  L_CANT_CHANGE, true);

  body += "</fieldset>";

  String page = addConfigurationBlock(L_BH1750_SENSOR, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";

    sprintf(_number, "%d", configuration.domoticz.idx);
    body += addItem("number", "d", "IDX", _number, "?", "0", "999999", "1");

    body += "</fieldset>";
    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }

  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page +=
        addConfigurationBlock(L_MQTT_TOPIC_BH1750, L_MQTT_TOPIC_EMPTY, body);
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
String AFESitesGenerator::addAS3935Configuration(uint8_t id) {
  AS3935 configuration = Data.getAS3935SensorConfiguration(id);
  char _number[2];

  String body = "<fieldset>";

  body += addDeviceI2CAddressSelection(configuration.i2cAddress);

  body += addItem("text", "n", L_NAME, configuration.name, "16");

  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO("g", configuration.irqGPIO);
  body += "</div>";

  body += "<div class=\"cf\"><label>";
  body += L_DISTANCE_UNIT;
  body += "</label><select name=\"u\"><option value=\"";
  body += AFE_DISTANCE_KM;
  body += "\"";
  body +=
      (configuration.unit == AFE_DISTANCE_KM ? " selected=\"selected\"" : "");
  body += ">";
  body += L_KM;
  body += "</option><option value=\"";
  body += AFE_DISTANCE_MIL;
  body += "\"";
  body +=
      (configuration.unit == AFE_DISTANCE_MIL ? " selected=\"selected\"" : "");
  body += ">";
  body += L_MILES;
  body += "</option></select></div>";

  sprintf(_number, "%d", configuration.watchdogThreshold);
  body += addItem("number", "e", L_WATCHDOG_THRESHOLD, _number, "?", "1", "10",
                  "1", L_WATCHDOG_THRESHOLD_HINT);

  sprintf(_number, "%d", configuration.spikesRejectionLevel);
  body += addItem("number", "s", L_SPIKES_REJECTION, _number, "?", "1", "11",
                  "1", L_SPIKES_REJECTION_HINT);

  body += "<div class=\"cf\"><label>";
  body += L_MIN_SPIKES;
  body += "</label><select name=\"m\"><option value=\"1\"";
  body += (configuration.minimumNumberOfLightningSpikes == 1
               ? " selected=\"selected\""
               : "");
  body += ">1</option><option value=\"5\"";
  body += (configuration.minimumNumberOfLightningSpikes == 5
               ? " selected=\"selected\""
               : "");
  body += ">5</option><option value=\"9\"";
  body += (configuration.minimumNumberOfLightningSpikes == 9
               ? " selected=\"selected\""
               : "");
  body += ">9</option><option value=\"16\"";
  body += (configuration.minimumNumberOfLightningSpikes == 16
               ? " selected=\"selected\""
               : "");
  body += ">16</option></select></div>";

  body += "<div class=\"cc\"><label><input name =\"f\" type=\"checkbox\" "
          "value=\"1\"";
  body +=
      configuration.setNoiseFloorAutomatically ? " checked=\"checked\">" : ">";
  body += L_AUTOMATIC_NOISE_FLOOR_CONTROL;
  body += "</label></div><p class=\"cm\">";

  body += L_SET_LEVEL_OF_NOISE_FLOOR;
  sprintf(_number, "%d", configuration.noiseFloor);
  body += addItem("number", "nf", L_NOISE_FLOOR, _number, "?", "1", "7", "1",
                  L_NOISE_FLOOR_HINT);

  body += "</p><div class=\"cf\"><label>";
  body += L_SENSOR_INDOOR_OUTDOOR;
  body += "</label><select name=\"w\"><option value=\"1\"";
  body += (configuration.indoor == true ? " selected=\"selected\"" : "");
  body += ">";
  body += L_INDOOR;
  body += "</option><option value=\"0\"";
  body += (configuration.indoor == false ? " selected=\"selected\"" : "");
  body += ">";
  body += L_OUTDOOR;
  body += "</option></select></div>";

  body += "</fieldset>";

  String page = addConfigurationBlock(L_AS3935_SENSOR, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";

    sprintf(_number, "%d", configuration.domoticz.idx);
    body += addItem("number", "d", "IDX", _number, "?", "0", "999999", "1");

    body += "</fieldset>";
    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }

  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page +=
        addConfigurationBlock(L_MQTT_TOPIC_AS3935, L_MQTT_TOPIC_EMPTY, body);
  }

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
String AFESitesGenerator::addAnalogInputConfiguration() {
  ADCINPUT configuration = Data.getADCInputConfiguration();

  String body = "<fieldset>";

  char _number[13];

  sprintf(_number, "%d", configuration.gpio);
  body += addItem("number", "g", "GPIO", _number, "?", "?", "?", "?");

  sprintf(_number, "%d", configuration.interval);
  body += addItem("number", "i", L_MEASURMENTS_INTERVAL, _number, "?", "1",
                  "86400", "1", L_SECONDS);

  sprintf(_number, "%d", configuration.numberOfSamples);
  body += addItem("number", "n", L_NUMBER_OF_SAMPLES, _number, "?", "1", "999",
                  "1");

  body += "</fieldset>";

  String page = addConfigurationBlock(L_ANALOG_INPUT, "", body);

  body = "<fieldset>";

  dtostrf((float)configuration.maxVCC, 3, 2, _number);

  body += addItem("number", "m", L_MEASURED_VOLTAGE, _number, "?", "0", "1000",
                  "0.01", "V");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Ra);
  body +=
      addItem("number", "ra", "R[a]", _number, "?", "0", "90000000", "1", "Om");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Rb);
  body +=
      addItem("number", "rb", "R[b]", _number, "?", "0", "90000000", "1", "Om");
  body += "</fieldset>";

  page += addConfigurationBlock(L_VOLTAGE_DIVIDER, "", body);

#ifdef AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
  if (Device->configuration.api.mqtt) {
    body = "<fieldset>";
    body += addItem("text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    body += "</fieldset>";
    page += addConfigurationBlock(L_RELAY_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY, body);
  }
#endif 

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz ||
      Device->configuration.api.mqtt) {
    body = "<fieldset>";
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.raw);
    body += addItem("number", "x0", L_RAW_DATA, _idx, "?", "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.percent);
    body += addItem("number", "x1", L_PERCENT, _idx, "?", "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.voltage);
    body += addItem("number", "x2", L_VOLTAGE, _idx, "?", "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.voltageCalculated);
    body += addItem("number", "x3", L_VOLTAGE_CALCULATED, _idx, "?", "0",
                    "999999", "1");
    body += "</fieldset>";

    page += addConfigurationBlock("Domoticz", L_NO_IF_IDX_0, body);
  }
#endif

  return page;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
String AFESitesGenerator::addSerialPortConfiguration() {
  SERIALPORT configuration = Data.getSerialPortConfiguration();

  String body = "<fieldset>";

  body += generateConfigParameter_GPIO("r", configuration.RXD, "GPIO RXD");
  body += generateConfigParameter_GPIO("t", configuration.TXD, "GPIO TXD");

  body += "</fieldset>";
  return addConfigurationBlock("UART", "", body);
  ;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
String AFESitesGenerator::addI2CPortConfiguration() {
  I2CPORT configuration = Data.getI2CPortConfiguration();

  String body = "<fieldset>";

  body += generateConfigParameter_GPIO("a", configuration.SDA, "GPIO SDA");
  body += generateConfigParameter_GPIO("l", configuration.SCL, "GPIO SCL");

  body += "</fieldset>";
  return addConfigurationBlock("I2C", "", body);
  ;
}

String AFESitesGenerator::addDeviceI2CAddressSelection(uint8_t address) {
  AFEI2CScanner I2CScanner;
  I2CScanner.begin();
  String body = "<div class=\"cf\"><label>I2C ";
  body += L_ADDRESS;
  body += ": </label><select name=\"a\">";
  body += "<option value=\"0\"";
  body += address == 0 ? " selected=\"selected\"" : "";
  body += ">";
  body += L_NONE;
  body += "</option>";

  for (byte addressToScan = 1; addressToScan < 127; addressToScan++) {
    if (I2CScanner.scan(addressToScan)) {
      body += "<option value=\"";
      body += addressToScan;
      body += "\"";
      body += addressToScan == address ? " selected=\"selected\"" : "";
      body += ">[0x";
      body += String(addressToScan, HEX);
      body += "] : ";
      body += I2CScanner.getName(addressToScan);
      body += "</option>";
    }
  }
  body += "</select></div>";
  return body;
}
#endif

String AFESitesGenerator::addUpgradeSection() {
  String body = "<form method=\"post\" action=\"upgrade?o=";
  body += AFE_CONFIG_SITE_POST_UPGRADE;
  body += "\" enctype=\"multipart/form-data\">";
  body += "<fieldset><div class=\"cf\"><label>";
  body += L_SELECT_FIRMWARE;
  body += "</label><input class=\"bs\" name=\"update\" type=\"file\" "
          "accept=\".bin\"></div><p class=\"cm\">";
  body += L_UPGRADE_INFO;
  body += ".</p><button type=\"submit\" class=\"b be\">";
  body += L_UPGRADE;
  body += "</button></fieldset></form>";
  return addConfigurationBlock(L_FIRMWARE_UPGRADE, L_DONT_PLUG_OFF, body);
}

String AFESitesGenerator::addPostUpgradeSection(boolean status) {

  String body = "<fieldset><ul>";
  if (status) {
    body += "<li style=\"color:red\">";
    body += L_UPGRADE_FAILED;
  } else {
    body += "<li>";
    body += L_UPGRADE_SUCCESSFUL;
  }
  body += "</li><li>";
  body += L_DEVICE_WILL_BE_REBOOTED;
  body += "...</li></fieldset>";
  return addConfigurationBlock(L_FIRMWARE_UPGRADE, "", body);
}

String AFESitesGenerator::addResetSection() {
  String body = "<fieldset><p class=\"cm\"><strong>";
  body += L_WARNING;
  body += "</strong>: ";
  body += "</p><p class=\"cm\">";
  body += L_CONFIGURATION_WILL_BE_REMOVED;
  body += ": </p><input type=\"submit\" class=\"b be\" value=\"";
  body += L_RESTORE_DEFAULT_SETTINGS;
  body += "\"></fieldset>";

  return addConfigurationBlock(L_RESTORING_DEFAULT_SETTING, "", body);
}

String AFESitesGenerator::addPostResetSection() {
  String body = "<fieldset><p class=\"cm\">";
  body += L_UPGRADE_IN_PROGRESS;
  body += "</p><p class=\"cm\">";
  body += L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE;
  body += ": </p><a href=\"http://192.168.5.1\">http://192.168.5.1</a>";
  body += "</fieldset>";
  return addConfigurationBlock(L_RESTORING_DEFAULT_SETTING, "", body);
}

String AFESitesGenerator::addExitSection(uint8_t command) {
  String body = "<fieldset><div class=\"cf\"><ul><li>";
  body += L_REBOOT_IN_PROGRESS;
  body += "</li><li>";
  if (command != AFE_MODE_ACCESS_POINT) {
    body += L_SITE_WILL_BE_RELOADED;
  } else {
    body += L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE;
    body += ": <a href=\"http://192.168.5.1\">http://192.168.5.1</a></p>";
  }
  body += "</li></ul></div></fieldset>";
  return addConfigurationBlock(L_REBOOT, "", body);
}

String AFESitesGenerator::addIndexSection(boolean authorized) {
  DEVICE configuration;
  configuration = Device->configuration;
  String body = "<fieldset>";
  if (!authorized) {
    body += "<h3>";
    body += L_WRONG_PASSWORD;
    body += "</h3>";
  }

  body += "<form method=\"post\"><div class=\"cf\"><input name=\"p\" "
          "type=\"password\" "
          "placeholder=\"";
  body += L_PASSWORD;
  body += "\"></div><div class=\"cf\"><input type=\"submit\" class=\"b bs\" "
          "value=\"";
  body += L_NORMAL_MODE;
  body += "\" formaction=\"/?o=0&i=";
  body += AFE_MODE_CONFIGURATION;
  body += "\" /> <input type=\"submit\" class=\"b be\" value=\"";
  body += L_HOTSPOT_MODE;
  body += "\" formaction=\"/?o=0&i=";
  body += AFE_MODE_ACCESS_POINT;
  body += "\" /></div></form>";

  String page = addConfigurationBlock(L_LAUNCH_CONFIGURATION_PANEL, "", body);
  return page;
}

String AFESitesGenerator::addProVersionSite() {
  PRO_VERSION configuration = Data.getProVersionConfiguration();
  String body;
  if (Device->getMode() == AFE_MODE_CONFIGURATION) {
    body = "<fieldset>";
    body += addItem("text", "k", L_KEY, configuration.serial, "18");
    body += "<div class=\"cf\"><label>";
    body += L_VALID;
    body += "?</label><span>";
    body += configuration.valid ? L_YES : L_NO;
    body += "</span></div>";
    body += "<input name=\"v\" type=\"hidden\" value=\"";
    body += configuration.valid;
    body += "\">";
    body += "</fieldset>";
  } else {
    body = "<h3>";
    body += L_PRO_CANNOT_BE_COFIGURED;
    body += "</h3>";
  }
  return addConfigurationBlock(L_PRO_VERSION, "", body);
}

const String AFESitesGenerator::generateFooter(boolean extended) {
  String body = "</div></div>";

  if (extended) {
    body += "<div style=\"padding: 5px 0\"><a "
            "href=\"https://www.smartnydom.pl/forum/afe-firmware/\" "
            "target=\"_blank\"><img src=\"https://img.shields.io/badge/";
    body += L_HELP;
    body += "-Forum-red.svg\"alt=\"Forum\" /></a> <a "
            "href=\"https://afe.smartnydom.pl/";
    body += L_LANGUAGE_SHORT;
    body += "/\" target=\"_blank\"><img src=\"https://img.shields.io/badge/";
    body += L_HELP;
    body += "-";
    body += L_DOCUMENTATION;
    body += "-green.svg\" alt=\"";
    body += L_DOCUMENTATION;
    body += "\" /></a> <a href=\"https://afe.smartnydom.pl/";
    body += L_LANGUAGE_SHORT;
    body += "/pliki-to-pobrania/historia-zmian\" "
            "target=\"_blank\"><img src=\"https://img.shields.io/badge/";
    body += L_VERSION;
    body += "%20-%20";
    body += AFE_FIRMWARE_VERSION;
    body += "[T";
    body += AFE_FIRMWARE_TYPE;
    body += "]-blue.svg\" alt=\"T";
    body += AFE_FIRMWARE_TYPE;
    body += "-";
    body += AFE_FIRMWARE_VERSION;
    body += "\" /></a> <img src=\"https://img.shields.io/badge/Device-";
    body += AFE_DEVICE_TYPE_NAME;
    body += "-lightgrey.svg\" alt=\"DeviceID ";
    body += AFE_DEVICE_TYPE_ID;
    body += "\" /> <img src=\"https://img.shields.io/badge/API-";
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    body += "Domoticz";
#else
    body += "Standard";
#endif
    body += "-yellow.svg\" /> <img src=\"https://img.shields.io/badge/PRO-";
    body += Firmware->Pro.valid ? L_YES : L_NO;
    body += "-orange.svg\" alt=\"PRO\" /></div>";
  }
  body += "</body></html>";
  return body;
}

String AFESitesGenerator::addConfigurationBlock(const String title,
                                                const String description,
                                                const String body) {
  String page = "<div class=\"ci\">";
  page += "<h1>" + title + "</h1>";
  page += "<p class=\"cd\">" + description + "</p>";
  page += body;
  page += "</div>";

  return page;
}

const String AFESitesGenerator::generateConfigParameter_GPIO(
    const char *field, uint8_t selected, const String title) {
  String page = "<div class=\"cf\"><label>";
  page += title;
  page += "</label><select name=\"";
  page += field;
  page += "\">";
  for (uint8_t i = 0; i <= 16; i++) {
    page += "<option value=\"" + String(i) + "\"" +
            (selected == i ? "selected=\"selected\"" : "") + ">" + String(i) +
            "</option>";
  }
  page += "</select></div>";
  return page;
}

const String AFESitesGenerator::generateHardwareList(
    uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label, uint8_t index, uint8_t noneValue) {
  String body = "<div class=\"cf\"><label>";
  body += label;
  body += "</label>";
  body += "<select name=\"";
  body += field;
  body += "\">";
  body += "<option value=\"";
  body += noneValue;
  body += "\"";
  body += (noOffConnected == noneValue ? " selected=\"selected\"" : "");
  body += ">";
  body += L_NONE;
  body += "</option>";
  /* @TODO not a nice code with this index. It can't be different than 0
   * or 1. So far only 0,1 are planned */
  for (uint8_t i = index; i < noOfItems + index; i++) {
    body += "<option value=\"";
    body += i;
    body += "\"";
    body += noOffConnected == i ? " selected=\"selected\"" : "";
    body += ">";
    body += i + (index == 0 ? 1 : 0);
    body += "</option>";
  }

  body += "</select>";
  body += "</div>";

  return body;
}

const String AFESitesGenerator::generateHardwareItemsList(
    uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label) {
  return generateHardwareList(noOfItems, noOffConnected, field, label, 1, 0);
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
const String
AFESitesGenerator::generateTwoValueController(REGULATOR configuration,
                                              uint8_t type) {

  String body = "<fieldset>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"te\" type=\"checkbox\" value=\"1\"";
  body += configuration.enabled ? " checked=\"checked\">" : ">";

  body += language == 0 ? " włączony" : "enabled";

  body += "?</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Włącz jeśli" : "On if";
  if (type == THERMOSTAT_REGULATOR) {
    body += " temp.";
  } else {
    body += language == 0 ? " wilgo." : " Humidity ";
  }
  body += language == 0 ? "jest" : "is";
  body += "</label>";

  body += "<select name=\"ta\"><option value=\"0\"";
  body += (configuration.turnOnAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOnAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<input name=\"tn\" type=\"number\" value=\"";
  body += configuration.turnOn;
  if (type == THERMOSTAT_REGULATOR) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (type == THERMOSTAT_REGULATOR) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }

  body += "</span></div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wyłącz jeśli" : "Off if";
  if (type == THERMOSTAT_REGULATOR) {
    body += " temp.";
  } else {
    body += language == 0 ? " wilgo." : " Humidity ";
  }
  body += language == 0 ? "jest" : "is";
  body += "</label>";
  body += "<select name=\"tb\"><option value=\"0\"";
  body += (configuration.turnOffAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOffAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<input name=\"tf\" type=\"number\" value=\"";
  body += configuration.turnOff;
  if (type == THERMOSTAT_REGULATOR) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (type == THERMOSTAT_REGULATOR) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }

  body += "</div></fieldset>";

  return body;
}
#endif

const String AFESitesGenerator::addItem(const char *type, const char *name,
                                        const char *label, const char *value,
                                        const char *size, const char *min,
                                        const char *max, const char *step,
                                        const char *hint, boolean readonly) {
  String item;
  item = "<div class=\"cf\"><label>";
  item += label;
  item += "</label><input name=\"";
  item += name;
  item += "\" type=\"";
  item += type;
  item += "\" ";
  if (readonly) {
    item += "readonly=\"readonly\" ";
  }
  if (strcmp(size, "?") != 0) {
    item += "maxlength=\"";
    item += size;
    item += "\" ";
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0) {
      item += "min=\"";
      item += min;
      item += "\" ";
    }
    if (strcmp(max, "?") != 0) {
      item += "max=\"";
      item += max;
      item += "\" ";
    }
    if (strcmp(step, "?") != 0) {
      item += "step=\"";
      item += step;
      item += "\" ";
    }
  }
  item += "value=\"";
  item += value;
  item += "\">";
  if (strcmp(size, "?") != 0) {
    item += "<span class=\"hint\">Max ";
    item += size;
    item += " ";
    item += L_NUMBER_OF_CHARS;
    item += "</span>";
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0 && strcmp(max, "?") != 0) {
      item += "<span class=\"hint\"> ";
      item += L_RANGE;
      item += " ";
      item += min;
      item += " - ";
      item += max;
      if (strcmp(hint, "?") != 0) {
        item += " ";
        item += hint;
      }
      item += "</span>";
    } else if (strcmp(hint, "?") != 0) {
      item += "<span class=\"hint\">";
      item += hint;
      item += "</span>";
    }
  }
  item += "</div>";
  return item;
}
