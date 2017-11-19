/*
  AFE Firmware for smart home devices build on ESP8266
  Version: T0
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#include "AFE-MQTT.h"
#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <Streaming.h>

AFEDataAccess Data;
AFEDevice Device;
AFEWiFi Network;
AFEMQTT Mqtt;
AFEWebServer WebServer;
AFELED Led;
AFESwitch Switch;
AFESwitch ExternalSwitch;

AFERelay Relay;

MQTT MQTTConfiguration;

void setup() {

  Serial.begin(115200);
  delay(10);

  // Comment below if you needed to debug it through Serial
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
    RELAY RelayConfiguration;
    RelayConfiguration = Data.getRelayConfiguration(0);
    if (RelayConfiguration.present) {
      Relay.begin(0);
      Relay.setRelayAfterRestoringPower();
    }
  }

  /* Initialzing network */
  Network.begin(Device.getMode());

  /* Initializing LED */
  Led.begin(0);
  /* If device in configuration mode then start LED blinking */
  if (Device.getMode() != MODE_NORMAL) {
    Led.blinkingOn(100);
  }

  /* Initializing Switches */
  Switch.begin(0);
  ExternalSwitch.begin(1);

  /* Initializing MQTT */
  if (Device.getMode() != MODE_ACCESS_POINT) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
    Network.connect();
  }

  /* Initializing HTTP API */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
}

void loop() {

  if (Device.getMode() != MODE_ACCESS_POINT) {
    if (Network.connected()) {

      if (Device.getMode() == MODE_NORMAL) {

        /* Connect to MQTT if not connected */
        Mqtt.connected() ? Mqtt.loop() : Mqtt.connect();

        WebServer.listener();

        /* Relay turn off event launched */
        if (Relay.autoTurnOff()) {
          Mqtt.publish(Relay.getMQTTTopic(), "state", "OFF");
        }

        /* One of the switches has been shortly pressed */
        if (Switch.isPressed() || ExternalSwitch.isPressed()) {
          Relay.toggle();
          if (Relay.get() == RELAY_ON) {
            Mqtt.publish(Relay.getMQTTTopic(), "state", "ON");
          } else {
            Mqtt.publish(Relay.getMQTTTopic(), "state", "OFF");
          }
        }
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

  Switch.listener();
  ExternalSwitch.listener();

  /* One of the Multifunction switches pressed for 10 seconds */
  if ((Switch.getFunctionality() == SWITCH_MULTI && Switch.is10s()) ||
      (ExternalSwitch.getFunctionality() == SWITCH_MULTI &&
       ExternalSwitch.is10s())) {
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                    : Device.reboot(MODE_NORMAL);
  }

  /* One of the Multifunction switches pressed for 5 seconds */
  if ((Switch.getFunctionality() == SWITCH_MULTI && Switch.is5s()) ||
      (ExternalSwitch.getFunctionality() == SWITCH_MULTI &&
       ExternalSwitch.is5s())) {
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                    : Device.reboot(MODE_NORMAL);
  }

  Led.loop();
}
