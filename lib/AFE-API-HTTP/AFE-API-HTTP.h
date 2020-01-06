/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_h
#define _AFE_API_HTTP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Device.h>
#include <AFE-HTTP-COMMAND-Structure.h>
#include <AFE-Web-Server.h>
#include <AFE-Data-Access.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-API-MQTT-Domoticz.h>
#include <AFE-API-HTTP-Domoticz.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHTTP {

private:
  AFEDevice *_Device;
  AFEWebServer *_HTTP;
  AFEDataAccess *_Data;

  boolean enabled = false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  AFEAPIMQTTDomoticz *_MqttAPI;
  AFEAPIHTTPDomoticz *_HttpAPIDomoticz;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  AFEAnalogInput *_AnalogInput;
#endif


  void processRequest(HTTPCOMMAND *);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void processRelay(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void processAnalogInput(HTTPCOMMAND *);
#endif

  void send(HTTPCOMMAND *request, boolean status, const char *value = "");
  void send(HTTPCOMMAND *request, boolean status, double value,
            uint8_t width = 2, uint8_t precision = 2);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
   * method which creates JSON respons and pushes it */
  void sendRelayStatus(HTTPCOMMAND *request, boolean status, byte value);
#endif

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTP();

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void begin(AFEDevice *, AFEWebServer *,AFEDataAccess *, AFEAPIMQTTDomoticz *, AFEAPIHTTPDomoticz *);
#endif

  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void addClass(AFERelay *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void addClass(AFEAnalogInput *);
#endif


};

#endif