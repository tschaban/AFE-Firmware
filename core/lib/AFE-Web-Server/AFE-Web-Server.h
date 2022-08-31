/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Web_Server_h
#define _AFE_Web_Server_h

#include <AFE-API-JSONRPC.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Sites-Generator.h>

#ifdef AFE_ESP32
#include <WebServer.h>
#else /* ESP8266 */
#include <ESP8266WebServer.h>
#endif // ESP32/ESP8266

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
#include <WiFiClient.h>
#ifdef AFE_ESP32 /* ESP32 */
#include <Update.h>
#else /* ESP8266 */
#include <WiFiUdp.h>
#include <esp8266httpupdate.h>
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <Wire.h>
#endif // AFE_CONFIG_HARDWARE_I2C

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
  AFEDevice *Device;
  AFEFirmwarePro *FirmwarePro;
  AFEDataAccess *Data;
  AFEJSONRPC *RestAPI;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *SystemLED;
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  TwoWire *WirePort0;
#ifdef AFE_ESP32
  TwoWire *WirePort1;
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_I2C

  /**
   * @brief Once HTTP API requet is recieved it's set to true
   *
   */
  boolean receivedHTTPCommand = false;

  /**
   * @brief when it's set to true device configuration is refreshed. Required by
   * generate() method
   *
   */
  boolean _refreshConfiguration = false;

  /**
   * @brief Used to auto-logoff from the config panel
   *
   */
  unsigned long howLongInConfigMode = 0;

  AFESitesGenerator Site;

  boolean upgradeSuccess = false;

#if defined(AFE_CONFIG_HARDWARE_LED) || defined(AFE_CONFIG_HARDWARE_I2C)
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *);
#endif // AFE_CONFIG_HARDWARE_LED || AFE_CONFIG_HARDWARE_I2C

  /**
   * @brief Method gets url Option parameter value
   *
   * @return boolean
   */
  boolean getOptionName();

  uint8_t getOption();
  uint8_t getCommand();
  uint8_t getSiteID();
  uint8_t getID();

  /**
   * @brief Generates HTML response (site)
   *
   * @param  siteConfig       desc
   * @param  page             desc
   * @return String
   */
  String generateSite(AFE_SITE_PARAMETERS *siteConfig, String &page);

  /**
   * @brief Methods get POST data (for saveing)
   *
   * @param  data             desc
   */
  void get(DEVICE &data);
  void get(NETWORK &data);
  void get(MQTT &data);
  void get(PASSWORD &data);
  void get(PRO_VERSION &data);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void get(DOMOTICZ &data);
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  void get(HOME_ASSISTANT_CONFIG &data);
#endif // AFE_FIRMWARE_API

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void get(RELAY &data);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void get(SWITCH &data);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_LED
  void get(LED &data);
  uint8_t getSystemLEDData();
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void get(DS18B20 &data);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  void get(DHT &data);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void get(REGULATOR &data);
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void get(THERMAL_PROTECTOR &data);
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#if defined(T3_CONFIG)
  void getPIRData(uint8_t id, PIR *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void get(CONTACTRON &data);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_GATE
  void get(GATE &data);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_UART
  void getSerialPortData(SERIALPORT *);
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
  void get(I2CPORT &data);
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void get(HPMA115S0 &data);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void get(BMEX80 &data);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void get(BH1750_CONFIG &data);
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void get(AS3935 &data);
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void get(ANEMOMETER &data);
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void get(RAINMETER &data);
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  void get(ADCINPUT &data);
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void get(BINARY_SENSOR &data);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  void get(PN532_SENSOR &data);
  void processMiFareCard();
  void get(MIFARE_CARD &data);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  /**
   * @brief Gets RGB LED configuration parameters from HTML form
   *
   * @param  data Configuration data
   */
  void get(CLED &data);

  /**
   * @brief Gets RGB LED Blinking effect configuration parameters from HTML form
   *
   * @param  data Configuration data
   */
  void get(CLED_EFFECT_BLINKING &data);

  /**
   * @brief Gets RGB LED Wave effect configuration parameters from HTML form
   *
   * @param  data Configuration data
   */
  void get(CLED_EFFECT_WAVE &data);

  /**
   * @brief Gets RGB LED Fade In/Out effect configuration parameters from HTML
   * form
   *
   * @param  data Configuration data
   */
  void get(CLED_EFFECT_FADE_INOUT &data);

#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  void get(TSL2561 &data);
#endif // AFE_CONFIG_HARDWARE_TSL2561

/**
 * @brief Gets MCP23XXX onfiguration parameters from HTML form
 *
 */
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  void get(MCP23XXX &data);
#endif

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  uint16_t getOTAFirmwareId();
  boolean upgradeOTAWAN(uint16_t firmwareId);
  boolean upgradOTAFile(void);
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

public:
#ifdef AFE_ESP32 /* ESP82xx */
  WebServer server;
#else  /* ESP8266 */
  ESP8266WebServer server;
#endif // ESP32/ESP8266


  /**
   * @brief  It stores last HTTP API request
   *
   */
  HTTPCOMMAND *httpAPICommand = new HTTPCOMMAND;


  AFEWebServer();

/**
 * @brief Method initialize WebServer and Updater server
 *
 */
#if defined(AFE_CONFIG_HARDWARE_LED) && !defined(AFE_CONFIG_HARDWARE_I2C)
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *,
             AFELED *);
#elif defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  void begin(AFEDataAccess *_Data, AFEDevice *_Device,
             AFEFirmwarePro *_FirmwarePro, AFEJSONRPC *_RestAPI, AFELED *_Led,
             TwoWire *_WirePort0, TwoWire *_WirePort1);
#else
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *,
             AFELED *, TwoWire *);
#endif // AFE_ESP32
#elif !defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *,
             TwoWire *, TwoWire *);
#else
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *,
             TwoWire *);
#endif // AFE_ESP32
#else
  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  /**
   * @brief Method inherits global system LED
   *
   */
  void initSystemLED(AFELED *);
#endif // AFE_CONFIG_HARDWARE_LED

  /**
   * @brief Method listens for HTTP requests
   *
   */
  void listener();

#ifdef AFE_ESP32
  /**
   * @brief Method listens for onNotFound
   *
   * @param  fn               desc
   */
  void onNotFound(WebServer::THandlerFunction fn);

  /**
   * @brief Method adds URL for listen
   *
   * @param  uri              desc
   * @param  handler          desc
   */
  void handle(const char *uri, WebServer::THandlerFunction handler);
  void handleFirmwareUpgrade(const char *uri,
                             WebServer::THandlerFunction handlerUpgrade,
                             WebServer::THandlerFunction handlerUpload);
#else // ESP8266
  /**
   * @brief Method listens for onNotFound
   *
   * @param  fn               desc
   */
  void onNotFound(ESP8266WebServer::THandlerFunction fn);

  /**
   * @brief Method adds URL for listen
   *
   * @param  uri              desc
   * @param  handler          desc
   */
  void handle(const char *uri, ESP8266WebServer::THandlerFunction handler);
  void handleFirmwareUpgrade(const char *uri,
                             ESP8266WebServer::THandlerFunction handlerUpgrade,
                             ESP8266WebServer::THandlerFunction handlerUpload);

#endif // ESP32/ESP8266

  String getHeaderValue(String header, String headerName);

  /**
   * @brief Method generate HTML side. It reads also data from HTTP requests
   * arguments and pass them to Configuration Panel class.
   * @param  upload           desc
   * @return boolean True: site generated, False: HTTP API
   */

  boolean generate(boolean upload = false);

  /**
   * @brief Method listens for HTTP API requests.
   *
   * @return boolean If True command is in httpAPICommand
   */
  boolean httpAPIlistener();

  /**
   * @brief Method pushes JSON response to HTTP API request
   *
   * @param  json             desc
   */
  void sendJSON(const String &json);
};

#endif // _AFE_Web_Server_h
