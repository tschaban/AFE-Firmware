/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Wifi.h"

AFEWiFi::AFEWiFi() {}

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWiFi::begin(uint8_t mode, AFEDevice *_Device, AFEDataAccess *_Data,
                    AFELED *_LED) {
  Led = _LED;
  begin(mode, _Device, _Data);
}
#endif

void AFEWiFi::begin(uint8_t mode, AFEDevice *_Device, AFEDataAccess *_Data) {
  Device = _Device;
  WiFiMode = mode;

  if (WiFiMode == AFE_MODE_NORMAL || WiFiMode == AFE_MODE_CONFIGURATION) {
    _Data->getConfiguration(&networkConfiguration);
  }

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Device is in mode: ") << WiFiMode;
#endif
  if (WiFi.hostname(Device->configuration.name)) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Hostname set: ") << Device->configuration.name;
  } else {
    Serial << endl
           << F("ERROR: WIFI: Hostname NOT set: ")
           << Device->configuration.name;
#endif
  }

  WiFi.setSleepMode(WIFI_NONE_SLEEP);
// WiFi.setOutputPower(10.0);
// WiFi.setPhyMode(WIFI_PHY_MODE_11B);
#ifdef DEBUG
  Serial << endl
         << F("INFO: WIFI: Phisical mode mode (1:B 2:G 3:N): ")
         << WiFi.getPhyMode();
#endif

  if (WiFiMode == AFE_MODE_ACCESS_POINT ||
      WiFiMode == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Starting HotSpot: ");
#endif
    IPAddress apIP(192, 168, 5, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(Device->configuration.name);
#ifdef DEBUG
    Serial << F("completed");
#endif
  } else {
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Starting WiFi: in WIFI_STA mode");
#endif
    if (!networkConfiguration.isDHCP) {
      IPAddress ip;
      if (!ip.fromString(networkConfiguration.ip)) {
#ifdef DEBUG
        Serial << endl
               << F("ERROR: WIFI: Problem with WIFI IP address: ")
               << networkConfiguration.ip;
#endif
      }
      IPAddress gateway;
      if (!gateway.fromString(networkConfiguration.gateway)) {
#ifdef DEBUG
        Serial << endl
               << F("ERROR: WIFI: Problem with WIFI gateway address: ")
               << networkConfiguration.gateway;
#endif
      }
      IPAddress subnet;
      if (!subnet.fromString(networkConfiguration.subnet)) {
#ifdef DEBUG
        Serial << endl
               << F("ERROR: WIFI: Problem with WIFI subnet address: ")
               << networkConfiguration.subnet;
#endif
      }

      WiFi.config(ip, gateway, subnet);
#ifdef DEBUG
      Serial << endl << F("INFO: WIFI: Settting fixed IP");
#endif
    }

    // WiFi.persistent(false);
    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
  }
}

void AFEWiFi::listener() {
  if (!(WiFiMode == AFE_MODE_ACCESS_POINT ||
        WiFiMode == AFE_MODE_NETWORK_NOT_SET)) {
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
              Serial
                  << endl
                  << F("ERROR: WiFI: is not configured. Going to configuration "
                       "mode");
#endif
              Device->reboot(AFE_MODE_NETWORK_NOT_SET);
            }

            WiFi.begin(networkConfiguration.ssid,
                       networkConfiguration.password);
#ifdef DEBUG
            Serial
                << endl
                << F("INFO: WIFI: Starting establishing WiFi connection to: ")
                << networkConfiguration.ssid;
#endif
          }
        }

#ifdef AFE_CONFIG_HARDWARE_LED
        if (ledStartTime == 0) {
          ledStartTime = millis();
        }

        if (millis() > ledStartTime + 500) {
          Led->toggle();
          ledStartTime = 0;
        }
#endif

        if (millis() > delayStartTime +
                           (networkConfiguration.waitTimeConnections * 1000)) {
          connections++;
          delay(10);
#ifdef DEBUG
          Serial << endl
                 << F("INFO: WIFI connection attempt: ") << connections
                 << F(" from ") << networkConfiguration.noConnectionAttempts
                 << F(", IP(") << WiFi.localIP() << F(")") << F(" WL-Status=")
                 << WiFi.status();
#endif
          delayStartTime = 0;
        }

        if (connections == networkConfiguration.noConnectionAttempts) {
          sleepMode = true;
          WiFi.disconnect();
          sleepStartTime = millis();
          delayStartTime = 0;

#ifdef AFE_CONFIG_HARDWARE_LED
          ledStartTime = 0;
          Led->off();
#endif

          connections = 0;
#ifdef DEBUG
          Serial
              << endl
              << F("WARN: WIFI: Not able to connect.Going to sleep mode for ")
              << networkConfiguration.waitTimeSeries << F("sec.");
#endif
        }
      }
    } else {
      if (connections > 0) {
        connections = 0;
        delayStartTime = 0;

#ifdef AFE_CONFIG_HARDWARE_LED
        ledStartTime = 0;
        Led->off();
#endif

#ifdef DEBUG
        Serial << endl
               << F("INFO: WIFI: Connection established") << F(", MAC: ")
               << WiFi.macAddress() << F(", IP: ") << WiFi.localIP();
#endif
      }
    }
  }
}

boolean AFEWiFi::connected() {
  if ((networkConfiguration.isDHCP &&
       WiFi.localIP().toString() != "(IP unset)") ||
      (!networkConfiguration.isDHCP && WiFi.status() == WL_CONNECTED)) {

    // if (WiFi.waitForConnectResult() == WL_NETWORK_CONNECTED) {

    delay(10);
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

boolean AFEWiFi::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}

uint16_t AFEWiFi::getJSON(const String &url, String &response) {
  http.begin(url);
  uint16_t httpCode = http.GET();
  if (httpCode == 200) {
    response = http.getString();
  } else {
    response = "";
  }
  http.end();
  return httpCode;
}