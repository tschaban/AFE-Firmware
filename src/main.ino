#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Defaults.h>
#include <AFE-Relay.h>
#include <AFE-Switch.h>

#include <AFE-LED.h>
#include <AFE-MQTT.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <Streaming.h>

AFEDataAccess Data;
FIRMWARE firmwareConfiguration;
NETWORK networkConfiguration;
MQTT MQTTConfiguration;
DOMOTICZ DomoticzConfiguration;
RELAY RelayConfiguration;
SWITCH SwitchConfiguration;
DS18B20 DS18B20Configuration;
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

  AFEDefaults Defaults;
  Defaults.set();

  firmwareConfiguration = Data.getFirmwareConfiguration();
  networkConfiguration = Data.getNetworkConfiguration();
  MQTTConfiguration = Data.getMQTTConfiguration();
  DomoticzConfiguration = Data.getDomoticzConfiguration();
  RelayConfiguration = Data.getRelayConfiguration(0);
  SwitchConfiguration = Data.getSwitchConfiguration(0);
  DS18B20Configuration = Data.getDS18B20Configuration();
  LEDConfiguration = Data.getLEDConfiguration();

  debugShowConfiguration();

  Network.connect();

  Led.begin(LEDConfiguration.gpio);
  Switch.begin(SwitchConfiguration.gpio, SwitchConfiguration.type,
               SwitchConfiguration.sensitiveness);
  Relay.begin(RelayConfiguration.gpio);

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
    Serial << endl << "INFO: Button pressed for 5s";
  }

  if (Switch.isPressed()) {
    Serial << endl << "INFO: pressed";
    Serial << endl << "INFO: state " << Switch.getState();
    if (Switch.getState()) {
      Led.on();
    } else {
      Led.off();
    }
  }
}
