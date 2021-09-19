/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_DOMOTICZ_h
#define _AFE_API_HTTP_DOMOTICZ_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED // Code excluded for not Standard API
                                       // version of Firmware

#include <AFE-API.h>
#include <WiFiClient.h>
#ifdef AFE_ESP32
#include <HTTPClient.h>
#else // ESP8266
#include <ESP8266HTTPClient.h>
#endif // ESPESP8266

#include <rBase64.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHTTPDomoticz : public AFEAPI {

private:
  HTTPClient http;
  WiFiClient client;
  DOMOTICZ configuration;

  char serverURL[AFE_CONFIG_API_DOMOTICZ_URL_LENGTH];

  /* Invokes API setup */
  void init();

  /* It creates Domoticz url with all required params */
  const String getApiCall(const char *param, unsigned int idx);

  /* Calls url. Publishes to Domoticz */
  boolean callURL(const String url);

  /* Sends to Domoticz switch update call */
  boolean sendSwitchCommand(unsigned int idx, const char *value);

  /* sends to Domoticz custome sensor call */
  boolean sendCustomSensorCommand(unsigned int idx, const char *value,
                                  uint16_t nvalue = 0);

  /* Replace space with %20 */
  //  void replaceSpaceinUrl(const char *inputString, const char &outputString);

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTPDomoticz();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Add reference to global class and adds IDX to cache */
  virtual void addClass(AFERelay *);
  /* Publishes current relay state to Domoticz */
  boolean publishRelayState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  virtual void addClass(AFESwitch *);
  boolean publishSwitchState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  virtual void addClass(AFEAnalogInput *);
#ifdef AFE_ESP32
  void publishADCValues(uint8_t id);
#else  // ESP32
  void publishADCValues();
#endif // ESP32/8266
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  virtual void addClass(AFESensorBMEX80 *);
  boolean publishBMx80SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  virtual void addClass(AFESensorHPMA115S0 *);
  boolean publishHPMA115S0SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
  virtual void addClass(AFESensorBH1750 *);
  boolean publishBH1750SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  virtual void addClass(AFESensorAS3935 *);
  boolean publishAS3935SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  virtual void addClass(AFEAnemometer *);
  void publishAnemometerSensorData();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  virtual void addClass(AFERainmeter *);
  void publishRainSensorData();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
  virtual void addClass(AFEGate *);
  boolean publishGateState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  virtual void addClass(AFEContactron *);
  boolean publishContactronState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  boolean publishBatteryMeterValues();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  virtual void addClass(AFESensorDS18B20 *);
  boolean publishDS18B20SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  virtual void addClass(AFERegulator *);
  boolean publishRegulatorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  virtual void addClass(AFEThermalProtector *);
  boolean publishThermalProtectorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  virtual void addClass(AFESensorDHT *);
  boolean publishDHTSensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  virtual void addClass(AFESensorBinary *);
  boolean publishBinarySensorState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  virtual void addClass(AFEMiFareCard *);
  boolean publishMiFareCardState(uint8_t id, uint8_t tagId, uint8_t state,
                                 const char *user);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_TLS2561
  virtual void addClass(AFESensorTLS2561 *);
  boolean publishTLS2561SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_TLS2561
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_HTTP_DOMOTICZ_h