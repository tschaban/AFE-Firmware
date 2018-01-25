/*
 Source code taken from:
 https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPUpdateServer
 Credits to all authors !!!!

 Modifications by tschaban https://github.com/tschaban
*/

/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_OTH_h
#define _AFE_OTH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "StreamString.h"
#include <AFE-Configuration-Panel.h>

#include <ESP32WebServer.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

class ESP8266WebServer;

class ESP8266HTTPUpdateServer {
public:
  ESP8266HTTPUpdateServer(bool serial_debug = false);

  void setup(ESP32WebServer *server) { setup(server, NULL, NULL); }

  void setup(ESP32WebServer *server, const char *path) {
    setup(server, path, NULL, NULL);
  }

  void setup(ESP32WebServer *server, const char *username,
             const char *password) {
    setup(server, "/update", username, password);
  }

  void setup(ESP32WebServer *server, const char *path, const char *username,
             const char *password);

protected:
  void _setUpdaterError();

private:
  bool _serial_output;
  ESP32WebServer *_server;
  char *_username;
  char *_password;
  bool _authenticated;
  String _updaterError;
  AFEConfigurationPanel ConfigurationPanel;
};

#endif
