/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_JSONRPC_h
#define _AFE_API_JSONRPC_h

#include <AFE-Configuration.h>
#include <AFE-Device.h>
#include <AFE-Site-components.h>
#include <ESP8266HTTPClient.h>
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

class AFEJSONRPC {
private:
  HTTPClient http;
  PRO_VERSION Pro;
  char deviceID[17];
  void generateMessage(String &message, const char *method, const char *params);

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *);
#endif

public:
  AFEDevice *Device;
  AFEDataAccess *Data;
  
#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *Led;
#endif

  AFEJSONRPC();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif
  int sent(String &response, const char *method);
  int sent(String &response, const char *method, const char *params);

  void getHTMLResponse(String *response, String &formatedHTML);


};

#endif // _AFE_API_JSONRPC_h