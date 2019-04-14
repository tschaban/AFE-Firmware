/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

void AFEWiFi::begin(uint8_t mode, AFEDevice *_Device) {

  AFEDataAccess Data;
  Device = _Device;
  WiFiMode = mode;
  if (WiFiMode == MODE_NORMAL || WiFiMode == MODE_CONFIGURATION) {
    networkConfiguration = Data.getNetworkConfiguration();
  }

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  // Init LED
  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0 && Device->configuration.isLED[systeLedID - 1]) {
    Led.begin(systeLedID - 1);
  }
#endif

#ifdef DEBUG
  Serial << endl << "Device is in mode: " << WiFiMode;
#endif

  WiFi.hostname(Device->configuration.name);
  if (WiFiMode == MODE_ACCESS_POINT || WiFiMode == MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << endl << "Starting HotSpot: ";
#endif
    IPAddress apIP(192, 168, 5, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(Device->configuration.name);
    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(53, "*", apIP);
#ifdef DEBUG
    Serial << "completed";
#endif
  } else {
#ifdef DEBUG
    Serial << endl << "Starting WiFi: in WIFI_STA mode";
#endif
    if (!networkConfiguration.isDHCP) {
      WiFi.config(networkConfiguration.ip, networkConfiguration.gateway,
                  networkConfiguration.subnet);
    }
    WiFi.mode(WIFI_STA);
  }
}

void AFEWiFi::listener() {
  if (!(WiFiMode == MODE_ACCESS_POINT || WiFiMode == MODE_NETWORK_NOT_SET)) {
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

            /* Checking if WiFi is configured */
            if (strlen(networkConfiguration.ssid) == 0 ||
                strlen(networkConfiguration.password) == 0) {
#ifdef DEBUG
              Serial << endl
                     << "WiFI is not configured. Going to configuration mode";
#endif
              Device->reboot(MODE_NETWORK_NOT_SET);
            }

            WiFi.begin(networkConfiguration.ssid,
                       networkConfiguration.password);
#ifdef DEBUG
            Serial << endl << "INFO: Starting establishing WiFi connection ";
            Serial << endl
                   << networkConfiguration.ssid << " - "
                   << networkConfiguration.password;
#endif
          }
        }

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        if (ledStartTime == 0) {
          ledStartTime = millis();
        }

        if (millis() > ledStartTime + 500) {
          Led.toggle();
          ledStartTime = 0;
        }
#endif

        if (millis() > delayStartTime +
                           (networkConfiguration.waitTimeConnections * 1000)) {
          connections++;
#ifdef DEBUG
          Serial << endl
                 << "INFO: WiFi connection attempt: " << connections << " from "
                 << networkConfiguration.noConnectionAttempts << ", IP("
                 << WiFi.localIP() << ")";
#endif
          delayStartTime = 0;
        }

        if (connections == networkConfiguration.noConnectionAttempts) {
          sleepMode = true;
          WiFi.disconnect();
          sleepStartTime = millis();
          delayStartTime = 0;

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
          ledStartTime = 0;
          Led.off();
#endif

          connections = 0;
#ifdef DEBUG
          Serial << endl
                 << "WARN: Not able to connect.Going to sleep mode for "
                 << networkConfiguration.waitTimeSeries << "sec.";
#endif
        }
      }
    } else {
      if (connections > 0) {
        connections = 0;
        delayStartTime = 0;

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
        ledStartTime = 0;
        Led.off();
#endif

#ifdef DEBUG
        Serial << endl
               << "INFO: Connection established"
               << ", MAC: " << WiFi.macAddress() << ", IP: " << WiFi.localIP();
#endif
      }
    }
  }
}
boolean AFEWiFi::connected() {
  // Serial << WiFi.status();
  //  if (WiFi.status() == WL_CONNECTED) {
  if (WiFi.localIP().toString() != "0.0.0.0") {

    delay(1);
    if (disconnected) {
      eventConnectionEstablished = true;
      disconnected = false;
    }
    return true;
  } else {
    disconnected = true;
    return false;
  }
}

void AFEWiFi::APListener() { dnsServer.processNextRequest(); }

boolean AFEWiFi::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}
