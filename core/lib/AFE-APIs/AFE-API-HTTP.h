/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_HTTP_h
#define _AFE_API_HTTP_h

#include <AFE-Configuration.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-HTTP-COMMAND-Structure.h>
#include <AFE-Web-Server.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#include <AFE-API-MQTT-Standard.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-HPMA115S0.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Sensor-BH1750.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-Sensor-AS3935.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-Sensor-Anemometer.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Sensor-Rainmeter.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Regulator.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Thermal-Protector.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Sensor-Binary.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIHTTP {

private:
  AFEDevice *_Device;
  AFEWebServer *_HTTP;
  AFEDataAccess *_Data;

  /* Ture if HTTP API is enabled */
  boolean enabled = false;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  AFEAPIMQTTDomoticz *_MqttAPI;
  AFEAPIHTTPDomoticz *_HttpAPIDomoticz;
#else
  AFEAPIMQTTStandard *_MqttAPI;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  AFEAnalogInput *_AnalogInput;
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  AFESensorBMEX80 *_BMx80Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  AFESensorHPMA115S0 *_HPMA115S0Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0];
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  AFESensorBH1750 *_BH1750Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750];
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  AFESensorAS3935 *_AS3935Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935];
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  AFEAnemometer *_AnemometerSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  AFERainmeter *_RainmeterSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  AFEGate *_Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  AFEContactron *_Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  AFESensorDS18B20 *_DS18B20Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  AFERegulator *_Regulator[AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  AFEThermalProtector
      *_ThermalProtector[AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  AFESensorDHT *_DHTSensor[AFE_CONFIG_HARDWARE_NUMBER_OF_DHT];
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  AFESensorBinary *_BinarySensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS];
#endif

  /* Classifies and invokes code for HTTP request processing */
  void processRequest(HTTPCOMMAND *);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void processRelay(HTTPCOMMAND *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
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

  void send(HTTPCOMMAND *request, boolean status, const char *value = "");
/* @TODO Check if it's still used
  void send(HTTPCOMMAND *request, boolean status, double value,
          uint8_t width = 2, uint8_t precision = 2);
*/

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
   * method which creates JSON respons and pushes it */
  void sendRelayStatus(HTTPCOMMAND *request, boolean status, byte value);
#endif

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIHTTP();

/* Depending on the API compilication different classes are referenced */
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void begin(AFEDevice *, AFEWebServer *, AFEDataAccess *, AFEAPIMQTTDomoticz *,
             AFEAPIHTTPDomoticz *);
#else
  void begin(AFEDevice *, AFEWebServer *, AFEDataAccess *,
             AFEAPIMQTTStandard *);
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  /* Listens fr HTTP requests */
  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void addClass(AFERelay *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void addClass(AFEAnalogInput *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void addClass(AFESensorBMEX80 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void addClass(AFESensorHPMA115S0 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void addClass(AFESensorBH1750 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void addClass(AFESensorAS3935 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void addClass(AFEAnemometer *);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void addClass(AFERainmeter *);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void addClass(AFEGate *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void addClass(AFEContactron *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void addClass(AFESensorDS18B20 *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void addClass(AFERegulator *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void addClass(AFEThermalProtector *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  void addClass(AFESensorDHT *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void addClass(AFESensorBinary *);
#endif
};

#endif