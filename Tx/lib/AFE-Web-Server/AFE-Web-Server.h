/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Web_Server_h
#define _AFE_Web_Server_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration-Panel.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-OTA.h>
#include <ESP8266WebServer.h>
//#include <Streaming.h>

class AFEWebServer {

private:
  ESP8266WebServer server;
  AFEConfigurationPanel ConfigurationPanel;
  ESP8266HTTPUpdateServer httpUpdater; // Class used for firmware upgrade
  AFEDevice Device;
  HTTPCOMMAND httpCommand; // It stores last HTTP API request
  boolean receivedHTTPCommand =
      false; // Once HTTP API requet is recieved it's set to true

  boolean _refreshConfiguration = false; // when it's set to true device
                                         // configuration is refreshed. Required
                                         // by generate() method

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
  DOMOTICZ getDomoticzServerData();
  RELAY getRelayData(uint8_t id);
  SWITCH getSwitchData(uint8_t id);

#ifndef T0_SHELLY_1_CONFIG
  LED getLEDData(uint8_t id);
  uint8_t getSystemLEDData();
#endif

#if defined(T1_CONFIG)
  DS18B20 getDS18B20Data();
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  DH getDHTData();
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  REGULATOR getRegulatorData();
#endif

#if defined(T5_CONFIG)
  CONTACTRON getContactronData(uint8_t id);
  GATE getGateData();
#endif

  uint8_t getLanguageData();

public:
  AFEWebServer();

  /* Method initialize WebServer and Updater server */
  void begin();

  /* Method listens for HTTP requests */
  void listener();

  /* Method adds URL for listen */
  void handle(const char *uri, ESP8266WebServer::THandlerFunction handler);

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
