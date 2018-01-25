/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

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
#include <ESP32WebServer.h>
//#include <Streaming.h>

class AFEWebServer {

private:
  ESP32WebServer server;
  AFEConfigurationPanel ConfigurationPanel;
  ESP8266HTTPUpdateServer httpUpdater; // Class used for firmware upgrade
  AFEDevice Device;
  HTTPCOMMAND httpCommand; // It stores last HTTP API request
  boolean receivedHTTPCommand =
      false; // Once HTTP API requet is recieved it's set to true

  /* Method pushes HTML site from WebServer */
  void publishHTML(String page);

  /* Method gets url Option parameter value */
  String getOptionName();

  /* Method gets url cmd parameter value */
  uint8_t getCommand();

  /* Methods get POST data (for saveing) */
  DEVICE getDeviceData();
  NETWORK getNetworkData();
  MQTT getMQTTData();
  RELAY getRelayData(uint8_t id);
  SWITCH getSwitchData(uint8_t id);
  LED getLEDData(uint8_t id);
  uint8_t getLanguageData();

  // @TODO DOMOTICZ getDomoticzData();
  // @TODO DS18B20 getDS18B20Data();

public:
  AFEWebServer();

  /* Method initialize WebServer and Updater server */
  void begin();

  /* Method listens for HTTP requests */
  void listener();

  /* Method adds URL for listen */
  void handle(const char *uri, ESP32WebServer::THandlerFunction handler);

  /* Method generate HTML side. It reads also data from HTTP requests arguments
   * and pass them to Configuration Panel class */
  void generate();

  /* Method listens for HTTP API requests. If get True command is in httpCommand
   */
  boolean httpAPIlistener();

  /* Method reads HTTP API Command */
  HTTPCOMMAND getHTTPCommand();

  /* Method pushes JSON response to HTTP API request */
  void sendJSON(String json);
};

#endif
