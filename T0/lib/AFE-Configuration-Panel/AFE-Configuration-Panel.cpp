#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
  language = Data.getLanguage();
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      boolean redirect) {

  String page;
  redirect == 0 ? page = Site.generateHeader(0)
                : page = Site.generateHeader(10);

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
  }

  String page = Site.generateHeader();
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
  String page = Site.generateHeader();
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

  String page = Site.generateHeader();
  page += "<form action=\"/?option=mqtt&cmd=1\"  method=\"post\">";
  page += Site.addMQTTBrokerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getLEDConfigurationSite(const String option,
                                                      uint8_t command,
                                                      LED data) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(0, data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=led&cmd=1\"  method=\"post\">";
  page += Site.addLEDConfiguration(0);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getRelayConfigurationSite(const String option,
                                                        uint8_t command,
                                                        RELAY data1,
                                                        RELAY data2) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(0, data1);
    //    Data.saveConfiguration(1, data2);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=relay&cmd=1\"  method=\"post\">";
  page += Site.addRelayConfiguration(0);
  //  page += Site.addRelayConfiguration(1);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getSwitchConfigurationSite(const String option,
                                                         uint8_t command,
                                                         SWITCH data1,
                                                         SWITCH data2) {

  Device.begin(); // Reading configuration data

  if (command == SERVER_CMD_SAVE) {
    if (Device.configuration.isSwitch[0]) {
      Data.saveConfiguration(0, data1);
    }
    if (Device.configuration.isSwitch[1]) {
      Data.saveConfiguration(1, data2);
    }
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=switch&cmd=1\"  method=\"post\">";

  if (Device.configuration.isSwitch[0]) {
    page += Site.addSwitchConfiguration(0);
  }
  if (Device.configuration.isSwitch[1]) {
    page += Site.addSwitchConfiguration(1);
  }
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::firmwareUpgradeSite() {
  String page = Site.generateHeader();
  page += "<form method=\"post\" action=\"\" "
          "enctype=\"multipart/form-data\">";
  page += Site.addUpgradeSection();
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::postFirmwareUpgradeSite(boolean status) {
  String page = Site.generateHeader(10);
  page += Site.addPostUpgradeSection(status);
  page += Site.generateFooter();
  return page;
}

/* @TODO DOMOTICZ
String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      DOMOTICZ data) {

  Serial << endl << "INFO: Generating Domoticz configuration site";

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=domoticz&cmd=1\"  method=\"post\">";
  page += Site.addDomoticzConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
*/

/* @TODO DS18B20
String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      DS18B20 data) {

  Serial << endl << "INFO: Generating DS18B20 configuration site";

  if (command == SERVER_CMD_SAVE) {    Data.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=ds18b20&cmd=1\"  method=\"post\">";
  page += Site.addDS18B20Configuration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
*/
