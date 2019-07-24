/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Firmware_h
#define _AFE_Firmware_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define AFE_WEBSERVICE_VALIDATE_KEY 0
#define AFE_WEBSERVICE_ADD_KEY 1

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmwarePro {

private:
  AFEDataAccess Data;
  unsigned long miliseconds = 0;
  unsigned long minutes = 0;

public:
  PRO_VERSION Pro;

  /* Constructor */
  AFEFirmwarePro();

  void begin();

  /* Validating if the key is valid */
  boolean callService(uint8_t method);

  /* Checking if the key is still valid */
  void reValidateKey();

  void listener();
};

#endif
