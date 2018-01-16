/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-MQTT.h"
#include <AFE-Configuration.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <AFE-PIR.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
//#include <Streaming.h>

AFEDataAccess Data;
AFEDevice Device;
AFEWiFi Network;
AFEMQTT Mqtt;
AFEWebServer WebServer;
AFELED Led;
AFESwitch Switch[sizeof(Device.configuration.isSwitch)];
AFERelay Relay[sizeof(Device.configuration.isRelay)];
AFEPIR Pir[sizeof(Device.configuration.isPIR)];
MQTT MQTTConfiguration;

void setup() {

  Serial.begin(115200);
  delay(10);

  /* Turn off publishing information to Serial */
  Serial.swap();

  /* Checking if the device is launched for a first time. If so it sets up
   * the device (EEPROM) */
  if (Device.isFirstTimeLaunch()) {
    Device.setDevice();
  }

  /* Perform post upgrade changes (if any) */
  AFEUpgrader Upgrader;
  if (Upgrader.upgraded()) {
    Upgrader.upgrade();
  }
  Upgrader = {};

  /* Checking if WiFi is onfigured, if not than it runs access point mode */
  if (Device.getMode() != MODE_ACCESS_POINT && !Device.isConfigured()) {
    Device.reboot(MODE_ACCESS_POINT);
  }

  /* Initializing relay and setting it's default state at power on*/
  if (Device.getMode() == MODE_NORMAL) {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        Relay[i].begin(i);
        Relay[i].setRelayAfterRestoringPower();
      }
    }
  }
  /* Initialzing network */
  Network.begin(Device.getMode());

  /* Initializing LED, checking if LED exists is made on Class level  */
  Led.begin(0);

  /* If device in configuration mode then start LED blinking */
  if (Device.getMode() != MODE_NORMAL) {
    Led.blinkingOn(100);
  }
  /* Initializing switches */
  initSwitch();
  /* Initializing PIRs */
  initPIR();
  /* Initializing MQTT */
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.mqttAPI) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
  }
  Network.connect();
  /* Initializing HTTP WebServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
}

void loop() {

  if (Device.getMode() != MODE_ACCESS_POINT) {
    if (Network.connected()) {
      if (Device.getMode() == MODE_NORMAL) {
        /* Connect to MQTT if not connected */
        if (Device.configuration.mqttAPI) {
          Mqtt.connected() ? Mqtt.loop() : Mqtt.connect();
        }
        WebServer.listener();

        mainHTTPRequestsHandler();
        mainSwitch();
        mainPIR();

      } else { // Configuration Mode
        WebServer.listener();
      }
    } else { // Device not connected to WiFi. Reestablish connection
      Network.connect();
    }
  } else { // Access Point Mode
    Network.APListener();
    WebServer.listener();
  }

  mainSwitchListener();

  Led.loop();
}
