/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include <AFE-API-Domoticz.h>
#include <AFE-API-MQTT.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <AFE-Relay.h>
#include <AFE-Sensor-DS18B20.h>
#include <AFE-Switch.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
//#include <Streaming.h>

AFEDataAccess Data;
AFEDevice Device;
AFEWiFi Network;
AFEMQTT Mqtt;
AFEDomoticz Domoticz;
AFEWebServer WebServer;
AFELED Led;
AFESwitch Switch[sizeof(Device.configuration.isSwitch)];
AFERelay Relay[sizeof(Device.configuration.isRelay)];
AFESensorDS18B20 SensorDS18B20;
MQTT MQTTConfiguration;

float temperature;

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

  /* Initializing relay */
  initRelay();

  /* Initialzing network */
  Network.begin(Device.getMode());

  /* Initializing LED, checking if LED exists is made on Class level  */
  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0) {
    Led.begin(systeLedID - 1);
  }

  /* If device in configuration mode then start LED blinking */
  if (Device.getMode() == MODE_ACCESS_POINT) {
    Led.blinkingOn(100);
  }

  /* Initializing MQTT */
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.mqttAPI) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
  }

  Network.listener();

  /* Initializing HTTP WebServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();

  /* Initializing switches */
  initSwitch();

  /* Initializing switches */
  initDS18B20Sensor();

  /* Initializing Domoticz API */
  if (Device.configuration.domoticzAPI) {
    Domoticz.begin();
  }
}

void loop() {

  if (Device.getMode() != MODE_ACCESS_POINT) {
    if (Network.connected()) {
      if (Device.getMode() == MODE_NORMAL) {

        /* Connect to MQTT if not connected */
        if (Device.configuration.mqttAPI) {
          Mqtt.listener();
        }

        WebServer.listener();

        /* Checking if there was received HTTP API Command */
        mainHTTPRequestsHandler();
        mainRelay();

        /* Sensor: DS18B20 related code */
        mainDS18B20Sensor();

      } else { // Configuration Mode
        if (!Led.isBlinking()) {
          Led.blinkingOn(100);
        }
        WebServer.listener();
      }
    } else {
      if (Device.getMode() == MODE_CONFIGURATION && Led.isBlinking()) {
        Led.blinkingOff();
      }
    }
    Network.listener();
  } else { // Access Point Mode
    Network.APListener();
    WebServer.listener();
  }

  /* Listens for switch events */
  mainSwitchListener();
  mainSwitch();

  /* Led listener */
  Led.loop();
}
