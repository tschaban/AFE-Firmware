/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_WiFi_h
#define _AFE_WiFi_h

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_ESP32
#include <WiFi.h>
#else // ESP8266
#include <ESP8266WiFi.h>
#endif // ESP32/ESP8266

#ifdef DEBUG
#include <Streaming.h>
#endif

/* WiFi Class */
class AFEWiFi {

private:
  AFEDevice *Device;
  NETWORK configuration;
  unsigned long delayStartTime = 0;
  uint8_t WiFiMode;

#ifdef AFE_CONFIG_HARDWARE_LED
  unsigned long ledStartTime = 0;
#endif // AFE_CONFIG_HARDWARE_LED

  /* Used to count connection attemps before going to sleep mode */
  uint8_t connections = 0;
  /* Used to switch between Primary and Backup configuration */
  uint8_t noOfFailures = 0;
  unsigned long sleepStartTime = 0;
  /* It's set to true after defined in configuration X number of connection
   * failures */
  boolean sleepMode = false;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *Led;
  void begin(uint8_t mode, AFEDevice *, AFEDataAccess *);
#endif // AFE_CONFIG_HARDWARE_LED

  boolean eventConnectionEstablished = false;
  boolean eventConnectionLost = false;
  boolean disconnected = true;

  /* Is set to true is backup configuratio exists */
  boolean isBackupConfigurationSet = false;

  void switchConfiguration();

public:
  /* Constructor: no actions */
  AFEWiFi();

#ifdef AFE_ESP32
  WiFiClass WirelessNetwork;
#else  // ESP8266
  ESP8266WiFiClass WirelessNetwork;
#endif // ESP32/ESP8266

  /* Indicates on to which router the device is connected */
  boolean isPrimaryConfiguration = false;

/* Sets connection parameters and host name. Must be invoked before connect
 * method */
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(uint8_t mode, AFEDevice *, AFEDataAccess *, AFELED *);
#else
  void begin(uint8_t mode, AFEDevice *, AFEDataAccess *);
#endif // AFE_CONFIG_HARDWARE_LED

  /* Return TRUE if device is connected to WiFi Acces Point */
  boolean connected();

  /* Returns true if device just connected to the network. It's set to true each
   * time it connected. */
  boolean eventConnected();
  boolean eventDisconnected();

  /* Method checks if device is connected to WiFi - if it's not then it connects
   * to it */
  void listener();
};

#endif // _AFE_WiFi_h
