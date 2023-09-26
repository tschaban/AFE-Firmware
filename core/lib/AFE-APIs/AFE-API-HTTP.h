/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_h
#define _AFE_API_HTTP_h

#include <AFE-Configuration.h>
#include <AFE-Firmware.h>
#include <AFE-Hardware.h>

#include <AFE-HTTP-COMMAND-Structure.h>
#include <AFE-Web-Server.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#include <AFE-API-MQTT-Standard.h>
#endif

class AFEAPIHTTP {

private:
  AFEFirmware *_Firmware;
  AFEHardware *_Hardware;
  AFEWebServer *_HTTP;

  /* Ture if HTTP API is enabled */
  boolean enabled = false;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  AFEAPIMQTTDomoticz *_MqttAPI;
  AFEAPIHTTPDomoticz *_HttpAPIDomoticz;
#else
  AFEAPIMQTTStandard *_MqttAPI;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  /* Classifies and invokes code for HTTP request processing */
  void processRequest(HTTPCOMMAND *);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void processRelay(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  void processAnalogInput(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  void processBatteryMeter(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void processBMEX80(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void processHPMA115S0(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void processBH1750(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void processAS3935(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void processAnemometerSensor(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void processRainSensor(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void processGate(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void processContactron(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void processDS18B20(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void processRegulator(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void processThermalProtector(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void processBinarySensor(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  void processDHT(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  void processTSL2561(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  void processCLED(HTTPCOMMAND *);
  void processCLEDEffect(HTTPCOMMAND *, uint8_t effectId);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void publishCLEDStates(uint8_t id, boolean fromDomoticz);
#else
  void publishCLEDStates(uint8_t id);
#endif

#endif // AFE_CONFIG_HARDWARE_CLED


#ifdef AFE_CONFIG_HARDWARE_FS3000
  void processFS3000(HTTPCOMMAND *);
#endif // AFE_CONFIG_HARDWARE_FS3000

#if defined(AFE_CONFIG_HARDWARE_RELAY) || defined(AFE_CONFIG_HARDWARE_CLED)
  /**
  * @brief Method creates and sends On/Off response
  *
  * @param  request          HTTP Command request
  * @param  status           Success or Failure
  * @param  value            On, Off
  */
  void sendOnOffStatus(HTTPCOMMAND *request, boolean status, byte value);
#endif

  /**
   * @brief Method creates JSON respons after processing HTTP API request, and
   * pushes it
   *
   * @param  request          HTTP Command request
   * @param  status           success or failure
   * @param  value            contain of data json tag
   */
  void send(HTTPCOMMAND *request, boolean status, const char *value = "");

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTP();

/* Depending on the API compilication different classes are referenced */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void begin(AFEFirmware *, AFEHardware *, AFEWebServer *, AFEAPIMQTTDomoticz *,
             AFEAPIHTTPDomoticz *);
#else
  void begin(AFEFirmware *, AFEHardware *, AFEWebServer *, AFEAPIMQTTStandard *);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  /* Listens fr HTTP requests */
  void listener();

};

#endif