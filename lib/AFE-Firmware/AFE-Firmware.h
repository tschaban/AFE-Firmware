/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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

#define AFE_WEBSERVICE_VALIDATE_KEY 0
#define AFE_WEBSERVICE_ADD_KEY 1

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmware {

private:
  AFEDataAccess Data;
  PRO_VERSION Pro;

public:
  /* Constructor */
  AFEFirmware();

  void begin();

  /* Returns if firmware version if pro */
  boolean isUnlocked();

  /* Validating if the key is valid */
  boolean callService(uint8_t method);
};

#endif
