/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Domoticz-API.h"

AFEDomoticz::AFEDomoticz() { begin(); }

void AFEDomoticz::begin() {
  configuration = Data.getDomoticzConfiguration();
  sprintf(serverURL, "%s%s:%d/",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port);
}

const String AFEDomoticz::getRelayUpdateUrl(uint16_t idx, const char *value) {
  char url[sizeof(serverURL) + 65];
  sprintf(url, "%sjson.htm?type=command&param=switchlight&idx=%s&switchcmd=%s",
          serverURL, idx, value);
  return url;
}

void AFEDomoticz::callURL(const char *url) {
  Serial << endl << "INFO: calling url: " << url;

  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial << endl << payload;
  http.end();
}
