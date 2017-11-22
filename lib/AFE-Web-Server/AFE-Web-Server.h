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

#include <AFE-Configuration-Panel.h>
#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-OTA.h>
#include <ESP8266WebServer.h>
#include <Streaming.h>

class AFEWebServer {

private:
  ESP8266WebServer server;
  AFEConfigurationPanel ConfigurationPanel;
  ESP8266HTTPUpdateServer httpUpdater;
  AFEDevice Device;
  HTTPCOMMAND httpCommand;
  boolean receivedHTTPCommand = false;

  void publishHTML(String page);
  String getOptionName();
  uint8_t getCommand();

  NETWORK getNetworkData();
  MQTT getMQTTData();
  RELAY getRelayData(uint8_t id);
  SWITCH getSwitchData(uint8_t id);
  uint8_t getLanguageData();
  boolean getHTTPAPIData();
  // @TODO DOMOTICZ getDomoticzData();
  // @TODO DS18B20 getDS18B20Data();

public:
  AFEWebServer();

  /* Method initialize WebServer and Updater server */
  void begin();

  /* Method listens for HTTP requests */
  void listener();

  boolean httpAPIlistener();

  /* Method adds URL for listen */
  void handle(const char *uri, ESP8266WebServer::THandlerFunction handler);

  /* Method generate HTML side. It reads also data from HTTP requests arguments
   * and pass them to Configuration Panel class */
  void generate();

  HTTPCOMMAND getHTTPCommand();
  void sendJSON(String json);
};

#endif
