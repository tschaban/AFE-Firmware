/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

void AFEWiFi::begin(uint8_t mode) {

  AFEDataAccess Data;
  AFEDevice Device;
  networkConfiguration = Data.getNetworkConfiguration();
  // Init LED
  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0) {
    Led.begin(systeLedID - 1);
  }

  // Cleaning @TODO is it neded?
  Data = {};
  WiFi.hostname(Device.configuration.name);
  if (mode == MODE_ACCESS_POINT) {
    IPAddress apIP(192, 168, 5, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(Device.configuration.name);
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(53, "www.example.com", apIP);
  } else {
    if (!networkConfiguration.isDHCP) {
      WiFi.config(networkConfiguration.ip, networkConfiguration.gateway,
                  networkConfiguration.subnet);
    }
    WiFi.mode(WIFI_STA);
  }
}

void AFEWiFi::listener() {
  if (!connected()) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          networkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {
      if (delayStartTime == 0) {
        delayStartTime = millis();
        if (connections == 0) {
          WiFi.begin(networkConfiguration.ssid, networkConfiguration.password);
          /*
                    Serial << endl << "INFO: Starting establishing WiFi
             connection"; Serial << endl
                           << networkConfiguration.ssid << "@"
                           << networkConfiguration.password;
          */
        }
      }

      if (ledStartTime == 0) {
        ledStartTime = millis();
      }

      if (millis() > ledStartTime + 500) {
        Led.toggle();
        ledStartTime = 0;
      }

      if (millis() >
          delayStartTime + (networkConfiguration.waitTimeConnections * 1000)) {
        connections++;
        /*
                Serial << endl
                       << "INFO: WiFi connection attempt: " << connections + 1
                       << " from " << networkConfiguration.noConnectionAttempts;
                       */
        delayStartTime = 0;
      }

      if (connections == networkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        WiFi.disconnect();
        sleepStartTime = millis();
        delayStartTime = 0;
        ledStartTime = 0;
        Led.off();
        connections = 0;
        /*
                Serial << endl
                       << "WARN: Not able to connect.Going to sleep mode for "
                       << networkConfiguration.waitTimeSeries << "sec.";
                       */
      }
    }
  } else {
    if (connections > 0) {
      connections = 0;
      delayStartTime = 0;
      ledStartTime = 0;
      Led.off();
      /*
      Serial << endl
             << "INFO: Connection established"
             << ", MAC: " << WiFi.macAddress() << ", IP: " << WiFi.localIP();
             */
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

void AFEWiFi::APListener() { dnsServer.processNextRequest(); }

boolean AFEWiFi::eventConnected() {
   boolean returnValue = eventConnectionEstablished;
   eventConnectionEstablished = false;
   return returnValue;
}
