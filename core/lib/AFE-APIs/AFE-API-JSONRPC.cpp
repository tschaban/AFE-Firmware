/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-JSONRPC.h"

/* Required to check access to WAN. Async ping */
volatile static boolean _PingResponded = false;

boolean _handlePingAnswer(const AsyncPingResponse &response) {
  IPAddress addr(response.addr);
  if (response.answer) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WAN ACCESS: Host ") << addr.toString().c_str()
           << F(" and got response in ") << response.time << F("ms");
  } else {
    Serial << endl
           << F("INFO: WAN ACCESS: No answer from host ")
           << addr.toString().c_str();

#endif
  }
  return false;
};

boolean _handlePingEnd(const AsyncPingResponse &response) {
  IPAddress addr(response.addr);
#ifdef DEBUG
  Serial << endl
         << F("INFO: WAN ACCESS: Host ") << addr.toString().c_str()
         << F(" sent pings: ") << response.total_sent << F(", recevied: ")
         << response.total_recv << F(", time ") << response.total_time
         << F(" ms");
#endif

  _PingResponded = response.total_recv > 0 ? true : false;
  return true;
};

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

  if (accessToWAN()) {

    char _text[3];
    generateMessage(message, method, params);

#ifdef AFE_CONFIG_HARDWARE_LED
    Led->on();
#endif

    unsigned long resposneTime = millis();

    http.begin(WirelessClient, AFE_CONFIG_JSONRPC_REST_API_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("afe-did", deviceID);
    if (strlen(Pro.serial) > 0) {
      http.addHeader("afe-key", Pro.serial);
    }
    sprintf(_text, "%d", AFE_FIRMWARE_TYPE);
    http.addHeader("afe-type", _text);
    http.addHeader("afe-version", AFE_FIRMWARE_VERSION);
    sprintf(_text, "%d", AFE_DEVICE_TYPE_ID);
    http.addHeader("afe-hid", _text);
    http.addHeader("afe-lang", L_LANGUAGE_SHORT);
    _httpCode = http.POST(message);
#ifdef DEBUG
    Serial << endl
           << F("INFO: API REST: Response code: ") << _httpCode << F(", time: ")
           << millis() - resposneTime << F("msec.");
#endif
    if (_httpCode == HTTP_CODE_OK) {
      response = http.getString();
#ifdef DEBUG
      Serial << endl
             << F("INFO: API REST: Response reply: ") << response
             << F(", Size: ") << response.length();

#endif
      const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
      DynamicJsonBuffer jsonBuffer(capacity);
      JsonObject &root = jsonBuffer.parseObject(response);
#ifdef DEBUG
      Serial << endl
             << F("INFO: API REST: JSON Buffer size: ") << capacity
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (AFE_CONFIG_JSONRPC_JSON_RESPONSE_SIE < jsonBuffer.size() + 10) {
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
    http.end();

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
  return _httpCode;
}

void AFEJSONRPC::checkAccessToWAN(void) {
  Pings.on(true, _handlePingAnswer);
  Pings.on(false, _handlePingEnd);
  setNoWANAccess();
  Pings.begin(AFE_WAN_ACCSSS_HOST, AFE_WAN_ACCSSS_PINGS,
              AFE_WAN_ACCSSS_PING_TIMEOUT);
#ifdef DEBUG
  Serial << endl
         << F("INFO: WAN ACCESS: Sent to ping to: ") << AFE_WAN_ACCSSS_HOST;
#endif
}

boolean AFEJSONRPC::accessToWAN() { return _PingResponded; }

void AFEJSONRPC::setNoWANAccess(void) {
  _PingResponded = false;
#ifdef DEBUG
  Serial << endl << F("INFO: WAN ACCESS: No access to WAN");
#endif 
}