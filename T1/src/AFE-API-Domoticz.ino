/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-API-Domoticz.h"

AFEDomoticz::AFEDomoticz() {}

void AFEDomoticz::begin() {
  configuration = Data.getDomoticzConfiguration();
  char _user[45] = {0}; // base64 conversion takes ceil(n/3)*4 size of mem
  char _pass[45] = {0};
  char authorization[20 + sizeof(_user) + sizeof(_pass) + 1] = {0};

  if (configuration.user && configuration.password) {
    rbase64.encode(configuration.user);
    sprintf(_user, rbase64.result());
    rbase64.encode(configuration.password);
    sprintf(_pass, rbase64.result());
    sprintf(authorization, "&username=%s&password=%s", _user, _pass);
  }

  sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port, authorization);

  initialized = true;
  /* Uncommencted can help to determin the size of serverURL
  Serial << endl << "ServerURL Size=" << String(serverURL).length();
  */
}

void AFEDomoticz::disconnect() { initialized = false; }

void AFEDomoticz::sendSwitchCommand(unsigned long idx, const char *value) {
  if (initialized) {
    String call = getApiCall("switchlight", idx);
    call += "&switchcmd=";
    call += value;
    callURL(call);
  }
}

void AFEDomoticz::sendTemperatureCommand(unsigned long idx, float value) {
  if (initialized) {
    char _temperatureChar[7];
    dtostrf(value, 4, 2, _temperatureChar);
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += _temperatureChar;
    callURL(call);
  }
}

const String AFEDomoticz::getApiCall(const char *param, unsigned long idx) {
  char url[sizeof(serverURL) + 18 + strlen(param)];
  sprintf(url, "%s&param=%s&idx=%u", serverURL, param, idx);
  return url;
}

void AFEDomoticz::callURL(const String url) {

  // Serial << endl << "Calling url: " << url;
  http.begin(url);
  int httpCode = http.GET();
  /*
  Serial << endl << "Response: " << httpCode;
  String payload = http.getString();
  Serial << endl << payload;
  */
  http.end();
}
