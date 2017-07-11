#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {
}

void AFEWiFi::begin(const char* network_ssid, const char* network_password, const char* network_hostname, uint8_t mode) {
        ssid = network_ssid;
        password = network_password;
        WiFi.hostname(network_hostname);
        if (mode==WIFI_MODE_AP) {
                IPAddress apIP(192, 168, 5, 1);
                WiFi.mode(WIFI_AP);
                WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
                WiFi.softAP(network_hostname);
                dnsServer.setTTL(300);
                dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
                dnsServer.start(53, "www.example.com", apIP);
        } else {

          IPAddress ip(192, 168, 0, 126);
          IPAddress gateway(192, 168, 0, 1);
          IPAddress subnet(255, 255, 255, 0);
          WiFi.config(ip, gateway, subnet);


          WiFi.mode(WIFI_STA);
        }
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
