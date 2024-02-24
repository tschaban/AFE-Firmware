/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_JSONRPC_h
#define _AFE_API_JSONRPC_h

#include <AFE-Configuration.h>
#include <AFE-Device.h>
#include <Arduino.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif
#include <ArduinoJson.h>

#include <WiFiClient.h>
// #include <AsyncPing.h>

#ifdef AFE_ESP32 /* ESP32 */
#include <ESP32Ping.h>
#include <HTTPClient.h>

#else /* ESP82x */
#include <ESP8266HTTPClient.h>
#include <ESP8266Ping.h>
#endif

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

const char JSONRPC_MESSAGE[] PROGMEM = "{\"jsonrpc\":\"2.0\",\"method\":\"{{"
                                       "json.method}}\",\"params\":{{json."
                                       "params}},\"id\":1}";

class AFEJSONRPC {
private:
  WiFiClient WirelessClient;
  HTTPClient *http = new HTTPClient();
  AFEDevice *Device;
  AFEDataAccess *Data;
  String message;
  // AsyncPing Pings;
  PingClass *Ping = new PingClass();
  boolean _PingResponded = false;
  PRO_VERSION *Pro = new PRO_VERSION;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *Led;
#endif

  void generateMessage(String &message, const char *method, const char *params);

public:
  boolean isStableConnection = true;

  AFEJSONRPC();

#ifdef DEBUG
  void addReference(AFEDebugger *_Debugger);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  void addReference(AFELED *_Led);
#endif

  void begin(AFEDataAccess *, AFEDevice *);

  int sent(boolean &response, const char *method);
  int sent(String &response, const char *method);
  int sent(String &response, const char *method, const char *params);

  void checkAccessToWAN(void);
  boolean accessToWAN();
  void setNoWANAccess(void);
};

#endif // _AFE_API_JSONRPC_h