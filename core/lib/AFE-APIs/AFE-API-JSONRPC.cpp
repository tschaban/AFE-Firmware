/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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
  Data->getConfiguration(&Pro);
  Data->getDeviceUID().toCharArray(deviceID, sizeof(deviceID) + 1);
#ifdef DEBUG
  Serial << endl << "API: REST: Initialized";
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
  Serial << endl << "API: REST: Messsage generated: " << message;
#endif
}

int AFEJSONRPC::sent(String &response, const char *method) {
  return sent(response, method, "");
}

int AFEJSONRPC::sent(String &response, const char *method, const char *params) {

  int _httpCode = 500;
  String message;
  char _text[3];

  generateMessage(message, method, params);

#ifdef AFE_CONFIG_HARDWARE_LED
  Led->on();
  unsigned long resposneTime = millis();
#endif

  http.begin(AFE_CONFIG_JSONRPC_REST_API_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("afe-did", deviceID);
  if (strlen(Pro.serial)>0) {
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
         << "API: REST: Response code: " << _httpCode
         << ", time: " << millis() - resposneTime << "msec.";
#endif
  if (_httpCode == 200) {
    response = http.getString();
#ifdef DEBUG
    Serial << endl << "API: REST: Response reply: " << response;
#endif
  }
  http.end();

#ifdef AFE_CONFIG_HARDWARE_LED
  Led->off();
#endif
  return _httpCode;
}

void AFEJSONRPC::getHTMLResponse(String *response, String &formatedHTML) {
  formatedHTML = "";
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(*response);

  if (!root.success()) {
#ifdef DEBUG
    Serial << endl << "API: REST: JSON Parsing error";
#endif
    return;
  } else {
    formatedHTML = root["result"] | "";
  }
}