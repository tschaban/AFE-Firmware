/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_STANDARD_h
#define _AFE_API_MQTT_STANDARD_h

#include <AFE-Configuration.h>

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-API.h>

#if defined(AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT) ||                   \
    defined(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT)
#include <ArduinoJson.h>

struct CLED_COMMAND {
  char command[10];
  uint32_t color;
  uint8_t brightness;
};

#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTStandard : public AFEAPI {

private:
  /* Classifies incomming MQTT Topics and invokes code for processing them */
  void processRequest();
  /* Size of the cache that stories MQTT Topics AFE has subsribed to */
  uint8_t currentCacheSize = 0;
  /* Cache with MQTT Topics AFE has subsribed to */
  MQTT_TOPICS_CACHE
  mqttTopicsCache[1
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
#endif
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCH
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES
#endif
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20
#endif
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_GATES
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_GATES
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS
#endif
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS
#endif
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_DHT
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_DHT
#endif
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935
#endif
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS
#endif
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS
#endif
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_PN532_SENSORS +
                  AFE_CONFIG_HARDWARE_NUMBER_OF_MIFARE_CARDS
#endif
/* Not yet implemented 
#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
                  + 1
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
                  + 1
#endif
*/
#ifdef AFE_CONFIG_HARDWARE_TLS2561
                  + AFE_CONFIG_HARDWARE_NUMBER_OF_TLS2561
#endif
  ];
/* Not yet implemented 
#if defined(AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT) ||                   \
    defined(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT)
  void getCLEDCommand(CLED_COMMAND *);
#endif
*/

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTStandard();
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif // AFE_CONFIG_HARDWARE_LED

  /* Subscribes to MQTT Topcis */
  void subscribe();

  /* Synchronize device's items values after connection to MQTT Broker is
   * established */
  void synchronize();

  /* Listens for MQTT Messages */
  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  boolean publishRelayState(uint8_t id);
  void processRelay(uint8_t *id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  boolean publishSwitchState(uint8_t id);
  void processSwitch(uint8_t *id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  boolean publishBinarySensorState(uint8_t id);
  void processBinarySensor(uint8_t *id);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSORS

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void publishADCValues();
  void processADC();
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  void processBatteryMeter();
  boolean publishBatteryMeterValues();
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void processBMEX80(uint8_t *id);
  boolean publishBMx80SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void processHPMA115S0(uint8_t *id);
  boolean publishHPMA115S0SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void processBH1750(uint8_t *id);
  boolean publishBH1750SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void processAS3935(uint8_t *id);
  boolean publishAS3935SensorData(uint8_t id);
#endif //  AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void processAnemometerSensor();
  void publishAnemometerSensorData();

#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void processRainSensor();
  void publishRainSensorData();
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
  void processGate(uint8_t *id);
  boolean publishGateState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void processContactron(uint8_t *id);
  boolean publishContactronState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void processDS18B20(uint8_t *id);
  boolean publishDS18B20SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void processRegulator(uint8_t *id);
  boolean publishRegulatorState(uint8_t id);
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void processThermalProtector(uint8_t *id);
  boolean publishThermalProtectorState(uint8_t id);
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
  void processDHT(uint8_t *id);
  boolean publishDHTSensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  boolean publishPN532SensorData(uint8_t id);
  boolean publishMiFareCardState(uint8_t id, uint8_t state);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
/* Not yet implemented 
#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
  void processEffectDeviceLight();
#endif // AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  void processEffectPN532Sensor();
#endif // AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
*/
#ifdef AFE_CONFIG_HARDWARE_TLS2561
  void processTLS2561(uint8_t *id);
  boolean publishTLS2561SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_TLS2561
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_MQTT_STANDARD_h