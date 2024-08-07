/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-JSONRPC.h"

AFEJSONRPC::AFEJSONRPC(){};

void AFEJSONRPC::begin(AFEDataAccess *_Data, AFEDevice *_Device) {
  Data = _Data;
  Device = _Device;
  message.reserve(AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE);
  Data->getConfiguration(Pro);

#ifdef DEBUG
  Debugger->printInformation(F("Initialized"), F("REST-API"));
#endif
}

#ifdef DEBUG
void AFEJSONRPC::addReference(AFEDebugger *_Debugger) { Debugger = _Debugger; }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEJSONRPC::addReference(AFELED *_Led) { Led = _Led; }
#endif // AFE_CONFIG_HARDWARE_LED

void AFEJSONRPC::generateMessage(String &message, const char *method,
                                 const char *params) {
  message = FPSTR(JSONRPC_MESSAGE);
  message.replace(F("{{json.method}}"), method);
  if (strlen(params) > 0) {
    message.replace(F("{{json.params}}"), params);
  } else {
    message.replace(F("{{json.params}}"), F("{}"));
  }

#ifdef DEBUG
  Debugger->printBulletPoint(F("Messsage generated: "));
  Serial << message;
  if (message.length() + 20 > AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE) {
    Debugger->printBulletPoint(F("Warn: Message might be too long"));
  }
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

//  if (Device->configuration.api.jsonRPC) {

#ifdef DEBUG
  Debugger->printHeader(1, 0, 72, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printBulletPoint(F("Sending REST Request"));
#endif

  if (isStableConnection) {

    response = "";

    if (accessToWAN()) {

      char _text[3];
      generateMessage(message, method, params);

#ifdef AFE_CONFIG_HARDWARE_LED
      Led->on();
#endif

      unsigned long resposneTime = millis();

      // if (http->begin(WirelessClient, AFE_CONFIG_JSONRPC_REST_API_URL)) {
      if (http->begin(WirelessClient, AFE_CONFIG_JSONRPC_REST_API_SERVER,
                      AFE_CONFIG_JSONRPC_REST_API_SERVER_PORT,
                      AFE_CONFIG_JSONRPC_REST_API_SERVER_URI, true)) {

        http->setTimeout(AFE_CONFIG_JSONRPC_JSON_TIMEOUT);
        http->setReuse(true);

#ifdef DEBUG
        Debugger->printBulletPoint(F("Connected to the api server"));
      } else {
        Debugger->printBulletPoint(F("Error: No connection to the API server"));
#endif
      }

#ifdef DEBUG
      Debugger->printBulletPoint(F("Setting HEADER"));
#endif

      http->addHeader("Content-Type", "application/json");
      char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
      Data->getDeviceID(_deviceId);
      http->addHeader("afe-did", _deviceId);

      /*
      #ifdef DEBUG
            Serial << endl << F(" : afe-did: ") << _deviceId;
      #endif
      */
      if (strlen(Pro->serial) > 0) {
        http->addHeader("afe-key", Pro->serial);
        
        #ifdef DEBUG
                Serial << endl << F(" : afe-key: ") << Pro->serial;
        #endif
        
      }
      sprintf(_text, "%d", AFE_FIRMWARE_TYPE);
      http->addHeader("afe-type", _text);
      /*
      #ifdef DEBUG
            Serial << endl << F(" : afe-type: ") << _text;
      #endif
        */
      http->addHeader("afe-version", AFE_FIRMWARE_VERSION);
      /*
      #ifdef DEBUG
            Serial << endl << F(" : afe-version: ") << AFE_FIRMWARE_VERSION;
      #endif
        */
      sprintf(_text, "%d", AFE_DEVICE_TYPE_ID);
      http->addHeader("afe-hid", _text);
      /*
#ifdef DEBUG
      Serial << endl << F(" : afe-hid: ") << AFE_DEVICE_TYPE_ID;
#endif
*/
      http->addHeader("afe-lang", L_LANGUAGE_SHORT);
/*
#ifdef DEBUG
Serial << endl << F(" : afe-lang: ") << L_LANGUAGE_SHORT;
#endif
*/
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
      http->addHeader("afe-api", "D");
/*
#ifdef DEBUG
Serial << endl << F(" : afe-api: D");
#endif
*/
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
      http->addHeader("afe-api", "H");
/*
#ifdef DEBUG
      Serial << endl << F(" : afe-api: H");
#endif
*/
#else
      http->addHeader("afe-api", "S");
/*
#ifdef DEBUG
Serial << endl << F(" : afe-api: S");
#endif
*/
#endif

#if defined(ESP8285)
      http->addHeader("afe-chip", "8285");
/*
#ifdef DEBUG
Serial << endl << F(" : afe-chip: 8285");
#endif
*/
#elif defined(ESP8266)
      http->addHeader("afe-chip", "8266");
/*
#ifdef DEBUG
      Serial << endl << F(" : afe-chip: 8286");
#endif
*/
#else
      http->addHeader("afe-chip", "32");
/*
#ifdef DEBUG
      Serial << endl << F(" : afe-chip: 32");
#endif
*/
#endif

#if defined(AFE_ESP_FLASH_4MB)
      http->addHeader("afe-size", "4");
/*
#ifdef DEBUG
      Serial << endl << F(" : afe-size: 4");
#endif
*/
#elif defined(AFE_ESP_FLASH_2MB)
      http->addHeader("afe-size", "2");
/*
#ifdef DEBUG
      Serial << endl << F(" : afe-size: 2");
#endif
*/
#else
      http->addHeader("afe-size", "1");
/*
#ifdef DEBUG
Serial << endl << F(" : afe-size: 1");
#endif
*/
#endif

#ifdef DEBUG
      http->addHeader("afe-debug", "1");
/*
      Serial << endl << F(" : afe-debug: 1");
*/
#else
      http->addHeader("afe-debug", "0");
/*
#ifdef DEBUG
Serial << endl << F(" : afe-debug: 0");
#endif
*/
#endif

      _httpCode = http->POST(message);

#ifdef DEBUG
      Debugger->printBulletPoint(F("Response code: "));
      Serial << _httpCode << F(", time: ") << millis() - resposneTime
             << F("msec.");
#endif
      if (_httpCode == HTTP_CODE_OK) {
        response = http->getString();
#ifdef DEBUG
        Debugger->printBulletPoint(F("Content: "));
        Serial << response << F(", Size: ") << response.length();

#endif
        const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
        DynamicJsonBuffer jsonBuffer(capacity);
        //   StaticJsonBuffer<AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIZE> jsonBuffer;

        JsonObject &root = jsonBuffer.parseObject(response);
#ifdef DEBUG

        Data->printBufforSizeInfo(capacity, jsonBuffer.size());
#endif
        if (!root.success()) {
#ifdef DEBUG
          Data->printJSONNotPharsed();
#endif
          response = "";
        } else {
          response = root["result"] | "";
        }
      }
#ifdef DEBUG
      else {
        response = http->getString();
        Debugger->printBulletPoint(F("Response code: "));
        Serial << _httpCode << F(" ")
               << (_httpCode < 0 ? http->errorToString(_httpCode) : "")
               << F(", content: ") << response << F(", Size: ")
               << response.length();

        if (_httpCode == -1 && (millis() - resposneTime) > 2000) {
          isStableConnection = false;
#ifdef DEBUG
          Debugger->printBulletPoint(
              F("Connection to the API server is not stable. Turning it OFF"));
#endif
        }
      }
#endif
      http->end();

#ifdef AFE_CONFIG_HARDWARE_LED
      Led->off();
#endif

      /* If too long response checking if there is still an access to the
       * interent
       */
      if (millis() - resposneTime > AFE_WAN_ACCSSS_TIMEOUT) {
#ifdef DEBUG
        Debugger->printBulletPoint(F("Warn: Too long response: "));
        Serial << ((millis() - resposneTime) / 1000)
               << F("s. checking access to the Internet");
#endif
        accessToWAN();
      }
    } else {
#ifdef DEBUG
      Debugger->printBulletPoint(F("Error: No access to Internet"));
#endif
    }

#ifdef DEBUG
    Debugger->getFreeMemorySize();
  } else {
    Debugger->printBulletPoint(F("Warn: API is off as not stable"));
#endif
  }

#ifdef DEBUG
  Debugger->printHeader(1, 1, 72, AFE_DEBUG_HEADER_TYPE_DASH);
#endif

  return _httpCode;
}

void AFEJSONRPC::checkAccessToWAN(void) {

#ifdef DEBUG
  Debugger->printBulletPoint(F("Checking access to: "));
  Debugger->printValue(F(AFE_WAN_ACCSSS_HOST));
#endif

  IPAddress ip;
  ip.fromString(AFE_WAN_ACCSSS_HOST);
  _PingResponded = Ping->ping(ip, AFE_WAN_ACCSSS_PINGS);
  char _text[17];
  if (_PingResponded) {
    Data->addLog(F("wan:access:yes"));
    Data->addLog(F("wan:ping:%dms"), (uint16_t)Ping->averageTime());
  } else {
    Data->addLog(F("wan:access:no"));
  }

#ifdef DEBUG
  Debugger->printBulletPoint((_PingResponded ? F("") : F("NOT ")));
  Serial << F("Connected to Internet");
  Debugger->printBulletPoint(F("Average time: "));
  Serial << Ping->averageTime() << F(" msec");
#endif
}

boolean AFEJSONRPC::accessToWAN() { return _PingResponded; }

void AFEJSONRPC::setNoWANAccess(void) {
  _PingResponded = false;
#ifdef DEBUG
  Debugger->printBulletPoint(F("No access to Internet"));
#endif
}