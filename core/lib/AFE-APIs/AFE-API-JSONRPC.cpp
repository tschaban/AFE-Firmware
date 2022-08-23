/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-JSONRPC.h"

AFEJSONRPC::AFEJSONRPC(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEJSONRPC::begin(AFEDataAccess *_Data, AFEDevice *_Device, AFELED *_Led) {
  Led = _Led;
  begin(_Data, _Device);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEJSONRPC::begin(AFEDataAccess *_Data, AFEDevice *_Device) {
  Data = _Data;
  Device = _Device;
  ;
  message.reserve(AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE); // @TODO is this well estimated

#ifdef DEBUG
  Serial << endl << F("INFO: API REST: Initialized");
#endif
}

void AFEJSONRPC::generateMessage(String &message, const char *method,
                                 const char *params) {
  message = FPSTR(JSONRPC_MESSAGE);
  message.replace("{{json.method}}", method);
  if (strlen(params) > 0) {
    message.replace("{{json.params}}", params);
  } else {
    message.replace("{{json.params}}", F("{}"));
  }

#ifdef DEBUG
  if (message.length() + 20 > AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE) {
    Serial << endl << F("INFO: API REST: Messsage generated: ") << message;
  }
  Serial << endl << F("INFO: API REST: Messsage generated: ") << message;
#endif
}

int AFEJSONRPC::sent(String &response, const char *method) {
  return sent(response, method, "");
}

int AFEJSONRPC::sent(boolean &response, const char *method) {
  String jsonResponseResult;
  jsonResponseResult.reserve(6);
  int htmlCode = sent(jsonResponseResult, method, "");
  if (htmlCode == HTTP_CODE_OK) {
    if (jsonResponseResult == "false") {
      response = false;
    } else if (jsonResponseResult == "true")
      response = true;
  }
  return htmlCode;
}

int AFEJSONRPC::sent(String &response, const char *method, const char *params) {

  int _httpCode = HTTP_CODE_INTERNAL_SERVER_ERROR;
  response = "";

  if (accessToWAN()) {

    char _text[3];
    generateMessage(message, method, params);

#ifdef AFE_CONFIG_HARDWARE_LED
    Led->on();
#endif

    unsigned long resposneTime = millis();

    PRO_VERSION *Pro = new PRO_VERSION;
    Data->getConfiguration(Pro);

    http->begin(WirelessClient, AFE_CONFIG_JSONRPC_REST_API_URL);
    http->addHeader("Content-Type", "application/json");
    http->addHeader("afe-did", Device->deviceId);
    if (strlen(Pro->serial) > 0) {
      http->addHeader("afe-key", Pro->serial);
    }
    sprintf(_text, "%d", AFE_FIRMWARE_TYPE);
    http->addHeader("afe-type", _text);
    http->addHeader("afe-version", AFE_FIRMWARE_VERSION);
    sprintf(_text, "%d", AFE_DEVICE_TYPE_ID);
    http->addHeader("afe-hid", _text);
    http->addHeader("afe-lang", L_LANGUAGE_SHORT);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
    http->addHeader("afe-api", "D");
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT    
    http->addHeader("afe-api", "H");
#else
    http->addHeader("afe-api", "S");
#endif

#if defined(ESP8285)
    http->addHeader("afe-chip", "8285");
#elif defined(ESP8266)
    http->addHeader("afe-chip", "8266");
#else
    http->addHeader("afe-chip", "32");
#endif

#if defined(AFE_ESP_FLASH_4MB)
    http->addHeader("afe-size", "4");
#elif defined(AFE_ESP_FLASH_2MBB)
    http->addHeader("afe-size", "2");
#else
    http->addHeader("afe-size", "1");
#endif

#ifdef DEBUG
    http->addHeader("afe-debug", "1");
#else
    http->addHeader("afe-debug", "0");
#endif

    _httpCode = http->POST(message);
#ifdef DEBUG
    Serial << endl
           << F("INFO: API REST: Response code: ") << _httpCode << F(", time: ")
           << millis() - resposneTime << F("msec.");
#endif
    if (_httpCode == HTTP_CODE_OK) {
      response = http->getString();
#ifdef DEBUG
      Serial << endl
             << F("INFO: API REST: Response reply: ") << response
             << F(", Size: ") << response.length();

#endif
      const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
      DynamicJsonBuffer jsonBuffer(capacity);
      //   StaticJsonBuffer<AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIZE> jsonBuffer;
       
      JsonObject &root = jsonBuffer.parseObject(response);
#ifdef DEBUG
      Serial << endl
             << F("INFO: API REST: JSON Buffer size: ") << capacity
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (/*AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIZE*/ capacity < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: API REST: Too small buffer size");
      }
#endif
      if (!root.success()) {
#ifdef DEBUG
        Serial << endl << F("ERROR: API REST: JSON Parsing error");
#endif
        response = "";
      } else {
        response = root["result"] | "";
      }
    }
#ifdef DEBUG
    else {
      response = http->getString();

      Serial << endl
             << F("INFO: API REST: Response reply code: ") << _httpCode
             << F(", content: ") << response << F(", Size: ")
             << response.length();

//      response = "";
    }
#endif
    http->end();

#ifdef AFE_CONFIG_HARDWARE_LED
    Led->off();
#endif

    /* If too long response checking if there is still an access to the interent
     */
    if (millis() - resposneTime > AFE_WAN_ACCSSS_TIMEOUT) {
#ifdef DEBUG
      Serial << endl
             << F("WARN: API REST: Too long response: ")
             << ((millis() - resposneTime) / 1000)
             << F("s. checking access to the WAN");
#endif
      accessToWAN();
    }

  } else {
#ifdef DEBUG
    Serial << endl << F("ERROR: API REST: No access to WAN");
#endif
  }


#if defined(DEBUG) && !defined(AFE_ESP32)
      Serial << endl
             << F("INFO: RAM: ") << system_get_free_heap_size() / 1024
             << F("kB: JSON returned");
#endif

  return _httpCode;
}

void AFEJSONRPC::checkAccessToWAN(void) {

#ifdef DEBUG
  Serial << endl
         << F("INFO: WAN ACCESS: checking access to : ") << AFE_WAN_ACCSSS_HOST;
#endif

  IPAddress ip;
  ip.fromString(AFE_WAN_ACCSSS_HOST);
  _PingResponded = Ping->ping(ip, AFE_WAN_ACCSSS_PINGS);

#ifdef DEBUG

  Serial << endl
         << F(" - ") << (_PingResponded ? F("connected to WAN")
                                        : F("NOT connected to WAN"));
  Serial << endl << F(" - average time: ") << Ping->averageTime();
#endif

  /*
  Pings.on(true, _handlePingAnswer);
  Pings.on(false, _handlePingEnd);
  setNoWANAccess();

  IPAddress ip;
  ip.fromString(AFE_WAN_ACCSSS_HOST);

  Pings.begin(ip, AFE_WAN_ACCSSS_PINGS, AFE_WAN_ACCSSS_PING_TIMEOUT);
#ifdef DEBUG
  Serial << endl
         << F("INFO: WAN ACCESS: Sent to ping to: ") << AFE_WAN_ACCSSS_HOST;
#endif
*/
}

boolean AFEJSONRPC::accessToWAN() { return _PingResponded; }

void AFEJSONRPC::setNoWANAccess(void) {
  _PingResponded = false;
#ifdef DEBUG
  Serial << endl << F("INFO: WAN ACCESS: No access to WAN");
#endif
}