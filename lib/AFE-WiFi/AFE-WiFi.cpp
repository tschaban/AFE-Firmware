#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

void AFEWiFi::begin(const char* network_ssid, const char* network_password, const char* network_hostname) {
  ssid = network_ssid;
  password = network_password;
  WiFi.hostname(network_hostname);
}

void AFEWiFi::setReconnectionParams(
  uint8_t no_connection_attempts,
  uint8_t duration_between_connection_attempts,
  uint8_t duration_between_next_connection_attempts_series) {
    noConnectionAttempts = no_connection_attempts;
    durationBetweenConnectionAttempts = duration_between_connection_attempts;
    durationBetweenNextConnectionAttemptsSeries = duration_between_next_connection_attempts_series;
}


void AFEWiFi::connect() {

  if (sleepMode) {
    if (millis() - sleepStartTime >= durationBetweenNextConnectionAttemptsSeries*1000) {
      sleepMode = false;
    }
  } else {
    uint8_t connections = 0;
    WiFi.begin(ssid,password);
    Serial << endl << "INFO: WiFi connection status: " << WiFi.status();
    while (WiFi.status() != WL_CONNECTED) {
      Serial << endl << "INFO: WiFi connection attempt: " << connections+1 << " from " << noConnectionAttempts;
      connections++;
      delay(durationBetweenConnectionAttempts * 1000);
      if (connections == noConnectionAttempts) {
        sleepMode = true;
        WiFi.disconnect();
        sleepStartTime = millis();
        Serial << endl << "WARN: Not able to connect.Going to sleep mode for " << durationBetweenNextConnectionAttemptsSeries << "sec.";
        break;
      }
    }
  }
}

boolean AFEWiFi::connected() {
   if (WiFi.status() == WL_CONNECTED) {
     return true;
   } else {
     return false;
   }
}
