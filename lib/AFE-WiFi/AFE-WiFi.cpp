#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

void AFEWiFi::connect(const char* ssid, const char* password) {
    uint8_t connection_try = 0;
    WiFi.hostname("TEST");
    WiFi.begin(ssid,password);
    Serial << endl << "INFO: WiFi connection status: " << WiFi.status();
    while (WiFi.status() != WL_CONNECTED) {
      Serial << endl << "INFO: WiFi connection attempt: " << connection_try << " from " << _no_connection_attempts;
      connection_try++;
      delay(_duration_between_connection_attempts * 1000);
      if (connection_try == _no_connection_attempts) {
      _sleepMode = true;
      break;
   }
 }
}
