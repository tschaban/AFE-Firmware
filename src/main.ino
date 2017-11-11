#include "AFE-MQTT.h"
#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <Streaming.h>

AFEDataAccess Data;
MQTT MQTTConfiguration;

SWITCH SwitchConfiguration;
LED LEDConfiguration;

AFEDevice Device;
AFEWiFi Network;
AFEMQTT Mqtt;
AFEWebServer WebServer;
AFELED Led;
AFESwitch Switch;
AFERelay Relay;

void setup() {

  Serial.begin(115200);
  delay(10);

  /* Checking if the device is launched for a first time. If so it sets up the
   * device (EEPROM) */
  if (Device.isFirstTimeLaunch()) {
    Device.setDevice();
  }

  /* Checking if WiFi is onfigured, if not than it runs access point mode */
  if (Device.getMode() != MODE_ACCESS_POINT && !Device.isConfigured()) {
    Device.reboot(MODE_ACCESS_POINT);
  }

  Network.begin(Device.getMode());

  LEDConfiguration = Data.getLEDConfiguration();
  if (LEDConfiguration.present) {
    Led.begin(LEDConfiguration.gpio);

    if (Device.getMode() != MODE_NORMAL) {
      Led.blinkingOn(100);
    }
  }

  SwitchConfiguration = Data.getSwitchConfiguration(0);
  if (SwitchConfiguration.present) {
    Switch.begin(SwitchConfiguration);
  }

  if (Device.getMode() != MODE_ACCESS_POINT) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
    Network.connect();

    // @TODO add checking present of a relay
    if (Device.getMode() == MODE_NORMAL) {
      Relay.begin(0);
      Relay.setRelayAfterRestoringPower();
    }
  }

  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
}

void loop() {

  if (Device.getMode() != MODE_ACCESS_POINT) {
    if (Network.connected()) {
      if (Device.getMode() == MODE_NORMAL) {
        Mqtt.connected() ? Mqtt.loop() : Mqtt.connect();

        WebServer.listener();

        if (Relay.autoTurnOff()) {
          Mqtt.publish(Relay.getMQTTTopic(), "state", "OFF");
          Switch.toggleState();
        }

        if (Switch.isPressed()) {
          if (Switch.getState()) {
            Relay.on();
            Mqtt.publish(Relay.getMQTTTopic(), "state", "ON");
          } else {
            Relay.off();
            Mqtt.publish(Relay.getMQTTTopic(), "state", "OFF");
          }
        }
      } else { // Configuration Mode
        WebServer.listener();
      }
    } else {
      Network.connect();
    }
  } else {
    Network.APListener();
    WebServer.listener(); // Access Point
  }

  Switch.listener();

  if (Switch.is10s()) {
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                    : Device.reboot(MODE_NORMAL);
  }

  if (Switch.is5s()) {
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                    : Device.reboot(MODE_NORMAL);
  }

  Led.loop();
}
