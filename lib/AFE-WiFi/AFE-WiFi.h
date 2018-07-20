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
#include <AFE-LED.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
//#include <Streaming.h>

class AFEWiFi {

private:
  NETWORK networkConfiguration;
  unsigned long delayStartTime = 0;
  unsigned long ledStartTime = 0;

  uint8_t connections = 0;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false; // It's set to true after defined in configuration
                             // X number of connection failures
  DNSServer dnsServer;
  AFELED Led;

  boolean eventConnectionEstablished = false;

public:
  /* Constructor: no actions */
  AFEWiFi();

  /* Sets connection parameters and host name. Must be invoked before connect
   * method */
  void begin(uint8_t mode);

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();

  /* Returns true if device just connected to the network. It's set to true each time it connected. */
  boolean eventConnected();

  /* Method listens for HTTP request while device is in Access Point mode */
  void APListener();

  /* Method checks if device is connected to WiFi - if it's not then it connects
   * to it */
  void listener();
};

#endif
