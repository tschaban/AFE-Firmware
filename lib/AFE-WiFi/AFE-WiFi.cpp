#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

void AFEWiFi::begin(uint8_t mode) {

  AFEDataAccess Data;
  LED LEDConfiguration;

  // Reading configuration from the EEPROM
  LEDConfiguration = Data.getLEDConfiguration();
  networkConfiguration = Data.getNetworkConfiguration();

  // Init LED
  if (LEDConfiguration.present) {
    Led.begin(LEDConfiguration.gpio);
  }

  // Cleaning @TODO is it neded?
  Data = {};
  LEDConfiguration = {};

  WiFi.hostname(networkConfiguration.host);
  if (mode == MODE_ACCESS_POINT) {
    Serial << endl << "INFO: Device mode: Access Point Configuration";
    Serial << endl << "INFO: launching access point";
    IPAddress apIP(192, 168, 5, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(networkConfiguration.host);
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(53, "www.example.com", apIP);
    Serial << endl << "INFO: Open: http://192.168.5.1/  ";
  } else {
    if (!networkConfiguration.isDHCP) {
      WiFi.config(networkConfiguration.ip, networkConfiguration.gateway,
                  networkConfiguration.subnet);
    }
    WiFi.mode(WIFI_STA);
  }
}

void AFEWiFi::connect() {

  if (sleepMode) {
    if (millis() - sleepStartTime >=
        networkConfiguration.waitTimeSeries * 1000) {
      sleepMode = false;
    }
  } else {
    uint8_t connections = 0;
    WiFi.begin(networkConfiguration.ssid, networkConfiguration.password);
    Serial << endl << "INFO: WiFi connection status: " << WiFi.status();
    while (WiFi.status() != WL_CONNECTED) {
      Led.on();
      Serial << endl
             << "INFO: WiFi connection attempt: " << connections + 1 << " from "
             << networkConfiguration.noConnectionAttempts;
      connections++;
      delay(networkConfiguration.waitTimeConnections * 500);
      Led.off();
      delay(networkConfiguration.waitTimeConnections * 500);
      if (connections == networkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        WiFi.disconnect();
        sleepStartTime = millis();
        Serial << endl
               << "WARN: Not able to connect.Going to sleep mode for "
               << networkConfiguration.waitTimeSeries << "sec.";
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

void AFEWiFi::DNSListener() { dnsServer.processNextRequest(); }
