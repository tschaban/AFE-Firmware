/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-API-Domoticz.h>
#include <AFE-API-MQTT.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#ifndef T0_SHELLY_1_CONFIG
#include <AFE-LED.h>
#endif
#include <AFE-Relay.h>
#include <AFE-Switch.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

AFEDataAccess Data;
AFEDevice Device;
AFEWiFi Network;
AFEMQTT Mqtt;
AFEDomoticz Domoticz;
AFEWebServer WebServer;
#ifndef T0_SHELLY_1_CONFIG
AFELED Led;
#endif
AFESwitch Switch[sizeof(Device.configuration.isSwitch)];
AFERelay Relay[sizeof(Device.configuration.isRelay)];
#ifdef T1_CONFIG
AFESensorDS18B20 SensorDS18B20;
#endif
MQTT MQTTConfiguration;

#ifdef T1_CONFIG
float temperature;
#endif
void setup() {

  Serial.begin(115200);
  delay(10);

/* Turn off publishing information to Serial */
#ifndef DEBUG
  Serial.swap();
#endif

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

  /* Initializing relay */
  initRelay();
  /* Initialzing network */
  Network.begin(Device.getMode());

#ifndef T0_SHELLY_1_CONFIG
  /* Initializing LED, checking if LED exists is made on Class level  */
  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0) {
    Led.begin(systeLedID - 1);
  }
  /* If device in configuration mode then start LED blinking */
  if (Device.getMode() == MODE_ACCESS_POINT) {
    Led.blinkingOn(100);
  }
#endif

  Network.listener();
  /* Initializing HTTP WebServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();

  /* Initializing switches */
  initSwitch();

#ifdef T1_CONFIG
  /* Initializing switches */
  initDS18B20Sensor();
#endif

  /* Initializing APIs */
  MQTTInit();
  DomoticzInit();
}

void loop() {

  if (Device.getMode() != MODE_ACCESS_POINT) {
    if (Network.connected()) {
      if (Device.getMode() == MODE_NORMAL) {

        /* It listens to events and process them */
        eventsListener();

        /* Connect to MQTT if not connected */
        if (Device.configuration.mqttAPI) {
          Mqtt.listener();
        }

        WebServer.listener();

        /* Checking if there was received HTTP API Command */
        mainHTTPRequestsHandler();
        mainRelay();

#ifdef T1_CONFIG
        /* Sensor: DS18B20 related code */
        mainDS18B20Sensor();
#endif

      } else { // Configuration Mode
#ifndef T0_SHELLY_1_CONFIG
        if (!Led.isBlinking()) {
          Led.blinkingOn(100);
        }
#endif
        WebServer.listener();
      }
    }
#ifndef T0_SHELLY_1_CONFIG
    else {
      if (Device.getMode() == MODE_CONFIGURATION && Led.isBlinking()) {
        Led.blinkingOff();
      }
    }
#endif
    Network.listener();
  } else { // Access Point Mode
    Network.APListener();
    WebServer.listener();
  }

  /* Listens for switch events */
  mainSwitchListener();
  mainSwitch();
#ifndef T0_SHELLY_1_CONFIG
  /* Led listener */
  Led.loop();
#endif
}
