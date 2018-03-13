/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-API-Domoticz.h"

AFEDomoticz::AFEDomoticz() { begin(); }

void AFEDomoticz::begin() {
  configuration = Data.getDomoticzConfiguration();
  char user[100]; //@TODO
  rbase64.encode(configuration.user).toCharArray(user, 100);
  char pass[100]; //@TODO
  rbase64.encode(configuration.password).toCharArray(pass, 100);

  char authorization[20 + 100];
  if (configuration.user && configuration.password) {
    Serial << endl << "There is user / password for domoticz";
    sprintf(authorization, "&username=%s&password=%s", user, pass);
  }

  sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port, authorization);

  http.setTimeout(1000); // @TODO is it working?
}

const String AFEDomoticz::getApiCall(const char *param, unsigned long idx) {
  char url[sizeof(serverURL) + 39 + sizeof(param)];
  sprintf(url, "%s&param=%s&idx=%u", serverURL, param, idx);
  return url;
}

void AFEDomoticz::sendSwitchCommand(unsigned long idx, const char *value) {
  String call = getApiCall("switchlight", idx);
  call += "&switchcmd=";
  call += value;
  callURL(call);
}

void AFEDomoticz::callURL(const String url) {
  Serial << endl << "INFO: calling url: " << url;

  http.begin(url);

  int httpCode = http.GET();
  Serial << endl << "Get: " << httpCode;
  String payload = http.getString();
  Serial << endl << payload;
  http.end();
}
