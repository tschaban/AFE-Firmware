/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-WiFi.h"

boolean AFEWiFi::eventConnectionEstablished = false;
boolean AFEWiFi::isConnected = false;
boolean AFEWiFi::eventConnectionLost = true;

AFEWiFi::AFEWiFi() {}

void AFEWiFi::begin(AFEDevice *_Device, AFEDataAccess *_Data) {
  Device = _Device;
  Data = _Data;

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

  // if (Device->getMode() == AFE_MODE_NORMAL || Device->getMode() ==
  // AFE_MODE_CONFIGURATION) {
  _Data->getConfiguration(configuration);
//}

#ifdef DEBUG
  Debugger->printInformation(F("Primary Network settings:"), F("WIFI"));
  Debugger->printBulletPoint(F("SSID: "));
  Debugger->printValue(configuration->primary.ssid);
  Debugger->printBulletPoint(F("IP: "));
  Debugger->printValue(configuration->primary.ip);
  Debugger->printBulletPoint(F("Gateway: "));
  Debugger->printValue(configuration->primary.gateway);
  Debugger->printBulletPoint(F("Subnet: "));
  Debugger->printValue(configuration->primary.subnet);
  Debugger->printBulletPoint(F("DNS1: "));
  Debugger->printValue(configuration->primary.dns1);
  Debugger->printBulletPoint(F("DNS2: "));
  Debugger->printValue(configuration->primary.dns2);

  Debugger->printInformation(F("Secondary Network Settings:"), F("WIFI"));
  Debugger->printBulletPoint(F("SSID: "));
  Debugger->printValue(configuration->secondary.ssid);
  Debugger->printBulletPoint(F("IP: "));
  Debugger->printValue(configuration->secondary.ip);
  Debugger->printBulletPoint(F("Gateway: "));
  Debugger->printValue(configuration->secondary.gateway);
  Debugger->printBulletPoint(F("Subnet: "));
  Debugger->printValue(configuration->secondary.subnet);
  Debugger->printBulletPoint(F("DNS1: "));
  Debugger->printValue(configuration->secondary.dns1);
  Debugger->printBulletPoint(F("DNS2: "));
  Debugger->printValue(configuration->secondary.dns2);

#endif

  /**
   * @brief Checking if backup configuration exists and setting a flag
   *
   */
  if (strlen(configuration->secondary.ssid) > 0 &&
      strcmp(configuration->secondary.ssid,
             AFE_CONFIG_NETWORK_DEFAULT_NONE_SSID) != 0 &&
      strlen(configuration->secondary.password) > 0) {
    isBackupConfigurationSet = true;
#ifdef DEBUG
    Debugger->printInformation(F("Bakcup configuration exist"), F("WIFI"));
  } else {
    Debugger->printInformation(F("Bakcup configuration does NOT exist"),
                               F("WIFI"));

#endif
  } /* Endif: Checking if backup configuration exists and setting a flag */

#ifdef DEBUG
  Debugger->printInformation(F("Device is in mode: "), F("WIFI"));
  Debugger->printValue(Device->getMode());
#endif

/**
 * @brief Setting WiFi Radio mode for ESP32 and the TX output power
 *
 */

#if !defined(ESP32)
  if (configuration->radioMode != AFE_NONE) {
    // wifi_set_phy_mode(configuration->radioMode);
    WirelessNetwork.setPhyMode(configuration->radioMode == 1
                                   ? WIFI_PHY_MODE_11B
                                   : configuration->radioMode == 2
                                         ? WIFI_PHY_MODE_11G
                                         : WIFI_PHY_MODE_11N);
#ifdef DEBUG
    Debugger->printInformation(F("Setting Radio mode (1:B 2:G 3:N) to: "),
                               F("WIFI"));
    Debugger->printValue(configuration->radioMode);
#endif
  }

  if (configuration->outputPower != AFE_NONE &&
      configuration->outputPower >=
          AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MIN &&
      configuration->outputPower <=
          AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MAX) {
    WirelessNetwork.setOutputPower(configuration->outputPower);
#ifdef DEBUG
    Debugger->printInformation(F("Setting TX Output power to: "), F("WIFI"));
    Debugger->printValue(configuration->outputPower, F("dBm"));
#endif
  }

#endif

  if (Device->getMode() == AFE_MODE_ACCESS_POINT ||
      Device->getMode() == AFE_MODE_NETWORK_NOT_SET) {
#ifdef DEBUG
    Debugger->printInformation(F("Starting..."), F("HotSpot"));
#endif

    WirelessNetwork.mode(WIFI_AP_STA);

    if (WirelessNetwork.softAPConfig(IPAddress(192, 168, 5, 1),
                                     IPAddress(192, 168, 5, 200),
                                     IPAddress(255, 255, 255, 0))) {

      WirelessNetwork.softAP("AFE Device");
#ifdef DEBUG
      Debugger->printValue(F("Ready"));
    } else {
      Debugger->printValue(F("Failed"));
#endif
    }
  } else {
    /* Add additional configuration parameters */
    switchConfiguration();
  }
#ifdef DEBUG
  Debugger->printInformation(F("Network initialized..."), F("BOOT"));
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
  if (isPrimaryConfiguration && !configuration->primary.isDHCP) {

    IPAddress dns1;
    if (!dns1.fromString(configuration->primary.dns1)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with DNS1: "), F("WIFI"));
      Debugger->printValue(configuration->primary.dns1);
#endif
    }
    IPAddress dns2;
    if (!dns2.fromString(configuration->primary.dns2)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with DNS2: "), F("WIFI"));
      Debugger->printValue(configuration->primary.dns2);
#endif
    }

    IPAddress ip;
    if (!ip.fromString(configuration->primary.ip)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI IP: "), F("WIFI"));
      Debugger->printValue(configuration->primary.ip);
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration->primary.gateway)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI gateway: "), F("WIFI"));
      Debugger->printValue(configuration->primary.gateway);
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration->primary.subnet)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI subnet: "), F("WIFI"));
      Debugger->printValue(configuration->primary.subnet);
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet, dns1, dns2);
#ifdef DEBUG
    Debugger->printInformation(F("Fixed IP set"), F("WIFI"));
#endif
  }
  /**
   * @brief set IPs to 0 for configuration over DHCP
   *
   */

  else if ((isPrimaryConfiguration && configuration->primary.isDHCP) ||
           (!isPrimaryConfiguration && configuration->secondary.isDHCP)) {
    WirelessNetwork.config((uint32_t)0x00000000, (uint32_t)0x00000000,
                           (uint32_t)0x00000000);
  }

  /**
   * @brief Setting fixed IP for backup WiFi configurations
   *
   */
  else if (!isPrimaryConfiguration && !configuration->secondary.isDHCP) {
#ifdef DEBUG
    Debugger->printInformation(F("Setting fixed IP ("), F("WIFI"));
    Debugger->printValue(configuration->secondary.ip);
    Debugger->printValue(F(") address for backup WiFi configuration"));
#endif

    IPAddress dns1;
    if (!dns1.fromString(configuration->secondary.dns1)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI DNS1: "), F("WIFI"));
      Debugger->printValue(configuration->secondary.dns1);
#endif
    }
    IPAddress dns2;
    if (!dns2.fromString(configuration->secondary.dns2)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI DNS2: "), F("WIFI"));
      Debugger->printValue(configuration->secondary.dns2);
#endif
    }

    IPAddress ip;
    if (!ip.fromString(configuration->secondary.ip)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI IP: "), F("WIFI"));
      Debugger->printValue(configuration->secondary.ip);
#endif
    }
    IPAddress gateway;
    if (!gateway.fromString(configuration->secondary.gateway)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI gateway: "), F("WIFI"));
      Debugger->printValue(configuration->secondary.gateway);
#endif
    }
    IPAddress subnet;
    if (!subnet.fromString(configuration->secondary.subnet)) {
#ifdef DEBUG
      Debugger->printError(F("Problem with WIFI subnet: "), F("WIFI"));
      Debugger->printValue(configuration->secondary.subnet);
#endif
    }

    WirelessNetwork.config(ip, gateway, subnet, dns1, dns2);
#ifdef DEBUG
    Debugger->printInformation(F("Fixed IP set"), F("WIFI"));
#endif
  } /* Endif: Setting Fixed IP for Primary Configuration if set */

  WirelessNetwork.mode(WIFI_STA);

#ifdef DEBUG
  Debugger->printInformation(
      (isPrimaryConfiguration ? F("Primary") : F("Backup")), F("WIFI"));
  Debugger->printValue(F(" configuration set"));
#endif
}

void AFEWiFi::listener() {
  if (!(Device->getMode() == AFE_MODE_ACCESS_POINT ||
        Device->getMode() == AFE_MODE_NETWORK_NOT_SET)) {
    if (!connected()) {
      if (sleepMode) {
        if (millis() - sleepStartTime >= configuration->waitTimeSeries * 1000) {
          sleepMode = false;
        }
      } else {
        if (delayStartTime == 0) {
          delayStartTime = millis();
          if (connections == 0) {

            /* Checking if WiFi is configured */
            if (strlen(configuration->primary.ssid) == 0 ||
                strlen(configuration->primary.password) == 0) {
#ifdef DEBUG
              Debugger->printError(
                  F("is not configured. Going to configuration mode"),
                  F("WIFI"));
#endif
              Device->reboot(AFE_MODE_NETWORK_NOT_SET);
            }

            if (isPrimaryConfiguration) {
              WirelessNetwork.begin(configuration->primary.ssid,
                                    configuration->primary.password);
            } else {
              WirelessNetwork.begin(configuration->secondary.ssid,
                                    configuration->secondary.password);
            }

#ifdef DEBUG

            Debugger->printInformation(
                F("Starting establishing WiFi connection"), F("WIFI"));
            Debugger->printBulletPoint(F("SSID: "));
            Serial << (isPrimaryConfiguration ? configuration->primary.ssid
                                              : configuration->secondary.ssid);

            Debugger->printBulletPoint(F("Auto Connected: "));
            Serial << WirelessNetwork.getAutoConnect();

            Debugger->printBulletPoint(F("Auto ReConnected: "));
            Serial << WirelessNetwork.getAutoReconnect();

            Debugger->printBulletPoint(F("Mode: "));
            Serial << WirelessNetwork.getMode();

#ifndef ESP32
            Debugger->printBulletPoint(F("Listen interval: "));
            Serial << WirelessNetwork.getListenInterval();

            Debugger->printBulletPoint(F("Persistent: "));
            Serial << WirelessNetwork.getPersistent();

            Debugger->printBulletPoint(F("PhyMode: "));
            Serial << WirelessNetwork.getPhyMode();

            Debugger->printBulletPoint(F("Sleep Mode: "));
            Serial << WirelessNetwork.getSleepMode();
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
            delayStartTime + (configuration->waitTimeConnections * 1000)) {
          connections++;

// yield();
// delay(10);
#ifdef DEBUG
          Debugger->printInformation(F("Connecting to: "), F("WIFI"));

          Serial << (isPrimaryConfiguration ? F("primary") : F("backup"))
                 << F(" router. Attempt: ") << connections << F("/")
                 << configuration->noConnectionAttempts << F(", IP(")
                 << WirelessNetwork.localIP() << F(")") << F(" WLStatus=")
                 << WirelessNetwork.status();
          if (isBackupConfigurationSet) {
            Serial << F(", Failures counter: ") << noOfFailures + 1 << F("/")
                   << configuration->noFailuresToSwitchNetwork;
          }
#endif
          delayStartTime = 0;
        }

        if (connections == configuration->noConnectionAttempts) {
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
          Debugger->printWarning(
              F("Not able to connect.Going to sleep mode for "), F("WIFI"));
          Debugger->printValue(configuration->waitTimeSeries, F("sec."));
#endif

          /**
           * @brief Switching configurations
           *
           */
          if (isBackupConfigurationSet) {
            noOfFailures++;
            if (noOfFailures == configuration->noFailuresToSwitchNetwork) {
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
        Debugger->printInformation(F("Setting hostname to: "), F("WIFI"));
        Debugger->printValue(Device->configuration.name);
#endif

        // yield();

        if (WirelessNetwork.hostname(Device->configuration.name)) {
// yield();
#ifdef DEBUG

          Debugger->printValue(F(" ... Success"));
        } else {
          Debugger->printValue(F(" ... Error"));
#endif
        }

#ifdef DEBUG

        Debugger->printHeader(2, 1, 40, AFE_DEBUG_HEADER_TYPE_DASH);
        Debugger->printValue(F("Connection established"));
        Debugger->printBulletPoint(F("MAC: "));
        Serial << WirelessNetwork.macAddress();
        Debugger->printBulletPoint(F("IP: "));
        Serial << WirelessNetwork.localIP();
#ifdef AFE_ESP32
        Debugger->printBulletPoint(F("IPv6: "));
        Serial << WirelessNetwork.localIPv6();
#endif
        Debugger->printBulletPoint(F("Subnet: "));
        Serial << WirelessNetwork.subnetMask();
#ifdef AFE_ESP32
        Serial << F("/") << WirelessNetwork.subnetCIDR();
#endif
        Debugger->printBulletPoint(F("Gateway: "));
        Serial << WirelessNetwork.gatewayIP();
#ifdef AFE_ESP32
        Debugger->printBulletPoint(F("Broadcast: "));
        Serial << WirelessNetwork.broadcastIP();
#endif
        Debugger->printBulletPoint(F("DNS1: "));
        Serial << WirelessNetwork.dnsIP(0);
        Debugger->printBulletPoint(F("DNS2: "));
        Serial << WirelessNetwork.dnsIP(1);
#ifdef AFE_ESP32
        Debugger->printBulletPoint(F("Network ID: "));
        Serial << WirelessNetwork.networkID();
#endif
        Debugger->printBulletPoint(F("RSSI: "));
        Serial << WirelessNetwork.RSSI();
#ifdef AFE_ESP32
        Debugger->printBulletPoint(F("Hostname: "));
        Serial << WirelessNetwork.getHostname();
#else
        Debugger->printBulletPoint(F("Hostname: "));
        Serial << WirelessNetwork.hostname();
#endif

        Debugger->printBulletPoint(F("LAN HTTP: http://"));
        char deviceIdExtended[AFE_CONFIG_DEVICE_ID_SIZE];
        Data->getDeviceID(deviceIdExtended, true);
        Serial << deviceIdExtended << F(".local");

        Debugger->printHeader(1, 1, 40, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
      }
    }
  }
}

boolean AFEWiFi::connected() {
  if (configuration->mDNSActive == AFE_CONFIG_NETWORK_MDNS_ACTIVE) {

#ifndef AFE_ESP32
    MDNS.update();
#else
// @TODO 3.8.0 what is ESP32 equivalent ESP32
#endif
  }

  return AFEWiFi::isConnected;
}

boolean AFEWiFi::eventConnected() {
  boolean returnValue = AFEWiFi::eventConnectionEstablished;

  if (AFEWiFi::eventConnectionEstablished == true) {
    if (configuration->mDNSActive == AFE_CONFIG_NETWORK_MDNS_ACTIVE) {
      char _mDNSDeviceID[AFE_CONFIG_DEVICE_ID_SIZE +
                         4]; // extended deviceId "afe-1234-5678"
      Data->getDeviceID(_mDNSDeviceID, true);

      if (MDNS.begin(_mDNSDeviceID)) {
#ifdef DEBUG
        Serial << endl << F("INFO: mDNS: Responder started: ") << _mDNSDeviceID;
#endif

        MDNS.addService("http", "tcp", 80);
#ifdef DEBUG
        Serial << endl << F("INFO: mDNS: HTTP service added");
      } else {
        Serial << endl << F("ERROR: mDNS: Responder not set");
#endif
      }
    }
    Data->addLog(F("wifi:connected"));
    AFEWiFi::eventConnectionEstablished = false;
  }

  return returnValue;
}

boolean AFEWiFi::eventDisconnected() {
  boolean returnValue = AFEWiFi::eventConnectionLost;

  if (returnValue) {
    Data->addLog(F("wifi:disconnected"));
  }

  AFEWiFi::eventConnectionLost = false;

  return returnValue;
}

#ifdef AFE_ESP32
void AFEWiFi::onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
  case SYSTEM_EVENT_STA_GOT_IP:
#ifdef DEBUG
    Serial << endl << F("INFO: WIFI: STA Got IP");
#endif
    AFEWiFi::eventConnectionEstablished = true;
    AFEWiFi::isConnected = true;
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
#ifdef DEBUG
//  Debugger->printInformation(F("Disconnected from Wi-Fi"), F("WIFI"));
#endif
    AFEWiFi::eventConnectionLost = true;
    AFEWiFi::isConnected = false;
    break;
  }
}

#else // ESP8266
void AFEWiFi::onWifiConnect(const WiFiEventStationModeGotIP &event) {
  AFEWiFi::eventConnectionEstablished = true;
  AFEWiFi::isConnected = true;
}

void AFEWiFi::onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
#ifdef DEBUG
  Serial << endl << F("WARN: WIFI: Disconnected");
#endif
  AFEWiFi::eventConnectionLost = true;
  AFEWiFi::isConnected = false;
}

#ifdef DEBUG

void AFEWiFi::onWiFiAPStationConnected(
    const WiFiEventSoftAPModeStationConnected &event) {

  Serial << endl
         << F("INFO: HotSpot: New device connected. (Connected: ")
         << WiFi.softAPgetStationNum() << F(" devices)");
}
#endif

#endif

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEWiFi::addReference(AFELED *_Led) { Led = _Led; }
#endif

#ifdef DEBUG
void AFEWiFi::addReference(AFEDebugger *_Debugger) { Debugger = _Debugger; }
#endif
