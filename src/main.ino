#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>

#include "AFE-MQTT.h"
#include <AFE-Device.h>
#include <AFE-LED.h>
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

  // AFEDefaults Defaults;
  // Defaults.set();

  Serial << endl << "INFO: Switch mode : " << Device.getMode();
  Network.begin(Device.getMode());

  LEDConfiguration = Data.getLEDConfiguration();
  Led.begin(LEDConfiguration.gpio);

  SwitchConfiguration = Data.getSwitchConfiguration(0);
  Switch.begin(SwitchConfiguration);

  if (Device.getMode() != MODE_ACCESS_POINT) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
    Network.connect();

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
          Serial << endl << "INFO: pressed";
          Serial << endl << "INFO: state " << Switch.getState();
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
    Network.DNSListener();
    WebServer.listener(); // Access Point
  }

  Switch.listener();

  if (Switch.is10s()) {
    Serial << endl << "INFO: Going to Access Point Mode";
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                    : Device.reboot(MODE_NORMAL);
  }

  if (Switch.is5s()) {
    Serial << endl << "INFO: Going to configuration mode";
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                    : Device.reboot(MODE_NORMAL);
  }

  Switch.listener();

  if (Switch.is10s()) {
    Serial << endl << "INFO: Going to Access Point Mode";
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_ACCESS_POINT)
                                    : Device.reboot(MODE_NORMAL);
  }

  if (Switch.is5s()) {
    Serial << endl << "INFO: Going to configuration mode";
    Device.getMode() == MODE_NORMAL ? Device.reboot(MODE_CONFIGURATION)
                                    : Device.reboot(MODE_NORMAL);
  }
}
