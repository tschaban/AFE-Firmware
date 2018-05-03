#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
  language = Data.getLanguage();
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      boolean redirect) {

  String page;
  redirect ? page = Site.generateHeader(10) : page = Site.generateHeader(0);

  if (option == "upgrade") {
    page += Site.addUpgradeSection();
  } else if (option == "reset") {
    page += Site.addResetSection(command);
  } else if (option == "exit") {
    page += Site.addExitSection();
  } else if (option == "help") {
    if (command == 0) {
      page += Site.addHelpSection();
    } else if (command == 1 || command == 2) {
      page += Site.addExitSection();
    }
  } else {
    page += "<h1>Page Not Found</h1>";
  }

  page += Site.generateFooter();
  delay(10);
  return page;
}

String AFEConfigurationPanel::getLanguageConfigurationSite(const String option,
                                                           uint8_t command,
                                                           uint8_t lang) {
  String page;
  if (command == SERVER_CMD_SAVE) {
    Data.saveLanguage(lang);
    page += Site.generateHeader(10);
    page += Site.addExitSection();
  } else {
    page += Site.generateHeader();
    page += "<form action=\"/?option=language&cmd=1\"  method=\"post\">";
    page += Site.addLanguageConfiguration();
    page += "<input type=\"submit\" class=\"b bs\" value=\"";
    page += language == 0 ? "Zapisz" : "Save";
    page += "\"></form>";
  }
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getDeviceConfigurationSite(const String option,
                                                         uint8_t command,
                                                         DEVICE data) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
    Device.begin();
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=device&cmd=1\"  method=\"post\">";
  page += Site.addDeviceConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getNetworkConfigurationSite(const String option,
                                                          uint8_t command,
                                                          NETWORK data) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=network&cmd=1\"  method=\"post\">";
  page += Site.addNetworkConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite(const String option,
                                                       uint8_t command,
                                                       MQTT data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=mqtt&cmd=1\"  method=\"post\">";
  page += Site.addMQTTBrokerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getDomoticzServerConfigurationSite(
    const String option, uint8_t command, DOMOTICZ data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=domoticz&cmd=1\" method=\"post\">";
  page += Site.addDomoticzServerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getLEDConfigurationSite(
    const String option, uint8_t command,
    LED data[sizeof(Device.configuration.isLED)], uint8_t dataLedID) {

  if (command == SERVER_CMD_SAVE) {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
      if (Device.configuration.isLED[i]) {
        Data.saveConfiguration(i, data[i]);
      } else {
        break;
      }
    }
    Data.saveSystemLedID(dataLedID);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=led&cmd=1\"  method=\"post\">";
  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i]) {
      page += Site.addLEDConfiguration(i);
    } else {
      break;
    }
  }
  page += Site.addSystemLEDConfiguration();

  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getRelayConfigurationSite(const String option,
                                                        uint8_t command,
                                                        RELAY data,
                                                        uint8_t relayIndex) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(relayIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=relay";
  page += relayIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addRelayConfiguration(relayIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getRelayStatConfigurationSite(
    const String option, uint8_t command, REGULATOR data, boolean thermostat) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data, thermostat);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=";
  page += thermostat ? "thermostat" : "humidistat";
  page += "&cmd=1\"  method=\"post\">";

  thermostat ? page += Site.addThermostatConfiguration()
             : page += Site.addHumidistatConfiguration();

  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getSwitchConfigurationSite(const String option,
                                                         uint8_t command,
                                                         SWITCH data,
                                                         uint8_t switchIndex) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(switchIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=switch";
  page += switchIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addSwitchConfiguration(switchIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getDHTConfigurationSite(const String option,
                                                      uint8_t command,
                                                      DH data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=DHT&cmd=1\"  method=\"post\">";
  page += Site.addDHTConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::firmwareUpgradeSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
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
  page = Site.generateHeader(15);
  page += Site.addPostUpgradeSection(status);
  page += Site.generateFooter();
  return page;
}
