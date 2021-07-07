/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Web_Server_h
#define _AFE_Web_Server_h

#include <AFE-API-JSONRPC.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Sites-Generator.h>
#include <ESP8266WebServer.h>

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
#include <WiFiUdp.h>
#include <esp8266httpupdate.h>
#include <WiFiClient.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

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
  uint16_t rebootTime = 0;
  boolean form = true;
  boolean formButton = true;
  uint8_t option = AFE_HARDWARE_ITEM_NOT_EXIST;
};

class AFEWebServer {

private:
  ESP8266WebServer server;
  AFEDevice *Device;
  AFEFirmwarePro *FirmwarePro;
  AFEDataAccess *Data;
  AFEJSONRPC *RestAPI;

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

  /* Used to auto-logoff from the config panel */
  unsigned long howLongInConfigMode = 0;

  AFESitesGenerator Site;

  boolean upgradeSuccess = false;

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *);
#endif


  /* Method gets url Option parameter value */
  boolean getOptionName();
  
  uint8_t getOption();
  uint8_t getCommand();
  uint8_t getSiteID();
  uint8_t getID();

  /* Generates HTML response (site) */
  String generateSite(AFE_SITE_PARAMETERS *siteConfig, String &page);

  /* Methods get POST data (for saveing) */
  void get(DEVICE &data);
  void get(NETWORK &data);
  void get(MQTT &data);
  void get(PASSWORD &data);
  void get(PRO_VERSION &data);

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void get(DOMOTICZ &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void get(RELAY &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void get(SWITCH &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  void get(LED &data);
  uint8_t getSystemLEDData();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void get(DS18B20 &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  void get(DHT &data);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void get(REGULATOR &data);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void get(THERMAL_PROTECTOR &data);
#endif

#if defined(T3_CONFIG)
  void getPIRData(uint8_t id, PIR *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void get(CONTACTRON &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void get(GATE &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  void getSerialPortData(SERIALPORT *);
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  void get(I2CPORT &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void get(HPMA115S0 &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void get(BMEX80 &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void get(BH1750 &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void get(AS3935 &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void get(ANEMOMETER &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void get(RAINMETER &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void get(ADCINPUT &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void get(BINARY_SENSOR &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  void get(PN532_SENSOR &data);
  void processMiFareCard();
  void get(MIFARE_CARD &data);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  void get(CLED &CLEDData,CLED_EFFECTS &CLEDEffectsData);
#endif


#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  uint16_t getOTAFirmwareId();
  boolean upgradeOTAWAN(uint16_t firmwareId);
  boolean upgradOTAFile(void);
#endif

public:
  AFEWebServer();

  /* Method pushes HTML site from WebServer */
  void publishHTML(const String &page);

/* Method initialize WebServer and Updater server */
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFELED *,
             AFEJSONRPC *);
#else
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *);
#endif

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

  /* Method generate HTML side. It reads also data from HTTP requests
   * arguments
   * and pass them to Configuration Panel class */
  void generate(boolean upload = false);

  /* Method listens for HTTP API requests. If get True command is in
   * httpCommand
   */
  boolean httpAPIlistener();

  /* Method reads HTTP API Command */
  HTTPCOMMAND getHTTPCommand();

  /* Method pushes JSON response to HTTP API request */
  // void sendJSON(String json);
  void sendJSON(const String &json);
};

#endif
