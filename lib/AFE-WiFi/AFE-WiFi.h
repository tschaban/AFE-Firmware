/*
   WiFi component
   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-LED.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <Streaming.h>

#define WIFI_MODE_CLIENT 0
#define WIFI_MODE_SERVER 1
#define WIFI_MODE_AP 2

class AFEWiFi {

private:
  NETWORK networkConfiguration;
  boolean accessPoint = false;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false;
  DNSServer dnsServer;
  AFELED Led;

public:
  /* Constructor: no actions */
  AFEWiFi();

  /* Sets connectionparameters and host name. Must be invoked before connect
   * method */
  void begin(uint8_t mode = WIFI_MODE_CLIENT);

  /* Connecting to WiFi Access point */
  void connect();

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();
};

#endif
