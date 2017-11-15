/*
  WebSites generator for configuration Panel
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-EEPROM.h>
#include <Streaming.h>

class AFESitesGenerator {

private:
  AFEEEPROM Eeprom;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;

  const String generateMQTTHelp(const char *label, const char *topic,
                                const char *command, const char *value);

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESitesGenerator();
  void refreshLanguage();
  String generateHTMLPage(String &page);
  String addConfigurationBlock(const String title, const String description,
                               const String body);
  const String generateHeader(uint8_t redirect = 0);
  const char *generateFooter();
  const String generateConfigParameter_GPIO(const char *field,
                                            uint8_t selected);
  String addLanguageConfiguration();
  String addNetworkConfiguration();
  String addMQTTBrokerConfiguration();
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);
  String addUpgradeSection();
  String addPostUpgradeSection(boolean status);
  String addResetSection(uint8_t command);
  String addExitSection();
  String addHelpSection();
  String addHelpMQTTTopicSection();
  // @TODO DOMOTICZ String addDomoticzConfiguration();
  // @TODO DS18B20 String addDS18B20Configuration();
};

#endif
