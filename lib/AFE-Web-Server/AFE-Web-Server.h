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
#include <AFE-Configuration-Panel.h>
#include <Streaming.h>

#define SERVER_CMD_SAVE 	1

class AFEWebServer {

private:
	ESP8266WebServer server;
	AFEConfigurationPanel ConfigurationPanel;

	void publishHTML(String page);
	String getOptionName();
	boolean isSave();


public:
	AFEWebServer();
	void begin();
  void listener();
	void handle(const char* uri,ESP8266WebServer::THandlerFunction handler);
	void generate();

};

#endif
