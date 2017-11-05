#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {}

String AFEConfigurationPanel::getSite(const String option, uint8_t command) {
  Serial << endl << "INFO: Generating help site with command = " << command;
  String page;
  AFEDevice Device;

  command == 0 ? page = Site.generateHeader(0) : page = Site.generateHeader(10);

  if (command == 0) {
    page += Site.addHelpSection();
    page += Site.addHelpMQTTTopicSection();
    page += Site.addDonationSection();
  } else if (command == 1 || command == 2) {
    page += Site.addExitSection();
  }

  page += Site.generateFooter();

  Serial << endl << "INFO: Site generated";
  return page;
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      boolean data) {

  Serial << endl << "INFO: Generating site";

  String page = Site.generateHeader();

  if (option == "upgrade") {
    page += Site.addUpgradeSection();
  } else if (option == "reset") {
    page += Site.addResetSection();
  } else if (option == "exit") {
    page += Site.addExitSection();
  } else {
    page += "<h1>Page Not Found</h1>";
  }
  page += Site.generateFooter();

  Serial << endl << "INFO: Site generated";
  return page;
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      NETWORK data) {

  Serial << endl << "INFO: Generating Network configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=basic&command=1\"  method=\"post\">";
  page += Site.addNetworkConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

/* @TODO Only for MQTT */
String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      MQTT data) {

  Serial << endl << "INFO: Generating MQTT configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=mqtt&command=1\"  method=\"post\">";
  page += Site.addMQTTBrokerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

/* @TODO Only for Domoticz */
String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      DOMOTICZ data) {

  Serial << endl << "INFO: Generating Domoticz configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=domoticz&command=1\"  method=\"post\">";
  page += Site.addDomoticzConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
  page += Site.generateFooter();
  return page;
}
//
String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      RELAY data1, RELAY data2) {

  Serial << endl << "INFO: Generating Relay configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(0, data1);
    //    save.saveConfiguration(1, data2);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=relay&command=1\"  method=\"post\">";
  page += Site.addRelayConfiguration(0);
  //  page += Site.addRelayConfiguration(1);
  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      SWITCH data1, SWITCH data2) {

  Serial << endl << "INFO: Generating Switch configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(0, data1);
    //  save.saveConfiguration(1, data2);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=switch&command=1\"  method=\"post\">";

  page += Site.addSwitchConfiguration(0);
  //  page += Site.addSwitchConfiguration(1);

  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      DS18B20 data) {

  Serial << endl << "INFO: Generating DS18B20 configuration site";

  if (command == SERVER_CMD_SAVE) {
    AFEDataAccess save;
    save.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=ds18b20&command=1\"  method=\"post\">";
  page += Site.addDS18B20Configuration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"Zapisz\">";
  page += "</form>";
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
