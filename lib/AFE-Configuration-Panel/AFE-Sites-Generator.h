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

#include <Streaming.h>
#include <AFE-EEPROM.h>
#include <AFE-Data-Structures.h>
#include <AFE-Data-Access.h>

class AFESitesGenerator {

private:
	AFEEEPROM Eeprom;
	AFEDataAccess Data;

public:
	/* Constructor: entry parameter is GPIO number where Sensor is connected to */
	AFESitesGenerator();
	String generateHTMLPage(String &page);
	String addConfigurationBlock(const String title,const String description, const String body);
	const String generateHeader();
	const char* generateFooter();
	const String  generateConfigParameter_GPIO(const char* field, uint8_t selected);

  String addNetworkConfiguration();
	String addMQTTBrokerConfiguration();
	String addDomoticzConfiguration();
  String addRelayConfiguration(uint8_t id);
	String addSwitchConfiguration(uint8_t id);
	String addDS18B20Configuration();


  String addUpgradeSection();
	String addResetSection();
	String addExitSection();


};

#endif
