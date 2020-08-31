/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() {}

void AFESitesGenerator::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                              AFEFirmwarePro *_FirmwarePro) {
  Device = _Device;
  FirmwarePro = _FirmwarePro;
  Data = _Data;
  Data->getConfiguration(&Firmware);
  Data->getDeviceUID().toCharArray(deviceID, sizeof(deviceID) + 1);
}

void AFESitesGenerator::generateHeader(String &page, uint16_t redirect) {

  page += FPSTR(HTTP_HEADER);

  if (redirect > 0) {
    page.replace("{{s.redirect}}", "<meta http-equiv=\"refresh\" content=\"" +
                                       String(redirect) + "; url=/\">");
  } else {
    page.replace("{{s.redirect}}", "");
  }

  if (Device->getMode() == AFE_MODE_CONFIGURATION ||
      Device->getMode() == AFE_MODE_NORMAL) {
    page.concat("<img src=\"http://api.smartnydom.pl/logo/T");
    page.concat(Firmware.type);
    page.concat("/");
    page.concat(Firmware.version);
    page.concat("/");
    page.concat(deviceID);
    page.concat("/");
    page.concat(AFE_DEVICE_TYPE_ID);
    page.concat(
        "\" style=\"width: 100%;display: block\" alt=\"AFE Firmware\">");
  } else {
    page.concat("<h3 class=\"la\">AFE Firmware: ");
    page.concat(AFE_DEVICE_TYPE_NAME);
    page.concat("</h3>");
  }

  page.concat("<div id=\"c\">");
}

void AFESitesGenerator::generateOneColumnLayout(String &page,
                                                uint16_t redirect) {
  generateHeader(page, redirect);
  page.concat("<div id=\"r\">");
}

void AFESitesGenerator::generateTwoColumnsLayout(String &page,
                                                 uint16_t redirect) {
  Device->begin();

  generateHeader(page, redirect);
  page.concat("<div id=\"l\">");
  if (Device->getMode() == AFE_MODE_ACCESS_POINT) {
    page.concat("<h3 class=\"ltit\">AFE FIRMWARE</h3>");
  }
  page.concat("<h4>");
  page.concat(L_FIRMWARE_NAME);
  page.concat("</h4><ul class=\"lst\">");

  /* Gnerating Menu */
  addMenuItem(page, L_DEVICE, AFE_CONFIG_SITE_DEVICE);
  addMenuItem(page, L_NETWORK, AFE_CONFIG_SITE_NETWORK);

  if (Device->configuration.api.mqtt) {
    addMenuItem(page, L_MQTT_BROKER, AFE_CONFIG_SITE_MQTT);
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz) {
    addMenuItem(page, L_DOMOTICZ_SERVER, AFE_CONFIG_SITE_DOMOTICZ);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  if (Device->configuration.noOfLEDs > 0) {
    addMenuHeaderItem(page, L_LEDS);
    addMenuSubItem(page, "LED", Device->configuration.noOfLEDs,
                   AFE_CONFIG_SITE_LED);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (Device->configuration.noOfGates > 0)
    page.concat("<li  class=\"itm\"><a><i>");
  page.concat(L_GATE_CONFIGURATION);
  page.concat("</i></a></li>");

  for (uint8_t i = 0; i < Device->configuration.noOfGates; i++) {
    page.concat("<li class=\"itm\"><a href=\"\\?o=");
    page.concat(AFE_CONFIG_SITE_GATE);
    page.concat("&i=");
    page.concat(i);
    page.concat("\">&#8227; ");
    page.concat(L_GATE);
    page.concat(": ");
    page.concat(i + 1);
    page.concat("</a></li>");
  }
}
#endif

/* Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
if (Device->configuration.noOfRelays > 0) {

  addMenuHeaderItem(page, L_RELAYS_CONFIGURATION);
  addMenuSubItem(page, L_SWITCH, Device->configuration.noOfRelays,
                 AFE_CONFIG_SITE_RELAY);

/* Thermostat */
#if defined(AFE_CONFIG_HARDWARE_DS18B20) &&                                    \
    defined(AFE_CONFIG_FUNCTIONALITY_THERMOSTAT)
  if (Device->configuration.isDS18B20) {
    page.concat(addThermostateMenuItem());
  }
#endif

/* Humidistat */
#if defined(AFE_CONFIG_HARDWARE_DHXX) &&                                       \
    defined(AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT)
  if (Device->configuration.isDHT) {
    page.concat(addThermostateMenuItem());
    page.concat(addHumidistatMenuItem());
  }
#endif
}

#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
if (Device->configuration.noOfSwitches > 0) {
  addMenuHeaderItem(page, L_BUTTONS_SWITCHES);
  addMenuSubItem(page, L_SWITCH, Device->configuration.noOfSwitches,
                 AFE_CONFIG_SITE_SWITCH);
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

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
  page.concat("<li class=\"itm\"><a><i>Konfiguracja czujników ruchu "
              "(PIR)</i></a></li>");
  for (uint8_t i = 0; i < 4; i++) {
    if (Device->configuration.isPIR[i]) {
      page.concat("<li class=\"itm\"><a href=\"\\?o=pir");
      page.concat(i);
      page.concat("\">&#8227; Czujnik: ");
      page.concat(i + 1);
      page.concat("</a></li>");
    }
  }
}
#endif

/* Contactrons and Gate */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

if (Device->configuration.noOfContactrons > 0) {
  page.concat("<li class=\"itm\"><a><i>");
  page.concat(L_MAGNETIC_SENSORS);
  page.concat("</i></a></li>");
  for (uint8_t i = 0; i < Device->configuration.noOfContactrons; i++) {
    page.concat("<li class=\"itm\"><a href=\"\\?o=");
    page.concat(AFE_CONFIG_SITE_CONTACTRON);
    page.concat("&i=");
    page.concat(i);
    page.concat("\">&#8227; ");
    page.concat(L_SENSOR);
    page.concat(": ");
    page.concat(i + 1);
    page.concat("</a></li>");
  }
}

#endif

/* Sensor DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
if (Device->configuration.noOfDS18B20s > 0) {

  addMenuHeaderItem(page, L_BUTTONS_SWITCHES);
  addMenuSubItem(page, L_SENSOR, Device->configuration.noOfDS18B20s,
                 AFE_CONFIG_SITE_DS18B20);
}
#endif

/* Sensor DHxx */
#ifdef AFE_CONFIG_HARDWARE_DHXX
if (Device->configuration.isDHT) {
  page.concat("<li class=\"itm\"><a href=\"\\?o=DHT\">");
  page.concat(language == 0 ? "Czujnik DHT" : "DHT sensor");
  page.concat("</a></li>");
}
#endif

/* UART */
#ifdef AFE_CONFIG_HARDWARE_UART

/* Don't show it if HPMA115SO sensor is not added to the device */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
if (Device->configuration.noOfHPMA115S0s > 0) {
#endif
  page.concat("<li class=\"itm\"><a href=\"\\?o=");
  page.concat(AFE_CONFIG_SITE_UART);
  page.concat("\">UART</a></li>");
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

  page.concat("<li class=\"itm\"><a href=\"\\?o=");
  page.concat(AFE_CONFIG_SITE_I2C);
  page.concat("\">I2C</a></li>");

#ifdef T6_CONFIG
}
#endif

#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
/* This is hardcoded for one sensor */
if (Device->configuration.noOfHPMA115S0s > 0) {
  page.concat("<li class=\"itm\"><a href=\"\\?i=0&o=");
  page.concat(AFE_CONFIG_SITE_HPMA115S0);
  page.concat("\">");
  page.concat(L_PARTICLE_SENSOR);
  page.concat("</a></li>");
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
if (Device->configuration.noOfBMEX80s > 0) {
  page.concat("<li  class=\"itm\"><a><i>");
  page.concat(L_BMEX80_SENSORS);
  page.concat("</i></a></li>");

  for (uint8_t i = 0; i < Device->configuration.noOfBMEX80s; i++) {
    page.concat("<li class=\"itm\"><a href=\"\\?o=");
    page.concat(AFE_CONFIG_SITE_BMEX80);
    page.concat("&i=");
    page.concat(i);
    page.concat("\">&#8227; ");
    page.concat(L_SENSOR);
    page.concat(": ");
    page.concat(i + 1);
    page.concat("</a></li>");
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
if (Device->configuration.noOfBH1750s > 0) {
  page.concat("<li  class=\"itm\"><a><i>");
  page.concat(L_BH1750_SENSORS);
  page.concat("</i></a></li>");

  for (uint8_t i = 0; i < Device->configuration.noOfBH1750s; i++) {
    page.concat("<li class=\"itm\"><a href=\"\\?o=");
    page.concat(AFE_CONFIG_SITE_BH1750);
    page.concat("&i=");
    page.concat(i);
    page.concat("\">&#8227; ");
    page.concat(L_SENSOR);
    page.concat(": ");
    page.concat(i + 1);
    page.concat("</a></li>");
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
if (Device->configuration.noOfAnemometerSensors > 0) {
  page.concat("<li class=\"itm\"><a href=\"\\?i=0&o=");
  page.concat(AFE_CONFIG_SITE_ANEMOMETER_SENSOR);
  page.concat("\">");
  page.concat(L_ANEMOMETER_SENSOR);
  page.concat("</a></li>");
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
if (Device->configuration.noOfRainmeterSensors > 0) {
  page.concat("<li class=\"itm\"><a href=\"\\?i=0&o=");
  page.concat(AFE_CONFIG_SITE_RAINMETER_SENSOR);
  page.concat("\">");
  page.concat(L_RAINMETER);
  page.concat("</a></li>");
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
if (Device->configuration.noOfAS3935s > 0) {
  page.concat("<li class=\"itm\"><a href=\"\\?i=0&o=");
  page.concat(AFE_CONFIG_SITE_AS3935);
  page.concat("\">");
  page.concat(L_AS3935_SENSOR);
  page.concat("</a></li>");
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
if (Device->configuration.isAnalogInput) {
  addMenuItem(page, L_ANALOG_INPUT, AFE_CONFIG_SITE_ANALOG_INPUT);
}
#endif

page.concat("<br />");

addMenuItem(page, L_SET_PASSWORD, AFE_CONFIG_SITE_PASSWORD);
addMenuItem(page, L_FIRMWARE_UPGRADE, AFE_CONFIG_SITE_UPGRADE);
addMenuItem(page, L_RESET_DEVICE, AFE_CONFIG_SITE_RESET);
addMenuItem(page, L_PRO_VERSION, AFE_CONFIG_SITE_PRO_VERSION);
addMenuItem(page, L_FINISH_CONFIGURATION, AFE_CONFIG_SITE_EXIT);

/* Information section */
page.concat("</ul></div><div id=\"r\">");
}

void AFESitesGenerator::addDeviceConfiguration(String &page) {
  DEVICE configuration = Device->configuration;
  boolean _itemDisabled = false;

  if (Device->upgraded != AFE_UPGRADE_NONE) {
    page.concat("<h4 class=\"bc\" style=\"padding:5px;\">");
    switch (Device->upgraded) {
    case AFE_UPGRADE_VERSION:
      page.concat(L_UPGRADED_TO_NEW_VERSION);
      break;
    case AFE_UPGRADE_VERSION_TYPE:
      page.concat(L_UPGRADED_TO_NEW_VERSION_TYPE);
      break;
    }
    page.concat("</h4>");
    Device->upgraded = AFE_UPGRADE_NONE;
  }

  /* Section: Device name */
  openSection(page, L_DEVICE, L_DEVICE_SECTION_INFO);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_DEVICE_NAME,
                   configuration.name, "16");
  closeSection(page);

  /* Section: Hardware */
  openSection(page, L_HARDWARE_CONFIGURATION, L_HARDWARE_CONFIGURATION_INFO);

/* LED */
#ifdef AFE_CONFIG_HARDWARE_LED
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS,
                            Device->configuration.noOfLEDs, "l",
                            L_NUMBER_OF_LEDS);
#endif

/* Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS,
                            Device->configuration.noOfContactrons, "co",
                            L_NUMBER_OF_MAGNETIC_SENSORS);

#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Relay */
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS,
                            Device->configuration.noOfRelays, "r",
                            L_NUMBER_OF_RELAYS);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Switch */
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES,
                            Device->configuration.noOfSwitches, "s",
                            L_NUMBER_OF_SWITCHES);
#endif

/* DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20,
                            Device->configuration.noOfDS18B20s, "ds",
                            L_NUMBER_OF_DS18B20_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
// TODO
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0,
                            Device->configuration.noOfHPMA115S0s, "hp",
                            L_NUMBER_OF_HPMA115S0_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750

#ifdef T5_CONFIG // Functionality is PRO for T5
  _itemDisabled = !Firmware->Pro.valid;
#else
  _itemDisabled = false;
#endif

  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750,
                            Device->configuration.noOfBH1750s, "bh",
                            L_NUMBER_OF_BH1750_SENSORS, _itemDisabled);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80

#ifdef T5_CONFIG // Functionality is PRO for T5
  _itemDisabled = !Firmware->Pro.valid;
#else
  _itemDisabled = false;
#endif

  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80,
                            Device->configuration.noOfBMEX80s, "b6",
                            L_NUMBER_OF_BMEX80_SENSORS, _itemDisabled);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935,
                            Device->configuration.noOfAS3935s, "a3",
                            L_NUMBER_OF_AS3935_SENSORS,
                            !FirmwarePro->Pro.valid);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  generateHardwareItemsList(
      page, AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS,
      Device->configuration.noOfAnemometerSensors, "w", L_ANEMOMETER_SENSOR);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  generateHardwareItemsList(
      page, AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS,
      Device->configuration.noOfRainmeterSensors, "d", L_RAINMETER);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  addCheckboxFormItem(
      page, "ad", L_DO_MEASURE_ADC, "1", configuration.isAnalogInput,
      (!FirmwarePro->Pro.valid ? L_PRO_VERSION : AFE_FORM_ITEM_SKIP_PROPERTY),
      !FirmwarePro->Pro.valid);
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

  closeSection(page);

#ifdef AFE_CONFIG_HARDWARE_GATE

  addConfigurationBlock(page, L_CONTROLLED_GATES, "");

  page.concat("<fieldset>");

  generateHardwareItemsList(page, AFE_CONFIG_HARDWARE_NUMBER_OF_GATES,
                            Device->configuration.noOfGates, "g",
                            L_NUMBER_OF_CONTROLLED_GATES);
  page.concat(FPSTR(HTTP_FORM_FIELD_CLOUSRE);
#endif


/* Section: APIs */
  openSection(page,L_DEVICE_CONTROLLING,L_DEVICE_CONTROLLING_INFO);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

  page.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  page.replace("{{item.label}}", L_DOMOTICZ_VERSION);
  page.replace("{{item.name}}", "v");
  
  page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  page.replace("{{item.value}}",
               String(AFE_DOMOTICZ_VERSION_0));
  page.replace("{{item.label}}", L_DOMOTICZ_VERSION_410);
  page.replace("{{item.selected}}",
               configuration.api.domoticzVersion == AFE_DOMOTICZ_VERSION_0
                   ? " selected=\"selected\""
                   : "");

  page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  page.replace("{{item.value}}",
               String(AFE_DOMOTICZ_VERSION_1));
  page.replace("{{item.label}}", L_DOMOTICZ_VERSION_2020);
  page.replace("{{item.selected}}",
               configuration.api.domoticzVersion == AFE_DOMOTICZ_VERSION_1
                   ? " selected=\"selected\""
                   : "");
 page.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));

#endif


  addCheckboxFormItem(page, "h", "HTTP API", "1", configuration.api.http);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  addRadioButtonFormItem(page, "m", "Domoticz HTTP API", "1", configuration.api.domoticz);
  addCheckboxFormItem(page, "m", "Domoticz MQTT API", "2", configuration.api.mqtt);
#else
  addCheckboxFormItem(page, "m", "MQTT API", "1", configuration.api.mqtt);
#endif

  closeSection(page);

  /* Section: auto logout */
  openSection(page,L_AUTOLOGOUT_TITLE, L_AUTOLOGOUT_DESCRIPTION);
  addCheckboxFormItem(page, "al", L_ENABLED, "1", configuration.timeToAutoLogOff > 0 );
  closeSection(page);
}

void AFESitesGenerator::addConnectingSite(String &page) {
  NETWORK configuration;
  Data->getConfiguration(&configuration);
  page += "<p>";
  page += F(L_DEVICE_CONNECTS);
  page += ": <strong>";
  page += configuration.ssid;
  page += "</strong> [";
  page += configuration.password;
  page += "]</p><ul><li>";
  page += F(L_CONNECT_TO);
  page += ": ";
  page += configuration.ssid;
  page += "</li><li>";
  page += F(L_SEARCH_FOR_IP_ADDRESS);
  page += ": ";
  page += WiFi.macAddress();
  page += "</li><li>";
  page += F(L_FINISH_NETWORK_CONFIGURATION);
  page += "</li></ul>";
}

void AFESitesGenerator::addNetworkConfiguration(String &page) {
  NETWORK configuration;
  Data->getConfiguration(&configuration);

  /* Section: WiFi selection, user and password */
  openSection(page, L_NETWORK_CONFIGURATION, L_NETWORK_CONFIGURATION_INFO);

  page.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  page.replace("{{item.label}}", L_SSID);
  page.replace("{{item.name}}", "s");

#ifdef DEBUG
  Serial << endl << "Searching for WiFi networks: ";
#endif
  int numberOfNetworks = WiFi.scanNetworks();
  char _ssid[sizeof(configuration.ssid)];

#ifdef DEBUG
  Serial << endl << " - found: " << numberOfNetworks;
#endif

  for (int i = 0; i < numberOfNetworks; i++) {

#ifdef DEBUG
    Serial << endl << " - " << WiFi.SSID(i);
#endif
    page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
    page.replace("{{item.value}}", WiFi.SSID(i));
    WiFi.SSID(i).toCharArray(_ssid, sizeof(_ssid));
    page.replace("{{item.label}}", _ssid);
    page.replace("{{item.selected}}", strcmp(_ssid, configuration.ssid) == 0
                                          ? " selected=\"selected\""
                                          : "");
  }
  page.concat("</select>");

  page += "<input type=\"submit\" class =\"b bc\" value=\"";
  page += L_REFRESH;
  page += "\" formaction=\"/?o=";
  page += AFE_CONFIG_SITE_NETWORK;
  page += "&c=0\"></div>";

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_PASSWORD, "p", L_PASSWORD,
                   configuration.password, "32");
  char _ip[18];
  WiFi.macAddress().toCharArray(_ip, 18);

  addItem(page, AFE_FORM_ITEM_TYPE_TEXT, "m", "MAC", _ip,
          AFE_FORM_ITEM_SKIP_PROPERTY, AFE_FORM_ITEM_SKIP_PROPERTY,
          AFE_FORM_ITEM_SKIP_PROPERTY, AFE_FORM_ITEM_SKIP_PROPERTY,
          AFE_FORM_ITEM_SKIP_PROPERTY, true);
  closeSection(page);

  /* Section: DHCP or Fixed IP */
  openSection(page, L_DEVICE_IP, L_DEVICE_IP_INFO);
  addCheckboxFormItem(page, "d", L_DHCP_ENABLED, "1", configuration.isDHCP);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i1", L_IP_ADDRESS,
                   configuration.ip);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i2", L_GATEWAY,
                   configuration.gateway);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i3", L_SUBNET,
                   configuration.subnet);
  closeSection(page);

  /* Section: Advanced settings */
  openSection(page, L_ADVANCED, "");
  char _int[4];
  sprintf(_int, "%d", configuration.noConnectionAttempts);
  
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "na",
                   L_NUMBER_OF_CONNECTIONS, _int, AFE_FORM_ITEM_SKIP_PROPERTY,
                   "1", "255", "1");

  sprintf(_int, "%d", configuration.waitTimeConnections);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "wc",
                   L_TIME_BETWEEN_CONNECTIONS, _int,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "255", "1", L_SECONDS);

  sprintf(_int, "%d", configuration.waitTimeSeries);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "ws", L_SLEEP_TIME, _int,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "255", "1", L_SECONDS);

  closeSection(page);
}

void AFESitesGenerator::addMQTTBrokerConfiguration(String &page) {
  MQTT configuration;
  Data->getConfiguration(&configuration);

  addConfigurationBlock(page, "MQTT Broker", L_MQTT_CONFIGURATION_INFO);
  page.concat("<fieldset>");
  addItem(page, "text", "h", "Hostname", configuration.host, "32");
  addItem(page, "text", "a", L_IP_ADDRESS, configuration.ip);
  char _number[7];
  sprintf(_number, "%d", configuration.port);
  addItem(page, "number", "p", "Port", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "65535", "1");
  addItem(page, "text", "u", L_USERNAME, configuration.user, "32");
  addItem(page, "password", "s", L_PASSWORD, configuration.password, "32");
  sprintf(_number, "%d", configuration.timeout);
  addItem(page, "number", "t", L_TIMEOUT, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "30000", "1",
          L_MILISECONDS);
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char _idx[7];
  sprintf(_idx, "%d", configuration.lwt.idx);
  addConfigurationBlock(page, L_MQTT_IDX_LWT, L_NO_IF_IDX_0);
  page.concat("<fieldset>");
  addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
#else
  addConfigurationBlock(page, L_MQTT_TOPIC_LWT, L_MQTT_TOPIC_EMPTY);
  page.concat("<fieldset>");
  addItem(page, "text", "t0", L_MQTT_TOPIC, configuration.lwt.topic, "64");
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
#endif
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFESitesGenerator::addDomoticzServerConfiguration(String &page) {
  DOMOTICZ configuration;
  Data->getConfiguration(&configuration);

  addConfigurationBlock(page, L_DOMOTICZ_CONFIGURATION,
                        L_DOMOTICZ_CONFIGURATION_INFO);
  page.concat("<fieldset>");
  page += "<div class=\"cf\"><label>";
  page += F(L_PROTOCOL);
  page += "</label><select name=\"t\"><option value=\"0\"";
  page += configuration.protocol == 0 ? " selected=\"selected\"" : "";
  page += ">http://</option><option value=\"1\"";
  page += configuration.protocol == 1 ? " selected=\"selected\"" : "";
  page += ">https://</option></select></div>";

  addItem(page, "text", "h", "Hostname/IP", configuration.host, "40");

  char _number[6];
  sprintf(_number, "%d", configuration.port);
  addItem(page, "number", "p", "Port", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "65535", "1");
  addItem(page, "text", "u", L_USERNAME, configuration.user, "32");
  addItem(page, "password", "s", L_PASSWORD, configuration.password, "32");

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}
#endif

void AFESitesGenerator::addPasswordConfigurationSite(String &page) {
  PASSWORD configuration;
  Data->getConfiguration(&configuration);
  addConfigurationBlock(page, L_SET_PASSWORD_TO_PANEL, "");
  page.concat("<fieldset>");
  page += "<div class=\"cc\"><label><input name=\"r\" "
          "type=\"checkbox\" value=\"1\"";
  page += (configuration.protect ? " checked=\"checked\"" : "");
  page += "> ";
  page += F(L_PROTECT_BY_PASSWORD);
  page += "?</label></div>";

  if (configuration.protect) {
    addItem(page, "text", "p", L_PASSWORD, configuration.password, "8");
  }
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFESitesGenerator::addLEDConfiguration(String &page, uint8_t id) {
  LED configuration;
  Data->getConfiguration(id, &configuration);

  char title[7];
  sprintf(title, "LED #%d", id + 1);
  addConfigurationBlock(page, title, "");
  page.concat("<fieldset>");
  char field[13];
  sprintf(field, "g%d", id);
  page += "<div class=\"cf\">";
  generateConfigParameter_GPIO(page, field, configuration.gpio);
  page += "<label style=\"width: 300px;\"><input name=\"o";
  page += id;
  page += "\" type=\"checkbox\" value=\"1\"";
  page += configuration.changeToOppositeValue ? " checked=\"checked\"" : "";
  page += ">";
  page += F(L_CHANGE_LED_INDICATION);
  page += "</label></div></fieldset></div>";
}

void AFESitesGenerator::addSystemLEDConfiguration(String &page) {
  uint8_t configuration = Data->getSystemLedID();
  addConfigurationBlock(page, L_SYSTEM_LED, L_SYSTEM_LED_INFO);
  page.concat("<fieldset>");
  generateHardwareList(page, Device->configuration.noOfLEDs, configuration, "s",
                       "LED", 0, AFE_HARDWARE_ITEM_NOT_EXIST);
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFESitesGenerator::addRelayConfiguration(String &page, uint8_t id) {
  RELAY configuration;
  Data->getConfiguration(id, &configuration);

  char title[23];
  sprintf(title, "%s #%d", L_RELAY, id + 1);

  addConfigurationBlock(page, title, "");

  page.concat("<fieldset>");

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE gateConfiguration;
  boolean isGateRelay = false;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES; i++) {
    gateConfiguration = Data->getConfiguration(i);

    if (gateConfiguration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST &&
        gateConfiguration.relayId == id) {
      isGateRelay = true;
      break;
    }
  }
#endif

  generateConfigParameter_GPIO(page, "g", configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif

    addItem(page, "text", "n", L_NAME, configuration.name, "16");
    page += "<p class=\"cm\">";
    page += F(L_DEFAULT_VALUES);
    page += "</p><div class=\"cf\"><label>";
    page += F(L_DEFAULT_POWER_RESTORED);
    page += "</label><select name=\"pr\"><option value=\"0\"";
    page += (configuration.state.powerOn == 0 ? " selected=\"selected\"" : "");
    page += ">";
    page += F(L_NO_ACTION);
    page += "</option><option value=\"1\"";
    page += (configuration.state.powerOn == 1 ? " selected=\"selected\"" : "");
    page += ">";
    page += F(L_OFF);
    page += "</option><option value=\"2\"";
    page += (configuration.state.powerOn == 2 ? " selected=\"selected\"" : "");
    page += ">";
    page += F(L_ON);
    page += "</option><option value=\"3\"";
    page += (configuration.state.powerOn == 3 ? " selected=\"selected\"" : "");
    page += ">";
    page += F(L_LAST_KNOWN_STATE);
    page += "</option><option value=\"4\"";
    page += (configuration.state.powerOn == 4 ? " selected=\"selected\"" : "");
    page += ">";
    page += F(L_OPPOSITE_TO_LAST_KNOWN_STATE);
    page += "</option></select></div>";

    if (Device->configuration.api.mqtt) {

      page += "<div class=\"cf\"><label>";
      page += F(L_DEFAULT_MQTT_CONNECTED);
      page += "</label><select  name=\"mc\"><option value=\"0\"";
      page += (configuration.state.MQTTConnected == 0 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_NO_ACTION);
      page += "</option><option value=\"1\"";
      page += (configuration.state.MQTTConnected == 1 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_OFF);
      page += "</option><option value=\"2\"";
      page += (configuration.state.MQTTConnected == 2 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_ON);
      page += "</option><option value=\"3\"";
      page += (configuration.state.MQTTConnected == 3 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_LAST_KNOWN_STATE);
      page += "</option><option value=\"4\"";
      page += (configuration.state.MQTTConnected == 4 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_OPPOSITE_TO_LAST_KNOWN_STATE);
      page += "</option>";

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      page += "<option value=\"5\"";
      page += (configuration.state.MQTTConnected == 5 ? " selected=\"selected\""
                                                      : "");
      page += ">";
      page += F(L_DEFAULT_GET_FROM_MQTT);
      page += "</option>";
#endif
      page += "</select></div>";
    }

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif
    page += "<br><p class=\"cm\">";
    page += F(L_AUTOMATIC_SWITCHING_OFF);
    page += "</p>";
#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

  char _number[9];
  dtostrf(configuration.timeToOff, 1, 1, _number);

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (isGateRelay) {
    addItem(page, "number", "ot", L_IMPULSE_DURATION, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1",
            "99999", "1", L_MILISECONDS);
  } else {
    addItem(page, "number", "ot", L_SWITCH_OFF_AFTER, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
            "86400", "0.1", L_SECONDS);
  }
#else // Not a GATE
  addItem(page, "number", "ot", L_SWITCH_OFF_AFTER, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "86400",
          "0.1", L_SECONDS);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  if (Device->configuration.noOfDS18B20s > 0)
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
    if (Device->isDHT)
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
    {
      page += "<br><p class=\"cm\">";
      page += F(L_THERMAL_PROTECTION);
      page += "</p><div class=\"cf\"><label>";
      page += F(L_SWITCH_OFF_ABOVE);
      page += "</label><input name=\"tp";
      page += id;
      page += "\" type=\"number\" step=\"1\" min=\"-67\" max=\"259\"  value=\"";
      page += configuration.thermalProtection;
      page += "\"><span class=\"hint\">";
      page += F(L_RANGE);
      page += ": -55C : +125C (-67F : +259F). ";
      page += F(L_NO_ACTION_IF_0);
      page += "</span></div>";
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    page += "<br><p class=\"cm\">";
    page += F(L_SELECT_LED_4_RELAY);
    page += "</p>";

    generateHardwareList(page, Device->configuration.noOfLEDs,
                         configuration.ledID, "l", "LED", 0,
                         AFE_HARDWARE_ITEM_NOT_EXIST);

#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    /* LED Exclusion for a relay assigned to ta gate */
  }
#endif

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
      addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
      page.concat("<fieldset>");
      char _idx[7];
      sprintf(_idx, "%d", configuration.domoticz.idx);
      addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
      page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
    }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_RELAY_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

String AFESitesGenerator::addRegulatorConfiguration(uint8_t type) {
  RELAY configuration = Data->getConfiguration(0);

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

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFESitesGenerator::addSwitchConfiguration(String &page, uint8_t id) {

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE gateConfiguration;
#endif

  char title[23];
  sprintf(title, "%s #%d", L_SWITCH_BUTTON, id + 1);
  addConfigurationBlock(page, title, "");
  page.concat("<fieldset>");
  SWITCH configuration;
  Data->getConfiguration(id, &configuration);
  page += "<div class=\"cf\">";

  generateConfigParameter_GPIO(page, "g", configuration.gpio);
  page += "</div><div class=\"cf\"><label>";
  page += F(L_FUNCTIONALITY);
  page += "</label><select name=\"f\"><option value=\"";
  page += AFE_SWITCH_FUNCTIONALITY_NONE;
  page += "\"";
  page += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_NONE
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_NONE);
  page += "</option><option value=\"";
  page += AFE_SWITCH_FUNCTIONALITY_MULTI;
  page += "\"";
  page += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_MULTI
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_SYSTEM_BUTTON);
  page += "</option>";

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_GATE)
  page += "<option value=\"";
  page += AFE_SWITCH_FUNCTIONALITY_RELAY;
  page += "\"";
  page += (configuration.functionality == AFE_SWITCH_FUNCTIONALITY_RELAY
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_CONTROL_RELAY);
  page += "</option></select></div><div class=\"cf\"><label>";
  page += F(L_RELAY_CONTROLLED_BY_SWITCH);
  page += "</label><select name=\"r\"><option value=\"";
  page += AFE_HARDWARE_ITEM_NOT_EXIST;
  page += "\"";
  page += configuration.relayID == AFE_HARDWARE_ITEM_NOT_EXIST
              ? " selected=\"selected\""
              : "";
  page += ">";
  page += F(L_NONE);
  page += "</option>";

#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t relayIsForGate;
#endif

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    page += "<option value=\"";
    page += i;
    page += "\"";
    page += configuration.relayID == i ? " selected=\"selected\"" : "";
    page += ">";
#ifdef AFE_CONFIG_HARDWARE_GATE
    relayIsForGate = false;
    for (uint8_t j = 0; j < Device->configuration.noOfGates; j++) {
      gateConfiguration = Data->getConfiguration(j);
      if (i == gateConfiguration.relayId) {
        page += F(L_GATE;
        page += ": ";
        page += gateConfiguration.name;
        relayIsForGate = true;
        break;
      }
    }
    if (!relayIsForGate) {
      page += F(L_RELAY;
      page += ": ";
      page += i + 1;
    }
#else
    page += F(L_RELAY);
    page += ": ";
    page += i + 1;
#endif
    page += "</option>";
  }
  page += "</select></div>";

#endif

  page += "<div class=\"cf\"><label>";
  page += F(L_TYPE);
  page += "</label><select name=\"m\"><option value=\"0\"";
  page += (configuration.type == 0 ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_MONOSTABLE);
  page += "</option><option value=\"1\"";
  page += (configuration.type == 1 ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_BISTABLE);
  page += "</option></select></div><br><p class=\"cm\">";
  page += F(L_SENSITIVENESS_HINT);
  page += "</p>";

  char _number[4];
  sprintf(_number, "%d", configuration.sensitiveness);

  addItem(page, "number", "s", L_SENSITIVENESS, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999", "1",
          L_MILISECONDS);
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFESitesGenerator::addDS18B20Configuration(String &page, uint8_t id) {

  AFESensorDS18B20 _Sensor;
  DS18B20Addresses _addresses;
  uint8_t numberOfFoundSensors;
  DS18B20 configuration;
  Data->getConfiguration(id, &configuration);
  char _number[13];

  openSection(page, L_DS18B20_SENSOR, "");

  /* Item: GPIO */
  addListOfGPIOs(page, "g", configuration.gpio, "GPIO");

  /* Item: Sensor address */
  numberOfFoundSensors = _Sensor.scan(configuration.gpio, _addresses);

  if (numberOfFoundSensors > 0) {

    page.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
    page.replace("{{item.label}}", L_ADDRESS);
    page.replace("{{item.name}}", "a");

    char _scannedAddressText[17];
    char _configAddressText[17];
    for (uint8_t i = 0; i < numberOfFoundSensors; i++) {
      _Sensor.addressToChar(_addresses[i], _scannedAddressText);
      _Sensor.addressToChar(configuration.address, _configAddressText);
      page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
      page.replace("{{item.label}}", _scannedAddressText);
      page.replace("{{item.value}}", _scannedAddressText);
      page.replace("{{item.selected}}",
                   memcmp(_addresses[i], configuration.address,
                          sizeof(_addresses[i])) == 0
                       ? " selected=\"selected\""
                       : "");
    }
    page.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));
  }

  /* Item: Name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: Interval */
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "2", "86400", "1",
                   L_SECONDS);

  /* Item: Send only changes */
  addCheckboxFormItem(page, "s", L_DS18B20_SENT_ONLY_CHANGES, "1",
                      configuration.sendOnlyChanges);

  /* Item: Correction */
  sprintf(_number, "%-.3f", configuration.correction);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "k",
                   L_DS18B20_TEMPERATURE_CORRECTION, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "-99.999", "99.999", "0.001");

  /* Item: Unit */
  page.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  page.replace("{{item.label}}", L_UNITS);
  page.replace("{{item.name}}", "u");
  page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  page.replace("{{item.label}}", "C");
  page.replace("{{item.value}}", String(AFE_TEMPERATURE_UNIT_CELSIUS));
  page.replace("{{item.selected}}",
               configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                   ? " selected=\"selected\""
                   : "");
  page.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  page.replace("{{item.label}}", "F");
  page.replace("{{item.value}}", String(AFE_TEMPERATURE_UNIT_FAHRENHEIT));
  page.replace("{{item.selected}}",
               configuration.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
                   ? " selected=\"selected\""
                   : "");
  page.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_NO_IF_IDX_0);
    sprintf(_number, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    closeSection(page);
  }
#else
  if (Device->configuration.api.mqtt) {
    openSection(page, L_DS18B20_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                     configuration.mqtt.topic, "64");

    closeSection(page);
  }
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
String AFESitesGenerator::addDHTConfiguration() {

  DH configuration = Data->getSensorConfiguration();
  DEVICE device = Data->getConfiguration();

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
  body += "</label><input name=\"in\" min=\"10\" max=\"86400\" step=\"1\" "
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
  PIR configuration = Data->getPIRConfiguration(id);
  DEVICE device = Data->getConfiguration();

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
void AFESitesGenerator::addContactronConfiguration(String &page, uint8_t id) {
  CONTACTRON configuration = Data->getConfiguration(id);
  char title[23];
  sprintf(title, "%s #%d", L_MAGNETIC_SENSOR, id + 1);
  addConfigurationBlock(page, title, "");
  page.concat("<fieldset>");
  page += "<div class=\"cf\">";
  generateConfigParameter_GPIO(page, "g", configuration.gpio);
  page += "</div>";

  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  page += "<div class=\"cf\"><label>";
  page += F(L_TYPE;
  page += "</label><select name=\"y\"><option value=\"0\"";
  page += (configuration.type == 0 ? " selected=\"selected\"" : "");
  page += ">NO</option><option value=\"1\"";
  page += (configuration.type == 1 ? " selected=\"selected\"" : "");
  page += ">NC</option></select></div>";

  generateHardwareList(page, Device->configuration.noOfLEDs,
                       configuration.ledID, "l", L_LED_ASSIGNED_TO_SENSOR, 0,
                       AFE_HARDWARE_ITEM_NOT_EXIST);

  page += "<br><p class=\"cm\">";
  page += F(L_SET_CONTACTRON_SENSITIVENESS);
  page += "</p>";

  char _number[5];
  sprintf(_number, "%d", configuration.bouncing);

  addItem(page, "number", "b", L_SENSITIVENESS, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "2000", "1",
          L_MILISECONDS);

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");

    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");

    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_CONTACTRON_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif
}
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFESitesGenerator::addGateConfiguration(String &page, uint8_t id) {
  GATE gateConfiguration = Data->getConfiguration(id);
  CONTACTRON contactronConfiguration[2];

  addConfigurationBlock(page, L_GATE_CONFIGURATION, "");

  page.concat("<fieldset>");
  addItem(page, "text", "n", L_NAME, gateConfiguration.name, "16");

  generateHardwareList(
      page, Device->configuration.noOfRelays, gateConfiguration.relayId, "r",
      L_RELAY_ID_CONTROLLING_GATE, 0, AFE_HARDWARE_ITEM_NOT_EXIST);

  if (Device->configuration.noOfContactrons > 0) {
    generateHardwareList(page, Device->configuration.noOfContactrons,
                         gateConfiguration.contactron.id[0], "c1",
                         L_MAGNETIC_SENSOR, 0, AFE_HARDWARE_ITEM_NOT_EXIST);
  }

  /* If there is more than a one contactron connected, add option to assigne
   * it to the gate */

  if (Device->configuration.noOfContactrons > 1) {

    generateHardwareList(page, Device->configuration.noOfContactrons,
                         gateConfiguration.contactron.id[1], "c2",
                         L_MAGNETIC_SENSOR, 0, AFE_HARDWARE_ITEM_NOT_EXIST);
  }

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

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
           << F("Number of contactros set for the gate: ")
           << numberOfContractons;
#endif

    if (numberOfContractons > 0) {

      addConfigurationBlock(page, L_GATES_STATES_CONFIGURATION, "");

      for (uint8_t i = 0; i < numberOfContractons; i++) {
        contactronConfiguration[i] =
            Data->getConfiguration(gateConfiguration.contactron.id[i]);
      }

      page.concat("<fieldset>");

      page += "<p class=\"cm\">";
      page += F(L_IF_MAGNETIC_SENSOR);
      page += ": <strong>";
      page += contactronConfiguration[0].name;

      if (numberOfContractons == 2) {
        page += "</strong> ";
        page += F(L_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_ARE_OPEN);
      } else {
        page += "</strong> ";
        page += F(L_IS_OPEN);
      }
      page += " ";
      page += F(L_THEN);
      page += ":</p>";
      generateGateStatesList(page, 0, gateConfiguration.states.state[0]);
      if (numberOfContractons == 2) {
        page += "<br><br><p class=\"cm\">";
        page += F(L_IF_MAGNETIC_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[0].name;
        page += "</strong> ";
        page += F(L_IS_OPEN);
        page += " ";
        page += F(L_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_IS_CLOSED);
        page += " ";
        page += F(L_THEN);
        page += ":";
        page += "</p>";
        generateGateStatesList(page, 1, gateConfiguration.states.state[1]);

        page += "<br><br><p class=\"cm\">";
        page += F(L_IF_MAGNETIC_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[0].name;
        page += "</strong> ";
        page += F(L_IS_CLOSED);
        page += " ";
        page += F(L_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_IS_OPEN);
        page += " ";
        page += F(L_THEN);
        page += ":";
        page += "</p>";

        generateGateStatesList(page, 2, gateConfiguration.states.state[2]);
      }

      page += "<br><br><p class=\"cm\">";

      page += F(L_IF_MAGNETIC_SENSOR);
      page += ": <strong>";
      page += contactronConfiguration[0].name;
      if (numberOfContractons == 2) {
        page += "</strong> ";
        page += F(L_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_ARE_CLOSED);
      } else {
        page += "</strong> ";
        page += F(L_IS_CLOSED);
      }
      page += " ";
      page += F(L_THEN);
      page += ":</p>";
      generateGateStatesList(page, 3, gateConfiguration.states.state[3]);
      page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
    }
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];

    if (Device->configuration.api.mqtt) {
      sprintf(_idx, "%d", gateConfiguration.domoticzControl.idx);
      addItem(page, "number", "z", "IDX Start/Stop", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999",
              "1");
    }

    sprintf(_idx, "%d", gateConfiguration.domoticz.idx);
    addItem(page, "number", "x", L_IDX_GATE_STATE, _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999",
            "1");

    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_GATE_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, gateConfiguration.mqtt.topic,
            "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif
}

void AFESitesGenerator::generateGateStatesList(String &item, uint8_t id,
                                               byte state) {
  item += "<div class=\"cf\"><label>";
  item += L_SET_GATE_STATE;
  item += "</label><select name=\"s" + String(id) + "\"><option value=\"";
  item += AFE_GATE_OPEN;
  item += "\"";
  item += (state == AFE_GATE_OPEN ? " selected=\"selected\"" : "");
  item += ">";
  item += L_OPENED;
  item += "</option><option value=\"";
  item += AFE_GATE_PARTIALLY_OPEN;
  item += "\"";
  item += (state == AFE_GATE_PARTIALLY_OPEN ? " selected=\"selected\"" : "");
  item += ">";
  item += L_PARTIALLY_OPENED;
  item += "</option><option value=\"";
  item += AFE_GATE_CLOSED;
  item += "\"";
  item += (state == AFE_GATE_CLOSED ? " selected=\"selected\"" : "");
  item += ">";
  item += L_CLOSED;
  item += "</option><option value=\"";
  item += AFE_GATE_UNKNOWN;
  item += "\"";
  item += (state == AFE_GATE_UNKNOWN ? " selected=\"selected\"" : "");
  item += ">";
  item += L_UNKNOWN_STATE;
  item += "</option></select></div>";
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFESitesGenerator::addHPMA115S0Configuration(String &page, uint8_t id) {
  HPMA115S0 configuration;
  Data->getConfiguration(id, &configuration);
  addConfigurationBlock(page, L_PARTICLE_SENSOR, "");
  page.concat("<fieldset>");
  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "f", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "5",
          "86400", "f", L_SECONDS);

  page += "<br><br>";
  page += "<p class=\"cm\">";
  page += F(L_SENSOR_POST_SLEEP_INTERVAL);
  page += "</p>";

  sprintf(_number, "%d", configuration.timeToMeasure);
  addItem(page, "number", "m", L_MEASURE_AFTER, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999", "1",
          L_SECONDS);
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_number, "%d", configuration.domoticz.pm25.idx);
    addItem(page, "number", "x2", "IDX PM2.5", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999",
            "1");
    sprintf(_number, "%d", configuration.domoticz.pm10.idx);
    addItem(page, "number", "x1", "IDX PM10", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_MQTT_TOPIC_HPMA115S0, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFESitesGenerator::addBMEX80Configuration(String &page, uint8_t id) {
  BMEX80 configuration;
  Data->getConfiguration(id, &configuration);

  char _number[7];

  addConfigurationBlock(page, L_BMEX80_SENSOR, "");
  page.concat("<fieldset>");
  addDeviceI2CAddressSelection(page, configuration.i2cAddress);

  page += "<div class=\"cf\"><label>";
  page += F(L_BMEX80_SENSOR_TYPE);
  page += "</label><select name=\"b\"><option value=\"";
  page += AFE_BMX_UNKNOWN_SENSOR;
  page += "\"";
  page +=
      (configuration.type == AFE_BMX_UNKNOWN_SENSOR ? " selected=\"selected\""
                                                    : "");
  page += ">";
  page += F(L_NONE);
  page += "</option><option value=\"";
  page += AFE_BMP180_SENSOR;
  page += "\"";
  page +=
      (configuration.type == AFE_BMP180_SENSOR ? " selected=\"selected\"" : "");
  page += ">BMx085/BMx180</option><option value=\"";
  page += AFE_BME280_SENSOR;
  page += "\"";
  page +=
      (configuration.type == AFE_BME280_SENSOR ? " selected=\"selected\"" : "");
  page += ">BMx280</option><option value=\"";
  page += AFE_BME680_SENSOR;
  page += "\"";
  page +=
      (configuration.type == AFE_BME680_SENSOR ? " selected=\"selected\"" : "");
  page += ">BMx680</option></select></div>";

  page += "<input type=\"submit\" class=\"b bw\" value=\"";
  page += F(L_REFRESH_SETTINGS_FOR_BMEX80_SENSOR);
  page += "\"><br><br>";

  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "f", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "5",
          "86400", "1", L_SECONDS);

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

  if (configuration.type != AFE_BMX_UNKNOWN_SENSOR) {

    /* Sensor's units */
    addConfigurationBlock(page, L_UNITS, "");
    page.concat("<fieldset>");
    page += "<div class=\"cf\"><label>";
    page += F(L_TEMPERATURE);
    page += "</label><select name=\"tu\"><option value=\"";
    page += AFE_TEMPERATURE_UNIT_CELSIUS;
    page += "\"";
    page += (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                 ? " selected=\"selected\""
                 : "");
    page += ">C</option><option value=\"";
    page += AFE_TEMPERATURE_UNIT_FAHRENHEIT;
    page += "\"";
    page += (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
                 ? " selected=\"selected\""
                 : "");
    page += ">F</option></select></div></fieldset></div>";

    /* Corrections of sensor values */
    addConfigurationBlock(page, L_CORRECTIONS, "");
    page.concat("<fieldset>");
    sprintf(_number, "%-.3f", configuration.temperature.correction);
    addItem(page, "number", "tc", L_TEMPERATURE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "-99.999",
            "99.999", "0.001");

    if (configuration.type != AFE_BMP180_SENSOR) {
      sprintf(_number, "%-.3f", configuration.humidity.correction);
      addItem(page, "number", "hc", L_HUMIDITY, _number, AFE_FORM_ITEM_SKIP_PROPERTY "-99.999",
              "99.999", "0.001");
    }

    sprintf(_number, "%-.3f", configuration.pressure.correction);
    addItem(page, "number", "pc", L_PRESSURE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "-999.999",
            "999.999", "0.001");

    sprintf(_number, "%d", configuration.altitude);
    addItem(page, "number", "hi", L_ALTITIDE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "-431", "8850", "1",
            L_METERS);

    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
      addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
      page.concat("<fieldset>");
      sprintf(_number, "%d", configuration.domoticz.temperature.idx);
      addItem(page, "number", "i1", L_IDX_TEMPERATURE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
              "999999", "1");

      if (configuration.type != AFE_BMP180_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.humidity.idx);
        addItem(page, "number", "i2", L_IDX_HUMIDITY, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.dewPoint.idx);
        addItem(page, "number", "i3", L_IDX_DEW_POINT, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.heatIndex.idx);
        addItem(page, "number", "i4", L_IDX_HEAT_INDEX, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d",
                configuration.domoticz.temperatureHumidityPressure.idx);
        addItem(page, "number", "i0", L_IDX_TEMP_HUM_BAR, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.temperatureHumidity.idx);
        addItem(page, "number", "i12", L_IDX_TEMP_HUM, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");
      }

      sprintf(_number, "%d", configuration.domoticz.pressure.idx);
      addItem(page, "number", "i5", L_IDX_PRESSURE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999",
              "1");

      sprintf(_number, "%d", configuration.domoticz.relativePressure.idx);
      addItem(page, "number", "i6", L_IDX_RELATIVE_PRESSURE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
              "999999", "1");

      if (configuration.type == AFE_BME680_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.iaq.idx);
        addItem(page, "number", "i7", L_IDX_IQA, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999",
                "1");

        sprintf(_number, "%d", configuration.domoticz.staticIaq.idx);
        addItem(page, "number", "i8", L_IDX_STATIC_IAQ, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.co2Equivalent.idx);
        addItem(page, "number", "i9", L_IDX_CO2_EQUVALENT, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.breathVocEquivalent.idx);
        addItem(page, "number", "i10", L_IDX_BVOC_EQUIVALENT, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");

        sprintf(_number, "%d", configuration.domoticz.gasResistance.idx);
        addItem(page, "number", "i11", L_IDX_GAS_SENSOR, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
                "999999", "1");
      }
      page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
    }
#else
    if (Device->configuration.api.mqtt) {
      addConfigurationBlock(page, L_MQTT_TOPIC_BMEX80, L_MQTT_TOPIC_EMPTY);
      page.concat("<fieldset>");
      addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
      page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFESitesGenerator::addBH1750Configuration(String &page, uint8_t id) {

  BH1750 configuration;
  Data->getConfiguration(id, &configuration);
  addConfigurationBlock(page, L_BH1750_SENSOR, "");
  page.concat("<fieldset>");
  addDeviceI2CAddressSelection(page, configuration.i2cAddress);
  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "f", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "5",
          "86400", "1", L_SECONDS);

  sprintf(_number, "%d", configuration.mode);
  addItem(page, "number", "m", L_SENSITIVENESS, _number, AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY
          L_CANT_CHANGE, true);

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_number, "%d", configuration.domoticz.idx);
    addItem(page, "number", "d", "IDX", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_MQTT_TOPIC_BH1750, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
// String AFESitesGenerator::addAS3935Configuration(uint8_t id) {
void AFESitesGenerator::addAS3935Configuration(String &page, uint8_t id) {
  AS3935 configuration;
  Data->getConfiguration(id, &configuration);
  char _number[2];

  addConfigurationBlock(page, L_AS3935_SENSOR, "");
  page.concat("<fieldset>");
  addDeviceI2CAddressSelection(page, configuration.i2cAddress);
  addItem(page, "text", "n", L_NAME, configuration.name, "16");
  page += "<div class=\"cf\">";
  generateConfigParameter_GPIO(page, "g", configuration.irqGPIO);
  page += "</div>";

  page += "<div class=\"cf\"><label>";
  page += F(L_DISTANCE_UNIT);
  page += "</label><select name=\"u\"><option value=\"";
  page += AFE_DISTANCE_KM;
  page += "\"";
  page +=
      (configuration.unit == AFE_DISTANCE_KM ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_KM);
  page += "</option><option value=\"";
  page += AFE_DISTANCE_MIL;
  page += "\"";
  page +=
      (configuration.unit == AFE_DISTANCE_MIL ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_MILES);
  page += "</option></select></div>";

  sprintf(_number, "%d", configuration.watchdogThreshold);
  addItem(page, "number", "e", L_WATCHDOG_THRESHOLD, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1", "10",
          "1", L_WATCHDOG_THRESHOLD_HINT);

  sprintf(_number, "%d", configuration.spikesRejectionLevel);
  addItem(page, "number", "s", L_SPIKES_REJECTION, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1", "11", "1",
          L_SPIKES_REJECTION_HINT);

  page += "<div class=\"cf\"><label>";
  page += F(L_MIN_SPIKES);
  page += "</label><select name=\"m\"><option value=\"1\"";
  page += (configuration.minimumNumberOfLightningSpikes == 1
               ? " selected=\"selected\""
               : "");
  page += ">1</option><option value=\"5\"";
  page += (configuration.minimumNumberOfLightningSpikes == 5
               ? " selected=\"selected\""
               : "");
  page += ">5</option><option value=\"9\"";
  page += (configuration.minimumNumberOfLightningSpikes == 9
               ? " selected=\"selected\""
               : "");
  page += ">9</option><option value=\"16\"";
  page += (configuration.minimumNumberOfLightningSpikes == 16
               ? " selected=\"selected\""
               : "");
  page += ">16</option></select></div>";

  page += "<div class=\"cc\"><label><input name =\"f\" type=\"checkbox\" "
          "value=\"1\"";
  page +=
      configuration.setNoiseFloorAutomatically ? " checked=\"checked\">" : ">";
  page += F(L_AUTOMATIC_NOISE_FLOOR_CONTROL);
  page += "</label></div><p class=\"cm\">";

  page += F(L_SET_LEVEL_OF_NOISE_FLOOR);
  sprintf(_number, "%d", configuration.noiseFloor);
  addItem(page, "number", "nf", L_NOISE_FLOOR, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1", "7", "1",
          L_NOISE_FLOOR_HINT);

  page += "</p><div class=\"cf\"><label>";
  page += F(L_SENSOR_INDOOR_OUTDOOR);
  page += "</label><select name=\"w\"><option value=\"1\"";
  page += (configuration.indoor == true ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_INDOOR);
  page += "</option><option value=\"0\"";
  page += (configuration.indoor == false ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_OUTDOOR);
  page += "</option></select></div>";

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_number, "%d", configuration.domoticz.idx);
    addItem(page, "number", "d", "IDX", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_MQTT_TOPIC_AS3935, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
void AFESitesGenerator::addAnemometerSensorConfiguration(String &page) {

  addConfigurationBlock(page, L_ANEMOMETER_SENSOR, "");
  page.concat("<fieldset>");
  ANEMOMETER configuration;
  Data->getConfiguration(&configuration);

  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  page += "<div class=\"cf\">";
  generateConfigParameter_GPIO(page, "g", configuration.gpio);
  page += "</div>";

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "f", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "5",
          "86400", "1", L_SECONDS);

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

  addConfigurationBlock(page, L_ANEMOMETER_CALIBRATION,
                        L_ANEMOMETER_IMPULSE_DISTANCE_HINT);
  page.concat("<fieldset>");

  sprintf(_number, "%-.2f", configuration.impulseDistance);

  addItem(page, "number", "l", L_ANEMOMETER_IMPULSE_DISTANCE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
          "999.99", "0.01");

  page += "<div class=\"cf\"><label>";
  page += F(L_DISTANCE_UNIT);
  page += "</label><select name=\"u\"><option value=\"";
  page += AFE_DISTANCE_CENTIMETER;
  page += "\"";
  page += (configuration.impulseDistanceUnit == AFE_DISTANCE_CENTIMETER
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_CM);
  page += "</option><option value=\"";
  page += AFE_DISTANCE_METER;
  page += "\"";
  page += (configuration.impulseDistanceUnit == AFE_DISTANCE_METER
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_M);
  page += "</option><option value=\"";
  page += AFE_DISTANCE_KILOMETER;
  page += "\"";
  page += (configuration.impulseDistanceUnit == AFE_DISTANCE_KILOMETER
               ? " selected=\"selected\""
               : "");
  page += ">";
  page += F(L_KM);
  page += "</option></select></div>";

  page += "<br><p class=\"cm\">";
  page += F(L_ANEMOMETER_SENSITIVENESS_HINT);
  page += "</p>";
  sprintf(_number, "%d", configuration.sensitiveness);
  addItem(page, "number", "s", L_SENSITIVENESS, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "255", "1",
          L_MILISECONDS);

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
void AFESitesGenerator::addRainmeterSensorConfiguration(String &page) {

  addConfigurationBlock(page, L_RAINMETER, "");
  page.concat("<fieldset>");
  RAINMETER configuration;
  Data->getConfiguration(&configuration);

  addItem(page, "text", "n", L_NAME, configuration.name, "16");

  page += "<div class=\"cf\">";
  generateConfigParameter_GPIO(page, "g", configuration.gpio);
  page += "</div>";

  char _number[8];
  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "f", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "5",
          "86400", "1", L_SECONDS);

  page += "</fieldset></div>";

  addConfigurationBlock(page, L_RAINMETER_CALIBRATION, "");

  sprintf(_number, "%-.2f", configuration.resolution);
  addItem(page, "number", "r", L_RAINMETER_RESOLUTION, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0",
          "9999.99", "0.01", "ml/m2");

  page += "</fieldset></div>";

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addItem(page, "number", "x", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += "</fieldset></div>";
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += "</fieldset></div>";
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFESitesGenerator::addAnalogInputConfiguration(String &page) {
  ADCINPUT configuration;
  Data->getConfiguration(&configuration);

  addConfigurationBlock(page, L_ANALOG_INPUT, "");
  page.concat("<fieldset>");
  char _number[13];

  sprintf(_number, "%d", configuration.gpio);
  addItem(page, "number", "g", "GPIO", _number, AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY "?");

  sprintf(_number, "%d", configuration.interval);
  addItem(page, "number", "v", L_MEASURMENTS_INTERVAL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1",
          "86400", "1", L_SECONDS);

  sprintf(_number, "%d", configuration.numberOfSamples);
  addItem(page, "number", "n", L_NUMBER_OF_SAMPLES, _number, AFE_FORM_ITEM_SKIP_PROPERTY "1", "999",
          "1");

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

  addConfigurationBlock(page, L_VOLTAGE_DIVIDER, "");
  page.concat("<fieldset>");

  sprintf(_number, "%-.2f", (float)configuration.maxVCC);
  addItem(page, "number", "m", L_MEASURED_VOLTAGE, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "1000",
          "0.01", "V");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Ra);
  addItem(page, "number", "ra", "R[a]", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "90000000", "1",
          "Om");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Rb);
  addItem(page, "number", "rb", "R[b]", _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "90000000", "1",
          "Om");
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char _idx[7];
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_idx, "%d", configuration.domoticz.raw);
    addItem(page, "number", "x0", L_RAW_DATA, _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.percent);
    addItem(page, "number", "x1", L_PERCENT, _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.voltage);
    addItem(page, "number", "x2", L_VOLTAGE, _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    sprintf(_idx, "%d", configuration.domoticz.voltageCalculated);
    addItem(page, "number", "x3", L_VOLTAGE_CALCULATED, _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0",
            "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_ADC_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "t", L_MQTT_TOPIC, configuration.mqtt.topic, "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  addConfigurationBlock(page, L_BATTERY_METER, "");

  page.concat("<fieldset>");

  sprintf(_number, "%-.3f", configuration.battery.minVoltage);
  addItem(page, "number", "lv", L_BATTERY_MIN_LEVEL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "1000",
          "0.001");

  sprintf(_number, "%-.3f", configuration.battery.maxVoltage);
  addItem(page, "number", "hv", L_BATTERY_MAX_LEVEL, _number, AFE_FORM_ITEM_SKIP_PROPERTY "0", "1000",
          "0.001");

  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    addConfigurationBlock(page, "Domoticz", L_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_idx, "%d", configuration.battery.domoticz.idx);
    addItem(page, "number", "bx", "IDX", _idx, AFE_FORM_ITEM_SKIP_PROPERTY "0", "999999", "1");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#else
  if (Device->configuration.api.mqtt) {
    addConfigurationBlock(page, L_BATTERY_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
    page.concat("<fieldset>");
    addItem(page, "text", "bt", L_MQTT_TOPIC, configuration.battery.mqtt.topic,
            "64");
    page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
}
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
void AFESitesGenerator::addSerialPortConfiguration(String &page) {
  SERIALPORT configuration;
  Data->getConfiguration(&configuration);
  addConfigurationBlock(page, "UART", "");
  page.concat("<fieldset>");
  generateConfigParameter_GPIO(page, "r", configuration.RXD, "GPIO RXD");
  generateConfigParameter_GPIO(page, "t", configuration.TXD, "GPIO TXD");
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFESitesGenerator::addI2CPortConfiguration(String &page) {
  I2CPORT configuration;
  Data->getConfiguration(&configuration);
  addConfigurationBlock(page, "I2C", "");
  page.concat("<fieldset>");
  generateConfigParameter_GPIO(page, "a", configuration.SDA, "GPIO SDA");
  generateConfigParameter_GPIO(page, "l", configuration.SCL, "GPIO SCL");
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}

void AFESitesGenerator::addDeviceI2CAddressSelection(String &page,
                                                     uint8_t address) {
  AFEI2CScanner I2CScanner;
  I2CScanner.begin();
  page += "<div class=\"cf\"><label>I2C ";
  page += F(L_ADDRESS);
  page += ": </label><select name=\"a\">";
  page += "<option value=\"0\"";
  page += address == 0 ? " selected=\"selected\"" : "";
  page += ">";
  page += F(L_NONE);
  page += "</option>";

  for (byte addressToScan = 1; addressToScan < 127; addressToScan++) {
    if (I2CScanner.scan(addressToScan)) {
      page += "<option value=\"";
      page += addressToScan;
      page += "\"";
      page += addressToScan == address ? " selected=\"selected\"" : "";
      page += ">[0x";
      page += String(addressToScan, HEX);
      page += "] : ";
      page += I2CScanner.getName(addressToScan);
      page += "</option>";
    }
  }
  page += "</select></div>";
}

#endif

void AFESitesGenerator::addUpgradeSection(String &page) {
  addConfigurationBlock(page, L_FIRMWARE_UPGRADE, L_DONT_PLUG_OFF);
  page.concat("<fieldset>");
  page.concat("<form method=\"post\" action=\"upgrade?o=");
  page.concat(AFE_CONFIG_SITE_POST_UPGRADE);
  page.concat("\" enctype=\"multipart/form-data\">");
  page.concat("<div class=\"cf\"><label>");
  page.concat(L_SELECT_FIRMWARE);
  page.concat("</label><input class=\"bs\" name=\"update\" type=\"file\" "
              "accept=\".bin\"></div><p class=\"cm\">");
  page.concat(L_UPGRADE_INFO);
  page.concat(".</p><button type=\"submit\" class=\"b be\">");
  page.concat(L_UPGRADE);
  page.concat("</button></form></fieldset></div>");
}

void AFESitesGenerator::addPostUpgradeSection(String &page, boolean status) {
  addConfigurationBlock(page, L_FIRMWARE_UPGRADE, "");
  page.concat("<fieldset>");
  page.concat("<ul>");
  if (status) {
    page.concat("<li style=\"color:red\">");
    page.concat(L_UPGRADE_FAILED);
  } else {
    page.concat("<li>");
    page.concat(L_UPGRADE_SUCCESSFUL);
  }
  page.concat("</li><li>");
  page.concat(L_DEVICE_WILL_BE_REBOOTED);
  page.concat("...</li></fieldset></div>");
}

void AFESitesGenerator::addResetSection(String &page) {
  addConfigurationBlock(page, L_RESTORING_DEFAULT_SETTING, "");
  page.concat("<fieldset>");
  page.concat("<p class=\"cm\"><strong>");
  page.concat(L_WARNING);
  page.concat("</strong>: </p><p class=\"cm\">");
  page.concat(L_CONFIGURATION_WILL_BE_REMOVED);
  page.concat(": </p><input type=\"submit\" class=\"b be\" value=\"");
  page.concat(L_RESTORE_DEFAULT_SETTINGS);
  page.concat("\"></fieldset></div>");
}

void AFESitesGenerator::addPostResetSection(String &page) {
  addConfigurationBlock(page, L_RESTORING_DEFAULT_SETTING, "");
  page += "<fieldset><p class=\"cm\">";
  page += F(L_UPGRADE_IN_PROGRESS);
  page += "</p><p class=\"cm\">";
  page += F(L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE);
  page += ": </p><a "
          "href=\"http://192.168.5.1\">http://192.168.5.1</a></fieldset></div>";
}

void AFESitesGenerator::addExitSection(String &page, uint8_t command) {
  addConfigurationBlock(page, L_REBOOT, "");
  page.concat("<fieldset>");
  page += "<div class=\"cf\"><ul><li>";
  page += F(L_REBOOT_IN_PROGRESS);
  page += "</li><li>";
  if (command != AFE_MODE_ACCESS_POINT) {
    page += F(L_SITE_WILL_BE_RELOADED);
  } else {
    page += F(L_CONNECT_TO_HOTSPOT_AFTER_UPGRADE);
    page += ": <a href=\"http://192.168.5.1\">http://192.168.5.1</a></p>";
  }
  page += "</li></ul></div></fieldset></div>";
}

void AFESitesGenerator::addIndexSection(String &page, boolean authorized) {
  DEVICE configuration;
  configuration = Device->configuration;
  addConfigurationBlock(page, L_LAUNCH_CONFIGURATION_PANEL, "");
  if (!authorized) {
    page += "<h3>";
    page += F(L_WRONG_PASSWORD);
    page += "</h3>";
  }
  page.concat("<fieldset>");
  page += "<form method=\"post\"><div class=\"cf\"><input name=\"p\" "
          "type=\"password\" "
          "placeholder=\"";
  page += F(L_PASSWORD);
  page += "\"></div><div class=\"cf\"><input type=\"submit\" class=\"b bs\" "
          "value=\"";
  page += F(L_NORMAL_MODE);
  page += "\" formaction=\"/?o=0&i=";
  page += AFE_MODE_CONFIGURATION;
  page += "\" /> <input type=\"submit\" class=\"b be\" value=\"";
  page += F(L_HOTSPOT_MODE);
  page += "\" formaction=\"/?o=0&i=";
  page += AFE_MODE_ACCESS_POINT;
  page += "\" /></div></form></fieldset></div>";
}

void AFESitesGenerator::addProVersionSite(String &page) {
  PRO_VERSION configuration;
  Data->getConfiguration(&configuration);
  addConfigurationBlock(page, L_PRO_VERSION, "");
  page.concat("<fieldset>");
  if (Device->getMode() == AFE_MODE_CONFIGURATION) {
    addItem(page, "text", "k", L_KEY, configuration.serial, "18");
    page += "<div class=\"cf\"><label>";
    page += F(L_VALID);
    page += "?</label><span>";
    page += configuration.valid ? F(L_YES) : F(L_NO);
    page += "</span></div><input name=\"v\" type=\"hidden\" value=\"";
    page += configuration.valid;
    page += "\">";
  } else {
    page += "<h3>";
    page += F(L_PRO_CANNOT_BE_COFIGURED);
    page += "</h3>";
  }
  page += FPSTR(HTTP_FORM_FIELD_CLOUSRE);
}

void AFESitesGenerator::generateFooter(String &page, boolean extended) {
  if (Device->getMode() == AFE_MODE_NORMAL) {
    page.concat(FPSTR(HTTP_FOOTER_CONNECTED));
  }

  page.concat("</div></div>");

  if (extended) {

    page.concat(FPSTR(HTTP_FOOTER_EXTENDED));

    page.replace("{{L_HELP}}", L_HELP);
    page.replace("{{L_DOCUMENTATION}}", L_DOCUMENTATION);
    page.replace("{{L_VERSION}}", L_VERSION);
    page.replace("{{f.deviceName}}", AFE_DEVICE_TYPE_NAME);
    page.replace("{{f.deviceID}}", String(AFE_DEVICE_TYPE_ID));
    page.replace("{{freeHeap}}", String(system_get_free_heap_size() / 1024));

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    page.replace("{{f.API}}", "Domoticz");
#else
    page.replace("{{f.API}}", "Standard");
#endif

#ifdef ESP_4MB
    page.replace("{{f.size}}", "4Mb");
#else
    page.replace("{{f.size}}", "1Mb");
#endif

    FirmwarePro->Pro.valid ? page.replace("{{f.Pro}}", L_YES)
                           : page.replace("{{f.Pro}}", L_NO);
  }

  page.replace("{{f.version}}", Firmware.version);
  page.replace("{{f.type}}", String(Firmware.type));

  page.concat("</body></html>");
}

void AFESitesGenerator::addConfigurationBlock(String &page, const char *title,
                                              const char *description) {
  page.concat(FPSTR(HTTP_FORM_BLOCK_HEADER));
  page.replace("{{title}}", title);
  page.replace("{{description}}", description);
}

void AFESitesGenerator::generateConfigParameter_GPIO(String &item,
                                                     const char *field,
                                                     uint8_t selected,
                                                     const char *title) {

  item += "<div class=\"cf\"><label>";
  item += title;
  item += "</label><select name=\"";
  item += field;
  item += "\">";
  for (uint8_t i = 0; i <= 16; i++) {
    item += "<option value=\"";
    item += i;
    item += "\"";
    item += (selected == i ? "selected=\"selected\"" : "");
    item += ">";
    item += i;
    item += "</option>";
  }
  item += "</select></div>";
}

void AFESitesGenerator::generateHardwareList(
    String &page, uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label, uint8_t index, uint8_t noneValue, boolean disabled) {
  page.concat("<div class=\"cf\"><label>");
  page.concat(label);
  page.concat("</label><select name=\"");
  page.concat(field);
  page.concat("\"");
  if (disabled) {
    page.concat(" disabled=\"disabled\"");
  }
  page.concat("><option value=\"");
  page.concat(noneValue);
  page.concat("\"");
  page.concat((noOffConnected == noneValue ? " selected=\"selected\"" : ""));
  page.concat(">");
  page.concat(L_NONE);
  page.concat("</option>");
  /* @TODO not a nice code with this index. It can't be different than 0
   * or 1. So far only 0,1 are planned */
  if (!disabled) { // generate other values if item is not vis
    for (uint8_t i = index; i < noOfItems + index; i++) {
      page.concat("<option value=\"");
      page.concat(i);
      page.concat("\"");
      page.concat(noOffConnected == i ? " selected=\"selected\"" : "");
      page.concat(">");
      page.concat(i + (index == 0 ? 1 : 0));
      page.concat("</option>");
    }
  }
  page.concat("</select>");
  if (disabled) {
    page.concat("<span class=\"hint\">(");
    page.concat(L_PRO_VERSION);
    page.concat(")</span>");
  }

  page.concat("</div>");
}

void AFESitesGenerator::generateHardwareItemsList(
    String &page, uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label, boolean disabled) {
  generateHardwareList(page, noOfItems, noOffConnected, field, label, 1, 0,
                       disabled);
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

void AFESitesGenerator::addItem(String &item, const char *type,
                                const char *name, const char *label,
                                const char *value, const char *size,
                                const char *min, const char *max,
                                const char *step, const char *hint,
                                boolean readonly) {
  item.concat("<div class=\"cf\"><label>");
  item.concat(label);
  item.concat("</label><input name=\"");
  item.concat(name);
  item.concat("\" type=\"");
  item.concat(type);
  item.concat("\" ");
  if (readonly) {
    item.concat("readonly=\"readonly\" ");
  }
  if (strcmp(size, "?") != 0) {
    item.concat("maxlength=\"");
    item.concat(size);
    item.concat("\" ");
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0) {
      item.concat("min=\"");
      item.concat(min);
      item.concat("\" ");
    }
    if (strcmp(max, "?") != 0) {
      item.concat("max=\"");
      item.concat(max);
      item.concat("\" ");
    }
    if (strcmp(step, "?") != 0) {
      item.concat("step=\"");
      item.concat(step);
      item.concat("\" ");
    }
  }
  item.concat("value=\"");
  item.concat(value);
  item.concat("\">");
  if (strcmp(size, "?") != 0) {
    item.concat("<span class=\"hint\">Max ");
    item.concat(size);
    item.concat(" ");
    item.concat(F(L_NUMBER_OF_CHARS));
    item.concat("</span>");
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0 && strcmp(max, "?") != 0) {
      item.concat("<span class=\"hint\"> ");
      item.concat(F(L_RANGE));
      item.concat(" ");
      item.concat(min);
      item.concat(" - ");
      item.concat(max);
      if (strcmp(hint, "?") != 0) {
        item.concat(" ");
        item.concat(hint);
      }
      item.concat("</span>");
    } else if (strcmp(hint, "?") != 0) {
      item.concat("<span class=\"hint\">");
      item.concat(hint);
      item.concat("</span>");
    }
  }
  item.concat("</div>");
}

void AFESitesGenerator::openSection(String &page, const char *title,
                                    const char *description) {
  page.concat(FPSTR(HTTP_FORM_BLOCK_HEADER));
  page.replace("{{title}}", title);
  page.replace("{{description}}", description);
}

void AFESitesGenerator::closeSection(String &page) {
  page.concat(FPSTR(HTTP_FORM_BLOCK_CLOSURE));
}

void AFESitesGenerator::addListOfGPIOs(String &item, const char *field,
                                       uint8_t selected, const char *title) {

  item.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  item.replace("{{item.name}}", field);
  item.replace("{{item.label}}", title);
  item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  item.replace("{{item.value}}", String(AFE_HARDWARE_ITEM_NOT_EXIST));
  item.replace("{{item.label}}", L_NONE);
  item.replace("{{item.selected}}", selected == AFE_HARDWARE_ITEM_NOT_EXIST
                                        ? " selected=\"selected\""
                                        : "");

  for (uint8_t i = 0; i < AFE_NUMBER_OF_GPIOS; i++) {
    item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));

    item.replace("{{item.value}}", String(pgm_read_byte(GPIOS + i)));
    item.replace("{{item.label}}", String(pgm_read_byte(GPIOS + i)));
    item.replace("{{item.selected}}", selected == pgm_read_byte(GPIOS + i)
                                          ? " selected=\"selected\""
                                          : "");
  }
  item.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));
}

void AFESitesGenerator::addInputFormItem(String &item, const char *type,
                                         const char *name, const char *label,
                                         const char *value, const char *size,
                                         const char *min, const char *max,
                                         const char *step, const char *hint,
                                         boolean readonly) {
  item.concat("<div class=\"cf\"><label>");
  item.concat(label);
  item.concat("</label><input name=\"");
  item.concat(name);
  item.concat("\" type=\"");
  item.concat(type);
  item.concat("\" ");
  if (readonly) {
    item.concat("readonly=\"readonly\" ");
  }
  if (strcmp(size, "?") != 0) {
    item.concat("maxlength=\"");
    item.concat(size);
    item.concat("\" ");
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0) {
      item.concat("min=\"");
      item.concat(min);
      item.concat("\" ");
    }
    if (strcmp(max, "?") != 0) {
      item.concat("max=\"");
      item.concat(max);
      item.concat("\" ");
    }
    if (strcmp(step, "?") != 0) {
      item.concat("step=\"");
      item.concat(step);
      item.concat("\" ");
    }
  }
  item.concat("value=\"");
  item.concat(value);
  item.concat("\">");
  if (strcmp(size, "?") != 0) {
    item.concat("<span class=\"hint\">Max ");
    item.concat(size);
    item.concat(" ");
    item.concat(F(L_NUMBER_OF_CHARS));
    item.concat("</span>");
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0 && strcmp(max, "?") != 0) {
      item.concat("<span class=\"hint\"> ");
      item.concat(F(L_RANGE));
      item.concat(" ");
      item.concat(min);
      item.concat(" - ");
      item.concat(max);
      if (strcmp(hint, "?") != 0) {
        item.concat(" ");
        item.concat(hint);
      }
      item.concat("</span>");
    } else if (strcmp(hint, "?") != 0) {
      item.concat("<span class=\"hint\">");
      item.concat(hint);
      item.concat("</span>");
    }
  }
  item.concat("</div>");
}

void AFESitesGenerator::addSelectionFormItem(
    String &item, boolean type, const char *name, const char *label,
    const char *value, boolean checked, const char *hint, boolean disabled) {
  item.concat(FPSTR(HTTP_ITEM_CHECKBOX));
  item.replace("{{item.type}}", type ? "checkbox" : "radio");
  item.replace("{{item.name}}", name);
  item.replace("{{item.label}}", label);
  item.replace("{{item.value}}", value);
  item.replace("{{item.checked}}", checked ? " checked=\"checked\"" : "");
  item.replace("{{item.disabled}}", disabled ? " disabled=\"disabled\"" : "");
  item.replace("{{item.hint}}",
               strcmp(hint, AFE_FORM_ITEM_SKIP_PROPERTY) != 0
                   ? "<span class=\"hint\">(" + String(hint) + ")</span>"
                   : "");
}

void AFESitesGenerator::addCheckboxFormItem(String &item, const char *name,
                                            const char *label,
                                            const char *value, boolean checked,
                                            const char *hint,
                                            boolean disabled) {
  addSelectionFormItem(item, true, name, label, value, checked, hint, disabled);
}

void AFESitesGenerator::addRadioButtonFormItem(
    String &item, const char *name, const char *label, const char *value,
    boolean checked, const char *hint, boolean disabled) {
  addSelectionFormItem(item, false, name, label, value, checked, hint,
                       disabled);
}

void AFESitesGenerator::addMenuItem(String &item, const char *title,
                                    uint8_t siteId) {
  item.concat(FPSTR(HTTP_MENU_ITEM));
  item.replace("{{item.title}}", title);
  item.replace("{{site.id}}", String(siteId));
}

void AFESitesGenerator::addMenuHeaderItem(String &item, const char *title) {
  item.concat(FPSTR(HTTP_MENU_SUBITEMS_HEADER));
  item.replace("{{item.title}}", title);
}

void AFESitesGenerator::addMenuSubItem(String &item, const char *title,
                                       uint8_t numberOfItems, uint8_t siteId) {
  for (uint8_t i = 0; i < numberOfItems; i++) {
    item.concat(FPSTR(HTTP_MENU_SUBITEM));
    item.replace("{{item.title}}", title);
    item.replace("{{item.id}}", String(i));
    item.replace("{{item.id.display}}", String(i + 1));
    item.replace("{{site.id}}", String(siteId));
  }
}