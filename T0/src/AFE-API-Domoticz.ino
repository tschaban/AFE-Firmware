/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-API-Domoticz.h"

AFEDomoticz::AFEDomoticz() { begin(); }

void AFEDomoticz::begin() {
  configuration = Data.getDomoticzConfiguration();
  sprintf(serverURL, "%s%s:%d/",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port);

  http.setTimeout(1000); // @TODO is it working?
}

void AFEDomoticz::sendSwitchCommand(unsigned long idx, const char *value) {
  char url[sizeof(serverURL) + 65];
  sprintf(url, "%sjson.htm?type=command&param=switchlight&idx=%u&switchcmd=%s",
          serverURL, idx, value);
  callURL(url);
}

void AFEDomoticz::callURL(const char *url) {
  Serial << endl << "INFO: calling url: " << url;

  http.begin(url);

  int httpCode = http.GET();
  Serial << endl << "Get: " << httpCode;
  String payload = http.getString();
  //  Serial << endl << payload;
  http.end();
}
