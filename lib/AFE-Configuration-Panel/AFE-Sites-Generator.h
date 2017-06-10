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

class AFESitesGenerator {

private:
	AFEEEPROM Eeprom;

public:
	/* Constructor: entry parameter is GPIO number where Sensor is connected to */
	AFESitesGenerator();
	String generateHTMLPage(String &page);
	String addConfigurationBlock(const String title,const String description, const String body,  const String link);
	const char* generateHeader();
	const char* generateFooter();
	const String  generateConfigParameter_GPIO(const char* label, const char* field, uint8_t selected);

  String addDeviceNameConfiguration();
	String addWiFiConfiguration();
  String addRelayConfiguration();
	String addMQTTBrokerConfiguration();
	String addDomoticzConfiguration();
	String addDS18B20Configuration();
	String addSwitchConfiguration();

};

#endif
