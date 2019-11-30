/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#include <ESP8266WiFi.h>
#ifdef DEBUG
#include <Streaming.h>
#endif
class AFEWiFi {

private:
  AFEDevice *Device;
  NETWORK networkConfiguration;
  unsigned long delayStartTime = 0;
  uint8_t WiFiMode;

#ifdef AFE_CONFIG_HARDWARE_LED
  unsigned long ledStartTime = 0;
#endif

  uint8_t connections = 0;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false; // It's set to true after defined in configuration
                             // X number of connection failures
#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif

  boolean eventConnectionEstablished = false;
  boolean disconnected = false;

public:
  /* Constructor: no actions */
  AFEWiFi();

  /* Sets connection parameters and host name. Must be invoked before connect
   * method */
  void begin(uint8_t mode, AFEDevice *);

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();

  /* Returns true if device just connected to the network. It's set to true each
   * time it connected. */
  boolean eventConnected();

  /* Method checks if device is connected to WiFi - if it's not then it connects
   * to it */
  void listener();
};

#endif
