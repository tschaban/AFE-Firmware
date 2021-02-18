/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-JSONRPC.h"

AFEJSONRPC::AFEJSONRPC(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEJSONRPC::begin(AFEDataAccess *_Data, AFEDevice *_Device, AFEWiFi *_WiFi,
                       AFELED *_Led) {
  Led = _Led;
  begin(_Data, _Device, _WiFi);
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEJSONRPC::begin(AFEDataAccess *_Data, AFEDevice *_Device,
                       AFEWiFi *_WiFi) {
  Data = _Data;
  Device = _Device;
  Network = _WiFi;
  Data->getConfiguration(&Pro);
  Data->getDeviceUID().toCharArray(deviceID, sizeof(deviceID) + 1);
  message.reserve(AFE_CONFIG_JSONRPC_MESSAGE_MAX_SIZE);

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
    Serial << endl << F("INFO: API: REST: Messsage generated: ") << message;
  }
  Serial << endl << F("INFO: API: REST: Messsage generated: ") << message;
#endif
}

int AFEJSONRPC::sent(String &response, const char *method) {
  return sent(response, method, "");
}

int AFEJSONRPC::sent(boolean &response, const char *method) {
  String jsonResponseResult;
  jsonResponseResult.reserve(6);
  int htmlCode = sent(jsonResponseResult, method, "");
  if (htmlCode == AFE_CONFIG_JSONRPC_REST_OK_RESPONSE) {
    if (jsonResponseResult == "false") {
      response = false;
    } else if (jsonResponseResult == "true")
      response = true;
  }
  return htmlCode;
}

int AFEJSONRPC::sent(String &response, const char *method, const char *params) {

  int _httpCode = AFE_CONFIG_JSONRPC_REST_FAILURE_RESPONSE;
  char _text[3];
  generateMessage(message, method, params);

#ifdef AFE_CONFIG_HARDWARE_LED
  Led->on();
  unsigned long resposneTime = millis();
#endif

  Network->http.begin(AFE_CONFIG_JSONRPC_REST_API_URL);
  Network->http.addHeader("Content-Type", "application/json");
  Network->http.addHeader("afe-did", deviceID);
  if (strlen(Pro.serial) > 0) {
    Network->http.addHeader("afe-key", Pro.serial);
  }
  sprintf(_text, "%d", AFE_FIRMWARE_TYPE);
  Network->http.addHeader("afe-type", _text);
  Network->http.addHeader("afe-version", AFE_FIRMWARE_VERSION);
  sprintf(_text, "%d", AFE_DEVICE_TYPE_ID);
  Network->http.addHeader("afe-hid", _text);
  Network->http.addHeader("afe-lang", L_LANGUAGE_SHORT);

  _httpCode = Network->http.POST(message);
#ifdef DEBUG
  Serial << endl
         << F("INFO: API REST: Response code: ") << _httpCode << F(", time: ")
         << millis() - resposneTime << F("msec.");
#endif
  if (_httpCode == AFE_CONFIG_JSONRPC_REST_OK_RESPONSE) {
    response = Network->http.getString();
#ifdef DEBUG
    Serial << endl
           << F("INFO: API REST: Response reply: ") << response << F(", Size: ")
           << response.length();

#endif

    
    JsonObject &root = jsonBuffer.parseObject(response);

#ifdef DEBUG
    Serial << endl
           << F("INFO: API REST: JSON Buffer size: ")
           << AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIE << F(", actual JSON size: ")
           << jsonBuffer.size();
    if (AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIE <
        jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: API REST: Too small buffer size");
    }
#endif

    if (!root.success()) {
#ifdef DEBUG
      Serial << endl << F("INFO: API REST: JSON Parsing error");
#endif
      response = "";
    } else {
      response = root["result"] | "";
    }
  }
  Network->http.end();

#ifdef AFE_CONFIG_HARDWARE_LED
  Led->off();
#endif
  return _httpCode;
}
