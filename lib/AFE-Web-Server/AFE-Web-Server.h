/*
  WebServer component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Web_Server_h
#define _AFE_Web_Server_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <ESP8266WebServer.h>
#include <Streaming.h>

class AFEWebServer {

private:
	ESP8266WebServer server;

public:
  typedef std::function<void(void)> THandlerFunction;
	/* Constructor: entry parameter is GPIO number where Sensor is connected to */
	AFEWebServer();
	void begin();
  void listener();
	void generatePage(const String &page);
  void handle(const char* uri,ESP8266WebServer::THandlerFunction handler);

};

#endif
