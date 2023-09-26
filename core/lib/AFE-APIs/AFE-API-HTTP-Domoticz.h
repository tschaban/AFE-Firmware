/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_DOMOTICZ_h
#define _AFE_API_HTTP_DOMOTICZ_h

#include <AFE-Configuration.h>

/**
 * @brief code excluded for none-Domoticz API
 *
 */

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-Firmware.h>
#include <AFE-Hardware.h>
#include <WiFiClient.h>
#include <rBase64.h>

#ifdef AFE_ESP32
#include <HTTPClient.h>
#else // ESP8266
#include <ESP8266HTTPClient.h>
#endif // ESP32 / 8266


class AFEAPIHTTPDomoticz {

private:
  AFEFirmware *_Firmware;
  AFEHardware *_Hardware;
  HTTPClient *http = new HTTPClient();
  WiFiClient client;
  boolean _initialized = false;

  char serverURL[AFE_CONFIG_API_DOMOTICZ_URL_LENGTH];

  /**
   * @brief It creates Domoticz url with all required params
   *
   * @param  param            desc
   * @param  idx              desc
   * @return const String
   */
  const String getApiCall(const char *param, unsigned int idx);

  /**
   * @brief Calls url. Publishes to Domoticz
   *
   * @param  url              desc
   * @return boolean
   */
  boolean callURL(const String url);

  /**
   * @brief Sends to Domoticz switch update call
   *
   * @param  idx              desc
   * @param  state            desc
   * @return boolean
   */
  boolean sendSwitchCommand(unsigned int idx, boolean state);

  /**
   * @brief sends to Domoticz custome sensor call
   *
   * @param  idx              desc
   * @param  value            desc
   * @param  nvalue           desc
   * @return boolean
   */
  boolean sendCustomSensorCommand(unsigned int idx, const char *value,
                                  uint16_t nvalue = 0);

public:
  /**
   * @brief Construct a new AFEAPIHTTPDomoticz object. it sets all necessary
   * parameters
   *
   */
  AFEAPIHTTPDomoticz();

  void begin(AFEFirmware *, AFEHardware *);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /**
   * @brief Publishes current relay state to Domoticz
   *
   * @param  id               desc
   * @return boolean
   */
  boolean publishRelayState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  boolean publishSwitchState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  void publishADCValues(uint8_t id);
#else  // ESP32
  void publishADCValues();
#endif // ESP32/8266
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  boolean publishBoschBMSensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  boolean publishHPMA115S0SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
  boolean publishBH1750SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  boolean publishAS3935SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void publishAnemometerSensorData();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void publishRainSensorData();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
  boolean publishGateState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  boolean publishContactronState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
  boolean publishBatteryMeterValues(uint8_t id);
#else
  boolean publishBatteryMeterValues();
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  boolean publishDS18B20SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  boolean publishRegulatorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  boolean publishThermalProtectorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  boolean publishDHTSensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  boolean publishBinarySensorState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  boolean publishMiFareCardState(uint8_t id, uint8_t tagId, uint8_t state,
                                 const char *user);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  boolean publishTSL2561SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_FS3000
  boolean publishFS3000SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_FS3000


  /* RGB LED are not supported in HTTP Domoticz API
  #ifdef AFE_CONFIG_HARDWARE_CLED
    virtual void addClass(AFECLED *);
    boolean publishCLEDState(uint8_t id);
    boolean publishCLEDEffectState(uint8_t id);
  #endif // AFE_CONFIG_HARDWARE_CLED
  */
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_HTTP_DOMOTICZ_h