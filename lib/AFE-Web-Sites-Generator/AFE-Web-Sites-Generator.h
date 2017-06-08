/*
  WebSites generator for configuration Panel
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Web_Sites_Generator_h
#define _AFE_Web_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Streaming.h>

class AFEWebSites {

private:
	String addMenuItem(const String url, const String name);
	String addLabel(uint8_t id);


public:
	/* Constructor: entry parameter is GPIO number where Sensor is connected to */
	AFEWebSites();
	String generateHTMLPage(String &page);
	String addConfigurationBlock(const String title,const String description, const String body, uint8_t labels[]);
	String generateHeader();
	String generateLeftPanel();
	String generateRightPanel();
	String generateFooter();
	String generateConfigParameter_GPIO(const char* label, const char* field, uint8_t selected);

};

#endif
