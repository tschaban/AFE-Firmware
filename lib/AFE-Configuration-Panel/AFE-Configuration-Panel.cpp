#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {

#ifdef DEBUG
  Serial << endl
         << endl
         << "#### WARNING: This should be visible only during the upgrade."
         << endl
         << "Init: ConfigurationPanel";
#endif

  AFEDevice _Device;
  _Device.begin();
  begin(&_Device);
}

AFEConfigurationPanel::AFEConfigurationPanel(AFEDevice *_Device) {
  begin(_Device);
}

void AFEConfigurationPanel::begin(AFEDevice *_Device) {
  Device = _Device;
  Site.begin(Device);
}

String AFEConfigurationPanel::getFirstLaunchConfigurationSite(uint8_t command,
                                                              NETWORK data) {
  if (command == SERVER_CMD_SAVE) {
#ifdef DEBUG
    Serial << endl << "Saving network parameters";
#endif
  }
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateOneColumnLayout();
  page += "<form action=\"/?option=start&cmd=1\" method=\"post\">";
  page += Site.addFirstLaunchConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += L_CONNECT;
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getConnectingSite() {
  String page;
  NETWORK data;

  page.reserve(siteBufferSize);
  page = Site.generateOneColumnLayout();
  page += Site.addConnectingSite();
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command) {

  String page;

  if (option == "upgrade") {
    page += Site.generateTwoColumnsLayout(0);
    page += Site.addUpgradeSection();
  } else if (option == "reset") {
    page += Site.generateOneColumnLayout(10);
    page += Site.addResetSection();
  } else if (option == "exit") {
    page += Site.generateOneColumnLayout(10);
    page += Site.addExitSection(command);
  } else {
    page += "<h1>Page Not Found</h1>";
  }

  page +=
      Site.generateFooter((option == "index" && command == 0) ? true : false);
  delay(10);
  return page;
}

String AFEConfigurationPanel::getIndexSite(boolean authorized) {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateOneColumnLayout(0);
  page += "<form method=\"post\">";
  page += Site.addIndexSection(authorized);
  page += "</form>";

#ifdef DEBUG
  Serial << endl << "Index page launch in the mode: " << Device->getMode();
#endif

  page += Site.generateFooter((Device->getMode() == MODE_NORMAL ||
                               Device->getMode() == MODE_CONFIGURATION)
                                  ? true
                                  : false);
  return page;
}

String AFEConfigurationPanel::getDeviceConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=device&cmd=1\"  method=\"post\">";
  page += Site.addDeviceConfiguration();
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getNetworkConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=network&cmd=1\"  method=\"post\">";
  page += Site.addNetworkConfiguration();
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=mqtt&cmd=1\"  method=\"post\">";
  page += Site.addMQTTBrokerConfiguration();
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getDomoticzServerConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=domoticz&cmd=1\" method=\"post\">";
  page += Site.addDomoticzServerConfiguration();
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getPasswordConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=password&cmd=1\" method=\"post\">";
  page += Site.addPasswordConfigurationSite();
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getProVersionConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=pro&cmd=1\" method=\"post\">";
  page += Site.addProVersionSite();
  page += generateFooter();
  return page;
}

#ifdef CONFIG_HARDWARE_LED
String AFEConfigurationPanel::getLEDConfigurationSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=led&cmd=1\"  method=\"post\">";
  for (uint8_t i = 0; i < sizeof(Device->configuration.isLED); i++) {
    if (Device->configuration.isLED[i]) {
      page += Site.addLEDConfiguration(i);
    } else {
      break;
    }
  }
  page += Site.addSystemLEDConfiguration();
  page += generateFooter();
  return page;
}
#endif

String AFEConfigurationPanel::getRelayConfigurationSite(uint8_t relayIndex) {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=relay";
  page += relayIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addRelayConfiguration(relayIndex);
  page += generateFooter();
  return page;
}

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
String
AFEConfigurationPanel::getRelayStatConfigurationSite(uint8_t regulatorType) {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=";
  page += regulatorType == THERMOSTAT_REGULATOR ? "thermostat" : "humidistat";
  page += "&cmd=1\"  method=\"post\">";

  regulatorType == THERMOSTAT_REGULATOR
      ? page += Site.addRegulatorConfiguration(THERMOSTAT_REGULATOR)
      : page += Site.addRegulatorConfiguration(HUMIDISTAT_REGULATOR);
  page += generateFooter();
  return page;
}
#endif

String AFEConfigurationPanel::getSwitchConfigurationSite(uint8_t switchIndex) {

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=switch";
  page += switchIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addSwitchConfiguration(switchIndex);
  page += generateFooter();
  return page;
}

/* DS18B20 and DHxx Sensors */
#ifdef CONFIG_HARDWARE_DS18B20
String AFEConfigurationPanel::getDS18B20ConfigurationSite(uint8_t command,
                                                          DS18B20 data)

#endif

#ifdef CONFIG_HARDWARE_DHXX
    String AFEConfigurationPanel::getDHTConfigurationSite(uint8_t command,
                                                          DH data)
#endif

#if defined(CONFIG_HARDWARE_DS18B20) || defined(CONFIG_HARDWARE_DHXX)
{
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();

#ifdef CONFIG_HARDWARE_DS18B20
  page += "<form action=\"/?option=ds18b20&cmd=1\"  method=\"post\">";
  page += Site.addDS18B20Configuration();
#else
  page += "<form action=\"/?option=DHT&cmd=1\"  method=\"post\">";
  page += Site.addDHTConfiguration();
#endif
  page += generateFooter();
  return page;
}
#endif

#if defined(T3_CONFIG)
String AFEConfigurationPanel::getPIRConfigurationSite(const String option,
                                                      uint8_t command, PIR data,
                                                      uint8_t PIRIndex) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(PIRIndex, data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=pir";
  page += PIRIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addPIRConfiguration(PIRIndex);
  page += generateFooter();
  return page;
}

#endif

#if defined(T5_CONFIG)
String AFEConfigurationPanel::getContactronConfigurationSite(
    const String option, uint8_t command, CONTACTRON data,
    uint8_t contactronIndex) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(contactronIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=contactron";
  page += contactronIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addContactronConfiguration(contactronIndex);
  page += generateFooter();
  return page;
}

String AFEConfigurationPanel::getGateConfigurationSite(const String option,
                                                       uint8_t command,
                                                       GATE data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=gate&cmd=1\"  method=\"post\">";
  page += Site.addGateConfiguration();
  page += generateFooter();
  return page;
}
#endif

#ifdef CONFIG_HARDWARE_UART
String AFEConfigurationPanel::getSerialPortConfigurationSite(uint8_t command,
                                                             SERIALPORT data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=UART&cmd=1\"  method=\"post\">";
  page += Site.addSerialPortConfiguration();
  page += generateFooter();
  return page;
}
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
String
AFEConfigurationPanel::getHPMA115S0SensorConfigurationSite(uint8_t command,
                                                           HPMA115S0 data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=HPMA115S0&cmd=1\"  method=\"post\">";
  page += Site.addHPMA115S0Configuration();
  page += generateFooter();
  return page;
}
#endif

#ifdef CONFIG_HARDWARE_BMX80
String AFEConfigurationPanel::getBMx80SensorConfigurationSite(uint8_t command,
                                                              BMx80 data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=BMx80&cmd=1\"  method=\"post\">";
  page += Site.addBMx80Configuration();
  page += generateFooter();
  return page;
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
String AFEConfigurationPanel::getBH1750SensorConfigurationSite(uint8_t command,
                                                               BH1750 data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=BH1750&cmd=1\"  method=\"post\">";
  page += Site.addBH1750Configuration();
  page += generateFooter();
  return page;
}
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
String AFEConfigurationPanel::getAnalogInputConfigurationSite(uint8_t command,
                                                              ADCINPUT data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateTwoColumnsLayout();
  page += "<form action=\"/?option=analogInput&cmd=1\"  method=\"post\">";
  page += Site.addAnalogInputConfiguration();

  return page;
}
#endif

String AFEConfigurationPanel::firmwareUpgradeSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateTwoColumnsLayout();
  page += "<form method=\"post\" action=\"\" "
          "enctype=\"multipart/form-data\">";
  page += Site.addUpgradeSection();
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::postFirmwareUpgradeSite(boolean status) {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateOneColumnLayout(15);
  page += Site.addPostUpgradeSection(status);
  page += Site.generateFooter();
  return page;
}

const String AFEConfigurationPanel::generateFooter() {
  String body = "<input type=\"submit\" class=\"b bs\" value=\"";
  body += L_SAVE;
  body += "\"></form>";
  body += Site.generateFooter();
  return body;
}
