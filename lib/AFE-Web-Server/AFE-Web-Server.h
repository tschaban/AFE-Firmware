/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Web_Server_h
#define _AFE_Web_Server_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Sites-Generator.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif


#ifdef DEBUG
#include <Streaming.h>
#endif

struct AFE_SITE_PARAMETERS {
  uint8_t ID;
  boolean twoColumns = true;
  int8_t deviceID = -1;
  boolean reboot = false;
  uint8_t rebootMode = 0;
  uint8_t rebootTime = 0;
  boolean form = true;
  boolean formButton = true;
};

class AFEWebServer {

private:
  ESP8266WebServer server;
  AFEDevice *Device;
  AFEFirmwarePro *Firmware;
  #ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *SystemLED;
  #endif
  // It stores last HTTP API request
  HTTPCOMMAND httpCommand;
  // Once HTTP API requet is recieved it's set to true
  boolean receivedHTTPCommand = false;

  /* when it's set to true device configuration is refreshed. Required by
   * generate() method */
  boolean _refreshConfiguration = false;

  AFEDataAccess Data;
  AFESitesGenerator Site;

  boolean upgradeFailed = false;

  /* Method gets url Option parameter value */
  boolean getOptionName();
  uint8_t getCommand();
  uint8_t getSiteID();
  uint8_t getID();

  /* Generates HTML response (site) */
  String generateSite(AFE_SITE_PARAMETERS *siteConfig);

  /* Methods get POST data (for saveing) */
  DEVICE getDeviceData();
  NETWORK getNetworkData();
  MQTT getMQTTData();
  DOMOTICZ getDomoticzServerData();
  RELAY getRelayData(uint8_t id);
  SWITCH getSwitchData(uint8_t id);
  PASSWORD getPasswordData();
  PRO_VERSION getSerialNumberData();

#ifdef AFE_CONFIG_HARDWARE_LED
  LED getLEDData(uint8_t id);
  uint8_t getSystemLEDData();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  DS18B20 getDS18B20Data();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  DH getDHTData();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  REGULATOR getRegulatorData();
#endif

#if defined(T3_CONFIG)
  PIR getPIRData(uint8_t id);
#endif

#if defined(T5_CONFIG)
  CONTACTRON getContactronData(uint8_t id);
  GATE getGateData();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  SERIALPORT getSerialPortData();
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  I2CPORT getI2CPortData();
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  HPMA115S0 getHPMA115S0SensorData();
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  BMEX80 getBMEX80SensorData();
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  BH1750 getBH1750SensorData();
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  AS3935 getAS3935SensorData();
#endif


#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  ADCINPUT getAnalogInputData();
#endif

public:
  AFEWebServer();

  /* Method pushes HTML site from WebServer */
  void publishHTML(String page);

  /* Method initialize WebServer and Updater server */
  void begin(AFEDevice *, AFEFirmwarePro *);


  #ifdef AFE_CONFIG_HARDWARE_LED
  /* Method inherits global system LED */
  void initSystemLED(AFELED *);
  #endif

  /* Method listens for HTTP requests */
  void listener();

  /* Method listens for onNotFound */
  void onNotFound(ESP8266WebServer::THandlerFunction fn);

  /* Method adds URL for listen */
  void handle(const char *uri, ESP8266WebServer::THandlerFunction handler);
  void handleFirmwareUpgrade(const char *uri,
                             ESP8266WebServer::THandlerFunction handlerUpgrade,
                             ESP8266WebServer::THandlerFunction handlerUpload);

  /* Method generate HTML side. It reads also data from HTTP requests arguments
   * and pass them to Configuration Panel class */
  void generate(boolean upload = false);

  /* Method listens for HTTP API requests. If get True command is in httpCommand
   */
  boolean httpAPIlistener();

  /* Method reads HTTP API Command */
  HTTPCOMMAND getHTTPCommand();

  /* Method pushes JSON response to HTTP API request */
  void sendJSON(String json);
};

#endif
