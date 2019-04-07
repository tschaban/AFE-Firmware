#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() {}

void AFESitesGenerator::begin(AFEDevice *_Device) {
  firmware = Data.getFirmwareConfiguration();
  Device = _Device;
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

  if (Device->getMode() == MODE_CONFIGURATION ||
      Device->getMode() == MODE_NORMAL) {
    page += "<img src=\"http://api.smartnydom.pl/logo/T";
    page += firmware.type;
    page += "/";
    page += firmware.version;
    page += "/";
    page += deviceID;
    page += "\" style=\"width: 100%;display: block\">";
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
  Firmware.begin();
  page += "<div id=\"l\">";
  if (Device->getMode() == MODE_ACCESS_POINT) {
    page += "<h3 class=\"ltit\">AFE FIRMWARE</h3>";
  }
  page += "<h4>";
  page += L_FIRMWARE_NAME;
  page += "</h4><ul class=\"lst\">";

  Device->begin(); // Reading configuration data
  page += "<li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_DEVICE;
  page += "\">";
  page += L_DEVICE;
  page += "</a></li><li class=\"itm\"><a href=\"\\?o=";
  page += AFE_CONFIG_SITE_NETWORK;
  page += "\">";
  page += L_NETWORK;
  page += "</a></li>";
  if (Device->configuration.api.mqtt) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_MQTT;
    page += "\">";
    page += L_MQTT_BROKER;
    page += "</a></li>";
  }
  if (Device->configuration.api.domoticz) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_DOMOTICZ;
    page += "\">";
    page += L_DOMOTICZ_SERVER;
    page += "</a></li>";
  }

  uint8_t itemPresent = 0;

  for (uint8_t i = 0; i < sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i]) {
      itemPresent++;
    } else {
      break;
    }
  }

  if (itemPresent > 0) {
    page += "<li class=\"itm\"><a href=\"\\?o=";
    page += AFE_CONFIG_SITE_LED;
    page += "\">";
    page += L_LEDS;
    page += "</a></li>";
  }

  /* Relay */
  itemPresent = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isRelay); i++) {
    if (Device->configuration.isRelay[i]) {
      itemPresent++;
    } else {
      break;
    }
  }

  if (itemPresent > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_RELAYS_CONFIGURATION;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < itemPresent; i++) {
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
#if defined(CONFIG_HARDWARE_DS18B20) && defined(CONFIG_FUNCTIONALITY_THERMOSTAT)
      if (Device->configuration.isDS18B20) {
        page += addThermostateMenuItem();
      }
#endif

/* Humidistat */
#if defined(CONFIG_HARDWARE_DHXX) && defined(CONFIG_FUNCTIONALITY_HUMIDISTAT)
      if (Device->configuration.isDHT) {
        page += addThermostateMenuItem();
        page += addHumidistatMenuItem();
      }
#endif
    }
  }

  /* Switch */
  itemPresent = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isSwitch); i++) {
    if (Device->configuration.isSwitch[i]) {
      itemPresent++;
    } else {
      break;
    }
  }

  if (itemPresent > 0) {
    page += "<li  class=\"itm\"><a><i>";
    page += L_BUTTONS_SWITCHES;
    page += "</i></a></li>";

    for (uint8_t i = 0; i < itemPresent; i++) {
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
#if defined(T5_CONFIG)
  itemPresent = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isContactron); i++) {
    if (Device->configuration.isContactron[i]) {
      itemPresent++;
    } else {
      break;
    }
  }

  if (itemPresent > 0) {
    page += "<li class=\"itm\"><a><i>";
    page += language == 0 ? "Czujniki magnetyczne" : "Magnetic sensors";
    page += "</i></a></li>";
    for (uint8_t i = 0; i < itemPresent; i++) {
      page += "<li class=\"itm\"><a href=\"\\?o=contactron";
      page += i;
      page += "\"> - ";
      page += language == 0 ? "Czujnik: " : "Sensor: ";
      page += i + 1;
      page += "</a></li>";
    }

    page += "<li class=\"itm\"><a href=\"\\?o=gate\">";
    page +=
        language == 0 ? "Konfiguracja bram/drzwi" : "Gate/Door configuration";
    page += "</a></li>";
  }
#endif

/* Sensor DS18B20 */
#ifdef CONFIG_HARDWARE_DS18B20
  if (Device->configuration.isDS18B20) {
    page += "<li class=\"itm\"><a href=\"\\?o=ds18b20\">";
    page += language == 0 ? "Czujnik temperatury" : "Temperature sensor";
    page += "</a></li>";
  }
#endif

/* Sensor DHxx */
#ifdef CONFIG_HARDWARE_DHXX
  if (Device->configuration.isDHT) {
    page += "<li class=\"itm\"><a href=\"\\?o=DHT\">";
    page += language == 0 ? "Czujnik DHT" : "DHT sensor";
    page += "</a></li>";
  }
#endif

/* UART */
#ifdef CONFIG_HARDWARE_UART
  page += "<li class=\"itm\"><a href=\"\\?o=HPMA115S0\">";
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  if (Device->configuration.isHPMA115S0) {
    page += "<li class=\"itm\"><a href=\"\\?o=UART\">UART</a></li>";

    page += language == 0 ? "Czujnik cząstek PM2.5/PM10"
                          : "PM2.5/PM10 Particle Sensor";
    page += "</a></li>";
  }
#endif

#ifdef CONFIG_HARDWARE_BMX80
  if (Device->configuration.isBMx80 != 0) {
    page += "<li class=\"itm\"><a href=\"\\?o=BMx80\">";
    page += language == 0 ? "Czujnik BMx80" : "BMx80 Sensor";
    page += "</a></li>";
  }
#endif

#ifdef CONFIG_HARDWARE_BH1750
  if (Device->configuration.isBH1750) {
    page += "<li class=\"itm\"><a href=\"\\?o=BH1750\">";
    page += language == 0 ? "Czujnik BH1750" : "BH1750 Sensor";
    page += "</a></li>";
  }
#endif

/* Sensor DS18B20 */
#ifdef CONFIG_HARDWARE_ADC_VCC
  if (Device->configuration.isAnalogInput && Firmware.isUnlocked()) {
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
  uint8_t itemsNumber = 0;

  String body = "<fieldset><div class=\"cf\"><label>";
  body += L_DEVICE_NAME;
  body += "</label><input name=\"dn\" type=\"text\" maxlength=\"16\" "
          "value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div></fieldset>";
  String page = addConfigurationBlock(L_DEVICE, L_DEVICE_SECTION_INFO, body);
  body = "<fieldset>";

/* LED */
#ifdef CONFIG_HARDWARE_LED
  for (uint8_t i = 0; i < sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(sizeof(Device->configuration.isLED),
                                    itemsNumber, "hl", L_NUMBER_OF_LEDS);

#endif

/* Contactrons */
#if defined(T5_CONFIG)
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isContactron); i++) {
    if (Device->configuration.isContactron[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }
  body += generateHardwareItemsList(
      sizeof(Device->configuration.isContactron), itemsNumber, "hc",
      language == 0 ? "Ilość czujników magnetycznych "
                    : "Number of magnetic sensors");

  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isSwitch); i++) {
    if (Device->configuration.isSwitch[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }
#else
  /* Relay */
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isRelay); i++) {
    if (Device->configuration.isRelay[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(sizeof(Device->configuration.isRelay),
                                    itemsNumber, "hr", L_NUMBER_OF_RELAYS);
#endif

  /* Switch */
  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device->configuration.isSwitch); i++) {
    if (Device->configuration.isSwitch[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(sizeof(Device->configuration.isSwitch),
                                    itemsNumber, "hs", L_NUMBER_OF_SWITCHES);

#ifdef CONFIG_HARDWARE_DS18B20
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isDS18B20 ? " checked=\"checked\">" : ">";
  body += " DS18B20";
  body += "</label></div>";
#endif

#ifdef CONFIG_HARDWARE_DHXX
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isDHT ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " DHT";
  body += "</label></div>";
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  body += "<div class=\"cc\"><label><input name =\"ds\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isHPMA115S0 ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " HPMA115S0";
  body += "</label></div>";
#endif

#ifdef CONFIG_HARDWARE_BH1750
  body += "<div class=\"cc\"><label><input name =\"bh\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.isBH1750 ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " BH1750";
  body += "</label></div>";
#endif

#ifdef CONFIG_HARDWARE_BMX80
  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " BMx80";
  body += "</label>";
  body += "<select name=\"b6\">";
  body += "<option value=\"0\"";
  body += (configuration.isBMx80 == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Brak" : "None";
  body += "</option>";

  body += "<option value=\"";
  body += TYPE_BMP180_SENSOR;
  body += "\"";
  body +=
      (configuration.isBMx80 == TYPE_BMP180_SENSOR ? " selected=\"selected\""
                                                   : "");
  body += ">";
  body += "BMP085/BMP180";
  body += "</option>";

  body += "<option value=\"";
  body += TYPE_BME280_SENSOR;
  body += "\"";
  body +=
      (configuration.isBMx80 == TYPE_BME280_SENSOR ? " selected=\"selected\""
                                                   : "");
  body += ">";
  body += "BME280";
  body += "</option>";

  body += "<option value=\"";
  body += TYPE_BME680_SENSOR;
  body += "\"";
  body +=
      (configuration.isBMx80 == TYPE_BME680_SENSOR ? " selected=\"selected\""
                                                   : "");
  body += ">";
  body += "BME680";
  body += "</option>";

  body += "</select>";
  body += "</div>";
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  if (Firmware.isUnlocked()) {
    body += "<div class=\"cc\"><label><input name =\"ai\" type=\"checkbox\" "
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
      sizeof(Device->configuration.isPIR), itemsNumber, "hp",
      language == 0 ? "Ilość czujników PIR" : "Number of PIRs");
#endif

  body += "</fieldset>";

  page += addConfigurationBlock(L_HARDWARE_CONFIGURATION,
                                L_HARDWARE_CONFIGURATION_INFO, body);

  body = "<fieldset><div class=\"cc\"><label><input name=\"m\" "
         "type=\"checkbox\" value=\"1\"";
  body += configuration.api.mqtt ? " checked=\"checked\"" : "";
  body += ">MQTT API ";
  body += L_ENABLED;
  body += "?</label></div><div class=\"cc\"><label><input name=\"h\" "
          "type=\"checkbox\" value=\"1\"";
  body += configuration.api.http ? " checked=\"checked\"" : "";
  body += ">HTTP API ";
  body += L_ENABLED;
  body += "?</label></div>";

  body += "<div class=\"cc\"><label><input name=\"d\" type=\"checkbox\" "
          "value=\"1\"";
  body += configuration.api.domoticz ? " checked=\"checked\"" : "";
  body += ">Domoticz API ";
  body += L_ENABLED;
  body += "?</label></div>";
  body += "</fieldset>";

  page += addConfigurationBlock(L_DEVICE_CONTROLLING, L_DEVICE_CONTROLLING_INFO,
                                body);

  return page;
}

String AFESitesGenerator::addFirstLaunchConfiguration() {
  String body = "<fieldset><div class=\"cf\"><label>";
  body += L_SSID;
  body += "</label><select name=\"s\">";
#ifdef DEBUG
  Serial << endl << "Searching for WiFi networks: ";
#endif
  int numberOfNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numberOfNetworks; i++) {
    body += "<option>";
    body += WiFi.SSID(i);
    body += "</option>";
  }
#ifdef DEBUG
  Serial << "Completed";
#endif

  body += "</select><input type=\"submit\" class =\"b bc\" value=\"";
  body += L_REFRESH;
  body += "\" formaction=\"/?o=start&c=0\"></div><div class=\"cf\"><label>";
  body += L_PASSWORD;
  body += "</label><input name=\"p\" type=\"text\" /></div></fieldset>";

  return addConfigurationBlock(L_NETWORK_CONFIGURATION, "", body);
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
  body += "<div class=\"cf\"><label>";
  body += L_SSID;
  body += "</label><input name=\"s\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.ssid;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>";
  body += L_PASSWORD;
  body += "</label><input type=\"password\" name=\"p\" maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>MAC</label><input "
          "type=\"text\" readonly=\"readonly\" value=\"";
  body += WiFi.macAddress();
  body += "\"></div></fieldset>";

  String page = addConfigurationBlock(L_NETWORK_CONFIGURATION,
                                      L_NETWORK_CONFIGURATION_INFO, body);

  body = "<fieldset><div class=\"cc\"><label><input name=\"d\" "
         "type=\"checkbox\" value=\"1\"";
  body += (configuration.isDHCP ? " checked=\"checked\"" : "");
  body += "> ";
  body += L_DHCP_ENABLED;
  body += "?</label></div>";

  body += "<div class=\"cf\"><label>";
  body += L_IP_ADDRESS;
  body += "</label><input name=\"d1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body +=
      "\">.<input name=\"d2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body +=
      "\">.<input name=\"d3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"d4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\"></div><div class=\"cf\"><label>";
  body += L_GATEWAY;
  body += "</label><input name=\"g1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[0];
  body +=
      "\">.<input name=\"g2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[1];
  body +=
      "\">.<input name=\"g3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[2];
  body +=
      "\">.<input name=\"g4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.gateway[3];
  body += "\"></div>";

  body += "<div class=\"cf\"><label>";
  body += L_SUBNET;
  body += "</label><input name=\"s1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" style=\"width:70px\" value=\"";
  body += configuration.subnet[0];
  body +=
      "\">.<input name=\"s2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[1];
  body +=
      "\">.<input name=\"s3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[2];
  body +=
      "\">.<input name=\"s4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.subnet[3];
  body += "\"></div></fieldset>";

  page += addConfigurationBlock(L_DEVICE_IP, L_DEVICE_IP_INFO, body);

  body = "<fieldset><div class=\"cf\"><label>";
  body += L_NUMBER_OF_CONNECTIONS;
  body += "</label><input name=\"na\" type=\"number\" min=\"1\" max=\"255\" "
          "step=\"1\" value=\"";
  body += configuration.noConnectionAttempts;
  body += "\"><span class=\"hint\">1-255</span></div><div class=\"cf\"><label>";
  body += L_TIME_BETWEEN_CONNECTIONS;
  body += "</label><input type=\"number\" name=\"wc\" "
          "min=\"1\" max=\"255\" step=\"1\" value=\"";
  body += configuration.waitTimeConnections;
  body += "\"><span class=\"hint\">1-255 (";
  body += L_SECONDS;
  body += ")</span></div><div class=\"cf\"><label>";
  body += L_SLEEP_TIME;
  body += "</label><input type=\"number\" name=\"ws\" min=\"1\" max=\"255\" "
          "step=\"1\" value=\"";
  body += configuration.waitTimeSeries;
  body += "\"><span class=\"hint\">1-255 (";
  body += L_SECONDS;
  body += ")</span></div></fieldset>";

  page += addConfigurationBlock(L_ADVANCED, "", body);

  return page;
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {
  MQTT configuration;
  configuration = Data.getMQTTConfiguration();

  String body = "<fieldset><div class=\"cf\"><label>Hostname</label><input "
                "name=\"h\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.host;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>";
  body += L_IP_ADDRESS;
  body += "</label><input name=\"m1\" type=\"number\" max=\"255\" min=\"0\" "
          "step=\"1\" style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body +=
      "\">.<input name=\"m2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body +=
      "\">.<input name=\"m3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body +=
      "\">.<input name=\"m4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
      "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\"></div><div class=\"cf\"><label>Port</label><input name=\"p\" "
          "type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\"></div><div class=\"cf\"><label>";
  body += L_USERNAME;
  body += "</label><input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>";
  body += L_PASSWORD;
  body += "</label><input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div></fieldset>";

  String page =
      addConfigurationBlock("MQTT Broker", L_MQTT_CONFIGURATION_INFO, body);
  page += addMQTTTopicItem(configuration.mqtt.topic, 0, L_MQTT_MAIN_TOPIC);
  return page;
}

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
  body += ">https://</option></select></div><div class=\"cf\">";
  body += "<label>Hostname/IP</label><input name=\"h\" type=\"text\" "
          "maxlength=\"40\" value=\"";
  body += configuration.host;
  body += "\">";
  body += "<span class=\"hint\">Max 40 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>Port</label><input "
          "name=\"p\" type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\"></div><div class=\"cf\"><label>";
  body += L_USERNAME;
  body += "</label><input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><div class=\"cf\"><label>";
  body += L_PASSWORD;
  body += "</label><input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div></fieldset>";

  return addConfigurationBlock(L_DOMOTICZ_CONFIGURATION,
                               L_DOMOTICZ_CONFIGURATION_INFO, body);
}

String AFESitesGenerator::addPasswordConfigurationSite() {
  PASSWORD configuration = Data.getPasswordConfiguration();

  String body = "<fieldset><div class=\"cc\"><label><input name=\"r\" "
                "type=\"checkbox\" value=\"1\"";
  body += (configuration.protect ? " checked=\"checked\"" : "");
  body += "> ";
  body += L_PROTECT_BY_PASSWORD;
  body += "?</label></div>";

  if (configuration.protect) {
    body += "<div class =\"cf\"><label>";
    body += L_PASSWORD;
    body += "</label><input name=\"p\" type=\"text\"  maxlength=\"8\" "
            "value=\"";
    body += configuration.password;
    body += "\"><span class=\"hint\">Max 8 ";
    body += L_NUMBER_OF_CHARS;
    body += "</span></div>";
  }
  body += "</fieldset>";

  return addConfigurationBlock(L_SET_PASSWORD_TO_PANEL, "", body);
}

#ifdef CONFIG_HARDWARE_LED
String AFESitesGenerator::addLEDConfiguration(uint8_t id) {
  LED configuration;
  configuration = Data.getLEDConfiguration(id);

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
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

  String body =
      "<fieldset><div class=\"cf\"><label>LED</label><select name=\"i\">";

  body += "<option value=\"0\"";
  body += configuration == 0 ? " selected=\"selected\"" : "";
  body += L_NONE;
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div></fieldset>";

  return addConfigurationBlock(L_SYSTEM_LED, L_SYSTEM_LED_INFO, body);
}
#endif

String AFESitesGenerator::addRelayConfiguration(uint8_t id) {
  RELAY configuration = Data.getRelayConfiguration(id);
  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);

  body += generateConfigParameter_GPIO(filed, configuration.gpio);

#ifdef CONFIG_FUNCTIONALITY_RELAY
  body += "<div class=\"cf\"><label>";
  body += L_NAME;
  body += "</label><input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div><p class=\"cm\">";
  body += L_DEFAULT_VALUES;
  body += "</p><div class=\"cf\"><label>";
  body += L_DEFAULT_POWER_RESTORED;
  body += "</label><select name=\"pr" + String(id) + "\"><option value=\"0\"";
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

  if (Device->configuration.api.mqtt) {
    body += "<div class=\"cf\">";
    body += "<label>";
    body += L_DEFAULT_MQTT_CONNECTED;
    body += "</label>";
    body += "<select  name=\"mc" + String(id) + "\">";
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
    body += "<option value=\"5\"";
    body += (configuration.state.MQTTConnected == 5 ? " selected=\"selected\""
                                                    : "");
    body += ">";
    body += L_DEFAULT_GET_FROM_MQTT;
    body += "</option>";
    body += "</select>";
    body += "</div>";
  }
#endif

/* Relay Time off / Impuls is not applicable for T3 & T6 */
#if !(defined(T3_CONFIG) || defined(T6_CONFIG))
  body += "<br><p class=\"cm\">";
  body += L_AUTOMATIC_SWITCHING_OFF;
  body += "</p><div class=\"cf\"><label>";
#if defined(T5_CONFIG)
  body += L_IMPULSE_DURATION;
#else
  body += L_SWITCH_OFF_AFTER;
#endif
  body += "</label>";

#if defined(T5_CONFIG)
  body += "<input name=\"ot" + String(id) +
          "\" type=\"number\" step=\"1\" max=\"9999\" min=\"1\" value=\"";
#else
  body += "<input name=\"ot" + String(id) +
          "\" type=\"number\" step=\"0.01\" min=\"0\" max=\"86400\"  value=\"";
#endif
  body += configuration.timeToOff;
  body += "\">";
#if defined(T5_CONFIG)
  body += "<span class=\"hint\">1 - 9999";
  body += L_MILISECONDS;
#else
  body += "<span class=\"hint\">0.01 - 86400 ";
  body += L_SECONDS;
  body += ". ";
  body += L_NO_ACTION_IF_0;
#endif
  body += "</span></div>";

#endif /* T3/T6 exclusion end */

#ifdef CONFIG_HARDWARE_DS18B20
  if (Device->isDS18B20)
#endif

#ifdef CONFIG_HARDWARE_DHXX
    if (Device->isDHT)
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
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

#if (defined(CONFIG_HARDWARE_LED) || !defined(T5_CONFIG))
  body += "<br><p class=\"cm\">";
  body += L_SELECT_LED_4_RELAY;
  body += "</p>";

  body += "<div class=\"cf\"><label>LED</label><select  name=\"l" + String(id) +
          "\"><option value=\"0\"";
  body += configuration.ledID == 0 ? " selected=\"selected\"" : "";
  body += L_NONE;
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.ledID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div>";
#endif

  body += "</fieldset>";

  char title[23];
  sprintf(title, "%s #%d", L_RELAY, id + 1);

  String page = addConfigurationBlock(title, "", body);

#if !defined(T5_CONFIG)
  if (Device->configuration.api.domoticz) {
    page += addDomoticzIDXItem(configuration.domoticz.idx, id);
  }
#endif

  if (Device->configuration.api.mqtt) {
    page += addMQTTTopicItem(configuration.mqtt.topic, id);
  }

  return page;
}

#ifdef CONFIG_FUNCTIONALITY_REGULATOR

String AFESitesGenerator::addRegulatorConfiguration(uint8_t type) {
  RELAY configuration = Data.getRelayConfiguration(0);

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
  if (type == THERMOSTAT_REGULATOR)
#endif
  {
    String body = generateTwoValueController(configuration.thermostat,
                                             THERMOSTAT_REGULATOR);

    return addConfigurationBlock(
        language == 0 ? "Termostat" : "Thermostat",
        language == 0
            ? "Termostat kontroluje przekaźnik w "
              "zależności od wartości temperatury"
            : "Thermostat controlls the relay depending on temperature value",
        body);

  }
#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
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

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div><div class=\"cf\"><label>";
  body += L_FUNCTIONALITY;
  body += "</label><select name=\"f" + String(id) + "\"><option value=\"";
  body += SWITCH_FUNCTIONALITY_NONE;
  body += "\"";
  body += (configuration.functionality == SWITCH_FUNCTIONALITY_NONE
               ? " selected=\"selected\""
               : "");
  body += ">";
  body += L_NONE;
  body += "</option><option value=\"";
  body += SWITCH_FUNCTIONALITY_MULTI;
  body += "\"";
  body += (configuration.functionality == SWITCH_FUNCTIONALITY_MULTI
               ? " selected=\"selected\""
               : "");
  body += ">";
  body += L_SYSTEM_BUTTON;
  body += "</option><option value=\"";
  body += SWITCH_FUNCTIONALITY_RELAY;
  body += "\"";
  body += (configuration.functionality == SWITCH_FUNCTIONALITY_RELAY
               ? " selected=\"selected\""
               : "");
  body += ">";
#ifdef CONFIG_FUNCTIONALITY_GATE
  body += L_CONTROL_GATE;
#endif
#ifdef CONFIG_FUNCTIONALITY_RELAY
  body += L_CONTROL_RELAY;
#endif
  body += "</option></select></div><div class=\"cf\"><label>";
  body += L_RELAY_CONTROLLED_BY_SWITCH;
  body += "</label><select  name=\"r" + String(id) + "\"><option value=\"0\"";
  body += configuration.relayID == 0 ? " selected=\"selected\"" : "";
  body += L_NONE;
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device->configuration.isRelay); i++) {
    if (Device->configuration.isRelay[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.relayID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div><div class=\"cf\"><label>";
  body += L_TYPE;
  body += "</label><select name=\"m" + String(id) + "\"><option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += L_MONOSTABLE;
  body += "</option><option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += L_BISTABLE;
  body += "</option></select></div><br><p class=\"cm\">";
  body += L__SENSITIVENESS_HINT;
  body += "</p><div class=\"cf\"><label>";
  body += L_SENSITIVENESS;
  body += "</label><input name=\"s" + String(id) +
          "\" type=\"number\" max=\"999\" min=\"0\" step=\"1\" "
          "value=\"";
  body += configuration.sensitiveness;
  body += "\"><span class=\"hint\">0-999 (";
  body += L_MILISECONDS;
  body += ")</span></div></fieldset>";

  char title[23];
  sprintf(title, "%s #%d", L_SWITCH_BUTTON, id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (Device->configuration.api.domoticz) {
    page += addDomoticzIDXItem(configuration.domoticz.idx, id);
  }

  if (Device->configuration.api.mqtt) {
    page += addMQTTTopicItem(configuration.mqtt.topic, id);
  }

  return page;
}

#ifdef CONFIG_HARDWARE_DS18B20
String AFESitesGenerator::addDS18B20Configuration() {

  DS18B20 configuration = Data.getSensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label>";
  body +=
      "<input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" type=\"number\" "
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
  body +=
      "</label><input name=\"c\" type=\"number\" min=\"-9.99\" max=\"9.99\" "
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

#ifdef CONFIG_HARDWARE_DHXX
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
  body +=
      language == 0
          ? ">Wysyłać dane tylko, gdy wartość temperatury lub wilgotności "
            "zmieni się"
          : ">Send data only if value of temperature or humidity has changed";
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
  body +=
      "</label><input name=\"c\" type=\"number\" min=\"-9.99\" max=\"9.99\" "
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
  body +=
      "</label><input name=\"d\" type=\"number\" min=\"-99.9\" max=\"99.9\" "
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

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
String AFESitesGenerator::addThermostateMenuItem() {
  String page = "<li class=\"itm\"><a href=\"\\?o=thermostat\">&#8227; ";
  page += language == 0 ? "Termostat" : "Thermostat";
  page += "</a></li>";
  return page;
}
#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
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
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
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
  body +=
      (configuration.outputDefaultState == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NO" : "NO";
  body += "</option><option value=\"1\"";
  body +=
      (configuration.outputDefaultState == 1 ? " selected=\"selected\"" : "");
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

#if defined(T5_CONFIG)
String AFESitesGenerator::addContactronConfiguration(uint8_t id) {
  CONTACTRON configuration = Data.getContactronConfiguration(id);
  DEVICE deviceConfiguration = Device->configuration;

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div><div class=\"cf\"><label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "</label><input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Typ" : "Type";
  body += "</label><select name=\"o" + String(id) + "\"><option value=\"0\"";
  body +=
      (configuration.outputDefaultState == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NO" : "NO";
  body += "</option><option value=\"1\"";
  body +=
      (configuration.outputDefaultState == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "NC" : "NC";
  body += "</option></select></div><div class=\"cf\"><label>LED ";
  body += language == 0 ? "przypisany do czujnika" : "assigned to the sensor";
  body += "</label><select  name=\"l" + String(id) + "\"><option value=\"0\"";
  body += configuration.ledID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.ledID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select></div><br><p class=\"cm\">";
  body += language == 0
              ? "Czułość należy ustawić eksperymentalnie, aż uzyska się "
                "pożądane działanie czujnika magnetycznego"
              : "Sensitiveness should be adjusted experimentally until "
                "sensor behaves as expected";

  body += "</p><div class=\"cf\"><label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "</label>";
  body += "<input name=\"b" + String(id) +
          "\" type=\"number\" max=\"2000\" min=\"0\" step=\"1\" "
          "value=\"";
  body += configuration.bouncing;
  body += "\"><span class=\"hint\">0 - 2000 (milise";
  body += language == 0 ? "kund" : "conds";
  body += ")</span></div>";

  char title[23];
  language == 0 ? sprintf(title, "Czujnik magnetyczny #%d", id + 1)
                : sprintf(title, "Magnetic sensor #%d", id + 1);

  String page = addConfigurationBlock(title, "", body);

  if (deviceConfiguration.api.domoticz) {
    body = "<div class=\"cf\"><label>IDX</label>";
    body += "<input name=\"x" + String(id) +
            "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span></div>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  body += "</fieldset>";

  return page;
}

String AFESitesGenerator::addGateConfiguration() {
  GATE gateConfiguration = Data.getGateConfiguration();
  DEVICE deviceConfiguration = Device->configuration;
  uint8_t noOfContactrons = deviceConfiguration.isContactron[1] ? 2 : 1;
  CONTACTRON configuration[noOfContactrons];

  for (uint8_t i = 0; i < noOfContactrons; i++) {
    configuration[i] = Data.getContactronConfiguration(i);
  }

  String body = "<fieldset>";

  body += "<p class=\"cm\">";
  if (language == 0) {
    body += "Jeśli czujnik magnetyczny: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> oraz czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> są otwarte";
    } else {
      body += "</strong> jest otwarty";
    }
    body += " to:";
  } else {
    body += "If magnetic sensor: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> and <strong>";
      body += configuration[1].name;
      body += "</strong> are open";
    } else {
      body += "</strong> is open";
    }
    body += " then:";
  }
  body += "</p>";
  body += generateGateStatesList(0, gateConfiguration.state[0]);
  if (noOfContactrons == 2) {
    body += "<br><br><p class=\"cm\">";
    if (language == 0) {
      body += "Jeśli czujnik magnetyczny: <strong>";
      body += configuration[0].name;
      body += "</strong> jest otwarty, a czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> jest zamknięty to:";
    } else {
      body += "If magnetic sensor: <strong>";
      body += configuration[0].name;
      body += "</strong> is open and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> is closed then:";
    }
    body += "</p>";
    body += generateGateStatesList(1, gateConfiguration.state[1]);

    body += "<br><br><p class=\"cm\">";
    if (language == 0) {
      body += "Jeśli czujnik magnetyczny: <strong>";
      body += configuration[0].name;
      body += "</strong> jest zamknięty, a czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> jest otwarty to:";
    } else {
      body += "If magnetic sensor: <strong>";
      body += configuration[0].name;
      body += "</strong> is closed and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> is open then:";
    }
    body += "</p>";
    body += generateGateStatesList(2, gateConfiguration.state[2]);
  }

  body += "<br><br><p class=\"cm\">";
  if (language == 0) {
    body += "Jeśli czujnik magnetyczny: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> oraz czujnik: <strong>";
      body += configuration[1].name;
      body += "</strong> są zamknięte";
    } else {
      body += "</strong> jest zamknięty ";
    }
    body += " to:";
  } else {
    body += "If magnetic sensor: <strong>";
    body += configuration[0].name;

    if (noOfContactrons == 2) {
      body += "</strong> and sensor: <strong>";
      body += configuration[1].name;
      body += "</strong> are closed";
    } else {
      body += "</strong> is closed ";
    }
    body += " then:";
  }
  body += "</p>";
  body += generateGateStatesList(3, gateConfiguration.state[3]);

  String page = addConfigurationBlock(
      language == 0 ? "Konfiguracja stanów bramy" : "Gate states configuration",
      "", body);

  if (deviceConfiguration.api.domoticz) {
    body = "<div class=\"cf\">";
    body += "<label>IDX</label>";
    body += "<input name=\"x\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += gateConfiguration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  page += "</fieldset>";

  return page;
}

const String AFESitesGenerator::generateGateStatesList(uint8_t id, byte state) {
  String body = "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Ustaw stan bramy na" : "Set gate's state to";
  body += "</label>";
  body += "<select name=\"s" + String(id) + "\">";
  body += "<option value=\"";
  body += GATE_OPEN;
  body += "\"";
  body += (state == GATE_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Otwarta" : "Open";
  body += "</option>";
  body += "<option value=\"";
  body += GATE_PARTIALLY_OPEN;
  body += "\"";
  body += (state == GATE_PARTIALLY_OPEN ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Częściowo otwarta" : "Partially open";
  body += "</option>";
  body += "<option value=\"";
  body += GATE_CLOSED;
  body += "\"";
  body += (state == GATE_CLOSED ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Zamknięta" : "Closed";
  body += "</option>";
  body += "</select>";
  body += "</div>";

  return body;
}
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
String AFESitesGenerator::addHPMA115S0Configuration() {

  HPMA115S0 configuration = Data.getHPMA115S0SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span></div><br><br>";
  body += "<p class=\"cm\">";
  body +=
      language == 0
          ? "Jeśli poniższa wartość jest większa od 0 to czujnik będzie "
            "usypiany między odczytami. Wartość poniżej definiuje na ile "
            "sekund przed odczytem czujnik ma zostać uruchomiony. Wartość musi "
            "być mniejsza niż interwał pomiarów"
          : "If the parameter below is different than 0, the sensor will go to "
            "sleep mode between measurements. The setting below defined how "
            "many seconds before a measurement the sensor should wake up. It "
            "should be lower than measurement's interval";
  body += "</p>";
  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Pomiar po czasie" : "Measure after";
  body += "</label><input name=\"t\" min=\"0\" max=\"999\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.timeToMeasure;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 1 do 999sek"
                        : "seconds. Range: 1 to 999sec";
  body += "</span></div>";

  body += "</fieldset>";

  String page =
      addConfigurationBlock(language == 0 ? "Czujnik cząstek PM2.5/PM10"
                                          : "PM2.5/PM10 Particle Sensor",
                            "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label> ";
    body += "IDX PM2.5 </label>";
    body += "<input name=\"x2\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pm25;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>IDX PM10";
    body += "</label><input name=\"x1\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pm10;
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

#ifdef CONFIG_HARDWARE_BMX80
String AFESitesGenerator::addBMx80Configuration() {

  BMx80 configuration = Data.getBMx80SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += addDeviceI2CAddressSelection(configuration.i2cAddress);

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"5\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 5 do 86400sek"
                        : "seconds. Range: 5 to 86400sec";
  body += " (24h)</span></div><br><br>";
  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik BMx80" : "BMx80 Sensor", "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";
    if (device.isBMx80 != TYPE_BMP180_SENSOR) {
      body += "<div class=\"cf\"><label>IDX Temp/";
      body += language == 0 ? "Wilg/Bar" : "Humi/Bar";
      body += "</label><input name=\"t\" type=\"number\" step=\"1\" min=\"0\" "
              "max=\"999999\"  value=\"";
      body += configuration.idx.temperatureHumidityPressure;
      body += "\">";
      body += "<span class=\"hint\">";
      body += language == 0 ? "Zakres: " : "Range: ";
      body += "0 - 999999</span>";
      body += "</div>";
    }

    body += "<div class=\"cf\"><label>IDX Temperatur";
    body += language == 0 ? "a" : "e";
    body += "</label><input name=\"e\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.temperature;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    if (device.isBMx80 != TYPE_BMP180_SENSOR) {
      body += "<div class=\"cf\"><label>IDX ";
      body += language == 0 ? "Wilgotność" : "Humidity";
      body += "</label><input name=\"h\" type=\"number\" step=\"1\" min=\"0\" "
              "max=\"999999\"  value=\"";
      body += configuration.idx.humidity;
      body += "\">";
      body += "<span class=\"hint\">";
      body += language == 0 ? "Zakres: " : "Range: ";
      body += "0 - 999999</span>";
      body += "</div>";
    }

    body += "<div class=\"cf\"><label>IDX ";
    body += language == 0 ? "Ciśnienie" : "Pressure";
    body += "</label><input name=\"p\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx.pressure;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    if (device.isBMx80 == TYPE_BME680_SENSOR) {
      body += "<div class=\"cf\"><label>IDX ";
      body += language == 0 ? "Czujnik gazu" : "Gas sensor";
      body += "</label><input name=\"g\" type=\"number\" step=\"1\" min=\"0\" "
              "max=\"999999\"  value=\"";
      body += configuration.idx.gasResistance;
      body += "\">";
      body += "<span class=\"hint\">";
      body += language == 0 ? "Zakres: " : "Range: ";
      body += "0 - 999999</span>";
      body += "</div>";
    }
    body += "</fieldset>";
    page += addConfigurationBlock(
        "Domoticz",
        language == 0
            ? "Jeśli IDX jest 0 to wartość nie będzie wysyłana do Domoticz"
            : "If IDX is set to 0 then a value won't be sent to Domoticz",
        body);
  }

  return page;
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
String AFESitesGenerator::addBH1750Configuration() {

  BH1750 configuration = Data.getBH1750SensorConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  body += addDeviceI2CAddressSelection(configuration.i2cAddress);

  body += "<div class=\"cf\"><label>";
  body += language == 0 ? "Interwał odczytów" : "Measurement's interval";
  body += "</label><input name=\"i\" min=\"1\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 1 do 86400sek"
                        : "seconds. Range: 1 to 86400sec";
  body += " (24h)</span></div><div class=\"cf\"><label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "</label><input name=\"m\" type=\"number\" value=\"";
  body += configuration.mode;
  body += "\" disabled><span class=\"hint\">";
  body += language == 0 ? "(brak możliwości zmiany)" : "(can't be set)";
  body += " </span></div>";

  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Czujnik BH1750" : "BH1750 Sensor", "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label>IDX</label><input name=\"d\" "
            "type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\"><span class=\"hint\">";
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

#ifdef CONFIG_HARDWARE_ADC_VCC
String AFESitesGenerator::addAnalogInputConfiguration() {
  ADCINPUT configuration = Data.getADCInputConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body =
      "<fieldset><div class=\"cf\"><label>GPIO</label><input name=\"g\" "
      "type=\"number\" value=\"";
  body += configuration.gpio;
  body += "\"><span class=\"hint\">(";
  body += L_CANT_CHANGE;
  body += ")</span></div><div class=\"cf\"><label>";
  body += L_MEASURMENTS_INTERVAL;
  body += "</label><input name=\"i\" min=\"1\" max=\"86400\" step=\"1\" "
          "type=\"number\" "
          "value=\"";
  body += configuration.interval;
  body += "\"><span class=\"hint\">1-86400 (24h) ";
  body += L_SECONDS;
  body += "</span></div><div class=\"cf\"><label>";
  body += L_MEASURED_VOLTAGE;
  body += "</label><input name=\"m\" min=\"0\" max=\"1000\"  step=\"0.01\""
          "type=\"number\" value=\"";
  body += configuration.maxVCC;
  body += "\"><span class=\"hint\"> 0-999.99V</span></div><div "
          "class=\"cf\"><label>";
  body += L_RESISTOR;
  body += " R[A]</label><input name=\"ra\" min=\"0\" max=\"90000000\"  "
          "step=\"0.01\" type=\"number\" value=\"";
  body += configuration.divider.Ra;
  body += "\"></div><div class=\"cf\"><label>";
  body += L_RESISTOR;
  body += " R[B]</label><input name=\"rb\" min=\"0\" max=\"90000000\"  "
          "step=\"0.01\" type=\"number\" value=\"";
  body += configuration.divider.Rb;
  body += "\"></div><div class=\"cf\"><label>";
  body += L_NUMBER_OF_SAMPLES;
  body += "</label><input name=\"n\" min=\"1\" max=\"999\" step=\"1\" "
          "type=\"number\" value=\"";
  body += configuration.numberOfSamples;
  body += "\"><span class=\"hint\">1-999</span></div><br><br></fieldset>";

  String page = addConfigurationBlock(L_ANALOG_INPUT, "", body);

  if (device.api.mqtt) {
    page += addMQTTTopicItem(configuration.mqtt.topic, 0);
  }

  if (Device->configuration.api.domoticz) {
    page += addDomoticzIDXItem(configuration.domoticz.raw, 0, L_RAW_DATA);
    page += addDomoticzIDXItem(configuration.domoticz.percent, 1, L_PERCENT);
    page += addDomoticzIDXItem(configuration.domoticz.voltage, 2, L_VOLTAGE);
    page += addDomoticzIDXItem(configuration.domoticz.voltageCalculated, 3,
                               L_VOLTAGE_CALCULATED);
  }

  return page;
}
#endif

#ifdef CONFIG_HARDWARE_UART
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

#ifdef CONFIG_HARDWARE_I2C
String AFESitesGenerator::addDeviceI2CAddressSelection(uint8_t address) {
  AFEI2CScanner I2CScanner;
  String body = "<div class=\"cf\"><label>I2C Ad";
  body += language == 0 ? "res" : "ddress";
  body += ": </label><select name=\"a\">";
  body += "<option value=\"0\"";
  body += address == 0 ? " selected=\"selected\"" : "";
  body += ">";
  body += language == 0 ? "Brak" : "Missing";
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
  if (command != MODE_ACCESS_POINT) {
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
  configuration = Data.getDeviceConfiguration();
  String body = "<fieldset>";
  if (!authorized) {
    body += "<h3>";
    body += L_WRONG_PASSWORD;
    body += "</h3>";
  }

  body +=
      "<form method=\"post\"><div class=\"cf\"><input name=\"p\" type=\"text\" "
      "placeholder=\"";
  body += L_PASSWORD;
  body += "\"></div><div class=\"cf\"><input type=\"submit\" class=\"b bs\" "
          "value=\"";
  body += L_NORMAL_MODE;
  body += "\" formaction=\"/?o=0&i=";
  body += MODE_CONFIGURATION;
  body += "\" /> <input type=\"submit\" class=\"b be\" value=\"";
  body += L_HOTSPOT_MODE;
  body += "\" formaction=\"/?o=0&i=";
  body += MODE_ACCESS_POINT;
  body += "\" /></div></form>";

  String page = addConfigurationBlock(L_LAUNCH_CONFIGURATION_PANEL, "", body);
  /*
    body = "<p class=\"cm\">Oprogramowanie dostępne jest za darmo. Jeśli spełnia
    " "Twoje oczekiwania to postaw <a "
           "href=\"https://www.smartnydom.pl/o-stronie/\" target=\"_blank\" "
           "style=\"color:#00e\">autorowi</a> browarka ;)</p><a "
           "href=\"https://pl.donate.afe-firmware.smartnydom.pl\" "
           "target=\"_blank\"><img "
           "src=\"http://adrian.czabanowski.com/afe/donation/T2/1.4.0/1rTA706u/"
           "\" border=\"0\" alt=\"PayPal\"></a>";

    page += addConfigurationBlock("Wsparcie", "", body);
  */
  return page;
}

String AFESitesGenerator::addProVersionSite() {
  PRO_VERSION configuration = Data.getProVersionConfiguration();
  String body;
  if (Device->getMode() == MODE_CONFIGURATION) {
    body = "<fieldset>";
    body += "<div class=\"cf\"><label>";
    body += L_KEY;
    body += "</label><input name=\"k\" type=\"text\" maxlength=\"18\" value=\"";
    body += configuration.serial;
    body += "\"></div>";
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
            "href=\"http://www.smartnydom.pl/afe-firmware-";
    body += L_LANGUAGE_SHORT;
    body += "/\" target=\"_blank\"><img src=\"https://img.shields.io/badge/";
    body += L_HELP;
    body += "-";
    body += L_DOCUMENTATION;
    body += "-green.svg\" alt=\"";
    body += L_DOCUMENTATION;
    body +=
        "\" /></a> <a href=\"https://www.smartnydom.pl/afe-firmware-pl/log\" "
        "target=\"_blank\"><img src=\"https://img.shields.io/badge/";
    body += L_VERSION;
    body += "%20-%20";
    body += FIRMWARE_VERSION;
    body += "[T";
    body += FIRMWARE_TYPE;
    body += "]-blue.svg\" alt=\"T";
    body += FIRMWARE_TYPE;
    body += "-";
    body += FIRMWARE_VERSION;
    body += "\" /></a> <img src=\"https://img.shields.io/badge/PRO-";
    body += Firmware.isUnlocked() ? L_YES : L_NO;
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

const String AFESitesGenerator::generateHardwareItemsList(
    uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label) {
  String body = "<div class=\"cf\"><label>";
  body += label;
  body += "</label>";
  body += "<select name=\"";
  body += field;
  body += "\">";
  body += "<option value=\"0\"";
  body += (noOffConnected == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += L_NONE;
  body += "</option>";

  for (uint8_t i = 1; i <= noOfItems; i++) {
    body += "<option value=\"";
    body += i;
    body += "\"";
    body += noOffConnected == i ? " selected=\"selected\"" : "";
    body += ">";
    body += i;
    body += "</option>";
  }

  body += "</select>";
  body += "</div>";

  return body;
}

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
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

const String AFESitesGenerator::addMQTTTopicItem(char *topic, uint8_t id,
                                                 const String title,
                                                 const String subtitle) {
  String body = "<fieldset><div class=\"cf\"><label>";
  body += L_MQTT_TOPIC;
  body += "</label><input name=\"t" + String(id) +
          "\" type=\"text\" maxlength=\"32\" value=\"";
  body += topic;
  body += "\"><span class=\"hint\">Max 32 ";
  body += L_NUMBER_OF_CHARS;
  body += "</span></div></fieldset>";
  return addConfigurationBlock(title, subtitle, body);
}

const String AFESitesGenerator::addDomoticzIDXItem(unsigned long idx,
                                                   uint8_t id,
                                                   const String title) {
  String body =
      "<fieldset><div class=\"cf\"><label>IDX</label><input name=\"x" +
      String(id) +
      "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
  body += idx;
  body += "\"><span class=\"hint\">";
  body += L_RANGE;
  body += " 0-999999</span></div></fieldset>";
  return addConfigurationBlock("Domoticz" + title, L_NO_IF_IDX_0, body);
}
