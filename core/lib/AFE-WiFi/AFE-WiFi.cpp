/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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
    _Data->getConfiguration(&configuration);
  }

  /* Checking if backup configuration exists and setting a flag */
  if (strlen(configuration.ssidBackup) > 0 &&
      strcmp(configuration.ssidBackup, AFE_CONFIG_NETWORK_DEFAULT_NONE_SSID) !=
          0 &&
      strlen(configuration.passwordBackup) > 0) {
    isBackupConfigurationSet = true;
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Bakcup configuration exist. SSID: ")
           << configuration.ssidBackup;
  } else {
    Serial << endl << F("INFO: WIFI: Bakcup configuration does NOT exist");
#endif
  } /* Endif: Checking if backup configuration exists and setting a flag */

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Device is in mode: ") << WiFiMode;
#endif

#if defined(DEBUG) && !defined(ESP32)
  Serial << endl
         << F("INFO: WIFI: Phisical mode (1:B 2:G 3:N): ")
         << WirelessNetwork.getPhyMode();
#endif

  if (WiFiMode == AFE_MODE_ACCESS_POINT ||
      WiFiMode == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Starting HotSpot: ");
#endif
    IPAddress apIP(192, 168, 5, 1);
    WirelessNetwork.mode(WIFI_AP_STA);
    WirelessNetwork.softAP("AFE Device");
    WirelessNetwork.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
#ifdef AFE_ESP32
    WirelessNetwork.softAPsetHostname(Device->configuration.name);
#else
    WirelessNetwork.hostname(Device->configuration.name);
#endif
#ifdef DEBUG
    Serial << F("completed");
#endif
  } else {
    /* Add additional configuration parameters */
    switchConfiguration();
  }
}

void AFEWiFi::switchConfiguration() {
  isPrimaryConfiguration = isPrimaryConfiguration ? false : true;
  noOfFailures = 0;
  WirelessNetwork.persistent(false);
  WirelessNetwork.disconnect(true);
  if (WirelessNetwork.hostname(Device->configuration.name)) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Hostname set to: ") << Device->configuration.name;
  } else {
    Serial << endl
           << F("ERROR: WIFI: Hostname NOT set: ")
           << Device->configuration.name;
#endif
  }

#ifndef AFE_ESP32
  WirelessNetwork.setSleepMode(WIFI_NONE_SLEEP);
#else
  WiFi.setSleep(false);
#endif

  /* Setting Fixed IP for Primary Configuration if set */
  if (isPrimaryConfiguration && !configuration.isDHCP) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Setting fixed IP (")
                << configuration.ip << F(")  address for primary WiFi "
                                       "configuration");
#endif
    IPAddress ip;
    if (!ip.fromString(configuration.ip)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI IP address: ")
             << configuration.ip;
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration.gateway)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI gateway address: ")
             << configuration.gateway;
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration.subnet)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI subnet address: ")
             << configuration.subnet;
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet);
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Fixed IP set");
#endif
  } else if ((isPrimaryConfiguration && configuration.isDHCP) ||
             (!isPrimaryConfiguration && configuration.isDHCPBackup)) {
    WirelessNetwork.config((uint32_t)0x00000000, (uint32_t)0x00000000,
                           (uint32_t)0x00000000);
  } else if (!isPrimaryConfiguration && !configuration.isDHCPBackup) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Setting fixed IP (") << configuration.ipBackup
                                                 << F(") address for backup WiFi "
                                                    "configuration");
#endif

    IPAddress ip;
    if (!ip.fromString(configuration.ipBackup)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI IP address: ")
             << configuration.ipBackup;
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration.gatewayBackup)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI gateway address: ")
             << configuration.gatewayBackup;
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration.subnetBackup)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI subnet address: ")
             << configuration.subnetBackup;
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet);
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Fixed IP set");
#endif
  } /* Endif: Setting Fixed IP for Primary Configuration if set */

  WirelessNetwork.mode(WIFI_STA);

#ifdef DEBUG
  Serial << endl
         << F("INFO: WIFI: ")
         << (isPrimaryConfiguration ? F("Primary") : F("Backup"))
         << F(" configuration set");
#endif
}

void AFEWiFi::listener() {
  if (!(WiFiMode == AFE_MODE_ACCESS_POINT ||
        WiFiMode == AFE_MODE_NETWORK_NOT_SET)) {
    if (!connected()) {
      if (sleepMode) {
        if (millis() - sleepStartTime >= configuration.waitTimeSeries * 1000) {
          sleepMode = false;
        }
      } else {
        if (delayStartTime == 0) {
          delayStartTime = millis();
          if (connections == 0) {

            /* Checking if WiFi is configured */
            if (strlen(configuration.ssid) == 0 ||
                strlen(configuration.password) == 0) {
#ifdef DEBUG
              Serial
                  << endl
                  << F("ERROR: WiFI: is not configured. Going to configuration "
                       "mode");
#endif
              Device->reboot(AFE_MODE_NETWORK_NOT_SET);
            }

            if (isPrimaryConfiguration) {
              WirelessNetwork.begin(configuration.ssid, configuration.password);
            } else {
              WirelessNetwork.begin(configuration.ssidBackup,
                                    configuration.passwordBackup);
            }

#ifdef DEBUG
            Serial
                << endl
                << F("INFO: WIFI: Starting establishing WiFi connection to: ")
                << (isPrimaryConfiguration ? configuration.ssid
                                           : configuration.ssidBackup);

            Serial << endl
                   << F("INFO: WIFI: Parameters: ") << endl
                   << F(" - getAutoConnect=") << WirelessNetwork.getAutoConnect()
                   << endl
                   << F(" - getAutoReconnect=")
                   << WirelessNetwork.getAutoReconnect() << endl
                   << F(" - getMode=") << WirelessNetwork.getMode();

#ifndef ESP32
            Serial << endl
                   << F(" - getListenInterval=")
                   << WirelessNetwork.getListenInterval() << endl

                   << F(" - getPersistent=") << WirelessNetwork.getPersistent()
                   << endl
                   << F(" - getPhyMode=") << WirelessNetwork.getPhyMode() << endl
                   << F(" - getSleepMode=") << WirelessNetwork.getSleepMode();
#endif // !ESP32
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

        if (millis() >
            delayStartTime + (configuration.waitTimeConnections * 1000)) {
          connections++;

//          yield();
          delay(10);
#ifdef DEBUG
          Serial << endl
                 << F("INFO: WIFI: Connection to ")
                 << (isPrimaryConfiguration ? F("primary") : F("backup"))
                 << F(" router. Attempt: ") << connections << F("/")
                 << configuration.noConnectionAttempts << F(", IP(")
                 << WirelessNetwork.localIP() << F(")") << F(" WL-Status=")
                 << WirelessNetwork.status();
          if (isBackupConfigurationSet) {
            Serial << F(", Failures counter: ") << noOfFailures + 1 << F("/")
                   << configuration.noFailuresToSwitchNetwork;
          }
#endif
          delayStartTime = 0;
        }

        if (connections == configuration.noConnectionAttempts) {
          sleepMode = true;
          WirelessNetwork.disconnect();
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
              << configuration.waitTimeSeries << F("sec.");
#endif

          /* Switching configurations */
          if (isBackupConfigurationSet) {
            noOfFailures++;
            if (noOfFailures == configuration.noFailuresToSwitchNetwork) {
              switchConfiguration();
            }
          } /* Endif: Switching configurations */
        }
      }
    } else {
      if (connections > 0) {
        connections = 0;
        noOfFailures = 0;
        delayStartTime = 0;

#ifdef AFE_CONFIG_HARDWARE_LED
        ledStartTime = 0;
        Led->off();
#endif

#ifdef DEBUG
        Serial << endl
               << F("INFO: WIFI: Setting hostname to: ")
               << Device->configuration.name;
#endif

    //    yield();

        if (WirelessNetwork.hostname(Device->configuration.name)) {
   //       yield();
#ifdef DEBUG
          Serial << F(" ... Success");
        } else {
          Serial << F(" ... Error");
#endif
        }

#ifdef DEBUG
        Serial << endl
               << F("INFO: WIFI: Connection established") << F(", MAC: ")
               << WirelessNetwork.macAddress() << F(", IP: ")
               << WirelessNetwork.localIP();
#endif
      }
    }
  }
}

boolean AFEWiFi::connected() {

#ifndef AFE_ESP32 /* ESP82xx */
  if ((((isPrimaryConfiguration && configuration.isDHCP) ||
        (!isPrimaryConfiguration && configuration.isDHCPBackup)) &&
       WirelessNetwork.localIP().toString() != "(IP unset)") ||
      (((isPrimaryConfiguration && !configuration.isDHCP) ||
        (!isPrimaryConfiguration && !configuration.isDHCPBackup)) &&
       WirelessNetwork.status() == WL_CONNECTED)) {
 //   yield();
    delay(10);
#else /* ESP32 */
  if (WiFi.status() == WL_CONNECTED) {
#endif

    if (disconnected) {
      eventConnectionEstablished = true;
      eventConnectionLost = false;
      disconnected = false;
    }
    return true;
  } else {
    if (!disconnected) {
      eventConnectionLost = true;
      eventConnectionEstablished = false;
    }
    disconnected = true;
    return false;
  }
}

boolean AFEWiFi::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEWiFi::eventDisconnected() {
  boolean returnValue = eventConnectionLost;
  eventConnectionLost = false;
  return returnValue;
}
