/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-WiFi.h"

boolean AFEWiFi::eventConnectionEstablished = false;
boolean AFEWiFi::isConnected = false;
boolean AFEWiFi::eventConnectionLost = true;

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

#ifdef AFE_ESP32
  WirelessNetwork.onEvent(AFEWiFi::onWiFiEvent);
#else
  wifiConnectHandler =
      WirelessNetwork.onStationModeGotIP(AFEWiFi::onWifiConnect);
  wifiDisconnectHandler =
      WirelessNetwork.onStationModeDisconnected(AFEWiFi::onWifiDisconnect);
#ifdef DEBUG
  wifiAPStationConnectedHandler = WirelessNetwork.onSoftAPModeStationConnected(
      AFEWiFi::onWiFiAPStationConnected);
#endif
#endif

  // if (WiFiMode == AFE_MODE_NORMAL || WiFiMode == AFE_MODE_CONFIGURATION) {
  _Data->getConfiguration(&configuration);
//}

#ifdef DEBUG
  Serial << endl
         << F("INFO: WIFI: Primary Network settings: ") << endl
         << F(" : SSID: ") << configuration.primary.ssid << endl
         << F(" : IP: ") << configuration.primary.ip << endl
         << F(" : Gateway: ") << configuration.primary.gateway << endl
         << F(" : Subnet: ") << configuration.primary.subnet << endl
         << F(" : DNS1: ") << configuration.primary.dns1 << endl
         << F(" : DNS2: ") << configuration.primary.dns2;

  Serial << endl
         << F("INFO: WIFI: Secondary Network Settings: ") << endl
         << F(" : SSID: ") << configuration.secondary.ssid << endl
         << F(" : IP: ") << configuration.secondary.ip << endl
         << F(" : Gateway: ") << configuration.secondary.gateway << endl
         << F(" : Subnet: ") << configuration.secondary.subnet << endl
         << F(" : DNS1: ") << configuration.secondary.dns1 << endl
         << F(" : DNS2: ") << configuration.secondary.dns2;

#endif

  /**
   * @brief Checking if backup configuration exists and setting a flag
   *
   */
  if (strlen(configuration.secondary.ssid) > 0 &&
      strcmp(configuration.secondary.ssid,
             AFE_CONFIG_NETWORK_DEFAULT_NONE_SSID) != 0 &&
      strlen(configuration.secondary.password) > 0) {
    isBackupConfigurationSet = true;
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Bakcup configuration exist.");
  } else {
    Serial << endl << F("INFO: WIFI: Bakcup configuration does NOT exist");
#endif
  } /* Endif: Checking if backup configuration exists and setting a flag */

#ifdef DEBUG
  Serial << endl << F("INFO: WIFI: Device is in mode: ") << WiFiMode;
#endif

/**
 * @brief Setting WiFi Radio mode for ESP32 and the TX output power
 *
 */

#if !defined(ESP32)
  if (configuration.radioMode != AFE_NONE) {
    // wifi_set_phy_mode(configuration.radioMode);
    WirelessNetwork.setPhyMode(configuration.radioMode == 1
                                   ? WIFI_PHY_MODE_11B
                                   : configuration.radioMode == 2
                                         ? WIFI_PHY_MODE_11G
                                         : WIFI_PHY_MODE_11N);
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Setting Radio mode (1:B 2:G 3:N) to: ")
           << configuration.radioMode;
#endif
  }

  if (configuration.outputPower != AFE_NONE &&
      configuration.outputPower >=
          AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MIN &&
      configuration.outputPower <=
          AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MAX) {
    WirelessNetwork.setOutputPower(configuration.outputPower);

#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Setting TX Output power to : ")
           << configuration.outputPower << F("dBm");
#endif
  }

#endif

#if defined(DEBUG) && !defined(ESP32)
  Serial << endl
         << F("INFO: WIFI: Phisical mode (1:B 2:G 3:N): ")
         << WirelessNetwork.getPhyMode();
#endif
  if (WiFiMode == AFE_MODE_ACCESS_POINT ||
      WiFiMode == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Serial << endl << F("INFO: HotSpot: Starting... ");
#endif

    WirelessNetwork.mode(WIFI_AP_STA);

    if (WirelessNetwork.softAPConfig(IPAddress(192, 168, 5, 1),
                                     IPAddress(192, 168, 5, 200),
                                     IPAddress(255, 255, 255, 0))) {

      WirelessNetwork.softAP("AFE Device");
#ifdef DEBUG
      Serial << endl << F("INFO: HotSpot: Ready");
      Serial << endl << F("INFO: HotSpot: IP: ") << WirelessNetwork.softAPIP();
    } else {
      Serial << endl << F("ERROR: HotSpot: Failed");
#endif
    }

  } else {
    /* Add additional configuration parameters */
    switchConfiguration();
  }
#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: Network initialized");
#endif
}

void AFEWiFi::switchConfiguration() {
  isPrimaryConfiguration = isPrimaryConfiguration ? false : true;
  noOfFailures = 0;
  WirelessNetwork.persistent(false);
  WirelessNetwork.disconnect(true);

#ifndef AFE_ESP32
  WirelessNetwork.setSleepMode(WIFI_NONE_SLEEP);
#else
  WiFi.setSleep(false);
#endif

  /**
   * @brief Setting Fixed IP for Primary Configuration if set
   *
   */
  if (isPrimaryConfiguration && !configuration.primary.isDHCP) {

    IPAddress dns1;
    if (!dns1.fromString(configuration.primary.dns1)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with DNS1: ")
             << configuration.primary.dns1;
#endif
    }
    IPAddress dns2;
    if (!dns2.fromString(configuration.primary.dns2)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with DNS2: ")
             << configuration.primary.dns2;
#endif
    }

    IPAddress ip;
    if (!ip.fromString(configuration.primary.ip)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI IP: ")
             << configuration.primary.ip;
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration.primary.gateway)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI gateway: ")
             << configuration.primary.gateway;
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration.primary.subnet)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI subnet: ")
             << configuration.primary.subnet;
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet, dns1, dns2);
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: Fixed IP set");
#endif
  }
  /**
   * @brief set IPs to 0 for configuration over DHCP
   *
   */

  else if ((isPrimaryConfiguration && configuration.primary.isDHCP) ||
           (!isPrimaryConfiguration && configuration.secondary.isDHCP)) {
    WirelessNetwork.config((uint32_t)0x00000000, (uint32_t)0x00000000,
                           (uint32_t)0x00000000);
  }

  /**
   * @brief Setting fixed IP for backup WiFi configurations
   *
   */
  else if (!isPrimaryConfiguration && !configuration.secondary.isDHCP) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: WIFI: Setting fixed IP (") << configuration.secondary.ip
           << F(") address for backup WiFi "
                "configuration");
#endif

    IPAddress dns1;
    if (!dns1.fromString(configuration.secondary.dns1)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with DNS1: ")
             << configuration.secondary.dns1;
#endif
    }
    IPAddress dns2;
    if (!dns2.fromString(configuration.secondary.dns2)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with DNS2: ")
             << configuration.secondary.dns2;
#endif
    }

    IPAddress ip;
    if (!ip.fromString(configuration.secondary.ip)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI IP: ")
             << configuration.secondary.ip;
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration.secondary.gateway)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI gateway: ")
             << configuration.secondary.gateway;
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration.secondary.subnet)) {
#ifdef DEBUG
      Serial << endl
             << F("ERROR: WIFI: Problem with WIFI subnet: ")
             << configuration.secondary.subnet;
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet, dns1, dns2);
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
            if (strlen(configuration.primary.ssid) == 0 ||
                strlen(configuration.primary.password) == 0) {
#ifdef DEBUG
              Serial
                  << endl
                  << F("ERROR: WiFI: is not configured. Going to configuration "
                       "mode");
#endif
              Device->reboot(AFE_MODE_NETWORK_NOT_SET);
            }

            if (isPrimaryConfiguration) {
              WirelessNetwork.begin(configuration.primary.ssid,
                                    configuration.primary.password);
            } else {
              WirelessNetwork.begin(configuration.secondary.ssid,
                                    configuration.secondary.password);
            }

#ifdef DEBUG
            Serial
                << endl
                << F("INFO: WIFI: Starting establishing WiFi connection to: ")
                << (isPrimaryConfiguration ? configuration.primary.ssid
                                           : configuration.secondary.ssid);

            Serial << endl
                   << F("INFO: WIFI: Parameters: ") << endl
                   << F(" - getAutoConnect=")
                   << WirelessNetwork.getAutoConnect() << endl
                   << F(" - getAutoReconnect=")
                   << WirelessNetwork.getAutoReconnect() << endl
                   << F(" - getMode=") << WirelessNetwork.getMode();

#ifndef ESP32
            Serial << endl
                   << F(" - getListenInterval=")
                   << WirelessNetwork.getListenInterval() << endl

                   << F(" - getPersistent=") << WirelessNetwork.getPersistent()
                   << endl
                   << F(" - getPhyMode=") << WirelessNetwork.getPhyMode()
                   << endl
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

// yield();
// delay(10);
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

          /**
           * @brief Switching configurations
           *
           */
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

        // yield();

        if (WirelessNetwork.hostname(Device->configuration.name)) {
// yield();
#ifdef DEBUG
          Serial << F(" ... Success");
        } else {
          Serial << F(" ... Error");
#endif
        }

#ifdef DEBUG
        Serial << endl
               << F("INFO: WIFI: Connection established") << endl
               << F(" : MAC: ") << WirelessNetwork.macAddress() << endl
               << F(" : IP: ") << WirelessNetwork.localIP() << endl
#ifdef AFE_ESP32
               << F(" : IPv6: ") << WirelessNetwork.localIPv6() << endl
#endif
               << F(" : Subnet Mask: ") << WirelessNetwork.subnetMask()
#ifdef AFE_ESP32
               << F("/") << WirelessNetwork.subnetCIDR()
#endif
               << endl
               << F(" : Gateway: ") << WirelessNetwork.gatewayIP() << endl
#ifdef AFE_ESP32
               << F(" : Broadcast: ") << WirelessNetwork.broadcastIP() << endl
#endif
               << F(" : DNS1: ") << WirelessNetwork.dnsIP(0) << endl
               << F(" : DNS2: ") << WirelessNetwork.dnsIP(1) << endl
#ifdef AFE_ESP32
               << F(" : Network ID: ") << WirelessNetwork.networkID() << endl
#endif
               << F(" : RSSI: ") << WirelessNetwork.RSSI() << endl
#ifdef AFE_ESP32
               << F(" : Hostname: ") << WirelessNetwork.getHostname();
#else
               << F(" : Hostname: ") << WirelessNetwork.hostname();
#endif
#endif
      }
    }
  }
}

boolean AFEWiFi::connected() { return AFEWiFi::isConnected; }

boolean AFEWiFi::eventConnected() {
  boolean returnValue = AFEWiFi::eventConnectionEstablished;
  AFEWiFi::eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEWiFi::eventDisconnected() {
  boolean returnValue = AFEWiFi::eventConnectionLost;
  AFEWiFi::eventConnectionLost = false;
  return returnValue;
}

#ifdef AFE_ESP32
void AFEWiFi::onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
  case SYSTEM_EVENT_STA_GOT_IP:
#ifdef DEBUG
    Serial << endl << F("INFO: WiFi: Disconnected from Wi-Fi");
#endif
    AFEWiFi::eventConnectionEstablished = true;
    AFEWiFi::isConnected = true;
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
#ifdef DEBUG
    Serial << endl << F("INFO: WiFi: Disconnected from Wi-Fi");
#endif
    AFEWiFi::eventConnectionLost = true;
    AFEWiFi::isConnected = false;
    break;
  }
}

#else // ESP8266
void AFEWiFi::onWifiConnect(const WiFiEventStationModeGotIP &event) {
#ifdef DEBUG
  Serial << endl << F("INFO: WiFi: Connected to Wi-Fi");
#endif
  AFEWiFi::eventConnectionEstablished = true;
  AFEWiFi::isConnected = true;
}

void AFEWiFi::onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
#ifdef DEBUG
  Serial << endl << F("INFO: WiFi: Disconnected from Wi-Fi");
#endif
  AFEWiFi::eventConnectionLost = true;
  AFEWiFi::isConnected = false;
}

#ifdef DEBUG
void AFEWiFi::onWiFiAPStationConnected(
    const WiFiEventSoftAPModeStationConnected &event) {
#ifdef DEBUG
  Serial << endl
         << F("INFO: WiFi: New device connected to HotSpot") << endl
         << F("INFO: HotSpot: Number of devices connected: ")
         << WiFi.softAPgetStationNum();
#endif
}
#endif

#endif
