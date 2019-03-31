/*
 Source code taken from:
 https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPUpdateServer
 Credits to all authors !!!!

 Modifications by tschaban https://github.com/tschaban
*/

/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_OTH_h
#define _AFE_OTH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "StreamString.h"
#include <AFE-Configuration-Panel.h>
#include <AFE-Device.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class ESP8266WebServer;

class ESP8266HTTPUpdateServer {
public:
  ESP8266HTTPUpdateServer();

  void setup(ESP8266WebServer *server);

protected:
  void _setUpdaterError();

private:
  ESP8266WebServer *_server;
  String _updaterError;
  AFEConfigurationPanel ConfigurationPanel;
  AFEDevice Device;
};

#endif
