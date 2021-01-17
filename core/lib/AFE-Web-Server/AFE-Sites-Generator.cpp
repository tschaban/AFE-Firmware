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
    page.concat(F("<img src=\"http://api.smartnydom.pl/logo/T"));
    page.concat(Firmware.type);
    page.concat(F("/"));
    page.concat(Firmware.version);
    page.concat(F("/"));
    page.concat(deviceID);
    page.concat(F("/"));
    page.concat(AFE_DEVICE_TYPE_ID);
    page.concat(
        F("\" style=\"width: 100%;display: block\" alt=\"AFE Firmware\">"));
  } else {
    page.concat("<h3 class=\"la\">AFE Firmware: " AFE_DEVICE_TYPE_NAME "</h3>");
  }

  page.concat(F("<div id=\"c\">"));
}

void AFESitesGenerator::generateOneColumnLayout(String &page,
                                                uint16_t redirect) {
  generateHeader(page, redirect);
  page.concat(F("<div id=\"r\">"));
}

void AFESitesGenerator::generateTwoColumnsLayout(String &page,
                                                 uint16_t redirect) {
  Device->begin();

  generateHeader(page, redirect);
  page.concat(F("<div id=\"l\">"));
  if (Device->getMode() == AFE_MODE_ACCESS_POINT) {
    page.concat(F("<h3 class=\"ltit\">AFE FIRMWARE</h3>"));
  }
  page.concat(F("<h4>"));
  page.concat(F(L_FIRMWARE_NAME));
  page.concat(F("</h4><ul class=\"lst\">"));

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

  page.concat(F("</ul><h4>&#10150; "));
  page.concat(F(L_HARDWARE));
  page.concat(F("</h4><ul class=\"lst\">"));

#ifdef AFE_CONFIG_HARDWARE_LED
  if (Device->configuration.noOfLEDs > 0) {
    addMenuHeaderItem(page, L_LEDS);
    addMenuSubItem(page, "LED", Device->configuration.noOfLEDs,
                   AFE_CONFIG_SITE_LED);
  }
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (Device->configuration.noOfGates > 0) {
    addMenuHeaderItem(page, L_GATE_CONFIGURATION);
    addMenuSubItem(page, L_GATE, Device->configuration.noOfGates,
                   AFE_CONFIG_SITE_GATE);
  }
#endif // AFE_CONFIG_HARDWARE_GATE

/* Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  if (Device->configuration.noOfRelays > 0) {

    addMenuHeaderItem(page, L_RELAYS_CONFIGURATION);
    addMenuSubItem(page, L_RELAY, Device->configuration.noOfRelays,
                   AFE_CONFIG_SITE_RELAY);
  }
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  if (Device->configuration.noOfSwitches > 0) {
    addMenuHeaderItem(page, L_BUTTONS_SWITCHES);
    addMenuSubItem(page, L_SWITCH, Device->configuration.noOfSwitches,
                   AFE_CONFIG_SITE_SWITCH);
  }
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  if (Device->configuration.noOfBinarySensors > 0) {
    addMenuHeaderItem(page, L_BINARY_SENSORS);
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfBinarySensors,
                   AFE_CONFIG_SITE_BINARY_SENSOR);
  }
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

/* Contactrons and Gate */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  if (Device->configuration.noOfContactrons > 0) {
    addMenuHeaderItem(page, L_CONTACTRONS);
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfContactrons,
                   AFE_CONFIG_SITE_CONTACTRON);
  }
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

/* Sensor DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  if (Device->configuration.noOfDS18B20s > 0) {

    addMenuHeaderItem(page, L_DS18B20_SENSORS);
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfDS18B20s,
                   AFE_CONFIG_SITE_DS18B20);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

/* Sensor DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
  if (Device->configuration.noOfDHTs > 0) {
    addMenuHeaderItem(page, "DHT");
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfDHTs,
                   AFE_CONFIG_SITE_DHT);
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
#endif // AFE_CONFIG_HARDWARE_HPMA115S0
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  /* This is hardcoded for one sensor */
  if (Device->configuration.noOfHPMA115S0s > 0) {
    page.concat(
        "<li class=\"itm\"><a href=\"\\?i=0&o=" AFE_CONFIG_SITE_HPMA115S0
        "\">" L_HPMA115S0_SENSOR "</a></li>");
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  if (Device->configuration.noOfBMEX80s > 0) {
    addMenuHeaderItem(page, L_BMEX80_SENSORS);
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfBMEX80s,
                   AFE_CONFIG_SITE_BMEX80);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  if (Device->configuration.noOfBH1750s > 0) {

    addMenuHeaderItem(page, L_BH1750_SENSORS);
    addMenuSubItem(page, L_SENSOR, Device->configuration.noOfBH1750s,
                   AFE_CONFIG_SITE_BH1750);
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

  page.concat(F("</ul><h4>&#10150; "));
  page.concat(F(L_INTERFACE));
  page.concat(F("</h4><ul class=\"lst\">"));

/* I2C */
#ifdef AFE_CONFIG_HARDWARE_I2C
/* Don't show it if I2C sensor is not added to the device, this is check for AFE
 * T6 only*/
#if defined(T6_CONFIG)
  if (Device->configuration.noOfBMEX80s > 0 ||
      Device->configuration.noOfBH1750s > 0 ||
      Device->configuration.noOfAS3935s > 0) {
#elif defined(T5_CONFIG)
  if (Device->configuration.noOfBMEX80s > 0 ||
      Device->configuration.noOfBH1750s > 0) {
#endif

    addMenuItem(page, "I2C", AFE_CONFIG_SITE_I2C);

#if defined(T6_CONFIG) || defined(T5_CONFIG)
  }
#endif // T6_CONFIG

#endif // AFE_CONFIG_HARDWARE_I2C

  page.concat(F("</ul><h4>&#10150; "));
  page.concat(F(L_FUNCTIONS));
  page.concat(F("</h4><ul class=\"lst\">"));

/* System LED */
#ifdef AFE_CONFIG_HARDWARE_LED
  if (Device->configuration.noOfLEDs > 0) {
    addMenuItem(page, L_LED_SYSTEM, AFE_CONFIG_SITE_SYSTEM_LED);
  }
#endif

/* Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  if (Device->configuration.noOfRegulators > 0) {
    addMenuHeaderItem(page, L_REGULATORS);
    addMenuSubItem(page, L_REGULATOR, Device->configuration.noOfRegulators,
                   AFE_CONFIG_SITE_REGULATOR);
  }
#endif

/* Thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  if (Device->configuration.noOfThermalProtectors > 0) {
    addMenuHeaderItem(page, L_THERMAL_PROTECTORS);
    addMenuSubItem(page, L_THERMAL_PROTECTOR,
                   Device->configuration.noOfThermalProtectors,
                   AFE_CONFIG_SITE_THERMAL_PROTECTOR);
  }
#endif

  page.concat(F("</ul><h4>&#10150; "));
  page.concat(F(L_FIRMWARE));
  page.concat(F("</h4><ul class=\"lst\">"));

  addMenuItem(page, L_PASSWORD_SET_PASSWORD, AFE_CONFIG_SITE_PASSWORD);
  addMenuItem(page, L_FIRMWARE_UPGRADE, AFE_CONFIG_SITE_UPGRADE);
  addMenuItem(page, L_RESET_DEVICE, AFE_CONFIG_SITE_RESET);
  addMenuItem(page, L_PRO_VERSION, AFE_CONFIG_SITE_PRO_VERSION);

  page.concat(F("</ul><h4></h4><ul class=\"lst\">"));

  addMenuItem(page, L_FINISH_CONFIGURATION, AFE_CONFIG_SITE_EXIT);

  /* Information section */
  page.concat(F("</ul></div><div id=\"r\">"));
}

void AFESitesGenerator::siteDevice(String &page) {
  DEVICE configuration = Device->configuration;
  boolean _itemDisabled = false;

  if (Device->upgraded != AFE_UPGRADE_NONE) {
    page.concat(F("<h4 class=\"bc\" style=\"padding:5px;\">"));
    switch (Device->upgraded) {
    case AFE_UPGRADE_VERSION:
      page.concat(F(L_UPGRADED_TO_NEW_VERSION));
      break;
    case AFE_UPGRADE_VERSION_TYPE:
      page.concat(F(L_UPGRADED_TO_NEW_VERSION_TYPE));
      break;
    }
    page.concat(F("</h4>"));
    Device->upgraded = AFE_UPGRADE_NONE;
  }

  /* Section: Device name */
  openSection(page, F(L_DEVICE), F(L_DEVICE_SECTION_INFO));
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_DEVICE_NAME,
                   configuration.name, "16");
  closeSection(page);

  /* Section: Hardware */
  openSection(page, F(L_DEVICE_HARDWARE_CONFIGURATION),
              F(L_DEVICE_HARDWARE_CONFIGURATION_INFO));

/* LED */
#ifdef AFE_CONFIG_HARDWARE_LED
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS,
                        Device->configuration.noOfLEDs, "l",
                        L_DEVICE_NUMBER_OF_LEDS);
#endif

/* Contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS,
                        Device->configuration.noOfContactrons, "co",
                        L_DEVICE_NUMBER_OF_CONTACTRONS);

#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Relay */
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS,
                        Device->configuration.noOfRelays, "r",
                        L_DEVICE_NUMBER_OF_RELAYS);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Switch */
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES,
                        Device->configuration.noOfSwitches, "s",
                        L_DEVICE_NUMBER_OF_SWITCHES);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  /* Binary sensor */
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS,
                        Device->configuration.noOfBinarySensors, "b",
                        L_DEVICE_NUMBER_OF_BINARY_SENSORS);
#endif

/* DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20

#ifdef T4_CONFIG // Functionality is PRO for T4
  _itemDisabled = !FirmwarePro->Pro.valid;
#else
    _itemDisabled = false;
#endif

  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20,
                        Device->configuration.noOfDS18B20s, "ds",
                        L_DEVICE_NUMBER_OF_DS18B20_SENSORS, _itemDisabled);
#endif

/* DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_DHT,
                        Device->configuration.noOfDHTs, "dh", L_DHT_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
// TODO
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0,
                        Device->configuration.noOfHPMA115S0s, "hp",
                        L_DEVICE_NUMBER_OF_HPMA115S0_SENSORS);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750

#ifdef T5_CONFIG // Functionality is PRO for T5
  _itemDisabled = !FirmwarePro->Pro.valid;
#else
    _itemDisabled = false;
#endif

  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750,
                        Device->configuration.noOfBH1750s, "bh",
                        L_DEVICE_NUMBER_OF_BH1750_SENSORS, _itemDisabled);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80

#ifdef T5_CONFIG // Functionality is PRO for T5
  _itemDisabled = !FirmwarePro->Pro.valid;
#else
    _itemDisabled = false;
#endif

  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80,
                        Device->configuration.noOfBMEX80s, "b6",
                        L_DEVICE_NUMBER_OF_BMEX80_SENSORS, _itemDisabled);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935,
                        Device->configuration.noOfAS3935s, "a3",
                        L_DEVICE_NUMBER_OF_AS3935_SENSORS,
                        !FirmwarePro->Pro.valid);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS,
                        Device->configuration.noOfAnemometerSensors, "w",
                        L_ANEMOMETER_SENSOR);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS,
                        Device->configuration.noOfRainmeterSensors, "d",
                        L_RAINMETER);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  addCheckboxFormItem(
      page, "ad", L_DEVICE_DO_MEASURE_ADC, "1", configuration.isAnalogInput,
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

  body += addListOfHardwareItem(
      sizeof(Device->configuration.isPIR), itemsNumber, "p",
      language == 0 ? "Ilość czujników PIR" : "Number of PIRs");
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  closeSection(page);
  openSection(page, F(L_DEVICE_CONTROLLED_GATES), F(""));
  addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_GATES,
                        Device->configuration.noOfGates, "g",
                        L_DEVICE_NUMBER_OF_CONTROLLED_GATES);
#endif

  closeSection(page);

#ifdef AFE_CONFIG_HARDWARE_RELAY

  if (Device->configuration.noOfRelays > 0) {

#if defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) ||                             \
    defined(AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR)
    /* Additional functionalities */
    openSection(page, F(L_DEVICE_ADDITIONAL_FUNCTIONALITIES), F(""));

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
    addListOfHardwareItem(page, AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS,
                          Device->configuration.noOfRegulators, "re",
                          L_DEVICE_NUMBER_OF_REGULATORS);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
    addListOfHardwareItem(page,
                          AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS,
                          Device->configuration.noOfThermalProtectors, "tp",
                          L_DEVICE_NUMBER_OF_THERMAL_PROTECTORS);
#endif
    closeSection(page);
#endif // defined(AFE_CONFIG_FUNCTIONALITY_REGULATOR) ||
       // defined(AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR)

  } // noOfRelays > 0

#endif // AFE_CONFIG_HARDWARE_RELAY

  /* Section: APIs */
  openSection(page, F(L_DEVICE_CONTROLLING), F(L_DEVICE_CONTROLLING_INFO));

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  addSelectFormItemOpen(page, "v", L_DOMOTICZ_VERSION);
  addSelectOptionFormItem(page, L_DEVICE_DOMOTICZ_VERSION_410, "0",
                          configuration.api.domoticzVersion ==
                              AFE_DOMOTICZ_VERSION_0);
  addSelectOptionFormItem(page, L_DEVICE_DOMOTICZ_VERSION_2020, "1",
                          configuration.api.domoticzVersion ==
                              AFE_DOMOTICZ_VERSION_1);
  addSelectFormItemClose(page);
#endif

  addCheckboxFormItem(page, "h", "HTTP API", "1", configuration.api.http);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  addRadioButtonFormItem(page, "m", "Domoticz HTTP API", "1",
                         configuration.api.domoticz);
  addRadioButtonFormItem(page, "m", "Domoticz MQTT API", "2",
                         configuration.api.mqtt);
#else
  addCheckboxFormItem(page, "m", "MQTT API", "1", configuration.api.mqtt);
#endif

  closeSection(page);

  /* Section: auto logout */
  openSection(page, F(L_DEVICE_AUTOLOGOUT_TITLE),
              F(L_DEVICE_AUTOLOGOUT_DESCRIPTION));
  addCheckboxFormItem(page, "al", L_ENABLED, "1",
                      configuration.timeToAutoLogOff > 0);
  closeSection(page);
}

void AFESitesGenerator::siteConnecting(String &page) {
  NETWORK configuration;
  Data->getConfiguration(&configuration);
  page.concat(F("<p>"));
  page.concat(F(L_NETWORK_DEVICE_CONNECTS));
  page.concat(F(": <strong>"));
  page += configuration.ssid;
  page.concat(F("</strong> ["));
  page += configuration.password;
  page.concat(F("]</p><ul><li>"));
  page.concat(F(L_NETWORK_CONNECT_TO));
  page.concat(F(": "));
  page += configuration.ssid;
  page.concat(F("</li><li>"));
  page.concat(F(L_NETWORK_SEARCH_FOR_IP_ADDRESS));
  page.concat(F(": "));
  page += WiFi.macAddress();
  page.concat(F("</li><li>"));
  page.concat(F(L_NETWORK_FINISH_NETWORK_CONFIGURATION));
  page.concat(F("</li></ul>"));
}

void AFESitesGenerator::siteNetwork(String &page) {
  NETWORK configuration;
  Data->getConfiguration(&configuration);

  /* Section: WiFi selection, user and password */
  openSection(page, F(L_NETWORK_CONFIGURATION),
              F(L_NETWORK_CONFIGURATION_INFO));

  addSelectFormItemOpen(page, "s", L_NETWORK_SSID);

#ifdef DEBUG
  Serial << endl << F("Searching for WiFi networks: ");
#endif
  int numberOfNetworks = WiFi.scanNetworks();
  char _ssid[sizeof(configuration.ssid)];

#ifdef DEBUG
  Serial << endl << F(" - found: ") << numberOfNetworks;
#endif

  for (int i = 0; i < numberOfNetworks; i++) {

#ifdef DEBUG
    Serial << endl << " - " << WiFi.SSID(i);
#endif

    WiFi.SSID(i).toCharArray(_ssid, sizeof(_ssid));
    addSelectOptionFormItem(page, _ssid, _ssid,
                            strcmp(_ssid, configuration.ssid) == 0);
  }
  page.concat(F("</select>"));

  page.concat(F("<input type=\"submit\" class =\"b bc\" value=\""));
  page.concat(F(L_NETWORK_REFRESH));
  page.concat(F("\" formaction=\"/?o="));
  page += AFE_CONFIG_SITE_NETWORK;
  page.concat(F("&c=0\"></div>"));

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_PASSWORD, "p", L_PASSWORD,
                   configuration.password, "32");
  char _ip[18];
  WiFi.macAddress().toCharArray(_ip, 18);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "m", "MAC", _ip,
                   AFE_FORM_ITEM_SKIP_PROPERTY, AFE_FORM_ITEM_SKIP_PROPERTY,
                   AFE_FORM_ITEM_SKIP_PROPERTY, AFE_FORM_ITEM_SKIP_PROPERTY,
                   AFE_FORM_ITEM_SKIP_PROPERTY, true);
  closeSection(page);

  /* Section: DHCP or Fixed IP */
  openSection(page, F(L_NETWORK_DEVICE_IP), F(L_NETWORK_DEVICE_IP_INFO));
  addCheckboxFormItem(page, "d", L_NETWORK_DHCP_ENABLED, "1",
                      configuration.isDHCP);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i1", L_IP_ADDRESS,
                   configuration.ip);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i2", L_NETWORK_GATEWAY,
                   configuration.gateway);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "i3", L_NETWORK_SUBNET,
                   configuration.subnet);
  closeSection(page);

  /* Section: Advanced settings */
  openSection(page, F(L_NETWORK_ADVANCED), F(""));
  char _int[4];
  sprintf(_int, "%d", configuration.noConnectionAttempts);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "na",
                   L_NETWORK_NUMBER_OF_CONNECTIONS, _int,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "255", "1");

  sprintf(_int, "%d", configuration.waitTimeConnections);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "wc",
                   L_NETWORK_TIME_BETWEEN_CONNECTIONS, _int,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "255", "1", L_SECONDS);

  sprintf(_int, "%d", configuration.waitTimeSeries);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "ws", L_NETWORK_SLEEP_TIME,
                   _int, AFE_FORM_ITEM_SKIP_PROPERTY, "1", "255", "1",
                   L_SECONDS);

  closeSection(page);
}

void AFESitesGenerator::siteMQTTBroker(String &page) {
  MQTT configuration;
  Data->getConfiguration(&configuration);

  /* Section: MQTT Broker configuration */
  openSection(page, F("MQTT Broker"), F(L_MQTT_CONFIGURATION_INFO));
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "h", "Hostname",
                   configuration.host, "32");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "a", L_IP_ADDRESS,
                   configuration.ip);
  char _number[7];
  sprintf(_number, "%d", configuration.port);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "p", "Port", _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "65535", "1");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "u", L_USERNAME,
                   configuration.user, "32");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_PASSWORD, "s", L_PASSWORD,
                   configuration.password, "32");
  sprintf(_number, "%d", configuration.timeout);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "t", L_MQTT_TIMEOUT,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "30000", "1",
                   L_MILISECONDS);
  closeSection(page);

/* Section: LWT */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char _idx[7];
  sprintf(_idx, "%d", configuration.lwt.idx);
  openSection(page, F(L_MQTT_IDX_LWT), F(L_DOMOTICZ_NO_IF_IDX_0));
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                   AFE_FORM_ITEM_SKIP_PROPERTY,
                   AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                   AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
  closeSection(page);
#else
  openSection(page, F(L_MQTT_TOPIC_LWT), F(L_MQTT_TOPIC_EMPTY));
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t0", L_MQTT_TOPIC,
                   configuration.lwt.topic, "64");
  closeSection(page);
#endif

  openSection(page, F(L_MQTT_RETAIN_SECTION), F(L_MQTT_RETAIN_SECTION_INFO));

  addCheckboxFormItem(page, "rl", L_MQTT_RETAIN_LWT, "1",
                      configuration.retainLWT);
  addCheckboxFormItem(page, "ra", L_MQTT_RETAIN_ALL, "1",
                      configuration.retainAll);
  closeSection(page);
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
void AFESitesGenerator::siteDomoticzServer(String &page) {
  DOMOTICZ configuration;
  Data->getConfiguration(&configuration);

  openSection(page, F(L_DOMOTICZ_CONFIGURATION),
              F(L_DOMOTICZ_CONFIGURATION_INFO));

  addSelectFormItemOpen(page, "t", L_DOMOTICZ_PROTOCOL);
  addSelectOptionFormItem(page, "http://", "0", configuration.protocol == 0);
  addSelectOptionFormItem(page, "https://", "1", configuration.protocol == 1);
  addSelectFormItemClose(page);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "h", "Hostname/IP",
                   configuration.host, "40");

  char _number[6];
  sprintf(_number, "%d", configuration.port);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "p", "Port", _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "65535", "1");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "u", L_USERNAME,
                   configuration.user, "32");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_PASSWORD, "s", L_PASSWORD,
                   configuration.password, "32");

  closeSection(page);
}
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

void AFESitesGenerator::sitePassword(String &page) {
  PASSWORD configuration;
  Data->getConfiguration(&configuration);
  openSection(page, F(L_PASSWORD_SET_PASSWORD_TO_PANEL), F(""));
  addCheckboxFormItem(page, "r", L_PASSWORD_PROTECT_BY_PASSWORD, "1",
                      configuration.protect);
  if (configuration.protect) {
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "p", L_PASSWORD,
                     configuration.password, "8");
  }
  closeSection(page);
}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFESitesGenerator::siteLED(String &page, uint8_t id) {
  char title[8];
  LED configuration;
  Data->getConfiguration(id, &configuration);
  sprintf(title, "LED: #%d", id + 1);
#ifdef AFE_CONFIG_HARDWARE_MCP23017
  openSection(page, title, F(L_MCP23017_LED_CONNECTION));
#else
    openSection(page, title, F(""));
#endif

  addListOfGPIOs(page, "g", configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_MCP23017_CONNECTION_VIA_MCP));
  addDeviceI2CAddressSelectionItem(page, configuration.mcp23017.address);
  addListOfMCP23017GPIOs(page, "mg", configuration.mcp23017.gpio);
  closeSection(page);
  openSection(page, F(L_MCP23107_LED_ADDITIONAL_SETTINGS), F(""));
#endif

  addCheckboxFormItem(page, "w", L_LED_CHANGE_INDICATION, "1",
                      configuration.changeToOppositeValue);
  closeSection(page);
}

void AFESitesGenerator::siteSystemLED(String &page) {
  uint8_t configuration = Data->getSystemLedID();
  openSection(page, F(L_LED_SYSTEM), F(L_LED_SYSTEM_INFO));
  addLEDSelectionItem(page, configuration);
  closeSection(page);
}
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFESitesGenerator::siteRelay(String &page, uint8_t id) {
  RELAY configuration;
  Data->getConfiguration(id, &configuration);

  char _number[9];
  char _text[23];
  sprintf(_text, "%s #%d", L_RELAY, id + 1);
  openSection(page, _text, F(""));

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Reading gate configuration */
  GATE gateConfiguration;
  boolean isGateRelay = false;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_GATES; i++) {
    Data->getConfiguration(i, &gateConfiguration);

    if (gateConfiguration.relayId != AFE_HARDWARE_ITEM_NOT_EXIST &&
        gateConfiguration.relayId == id) {
      isGateRelay = true;
      break;
    }
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif

    addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                     configuration.name, "16");
#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  closeSection(page);
  openSection(page, F(L_MCP23017_CONNECTION), F(L_MCP23017_RELAY_CONNECTION));
#endif // AFE_CONFIG_HARDWARE_MCP23017

  addListOfGPIOs(page, "g", configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_MCP23017_CONNECTION_VIA_MCP));
  addDeviceI2CAddressSelectionItem(page, configuration.mcp23017.address);
  addListOfMCP23017GPIOs(page, "mg", configuration.mcp23017.gpio);

  closeSection(page);
  openSection(page, F(L_MCP23017_RELAY_TRIGGERED), F(""));

#endif // AFE_CONFIG_HARDWARE_MCP23017

  addSelectFormItemOpen(page, "ts", L_RELAY_TRIGGERED);
  addSelectOptionFormItem(page, L_RELAY_TRIGGERED_HIGH_SIGNAL, "1",
                          configuration.triggerSignal ==
                              AFE_RELAY_SIGNAL_TRIGGER_HIGH);
  addSelectOptionFormItem(page, L_RELAY_TRIGGERED_LOW_SIGNAL, "0",
                          configuration.triggerSignal ==
                              AFE_RELAY_SIGNAL_TRIGGER_LOW);
  addSelectFormItemClose(page);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif

    closeSection(page);

    openSection(page, F(L_RELAY_DEFAULT_VALUES), F(""));
    addSelectFormItemOpen(page, "pr", L_RELAY_DEFAULT_POWER_RESTORED);
    addSelectOptionFormItem(page, L_RELAY_NO_ACTION, "0",
                            configuration.state.powerOn == 0);
    addSelectOptionFormItem(page, L_RELAY_OFF, "1",
                            configuration.state.powerOn == 1);
    addSelectOptionFormItem(page, L_RELAY_ON, "2",
                            configuration.state.powerOn == 2);
    addSelectOptionFormItem(page, L_RELAY_LAST_KNOWN_STATE, "3",
                            configuration.state.powerOn == 3);
    addSelectOptionFormItem(page, L_RELAY_OPPOSITE_TO_LAST_KNOWN_STATE, "4",
                            configuration.state.powerOn == 4);
    addSelectFormItemClose(page);

    if (Device->configuration.api.mqtt) {

      addSelectFormItemOpen(page, "mc", L_RELAY_DEFAULT_MQTT_CONNECTED);
      addSelectOptionFormItem(page, L_RELAY_NO_ACTION, "0",
                              configuration.state.MQTTConnected == 0);
      addSelectOptionFormItem(page, L_RELAY_OFF, "1",
                              configuration.state.MQTTConnected == 1);
      addSelectOptionFormItem(page, L_RELAY_ON, "2",
                              configuration.state.MQTTConnected == 2);
      addSelectOptionFormItem(page, L_RELAY_LAST_KNOWN_STATE, "3",
                              configuration.state.MQTTConnected == 3);
      addSelectOptionFormItem(page, L_RELAY_OPPOSITE_TO_LAST_KNOWN_STATE, "4",
                              configuration.state.MQTTConnected == 4);

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
      addSelectOptionFormItem(page, L_RELAY_DEFAULT_GET_FROM_MQTT, "5",
                              configuration.state.MQTTConnected == 5);
#endif
      addSelectFormItemClose(page);
    }

    closeSection(page);
#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Below code is conditioned for the Gate functionality only. It's not
   * shown if the relay is assigned to the Gate */
  if (!isGateRelay) {
#endif

    openSection(page, F(L_RELAY_AUTOMATIC_SWITCHING_OFF), F(""));

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif

  dtostrf(configuration.timeToOff, 1, 1, _number);

#ifdef AFE_CONFIG_HARDWARE_GATE
  if (isGateRelay) {
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "ot", L_RELAY_IMPULSE_DURATION,
        _number, AFE_FORM_ITEM_SKIP_PROPERTY, "1", "99999", "1", L_MILISECONDS);
  } else {
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "ot", L_RELAY_SWITCH_OFF_AFTER,
        _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "86400", "0.1", L_SECONDS);
  }
#else // Not a GATE
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "ot", L_RELAY_SWITCH_OFF_AFTER,
        _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "86400", "0.1", L_SECONDS);

#endif

  closeSection(page);

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    openSection(page, F(L_RELAY_SELECT_LED_4_RELAY), F(""));
    addLEDSelectionItem(page, configuration.ledID);
    closeSection(page);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
    /* LED Exclusion for a relay assigned to ta gate */
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /* Excluded code below for Gate functionality and the relay assigned to
   * the gate */
  if (!isGateRelay) {
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
      openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
      char _idx[7];
      sprintf(_idx, "%d", configuration.domoticz.idx);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                       AFE_FORM_ITEM_SKIP_PROPERTY,
                       AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                       AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
      closeSection(page);
    }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, F(L_RELAY_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

void AFESitesGenerator::siteRegulator(String &page, uint8_t id) {
  REGULATOR configuration;
  RELAY relayConfiguration;
  char text[30];
  char value[4];
  Data->getConfiguration(id, &configuration);

  openSection(page, L_REGULATOR, "");

  /* Item: name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: relay */
  addSelectFormItemOpen(page, "r", L_RELAY);
  sprintf(value, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(
      page, L_NONE, value,
      configuration.relayId == AFE_HARDWARE_ITEM_NOT_EXIST ? true : false);

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    Data->getConfiguration(i, &relayConfiguration);
    sprintf(text, "%d: %s", i + 1, relayConfiguration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.relayId == i ? true : false);
  }
  addSelectFormItemClose(page);

  /* Item: sensor */
  addSelectFormItemOpen(page, "s", L_SENSOR);
  sprintf(value, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(
      page, L_NONE, value,
      configuration.sensorId == AFE_HARDWARE_ITEM_NOT_EXIST ? true : false);

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  DS18B20 ds18b20Configuration;
  for (uint8_t i = 0; i < Device->configuration.noOfDS18B20s; i++) {
    Data->getConfiguration(i, &ds18b20Configuration);
    sprintf(text, "DS18B20 %d: %s", i + 1, ds18b20Configuration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.sensorId == i ? true : false);
  }
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  DHT DHTConfiguration;
  for (uint8_t i = 0; i < Device->configuration.noOfDHTs; i++) {
    Data->getConfiguration(i, &DHTConfiguration);
    sprintf(text, "DHT %d: %s", i + 1, DHTConfiguration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.sensorId == i ? true : false);
  }
  addSelectFormItemClose(page);

  /* Item: controlling parameter */
  addSelectFormItemOpen(page, "cp", L_DHT_CONTROLLING_PARAMETER);
  sprintf(value, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(page, L_NONE, value,
                          configuration.controllingParameter ==
                                  AFE_HARDWARE_ITEM_NOT_EXIST
                              ? true
                              : false);

  sprintf(value, "%d", AFE_FUNCTIONALITY_REGULATOR_CP_TEMPERATURE);
  addSelectOptionFormItem(page, L_REGULATOR_CP_TEMPERATURE, value,
                          configuration.controllingParameter ==
                                  AFE_FUNCTIONALITY_REGULATOR_CP_TEMPERATURE
                              ? true
                              : false);

  sprintf(value, "%d", AFE_FUNCTIONALITY_REGULATOR_CP_HEAT_INDEX);
  addSelectOptionFormItem(page, L_REGULATOR_CP_HEAT_INDEX, value,
                          configuration.controllingParameter ==
                                  AFE_FUNCTIONALITY_REGULATOR_CP_HEAT_INDEX
                              ? true
                              : false);

  sprintf(value, "%d", AFE_FUNCTIONALITY_REGULATOR_CP_HUMIDITY);
  addSelectOptionFormItem(page, L_REGULATOR_CP_HUMIDITY, value,
                          configuration.controllingParameter ==
                                  AFE_FUNCTIONALITY_REGULATOR_CP_HUMIDITY
                              ? true
                              : false);

  sprintf(value, "%d", AFE_FUNCTIONALITY_REGULATOR_CP_ABSOLOUTE_HUMIDITY);
  addSelectOptionFormItem(
      page, L_REGULATOR_CP_ABSOLOUTE_HUMIDITY, value,
      configuration.controllingParameter ==
              AFE_FUNCTIONALITY_REGULATOR_CP_ABSOLOUTE_HUMIDITY
          ? true
          : false);

  sprintf(value, "%d", AFE_FUNCTIONALITY_REGULATOR_CP_DEW_POINT);
  addSelectOptionFormItem(page, L_REGULATOR_CP_DEW_POINT, value,
                          configuration.controllingParameter ==
                                  AFE_FUNCTIONALITY_REGULATOR_CP_DEW_POINT
                              ? true
                              : false);

#endif // AFE_CONFIG_HARDWARE_DHT

  addSelectFormItemClose(page);

  /* Item: regulator enabled? */
  addCheckboxFormItem(page, "e", L_REGULATOR_ENABLED, "1",
                      configuration.enabled);

  closeSection(page);

  addRegulatorControllerItem(page, &configuration);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_REGULATOR_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFESitesGenerator::siteThermalProtector(String &page, uint8_t id) {
  THERMAL_PROTECTOR configuration;
  RELAY relayConfiguration;
  char text[30];
  char value[4];
  Data->getConfiguration(id, &configuration);

  openSection(page, L_THERMAL_PROTECTORS,
              L_THERMAL_PROTECTOR_AUTOMATIC_SWITCHING_OFF);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  addSelectFormItemOpen(page, "r", L_RELAY);
  sprintf(value, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(
      page, L_NONE, value,
      configuration.relayId == AFE_HARDWARE_ITEM_NOT_EXIST ? true : false);

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    Data->getConfiguration(i, &relayConfiguration);
    sprintf(text, "%d: %s", i + 1, relayConfiguration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.relayId == i ? true : false);
  }
  addSelectFormItemClose(page);

  addSelectFormItemOpen(page, "s", L_SENSOR);
  sprintf(value, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(
      page, L_NONE, value,
      configuration.sensorId == AFE_HARDWARE_ITEM_NOT_EXIST ? true : false);

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  DS18B20 ds18b20Configuration;
  for (uint8_t i = 0; i < Device->configuration.noOfDS18B20s; i++) {
    Data->getConfiguration(i, &ds18b20Configuration);
    sprintf(text, "DS18B20 %d: %s", i + 1, ds18b20Configuration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.sensorId == i ? true : false);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  DHT DHTConfiguration;
  for (uint8_t i = 0; i < Device->configuration.noOfDHTs; i++) {
    Data->getConfiguration(i, &DHTConfiguration);
    sprintf(text, "DHT %d: %s", i + 1, DHTConfiguration.name);
    sprintf(value, "%d", i);
    addSelectOptionFormItem(page, text, value,
                            configuration.sensorId == i ? true : false);
  }
#endif

  addSelectFormItemClose(page);

  addCheckboxFormItem(page, "e", L_THERMAL_PROTECTOR_ENABLED, "1",
                      configuration.enabled);
  sprintf(text, "%-.3f", configuration.temperature);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "m",
                   L_RELAY_SWITCH_OFF_ABOVE, text, AFE_FORM_ITEM_SKIP_PROPERTY,
                   "-999", "999", "any");

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_THERMAL_PROTECTOR_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFESitesGenerator::siteSwitch(String &page, uint8_t id) {
  SWITCH configuration;
  Data->getConfiguration(id, &configuration);
  char text[25];

#ifdef AFE_CONFIG_HARDWARE_GATE
  GATE gateConfiguration;
#endif

  sprintf(text, "%s #%d", L_SWITCH_BUTTON, id + 1);

  openSection(page, text, F(""));

#ifndef AFE_CONFIG_HARDWARE_MCP23017
  addListOfGPIOs(page, "g", configuration.gpio);
#endif

  addSelectFormItemOpen(page, "f", L_SWITCH_FUNCTIONALITY);
  addSelectOptionFormItem(page, L_NONE, "0", configuration.functionality ==
                                                 AFE_SWITCH_FUNCTIONALITY_NONE);
  addSelectOptionFormItem(page, L_SWITCH_SYSTEM_BUTTON, "1",
                          configuration.functionality ==
                              AFE_SWITCH_FUNCTIONALITY_MULTI);

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_GATE)

  addSelectOptionFormItem(page, L_SWITCH_CONTROL_RELAY, "2",
                          configuration.functionality ==
                              AFE_SWITCH_FUNCTIONALITY_RELAY);
#endif // defined(AFE_CONFIG_HARDWARE_RELAY) ||
       // defined(AFE_CONFIG_HARDWARE_GATE)

  addSelectFormItemClose(page);

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_GATE)

  addSelectFormItemOpen(page, "r", L_SWITCH_RELAY_CONTROLLED);
  addSelectOptionFormItem(page, L_NONE, "255",
                          configuration.relayID == AFE_HARDWARE_ITEM_NOT_EXIST);

#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t relayIsForGate;
#endif
  RELAY relayConfiguration;
  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    page += "<option value=\"";
    page += i;
    page += "\"";
    page += configuration.relayID == i ? " selected=\"selected\"" : "";
    page += ">";
#ifdef AFE_CONFIG_HARDWARE_GATE
    relayIsForGate = false;
    for (uint8_t j = 0; j < Device->configuration.noOfGates; j++) {
      Data->getConfiguration(j, &gateConfiguration);
      if (i == gateConfiguration.relayId) {
        page += F(L_GATE);
        page += ": ";
        page += gateConfiguration.name;
        relayIsForGate = true;
        break;
      }
    }
    if (!relayIsForGate) {
      Data->getConfiguration(i, &relayConfiguration);
      sprintf(text, "%d: %s", i + 1, relayConfiguration.name);
      page.concat(text);
    }
#else
    Data->getConfiguration(i, &relayConfiguration);
    sprintf(text, "%d: %s", i + 1, relayConfiguration.name);
    page.concat(text);
#endif
    page += "</option>";
  }
  addSelectFormItemClose(page);

#endif // // defined(AFE_CONFIG_HARDWARE_RELAY) ||
       // defined(AFE_CONFIG_HARDWARE_GATE)

  addSelectFormItemOpen(page, "m", L_SWITCH_TYPE);
  addSelectOptionFormItem(page, L_SWITCH_MONOSTABLE, "0",
                          configuration.type == 0);
  addSelectOptionFormItem(page, L_SWITCH_BISTABLE, "1",
                          configuration.type == 1);
  addSelectFormItemClose(page);

  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_SWITCH_SENSITIVENESS_HINT));

  char _number[4];
  sprintf(_number, "%d", configuration.sensitiveness);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "s", L_SENSITIVENESS,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "999", "1",
                   L_MILISECONDS);
  closeSection(page);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  openSection(page, F(L_MCP23017_CONNECTION), F(L_MCP23017_SWITCH_CONNECTION));
  addListOfGPIOs(page, "g", configuration.gpio);
  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_MCP23017_CONNECTION_VIA_MCP));
  addDeviceI2CAddressSelectionItem(page, configuration.mcp23017.address);
  addListOfMCP23017GPIOs(page, "mg", configuration.mcp23017.gpio);

  closeSection(page);
#endif // AFE_CONFIG_HARDWARE_MCP23017

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, F(L_SWITCH_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFESitesGenerator::siteDS18B20Sensor(String &page, uint8_t id) {

  AFESensorDS18B20 _Sensor;
  DS18B20Addresses _addresses;
  uint8_t numberOfFoundSensors;
  DS18B20 configuration;
  Data->getConfiguration(id, &configuration);
  char _number[13];

  openSection(page, F(L_DS18B20_SENSOR), F(""));

  /* Item: GPIO */
  addListOfGPIOs(page, "g", configuration.gpio, "GPIO");

  /* Item: Sensor address */

  numberOfFoundSensors = _Sensor.scan(configuration.gpio, _addresses);

  if (numberOfFoundSensors > 0) {
    addSelectFormItemOpen(page, "a", L_ADDRESS);
    char _scannedAddressText[17];
    char _configAddressText[17];
    for (uint8_t i = 0; i < numberOfFoundSensors; i++) {
      _Sensor.addressToChar(_addresses[i], _scannedAddressText);
      _Sensor.addressToChar(configuration.address, _configAddressText);
      addSelectOptionFormItem(page, _scannedAddressText, _scannedAddressText,
                              memcmp(_addresses[i], configuration.address,
                                     sizeof(_addresses[i])) == 0);
    }
    page += "</select><input type=\"submit\" class =\"b bc\" "
            "value=\"" L_DS18B20_SEARCH "\"></div>";
  } else {
    page += "<p class=\"cm\">" L_DS18B20_NO_SENSOR "</p>";
    page += "<input type=\"submit\" class =\"b bc\" value=\"" L_DS18B20_SEARCH
            "\"><br /><br />";
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
  addSelectFormItemOpen(page, "u", L_UNITS);
  addSelectOptionFormItem(page, "C", "1",
                          configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS);
  addSelectOptionFormItem(page, "F", "2", configuration.unit ==
                                              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
  addSelectFormItemClose(page);

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
    sprintf(_number, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, F(L_DS18B20_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");

      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFESitesGenerator::siteDHTSensor(String &page, uint8_t id) {

  AFESensorDHT _Sensor;
  DHT configuration;
  Data->getConfiguration(id, &configuration);
  char _number[13];
  char _text[13];

  sprintf(_text, "DHT: #%d", id + 1);
  openSection(page, _text, "");

  /* Item: GPIO */
  addListOfGPIOs(page, "g", configuration.gpio, "GPIO");

  /* Item: Name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: type of the sensor */
  addSelectFormItemOpen(page, "t", L_DHT_SENSOR_TYPE);
  addSelectOptionFormItem(page, L_NONE, "255",
                          configuration.type == AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(page, L_DHT_AUTO_DETECT, "0",
                          configuration.type ==
                              AFE_CONFIG_HARDWARE_DHT_TYPE_AUTO);
  addSelectOptionFormItem(page, "DHT11", "1",
                          configuration.type ==
                              AFE_CONFIG_HARDWARE_DHT_TYPE_DHT11);
  addSelectOptionFormItem(page, "DHT22", "2",
                          configuration.type ==
                              AFE_CONFIG_HARDWARE_DHT_TYPE_DHT22);
  addSelectOptionFormItem(page, "AM2302", "3",
                          configuration.type ==
                              AFE_CONFIG_HARDWARE_DHT_TYPE_AM2302);
  addSelectOptionFormItem(page, "RHT03", "4",
                          configuration.type ==
                              AFE_CONFIG_HARDWARE_DHT_TYPE_RHT03);
  addSelectFormItemClose(page);

  /* Item: Interval */
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "2", "86400", "1",
                   L_SECONDS);

  /* Item: Send only changes */

  addCheckboxFormItem(page, "s", L_DHT_SENT_ONLY_CHANGES, "1",
                      configuration.sendOnlyChanges);

  closeSection(page);

  /* Item: Unit */
  openSection(page, L_UNITS, "");
  addSelectFormItemOpen(page, "tu", L_TEMPERATURE);
  addSelectOptionFormItem(page, "C", "1", configuration.temperature.unit ==
                                              AFE_TEMPERATURE_UNIT_CELSIUS);
  addSelectOptionFormItem(page, "F", "2", configuration.temperature.unit ==
                                              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
  addSelectFormItemClose(page);
  closeSection(page);

  /* Item: Corrections of sensor values */
  openSection(page, L_CORRECTIONS, "");
  sprintf(_number, "%-.3f", configuration.temperature.correction);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "tc", L_TEMPERATURE,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "-99.999", "99.999",
                   "0.001");

  /* Item: humidity correction */
  sprintf(_number, "%-.3f", configuration.humidity.correction);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "hc", L_HUMIDITY, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "-99.999", "99.999", "0.001");

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);

    sprintf(_number, "%d", configuration.domoticz.temperature.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i1", L_TEMPERATURE_IDX,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.humidity.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i2", L_HUMIDITY_IDX,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.temperatureHumidity.idx);
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "i5", L_HUMIDITY_IDX_TEMP_HUM, _number,
        AFE_FORM_ITEM_SKIP_PROPERTY, AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
        AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.absoluteHumidity.idx);
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "i8", L_HUMIDITY_IDX_ABSOLUTE, _number,
        AFE_FORM_ITEM_SKIP_PROPERTY, AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
        AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.dewPoint.idx);
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "i3", L_HUMIDITY_IDX_DEW_POINT,
        _number, AFE_FORM_ITEM_SKIP_PROPERTY, AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
        AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.heatIndex.idx);
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "i4", L_HUMIDITY_IDX_HEAT_INDEX,
        _number, AFE_FORM_ITEM_SKIP_PROPERTY, AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
        AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.perception.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i6", L_PERCEPTION_IDX,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    sprintf(_number, "%d", configuration.domoticz.comfort.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i7", L_COMFORT_IDX,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_DHT_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "m", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");

      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_DHT

#if defined(T3_CONFIG)
String AFESitesGenerator::sitePIR(uint8_t id) {
  PIR configuration = Data->getPIRConfiguration(id);
  DEVICE device = Data->getConfiguration();

  String body = "<fieldset>";
  char field[13];
  sprintf(field, "g%d", id);
  body += "<div class=\"cf\">";
  body += addListOfGPIOs(field, configuration.gpio);
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

  String page = openSection(title, "", body);

  if (Device->configuration.api.domoticz) {
    body = "<fieldset><div class=\"cf\"><label>IDX</label><input name=\"x";
    body += id;
    body += "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\"><span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span></div></fieldset>";
    page += openSection(
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
void AFESitesGenerator::siteContactron(String &page, uint8_t id) {
  CONTACTRON configuration;
  Data->getConfiguration(id, &configuration);
  char title[23];
  sprintf(title, "%s #%d", L_CONTACTRON, id + 1);
  openSection(page, title, "");

  /* Item: GPIO */
  addListOfGPIOs(page, "g", configuration.gpio, "GPIO");

  /* Item: name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: type */
  addSelectFormItemOpen(page, "y", L_SWITCH_TYPE);
  addSelectOptionFormItem(page, "NO", "0",
                          configuration.type == AFE_CONTACTRON_NO);
  addSelectOptionFormItem(page, "NC", "1",
                          configuration.type == AFE_CONTACTRON_NC);

  addSelectFormItemClose(page);

  page.concat(FPSTR(HTTP_ITEM_HINT));
  page.replace("{{item.hint}}", F(L_CONTACTRON_SET_SENSITIVENESS));

  /* Item: bouncing */
  char _number[5];
  sprintf(_number, "%d", configuration.bouncing);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "b", L_SENSITIVENESS,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "2000", "1",
                   L_MILISECONDS);
  closeSection(page);

  /* Item: LED */
  openSection(page, L_CONTACTRON_LED_ASSIGNED_TO_SENSOR, "");
  addLEDSelectionItem(page, configuration.ledID);
  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);

    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_CONTACTRON_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFESitesGenerator::siteGate(String &page, uint8_t id) {
  GATE gateConfiguration;
  Data->getConfiguration(id, &gateConfiguration);
  CONTACTRON contactronConfiguration[2];

  openSection(page, L_GATE_CONFIGURATION, "");

  /* Item: Gate name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   gateConfiguration.name, "16");

  /* Item: relay */
  char _relayName[20];
  char _relayId[4];
  RELAY relayConfiguration;
  addSelectFormItemOpen(page, "r", L_GATE_RELAY_ID_CONTROLLING_GATE);
  sprintf(_relayId, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);

  addSelectOptionFormItem(page, L_NONE, _relayId,
                          gateConfiguration.relayId ==
                              AFE_HARDWARE_ITEM_NOT_EXIST);

  for (uint8_t i = 0; i < Device->configuration.noOfRelays; i++) {
    Data->getConfiguration(i, &relayConfiguration);

    if (relayConfiguration.name[0] == '\0') {
      sprintf(_relayName, "%d: %s", i + 1, L_RELAY);
    } else {
      sprintf(_relayName, "%d: %s", i + 1, relayConfiguration.name);
    }
    sprintf(_relayId, "%d", i);
    addSelectOptionFormItem(page, _relayName, _relayId,
                            gateConfiguration.relayId == i);
  }
  relayConfiguration = {0};
  addSelectFormItemClose(page);

  /* Item: contactron 1 */
  if (Device->configuration.noOfContactrons > 0) {
    addGateContactronsListItem(page, "c1", gateConfiguration.contactron.id[0]);
  }

  /* If there is more than a one contactron connected, add option to assigne
   * it to the gate */

  if (Device->configuration.noOfContactrons > 1) {
    /* Item: contactron 2 */
    addGateContactronsListItem(page, "c2", gateConfiguration.contactron.id[1]);
  }

  closeSection(page);

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

      openSection(page, L_GATE_STATES_CONFIGURATION, "");

      for (uint8_t i = 0; i < numberOfContractons; i++) {

        Data->getConfiguration(gateConfiguration.contactron.id[i],
                               &contactronConfiguration[i]);
      }

      page.concat("<fieldset>");

      page += "<p class=\"cm\">";
      page += F(L_GATE_IF_MAGNETIC_SENSOR);
      page += ": <strong>";
      page += contactronConfiguration[0].name;

      if (numberOfContractons == 2) {
        page += "</strong> ";
        page += F(L_GATE_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_GATE_ARE_OPEN);
      } else {
        page += "</strong> ";
        page += F(L_GATE_IS_OPEN);
      }
      page += " ";
      page += F(L_GATE_THEN);
      page += ":</p>";
      addGateStatesListItem(page, 0, gateConfiguration.states.state[0]);
      if (numberOfContractons == 2) {
        page += "<br><br><p class=\"cm\">";
        page += F(L_GATE_IF_MAGNETIC_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[0].name;
        page += "</strong> ";
        page += F(L_GATE_IS_OPEN);
        page += " ";
        page += F(L_GATE_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_GATE_IS_CLOSED);
        page += " ";
        page += F(L_GATE_THEN);
        page += ":";
        page += "</p>";
        addGateStatesListItem(page, 1, gateConfiguration.states.state[1]);

        page += "<br><br><p class=\"cm\">";
        page += F(L_GATE_IF_MAGNETIC_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[0].name;
        page += "</strong> ";
        page += F(L_GATE_IS_CLOSED);
        page += " ";
        page += F(L_GATE_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_GATE_IS_OPEN);
        page += " ";
        page += F(L_GATE_THEN);
        page += ":";
        page += "</p>";

        addGateStatesListItem(page, 2, gateConfiguration.states.state[2]);
      }

      page += "<br><br><p class=\"cm\">";

      page += F(L_GATE_IF_MAGNETIC_SENSOR);
      page += ": <strong>";
      page += contactronConfiguration[0].name;
      if (numberOfContractons == 2) {
        page += "</strong> ";
        page += F(L_GATE_AND_SENSOR);
        page += ": <strong>";
        page += contactronConfiguration[1].name;
        page += "</strong> ";
        page += F(L_GATE_ARE_CLOSED);
      } else {
        page += "</strong> ";
        page += F(L_GATE_IS_CLOSED);
      }
      page += " ";
      page += F(L_GATE_THEN);
      page += ":</p>";
      addGateStatesListItem(page, 3, gateConfiguration.states.state[3]);
      closeSection(page);
    }
  }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    char _idx[7];

    if (Device->configuration.api.mqtt) {
      sprintf(_idx, "%d", gateConfiguration.domoticzControl.idx);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "z", "IDX Start/Stop",
                       _idx, AFE_FORM_ITEM_SKIP_PROPERTY,
                       AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                       AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    }

    sprintf(_idx, "%d", gateConfiguration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", L_GATE_IDX_STATE,
                     _idx, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_GATE_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       gateConfiguration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFESitesGenerator::siteHPMA115S0Sensor(String &page, uint8_t id) {
  HPMA115S0 configuration;
  Data->getConfiguration(id, &configuration);
  openSection(page, L_HPMA115S0_SENSOR, "");
  page.concat("<fieldset>");
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "5", "86400", "f",
                   L_SECONDS);

  page += "<br><br>";
  page += "<p class=\"cm\">";
  page += F(L_HPMA115S0_POST_SLEEP_INTERVAL);
  page += "</p>";

  sprintf(_number, "%d", configuration.timeToMeasure);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "m",
                   L_HPMA115S0_MEASURE_AFTER, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "999", "1", L_SECONDS);
  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_number, "%d", configuration.domoticz.pm25.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x2", "IDX PM2.5",
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    sprintf(_number, "%d", configuration.domoticz.pm10.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x1", "IDX PM10", _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_HPMA115S0_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      page.concat("<fieldset>");
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFESitesGenerator::siteBMEX80Sensor(String &page, uint8_t id) {
  char _number[7];
  BMEX80 configuration;
  Data->getConfiguration(id, &configuration);

  openSection(page, L_BMEX80_SENSOR, "");

  /* Item: IIC address selection */
  addDeviceI2CAddressSelectionItem(page, configuration.i2cAddress);

  /* Item: type of the sensor */
  addSelectFormItemOpen(page, "b", L_BMEX80_SENSOR_TYPE);
  addSelectOptionFormItem(page, L_NONE, "255",
                          configuration.type == AFE_BMX_UNKNOWN_SENSOR);
  addSelectOptionFormItem(page, "BMx085/BMx180", "1",
                          configuration.type == AFE_BMP180_SENSOR);
  addSelectOptionFormItem(page, "BMx280", "2",
                          configuration.type == AFE_BME280_SENSOR);
  addSelectOptionFormItem(page, "BMx680", "3",
                          configuration.type == AFE_BME680_SENSOR);
  addSelectFormItemClose(page);

  page.concat(
      "<input type=\"submit\" class=\"b bw\" value=\"" L_BMEX80_REFRESH_SETTINGS
      "\"><br><br>");

  /* Item: name */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: interval */
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "5", "86400", "1",
                   L_SECONDS);

  closeSection(page);

  if (configuration.type != AFE_BMX_UNKNOWN_SENSOR) {

    /* Item: Unit */
    openSection(page, L_UNITS, "");
    addSelectFormItemOpen(page, "tu", L_TEMPERATURE);
    addSelectOptionFormItem(page, "C", "1", configuration.temperature.unit ==
                                                AFE_TEMPERATURE_UNIT_CELSIUS);
    addSelectOptionFormItem(page, "F", "2",
                            configuration.temperature.unit ==
                                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    addSelectFormItemClose(page);
    closeSection(page);

    /* Item: Corrections of sensor values */
    openSection(page, L_CORRECTIONS, "");
    sprintf(_number, "%-.3f", configuration.temperature.correction);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "tc", L_TEMPERATURE,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY, "-99.999", "99.999",
                     "0.001");

    if (configuration.type != AFE_BMP180_SENSOR) {
      /* Item: humidity correction */
      sprintf(_number, "%-.3f", configuration.humidity.correction);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "hc", L_HUMIDITY,
                       _number, AFE_FORM_ITEM_SKIP_PROPERTY, "-99.999",
                       "99.999", "0.001");
    }

    /* Item: pressure correction */
    sprintf(_number, "%-.3f", configuration.pressure.correction);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "pc", L_PRESSURE, _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY, "-999.999", "999.999",
                     "0.001");

    /* Item: altitude */
    sprintf(_number, "%d", configuration.altitude);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "hi", L_BMEX80_ALTITIDE,
                     _number, AFE_FORM_ITEM_SKIP_PROPERTY, "-431", "8850", "1",
                     L_BMEX80_METERS);

    closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
      openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
      sprintf(_number, "%d", configuration.domoticz.temperature.idx);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i1", L_TEMPERATURE_IDX,
                       _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                       AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                       AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

      if (configuration.type != AFE_BMP180_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.humidity.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i2", L_HUMIDITY_IDX,
                         _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.dewPoint.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i3",
                         L_HUMIDITY_IDX_DEW_POINT, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.heatIndex.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i4",
                         L_HUMIDITY_IDX_HEAT_INDEX, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d",
                configuration.domoticz.temperatureHumidityPressure.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i0",
                         L_HUMIDITY_IDX_TEMP_HUM_BAR, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.temperatureHumidity.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i12",
                         L_HUMIDITY_IDX_TEMP_HUM, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
      }

      sprintf(_number, "%d", configuration.domoticz.pressure.idx);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i5", L_PRESSURE_IDX,
                       _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                       AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                       AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

      sprintf(_number, "%d", configuration.domoticz.relativePressure.idx);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i6",
                       L_PRESSURE_IDX_RELATIVE_PRESSURE, _number,
                       AFE_FORM_ITEM_SKIP_PROPERTY,
                       AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                       AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

      if (configuration.type == AFE_BME680_SENSOR) {
        sprintf(_number, "%d", configuration.domoticz.iaq.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i7",
                         L_BMEX80_IDX_IQA, _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.staticIaq.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i8",
                         L_BMEX80_IDX_STATIC_IAQ, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.co2Equivalent.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i9",
                         L_BMEX80_IDX_CO2_EQUVALENT, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.breathVocEquivalent.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i10",
                         L_BMEX80_IDX_BVOC_EQUIVALENT, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");

        sprintf(_number, "%d", configuration.domoticz.gasResistance.idx);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "i11",
                         L_BMEX80_IDX_GAS_SENSOR, _number,
                         AFE_FORM_ITEM_SKIP_PROPERTY,
                         AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                         AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
      }
      closeSection(page);
    }
#else
      if (Device->configuration.api.mqtt) {
        openSection(page, L_BMEX80_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
        addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                         configuration.mqtt.topic, "64");
        closeSection(page);
      }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
  }
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFESitesGenerator::siteBH1750Sensor(String &page, uint8_t id) {

  BH1750 configuration;
  Data->getConfiguration(id, &configuration);
  openSection(page, L_BH1750_SENSOR, "");

  /* Item: I2C Address selection */
  addDeviceI2CAddressSelectionItem(page, configuration.i2cAddress);

  /* Item: name of the sensor */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: interval */
  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "5", "86400", "1",
                   L_SECONDS);

  /* Item: sensitivness, not possiblity to change */
  sprintf(_number, "%d", configuration.mode);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "m", L_SENSITIVENESS,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY,
                   AFE_FORM_ITEM_SKIP_PROPERTY, AFE_FORM_ITEM_SKIP_PROPERTY,
                   AFE_FORM_ITEM_SKIP_PROPERTY, L_ADC_CANT_CHANGE, true);

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    sprintf(_number, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "d", "IDX", _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_BH1750_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
// String AFESitesGenerator::siteAS3935Sensor(uint8_t id) {
void AFESitesGenerator::siteAS3935Sensor(String &page, uint8_t id) {
  AS3935 configuration;
  Data->getConfiguration(id, &configuration);
  char _number[2];

  openSection(page, L_AS3935_SENSOR, "");
  page.concat("<fieldset>");
  addDeviceI2CAddressSelectionItem(page, configuration.i2cAddress);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");
  page += "<div class=\"cf\">";
  addListOfGPIOs(page, "g", configuration.irqGPIO);
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
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "e",
                   L_AS3935_WATCHDOG_THRESHOLD, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "10", "1",
                   L_AS3935_WATCHDOG_THRESHOLD_HINT);

  sprintf(_number, "%d", configuration.spikesRejectionLevel);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "s",
                   L_AS3935_SPIKES_REJECTION, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "11", "1",
                   L_AS3935_SPIKES_REJECTION_HINT);

  page += "<div class=\"cf\"><label>";
  page += F(L_AS3935_MIN_SPIKES);
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
  page += F(L_AS3935_AUTOMATIC_NOISE_FLOOR_CONTROL);
  page += "</label></div><p class=\"cm\">";

  page += F(L_AS3935_SET_LEVEL_OF_NOISE_FLOOR);
  sprintf(_number, "%d", configuration.noiseFloor);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "nf", L_AS3935_NOISE_FLOOR,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "1", "7", "1",
                   L_AS3935_NOISE_FLOOR_HINT);

  page += "</p><div class=\"cf\"><label>";
  page += F(L_AS3935_SENSOR_INDOOR_OUTDOOR);
  page += "</label><select name=\"w\"><option value=\"1\"";
  page += (configuration.indoor == true ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_AS3935_INDOOR);
  page += "</option><option value=\"0\"";
  page += (configuration.indoor == false ? " selected=\"selected\"" : "");
  page += ">";
  page += F(L_AS3935_OUTDOOR);
  page += "</option></select></div>";

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    page.concat("<fieldset>");
    sprintf(_number, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "d", "IDX", _number,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_AS3935_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      page.concat("<fieldset>");
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
void AFESitesGenerator::siteAnemometerSensor(String &page) {

  openSection(page, L_ANEMOMETER_SENSOR, "");
  page.concat("<fieldset>");
  ANEMOMETER configuration;
  Data->getConfiguration(&configuration);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  page += "<div class=\"cf\">";
  addListOfGPIOs(page, "g", configuration.gpio);
  page += "</div>";

  char _number[7];
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "5", "86400", "1",
                   L_SECONDS);

  closeSection(page);

  openSection(page, L_ANEMOMETER_CALIBRATION,
              L_ANEMOMETER_IMPULSE_DISTANCE_HINT);
  page.concat("<fieldset>");

  sprintf(_number, "%-.2f", configuration.impulseDistance);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "l",
                   L_ANEMOMETER_IMPULSE_DISTANCE, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "999.99", "0.01");

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
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "s", L_SENSITIVENESS,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "255", "1",
                   L_MILISECONDS);

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      page.concat("<fieldset>");
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
void AFESitesGenerator::siteRainmeterSensor(String &page) {

  openSection(page, L_RAINMETER, "");
  page.concat("<fieldset>");
  RAINMETER configuration;
  Data->getConfiguration(&configuration);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  page += "<div class=\"cf\">";
  addListOfGPIOs(page, "g", configuration.gpio);
  page += "</div>";

  char _number[8];
  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "f", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "5", "86400", "1",
                   L_SECONDS);

  page += "</fieldset></div>";

  openSection(page, L_RAINMETER_CALIBRATION, "");

  sprintf(_number, "%-.2f", configuration.resolution);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "r", L_RAINMETER_RESOLUTION,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "9999.99", "0.01",
                   "ml/m2");

  page += "</fieldset></div>";

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, "Domoticz", L_DOMOTICZ_NO_IF_IDX_0);
    page.concat("<fieldset>");
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    page += "</fieldset></div>";
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, L_SWITCH_MQTT_TOPIC, L_MQTT_TOPIC_EMPTY);
      page.concat("<fieldset>");
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      page += "</fieldset></div>";
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFESitesGenerator::siteADCInput(String &page) {
  ADCINPUT configuration;
  Data->getConfiguration(&configuration);

  openSection(page, F(L_ANALOG_INPUT), F(""));
  char _number[13];

  sprintf(_number, "%d", configuration.gpio);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "g", "GPIO", _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY AFE_FORM_ITEM_SKIP_PROPERTY
                       AFE_FORM_ITEM_SKIP_PROPERTY,
                   "?");

  sprintf(_number, "%d", configuration.interval);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "v", L_MEASURMENTS_INTERVAL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "1", "86400", "1",
                   L_SECONDS);

  sprintf(_number, "%d", configuration.numberOfSamples);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "n",
                   L_ADC_NUMBER_OF_SAMPLES, _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "1", "999", "1");

  closeSection(page);

  openSection(page, F(L_ADC_VOLTAGE_DIVIDER), F(""));

  sprintf(_number, "%-.2f", (float)configuration.maxVCC);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "m", L_ADC_MEASURED_VOLTAGE,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "1000", "0.01",
                   "V");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Ra);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "ra", "R[a]", _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "90000000", "1", "Om");

  sprintf(_number, "%d", (uint32_t)configuration.divider.Rb);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "rb", "R[b]", _number,
                   AFE_FORM_ITEM_SKIP_PROPERTY, "0", "90000000", "1", "Om");
  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char _idx[7];
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
    sprintf(_idx, "%d", configuration.domoticz.raw);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x0", L_ADC_IDX_RAW_DATA,
                     _idx, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    sprintf(_idx, "%d", configuration.domoticz.percent);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x1", L_ADC_IDX_PERCENT,
                     _idx, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    sprintf(_idx, "%d", configuration.domoticz.voltage);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x2", L_ADC_IDX_VOLTAGE,
                     _idx, AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    sprintf(_idx, "%d", configuration.domoticz.voltageCalculated);
    addInputFormItem(
        page, AFE_FORM_ITEM_TYPE_NUMBER, "x3", L_ADC_IDX_VOLTAGE_CALCULATED,
        _idx, AFE_FORM_ITEM_SKIP_PROPERTY, AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
        AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, F(L_ADC_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  openSection(page, F(L_BATTERY_METER), F(""));

  sprintf(_number, "%-.3f", configuration.battery.minVoltage);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "lv", L_BATTERY_MIN_LEVEL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "1000", "0.001");

  sprintf(_number, "%-.3f", configuration.battery.maxVoltage);
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "hv", L_BATTERY_MAX_LEVEL,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "1000", "0.001");

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
    sprintf(_idx, "%d", configuration.battery.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "bx", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
  if (Device->configuration.api.mqtt) {
    openSection(page, F(L_BATTERY_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "bt", L_MQTT_TOPIC,
                     configuration.battery.mqtt.topic, "64");
    closeSection(page);
  }
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_UART
void AFESitesGenerator::siteUARTBUS(String &page) {
  SERIALPORT configuration;
  Data->getConfiguration(&configuration);
  openSection(page, "UART", "");
  page.concat("<fieldset>");
  addListOfGPIOs(page, "r", configuration.RXD, "GPIO RXD");
  addListOfGPIOs(page, "t", configuration.TXD, "GPIO TXD");
  closeSection(page);
}
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFESitesGenerator::siteI2CBUS(String &page) {
  I2CPORT configuration;
  Data->getConfiguration(&configuration);
  openSection(page, F("I2C"), F(""));
  addListOfGPIOs(page, "a", configuration.SDA, "GPIO SDA");
  addListOfGPIOs(page, "l", configuration.SCL, "GPIO SCL");
  closeSection(page);
}
#endif // AFE_CONFIG_HARDWARE_I2C

void AFESitesGenerator::siteUpgrade(String &page) {
  openSection(page, F(L_FIRMWARE_UPGRADE), F(L_UPGRADE_DONT_PLUG_OFF));
  page.concat(FPSTR(HTTP_SITE_UPGRADE));
  page.replace("{{L_UPGRADE_SELECT_FIRMWARE}}", F(L_UPGRADE_SELECT_FIRMWARE));
  page.replace("{{L_UPGRADE_INFO}}", F(L_UPGRADE_INFO));
  page.replace("{{L_UPGRADE}}", F(L_UPGRADE));
  closeSection(page);
}

void AFESitesGenerator::sitePostUpgrade(String &page, boolean status) {
  openSection(page, F(L_FIRMWARE_UPGRADE), F(""));
  page.concat(F("<ul>"));
  if (status) {
    page.concat(F("<li style=\"color:red\">"));
    page.concat(F(L_UPGRADE_FAILED));
  } else {
    page.concat(F("<li>"));
    page.concat(F(L_UPGRADE_SUCCESSFUL));
  }
  page.concat(F("</li><li>"));
  page.concat(F(L_UPGRADE_DEVICE_WILL_BE_REBOOTED));
  page.concat(F("...</li>"));
  closeSection(page);
}

void AFESitesGenerator::siteReset(String &page) {
  openSection(page, F(L_UPGRADE_RESTORING_DEFAULT_SETTING), F(""));
  page.concat(FPSTR(HTTP_SITE_RESET_TO_DEFAULTS));
  page.replace("{{L_UPGRADE_WARNING}}", F(L_UPGRADE_WARNING));
  page.replace("{{L_UPGRADE_CONFIGURATION_WILL_BE_REMOVED}}",
               F(L_UPGRADE_CONFIGURATION_WILL_BE_REMOVED));
  page.replace("{{L_UPGRADE_RESTORE_DEFAULT_SETTINGS}}",
               F(L_UPGRADE_RESTORE_DEFAULT_SETTINGS));
  closeSection(page);
}

void AFESitesGenerator::sitePostReset(String &page) {
  openSection(page, F(L_UPGRADE_RESTORING_DEFAULT_SETTING), F(""));
  page.concat(FPSTR(HTTP_SITE_POST_RESET));
  page.replace("{{L_UPGRADE_IN_PROGRESS}}", F(L_UPGRADE_IN_PROGRESS));
  page.replace("{{L_UPGRADE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE}}",
               F(L_UPGRADE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE));
  closeSection(page);
}

void AFESitesGenerator::siteExit(String &page, uint8_t command) {
  openSection(page, F(L_UPGRADE_REBOOT), F(""));
  page.concat(F("<fieldset><div class=\"cf\"><ul><li>"));
  page.concat(F(L_UPGRADE_REBOOT_IN_PROGRESS));
  page.concat(F("</li><li>"));
  if (command != AFE_MODE_ACCESS_POINT) {
    page.concat(F(L_UPGRADE_SITE_WILL_BE_RELOADED));
  } else {
    page.concat(F(L_UPGRADE_NETWORK_CONNECT_TO_HOTSPOT_AFTER_UPGRADE));
    page.concat(
        F(": <a href=\"http://192.168.5.1\">http://192.168.5.1</a></p>"));
  }
  page.concat(F("</li></ul></div></fieldset></div>"));
}

void AFESitesGenerator::siteIndex(String &page, boolean authorized) {
  DEVICE configuration;
  configuration = Device->configuration;
  openSection(page, F(L_INDEX_LAUNCH_CONFIGURATION_PANEL), F(""));
  if (!authorized) {
    page.concat(F("<h3>"));
    page.concat(F(L_INDEX_WRONG_PASSWORD));
    page.concat(F("</h3>"));
  }
  page.concat(
      F("<fieldset><form method=\"post\"><div class=\"cf\"><input name=\"p\" "
        "type=\"password\" "
        "placeholder=\""));
  page.concat(F(L_PASSWORD));
  page.concat(
      F("\"></div><div class=\"cf\"><input type=\"submit\" class=\"b bs\" "
        "value=\""));
  page.concat(F(L_INDEX_NORMAL_MODE));
  page.concat(F("\" formaction=\"/?o=0&i="));
  page += AFE_MODE_CONFIGURATION;
  page.concat(F("\" /> <input type=\"submit\" class=\"b be\" value=\""));
  page.concat(F(L_INDEX_HOTSPOT_MODE));
  page.concat(F("\" formaction=\"/?o=0&i="));
  page += AFE_MODE_ACCESS_POINT;
  page.concat(F("\" /></div></form></fieldset></div>"));
}

void AFESitesGenerator::siteProKey(String &page) {
  PRO_VERSION configuration;
  Data->getConfiguration(&configuration);
  openSection(page, F(L_PRO_VERSION), F(""));
  if (Device->getMode() == AFE_MODE_CONFIGURATION) {
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "k", L_PRO_KEY,
                     configuration.serial, "18");
    page.concat(F("<div class=\"cf\"><label>"));
    page.concat(F(L_PRO_VALID));
    page.concat(F("?</label><span>"));
    page.concat(configuration.valid ? F(L_YES) : F(L_NO));
    page.concat(F("</span></div><input name=\"v\" type=\"hidden\" value=\""));
    page += configuration.valid;
    page.concat(F("\">"));
  } else {
    page.concat(F("<h3>"));
    page.concat(F(L_PRO_CANNOT_BE_CONFIGURED));
    page.concat(F("</h3>"));
  }
  closeSection(page);
}

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFESitesGenerator::siteBinarySensor(String &page, uint8_t id) {
  BINARY_SENSOR configuration;
  Data->getConfiguration(id, &configuration);
  char text[19];

  sprintf(text, "%s #%d", L_BINARY_SENSOR, id + 1);

  openSection(page, text, F(""));

  /* Item: name of the sensor */
  addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "n", L_NAME,
                   configuration.name, "16");

  /* Item: send as switch*/
  addCheckboxFormItem(page, "ss", L_BINARY_SEND_AS_SWITCH, "1",
                      configuration.sendAsSwitch, L_BINARY_SEND_AS_SWITCH_HINT);

  closeSection(page);

  openSection(page, F(L_MCP23017_CONNECTION),
              F(L_BINARY_SENSOR_MCP23017_CONNECTION));

  /* Item: GPIO */
  addListOfGPIOs(page, "g", configuration.gpio);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  /* Item: GPIO from expander */
  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_MCP23017_CONNECTION_VIA_MCP));
  addDeviceI2CAddressSelectionItem(page, configuration.mcp23017.address);
  addListOfMCP23017GPIOs(page, "mg", configuration.mcp23017.gpio);
#endif // AFE_CONFIG_HARDWARE_MCP23017

  closeSection(page);

  openSection(page, F(L_NETWORK_ADVANCED), F(""));

  /* Item: internal pull-up */
  addCheckboxFormItem(page, "pr", L_BINARY_PULLUP_RESISTOR, "1",
                      configuration.internalPullUp);

  /* Item: revert signal */
  addCheckboxFormItem(page, "rs", L_BINARY_SENSOR_SENT_REVERTED_STATE, "1",
                      configuration.revertSignal);

  /* Item: Bouncing */
  page.concat(FPSTR(HTTP_INFO_TEXT));
  page.replace("{{item.value}}", F(L_SWITCH_SENSITIVENESS_HINT));

  char _number[4];
  sprintf(_number, "%d", configuration.bouncing);

  addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "b", L_SENSITIVENESS,
                   _number, AFE_FORM_ITEM_SKIP_PROPERTY, "0", "999", "1",
                   L_MILISECONDS);

  closeSection(page);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (Device->configuration.api.domoticz || Device->configuration.api.mqtt) {
    openSection(page, F("Domoticz"), F(L_DOMOTICZ_NO_IF_IDX_0));
    char _idx[7];
    sprintf(_idx, "%d", configuration.domoticz.idx);
    addInputFormItem(page, AFE_FORM_ITEM_TYPE_NUMBER, "x", "IDX", _idx,
                     AFE_FORM_ITEM_SKIP_PROPERTY,
                     AFE_DOMOTICZ_IDX_MIN_FORM_DEFAULT,
                     AFE_DOMOTICZ_IDX_MAX_FORM_DEFAULT, "1");
    closeSection(page);
  }
#else
    if (Device->configuration.api.mqtt) {
      openSection(page, F(L_BINARY_SENSOR_MQTT_TOPIC), F(L_MQTT_TOPIC_EMPTY));
      addInputFormItem(page, AFE_FORM_ITEM_TYPE_TEXT, "t", L_MQTT_TOPIC,
                       configuration.mqtt.topic, "64");
      closeSection(page);
    }
#endif
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

void AFESitesGenerator::generateFooter(String &page, boolean extended) {
  if (Device->getMode() == AFE_MODE_NORMAL) {
    page.concat(FPSTR(HTTP_FOOTER_CONNECTED));
  }

  page.concat(F("</div></div>"));

  if (extended) {
    page.concat(FPSTR(HTTP_FOOTER_EXTENDED));
    page.replace("{{L_HELP}}", F(L_HELP));
    page.replace("{{L_DOCUMENTATION}}", F(L_DOCUMENTATION));
    page.replace("{{L_VERSION}}", F(L_VERSION));
    page.replace("{{f.deviceName}}", F(AFE_DEVICE_TYPE_NAME));
    page.replace("{{f.deviceID}}", String(AFE_DEVICE_TYPE_ID));
    page.replace("{{freeHeap}}", String(system_get_free_heap_size() / 1024));

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    page.replace("{{f.API}}", F("Domoticz"));
#else
    page.replace("{{f.API}}", F("Standard"));
#endif

#ifdef ESP_4MB
    page.replace("{{f.size}}", F("4Mb"));
#else
    page.replace("{{f.size}}", F("1Mb"));
#endif

    FirmwarePro->Pro.valid ? page.replace("{{f.Pro}}", F(L_YES))
                           : page.replace("{{f.Pro}}", F(L_NO));
  }

#ifdef AFE_CONFIG_USE_MAX_HARDWARE
  char _version[sizeof(Firmware.version) + 6];
  sprintf(_version, "%s %s", Firmware.version, "MEGA");
  page.replace("{{f.version}}", _version);
#else
  page.replace("{{f.version}}", Firmware.version);
#endif
  page.replace("{{f.type}}", String(Firmware.type));

  page.concat(F("</body></html>"));
}

void AFESitesGenerator::openSection(String &page, const char *title,
                                    const __FlashStringHelper *description) {
  page.concat(FPSTR(HTTP_FORM_BLOCK_HEADER));
  page.replace("{{title}}", title);
  page.replace("{{description}}", description);
}

void AFESitesGenerator::openSection(String &page,
                                    const __FlashStringHelper *title,
                                    const __FlashStringHelper *description) {
  page.concat(FPSTR(HTTP_FORM_BLOCK_HEADER));
  page.replace("{{title}}", title);
  page.replace("{{description}}", description);
}

void AFESitesGenerator::addListOfHardwareItem(String &page, uint8_t noOfItems,
                                              uint8_t noOffConnected,
                                              const char *field,
                                              const char *label,
                                              boolean disabled) {
  page.concat(F("<div class=\"cf\"><label>"));
  page.concat(label);
  page.concat(F("</label><select name=\""));
  page.concat(field);
  page.concat(F("\""));
  if (disabled) {
    page.concat(F(" disabled=\"disabled\""));
  }
  page.concat(F("><option value=\"0\""));
  page.concat((noOffConnected == 0 ? F(" selected=\"selected\"") : F("")));
  page.concat(F(">"));
  page.concat(F(L_NONE));
  page.concat(F("</option>"));
  if (!disabled) {
    for (uint8_t i = 1; i < (noOfItems + 1); i++) {
      page.concat(F("<option value=\""));
      page.concat(i);
      page.concat(F("\""));
      page.concat(noOffConnected == i ? F(" selected=\"selected\"") : F(""));
      page.concat(F(">"));
      page.concat(i);
      page.concat(F("</option>"));
    }
  }
  page.concat(F("</select>"));
  if (disabled) {
    page.concat(F("<span class=\"hint\">("));
    page.concat(F(L_PRO_VERSION));
    page.concat(F(")</span>"));
  }
  page.concat(F("</div>"));
}

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFESitesGenerator::addRegulatorControllerItem(String &page,
                                                   REGULATOR *configuration) {

  char _value[20];

  openSection(page, L_REGULATOR_REGULATION, "");
  page.concat(FPSTR(HTTP_ITEM_REGULATOR));
  page.replace("{{L_REGULATOR_TURN_IF}}", F(L_REGULATOR_TURN_ON_IF));
  page.replace("{{item.selected-0}}",
               configuration->turnOnAbove == 0 ? " selected=\"selected\"" : "");
  page.replace("{{item.selected-1}}",
               configuration->turnOnAbove == 1 ? " selected=\"selected\"" : "");
  sprintf(_value, "%-.4f", configuration->turnOn);
  page.replace("{{item.value}}", _value);
  page.replace("{{item.name}}", "ta");
  page.replace("{{item.input.name}}", "on");

  page.concat(FPSTR(HTTP_ITEM_REGULATOR));
  page.replace("{{L_REGULATOR_TURN_IF}}", F(L_REGULATOR_TURN_OFF_IF));
  page.replace("{{item.selected-0}}", configuration->turnOffAbove == 0
                                          ? " selected=\"selected\""
                                          : "");
  page.replace("{{item.selected-1}}", configuration->turnOffAbove == 1
                                          ? " selected=\"selected\""
                                          : "");
  sprintf(_value, "%-.4f", configuration->turnOff);
  page.replace("{{item.value}}", _value);
  page.replace("{{item.name}}", "tb");
  page.replace("{{item.input.name}}", "off");

  page.replace("{{L_REGULATOR_LOWER}}", F(L_REGULATOR_LOWER));
  page.replace("{{L_REGULATOR_HIGHER}}", F(L_REGULATOR_HIGHER));

  closeSection(page);
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

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
  item.replace("{{item.label}}", F(L_NONE));
  item.replace("{{item.selected}}", selected == AFE_HARDWARE_ITEM_NOT_EXIST
                                        ? F(" selected=\"selected\"")
                                        : F(""));

  for (uint8_t i = 0; i < AFE_NUMBER_OF_GPIOS; i++) {
    item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));

    item.replace("{{item.value}}", String(pgm_read_byte(GPIOS + i)));
    item.replace("{{item.label}}", String(pgm_read_byte(GPIOS + i)));
    item.replace("{{item.selected}}", selected == pgm_read_byte(GPIOS + i)
                                          ? F(" selected=\"selected\"")
                                          : F(""));
  }
  item.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));
}

#ifdef AFE_CONFIG_HARDWARE_MCP23017
void AFESitesGenerator::addListOfMCP23017GPIOs(String &item, const char *field,
                                               uint8_t selected,
                                               const char *title) {

  item.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  item.replace("{{item.name}}", field);
  item.replace("{{item.label}}", title);
  item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  item.replace("{{item.value}}", String(AFE_HARDWARE_ITEM_NOT_EXIST));
  item.replace("{{item.label}}", F(L_NONE));
  item.replace("{{item.selected}}", selected == AFE_HARDWARE_ITEM_NOT_EXIST
                                        ? F(" selected=\"selected\"")
                                        : F(""));
  char gpioName[3];

  for (uint8_t i = 0; i < AFE_NUMBER_OF_MCP23017_GPIOS; i++) {

    sprintf(gpioName, "%s%d", i < 8 ? "A" : "B",
            pgm_read_byte(MCP23017_GPIOS_ID + i) - (i < 8 ? 0 : 8));
    item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
    item.replace("{{item.value}}",
                 String(pgm_read_byte(MCP23017_GPIOS_ID + i)));
    item.replace("{{item.label}}", gpioName);
    item.replace("{{item.selected}}",
                 selected == pgm_read_byte(MCP23017_GPIOS_ID + i)
                     ? F(" selected=\"selected\"")
                     : F(""));
  }
  item.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));
}
#endif

void AFESitesGenerator::addInputFormItem(String &item, const char *type,
                                         const char *name, const char *label,
                                         const char *value, const char *size,
                                         const char *min, const char *max,
                                         const char *step, const char *hint,
                                         boolean readonly) {
  item.concat(F("<div class=\"cf\"><label>"));
  item.concat(label);
  item.concat(F("</label><input name=\""));
  item.concat(name);
  item.concat(F("\" type=\""));
  item.concat(type);
  item.concat(F("\" "));
  if (readonly) {
    item.concat(F("readonly=\"readonly\" "));
  }
  if (strcmp(size, "?") != 0) {
    item.concat(F("maxlength=\""));
    item.concat(size);
    item.concat(F("\" "));
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0) {
      item.concat(F("min=\""));
      item.concat(min);
      item.concat(F("\" "));
    }
    if (strcmp(max, "?") != 0) {
      item.concat(F("max=\""));
      item.concat(max);
      item.concat(F("\" "));
    }
    if (strcmp(step, "?") != 0) {
      item.concat(F("step=\""));
      item.concat(step);
      item.concat(F("\" "));
    }
  }
  item.concat(F("value=\""));
  item.concat(value);
  item.concat(F("\">"));
  if (strcmp(size, "?") != 0) {
    item.concat(F("<span class=\"hint\">Max "));
    item.concat(size);
    item.concat(F(" "));
    item.concat(F(L_NUMBER_OF_CHARS));
    item.concat(F("</span>"));
  }
  if (strcmp(type, "number") == 0) {
    if (strcmp(min, "?") != 0 && strcmp(max, "?") != 0) {
      item.concat(F("<span class=\"hint\">"));
      item.concat(F(L_RANGE));
      item.concat(F(" "));
      item.concat(min);
      item.concat(F(" - "));
      item.concat(max);
      if (strcmp(hint, "?") != 0) {
        item.concat(F(" "));
        item.concat(hint);
      }
      item.concat(F("</span>"));
    } else if (strcmp(hint, "?") != 0) {
      item.concat(F("<span class=\"hint\">"));
      item.concat(hint);
      item.concat(F("</span>"));
    }
  }
  item.concat(F("</div>"));
}

void AFESitesGenerator::_addSelectionFormItem(
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
  _addSelectionFormItem(item, true, name, label, value, checked, hint,
                        disabled);
}

void AFESitesGenerator::addRadioButtonFormItem(
    String &item, const char *name, const char *label, const char *value,
    boolean checked, const char *hint, boolean disabled) {
  _addSelectionFormItem(item, false, name, label, value, checked, hint,
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

void AFESitesGenerator::addSelectFormItemOpen(String &item, const char *name,
                                              const char *label) {
  item.concat(FPSTR(HTTP_ITEM_SELECT_OPEN));
  item.replace("{{item.label}}", label);
  item.replace("{{item.name}}", name);
}

void AFESitesGenerator::addSelectOptionFormItem(String &item, const char *label,
                                                const char *value,
                                                boolean selected) {
  item.concat(FPSTR(HTTP_ITEM_SELECT_OPTION));
  item.replace("{{item.value}}", value);
  item.replace("{{item.label}}", label);
  item.replace("{{item.selected}}", selected ? " selected=\"selected\"" : "");
}

void AFESitesGenerator::addSelectFormItemClose(String &item) {
  item.concat(FPSTR(HTTP_ITEM_SELECT_CLOSE));
}

#ifdef AFE_CONFIG_HARDWARE_I2C
void AFESitesGenerator::addDeviceI2CAddressSelectionItem(String &item,
                                                         uint8_t address) {
  AFEI2CScanner I2CScanner;
  I2CScanner.begin();

  addSelectFormItemOpen(item, "a", "I2C " L_ADDRESS);
  addSelectOptionFormItem(item, L_NONE, "0", address == 0);

  char _i2cItemName[90];
  char _number[4];
  for (uint8_t addressToScan = 1; addressToScan < 127; addressToScan++) {
    if (I2CScanner.scan(addressToScan)) {
      sprintf(_i2cItemName, "[0x%X] : %s", addressToScan,
              I2CScanner.getName(addressToScan));
      sprintf(_number, "%d", addressToScan);
      addSelectOptionFormItem(item, _i2cItemName, _number,
                              address == addressToScan);
    }
  }
  addSelectFormItemClose(item);
}
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFESitesGenerator::addGateStatesListItem(String &item, uint8_t id,
                                              byte state) {
  char _gateStateId[3];
  sprintf(_gateStateId, "s%d", id);
  addSelectFormItemOpen(item, _gateStateId, L_GATE_SET_STATE);
  addSelectOptionFormItem(item, L_GATE_OPENED, "0", state == AFE_GATE_OPEN);
  addSelectOptionFormItem(item, L_GATE_PARTIALLY_OPENED, "2",
                          state == AFE_GATE_PARTIALLY_OPEN);
  addSelectOptionFormItem(item, L_GATE_CLOSED, "1", state == AFE_GATE_CLOSED);
  addSelectOptionFormItem(item, L_GATE_UNKNOWN_STATE, "9",
                          state == AFE_GATE_UNKNOWN);
  addSelectFormItemClose(item);
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFESitesGenerator::addGateContactronsListItem(String &item,
                                                   const char *name,
                                                   uint8_t contactronId) {
  char _name[23];
  char _id[4];
  CONTACTRON configuration;

  addSelectFormItemOpen(item, name, L_CONTACTRON);
  sprintf(_id, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(item, L_NONE, _id,
                          contactronId == AFE_HARDWARE_ITEM_NOT_EXIST);

  for (uint8_t i = 0; i < Device->configuration.noOfContactrons; i++) {
    Data->getConfiguration(i, &configuration);

    if (configuration.name[0] == '\0') {
      sprintf(_name, "%d: %s", i + 1, L_CONTACTRON);
    } else {
      sprintf(_name, "%d: %s", i + 1, configuration.name);
    }
    sprintf(_id, "%d", i);
    addSelectOptionFormItem(item, _name, _id, contactronId == i);
  }
  addSelectFormItemClose(item);
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_LED
void AFESitesGenerator::addLEDSelectionItem(String &item, uint8_t id) {
  char _id[4];
  char _label[4];
  addSelectFormItemOpen(item, "l", "LED");
  sprintf(_id, "%d", AFE_HARDWARE_ITEM_NOT_EXIST);
  addSelectOptionFormItem(item, L_NONE, _id, id == AFE_HARDWARE_ITEM_NOT_EXIST);

  for (uint8_t i = 0; i < Device->configuration.noOfLEDs; i++) {
    sprintf(_label, "%d", i + 1);
    sprintf(_id, "%d", i);
    addSelectOptionFormItem(item, _label, _id, id == i);
  }
  addSelectFormItemClose(item);
}
#endif // AFE_CONFIG_HARDWARE_LED
