/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-LED.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
//#include <Streaming.h>

class AFEWiFi {

private:
  NETWORK networkConfiguration;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false; // It's set to true after defined in configuration
                             // X number of connection failures
  DNSServer dnsServer;
  AFELED Led;

public:
  /* Constructor: no actions */
  AFEWiFi();

  /* Sets connection parameters and host name. Must be invoked before connect
   * method */
  void begin(uint8_t mode);

  /* Connecting to WiFi Access point */
  void connect();

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();

  /* Method listens for HTTP request while device is in Access Point mode */
  void APListener();
};

#endif
