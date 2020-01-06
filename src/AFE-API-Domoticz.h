  /* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Domoticz_API_h
#define _AFE_Domoticz_API_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif 

#include <AFE-Data-Access.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <rBase64.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

#ifdef AFE_CONFIG_HUMIDITY
#define HUMIDITY_NORMAL 0
#define HUMIDITY_COMFORTABLE 1
#define HUMIDITY_DRY 2
#define HUMIDITY_WET 3
#endif

class AFEDomoticz {

private:
  AFEDataAccess Data;
  HTTPClient http;
  char serverURL[184];
  boolean initialized = false;

  const String getApiCall(const char *param, unsigned int idx);
  void callURL(const String url);
  WiFiClient client;

public:
  DOMOTICZ configuration;

  AFEDomoticz();
  void begin();
  void disconnect();

  /* It send to Domoticz switch state using following API call
    json.htm?type=command&param=switchlight&idx=IDX&switchcmd=STATE
  */
  void sendSwitchCommand(unsigned int idx, const char *value);

#ifdef AFE_CONFIG_TEMPERATURE
  /* It send to Domoticz temperature using following API call
     json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP
  */
  void sendTemperatureCommand(unsigned int idx, float value);
#endif

#ifdef AFE_CONFIG_HUMIDITY
  /* It send to Domoticz humidity using following API call
     /json.htm?type=command&param=udevice&idx=IDX&nvalue=HUM&svalue=HUM_STAT
  */
  void sendHumidityCommand(unsigned int idx, float value);

#endif

#if defined(AFE_CONFIG_TEMPERATURE) || defined(AFE_CONFIG_HUMIDITY)
  /* It send to Domoticz temperature and humidity using following API call
     /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT
  */
  void sendTemperatureAndHumidityCommand(unsigned int idx,
                                         float temperatureValue,
                                         float humidityValue);
#endif

#ifdef AFE_CONFIG_HUMIDITY
  /* It returns humidity state using humidity ranges. Look for value meaning at
   * HUMIDITY_ constant
   */
  uint8_t getHumidityState(float value);
#endif

#if defined(T3_CONFIG)
  void sendPirCommand(unsigned int idx, const char *value);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void sendGateCommand(unsigned int idx, const char *value);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void sendContactronCommand(unsigned int idx, const char *value);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void sendSValueCommand(unsigned int idx, float value);
#endif

#ifdef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
  void sendCustomSensorCommand(unsigned int idx, uint16_t value);
  void sendCustomSensorCommand(unsigned int idx, double value,
                               uint8_t precision = 6);
  void sendCustomSensorCommand(unsigned int idx, float value,
                               uint8_t precision = 2);
#endif

#if defined(AFE_CONFIG_PRESSURE)
  void sendPressureCommand(unsigned int idx, float pressureValue);
#endif

#if (defined(AFE_CONFIG_TEMPERATURE) && defined(AFE_CONFIG_HUMIDITY))
#if defined(AFE_CONFIG_PRESSURE)
  /* Definition:
   * https://www.domoticz.com/wiki/Domoticz_API/JSON_URL's#Temperature.2Fhumidity.2Fbarometer
   */
  void sendTemperatureAndHumidityAndPressureCommand(unsigned int idx,
                                                    float temperatureValue,
                                                    float humidityValue,
                                                    float pressureValue);
#endif
#endif
};

#endif
