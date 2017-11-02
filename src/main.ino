#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>

#include "AFE-MQTT.h"
#include <AFE-LED.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <Streaming.h>

AFEDataAccess Data;
MQTT MQTTConfiguration;

SWITCH SwitchConfiguration;
LED LEDConfiguration;

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

  MQTTConfiguration = Data.getMQTTConfiguration();
  SwitchConfiguration = Data.getSwitchConfiguration(0);
  LEDConfiguration = Data.getLEDConfiguration();

  // Setting Relay state on powerOn
  Relay.begin(0);

  Serial << endl << "DEBUG: Relay mqttTopic : " << Relay.getMQTTTopic();

  /*
    if (RelayConfiguration.statePowerOn == 0) {
      Relay.off();
    } else if (RelayConfiguration.statePowerOn == 1) {
      Relay.on();
    } else if (RelayConfiguration.statePowerOn == 2) {
      Data.getRelayState(1) ? Relay.on() : Relay.off();
    } else {
      Data.getRelayState(1) ? Relay.off() : Relay.on();
    }
  */
  Network.begin();
  Mqtt.begin();

  Network.connect();

  Led.begin(LEDConfiguration.gpio);
  Switch.begin(SwitchConfiguration.gpio, SwitchConfiguration.type,
               SwitchConfiguration.sensitiveness);

  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
}

void loop() {

  if (Network.connected()) {
    WebServer.listener();
    Mqtt.connected() ? Mqtt.loop() : Mqtt.connect();
  } else {
    Network.connect();
  }

  Switch.listener();

  if (Switch.is10s()) {
    Serial << endl << "INFO: Button pressed for  10s";
  }

  if (Switch.is5s()) {
    Serial << endl << "INFO: Button pressed 5s";
  }

  if (Switch.isPressed()) {
    Serial << endl << "INFO: pressed";
    Serial << endl << "INFO: state " << Switch.getState();
    if (Switch.getState()) {
      Led.on();
      Relay.on();
      Mqtt.publish(Relay.getMQTTTopic(), "state", "ON");
    } else {
      Led.off();
      Relay.off();
      Mqtt.publish(Relay.getMQTTTopic(), "state", "OFF");
    }
  }
}
