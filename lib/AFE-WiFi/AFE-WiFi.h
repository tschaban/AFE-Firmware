/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifndef T0_SHELLY_1_CONFIG
#include <AFE-LED.h>
#endif

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#ifdef DEBUG
#include <Streaming.h>
#endif
class AFEWiFi {

private:
  NETWORK networkConfiguration;
  unsigned long delayStartTime = 0;

#ifndef T0_SHELLY_1_CONFIG
  unsigned long ledStartTime = 0;
#endif

  uint8_t connections = 0;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false; // It's set to true after defined in configuration
                             // X number of connection failures
  DNSServer dnsServer;

#ifndef T0_SHELLY_1_CONFIG
  AFELED Led;
#endif

  boolean eventConnectionEstablished = false;
  boolean disconnected = false;

public:
  /* Constructor: no actions */
  AFEWiFi();

  /* Sets connection parameters and host name. Must be invoked before connect
   * method */
  void begin(uint8_t mode);

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();

  /* Returns true if device just connected to the network. It's set to true each
   * time it connected. */
  boolean eventConnected();

  /* Method listens for HTTP request while device is in Access Point mode */
  void APListener();

  /* Method checks if device is connected to WiFi - if it's not then it connects
   * to it */
  void listener();
};

#endif
