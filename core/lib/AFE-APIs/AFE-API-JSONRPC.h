/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_JSONRPC_h
#define _AFE_API_JSONRPC_h

#include <arduino.h>
#include <AFE-Configuration.h>
#include <AFE-WiFi.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <ArduinoJson.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

const char JSONRPC_MESSAGE[] PROGMEM = "{\"jsonrpc\":\"2.0\",\"method\":\"{{"
                                       "json.method}}\",\"params\":{{json."
                                       "params}},\"id\":1}";

class AFEJSONRPC {
private:
  AFEWiFi *Network;
  PRO_VERSION Pro;
  char deviceID[17];
  void generateMessage(String &message, const char *method, const char *params);

  String message;
  StaticJsonBuffer<AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIE> jsonBuffer;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *Led;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFEWiFi *);
#endif

public:
  AFEDevice *Device;
  AFEDataAccess *Data;

  AFEJSONRPC();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFEWiFi *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *, AFEWiFi *);
#endif

  int sent(boolean &response, const char *method);
  int sent(String &response, const char *method);
  int sent(String &response, const char *method, const char *params);

};

#endif // _AFE_API_JSONRPC_h