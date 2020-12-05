/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_DOMOTICZ_h
#define _AFE_API_MQTT_DOMOTICZ_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

#include <AFE-API.h>
#include <ArduinoJson.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTDomoticz : public AFEAPI {

private:
  /* Generates switch update JSON */
  void generateSwitchMessage(char *json, uint32_t idx, boolean relayState);

  /* Generates custome sensor JSON */
  void generateDeviceValue(char *json, uint32_t idx, char *svalue,
                           uint8_t nvalue = 0);

  /* Cache that stories IDXs */
  uint8_t lastIDXChacheIndex = 0;
  DOMOTICZ_IDX_CACHE idxCache[1
#ifdef AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
                              + AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS
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

  ];

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* It stories IDX of a device that should be excluded from processing */
  DOMOTICZ_BASIC_CONFIG bypassProcessing;
#endif

  /* Get domoticz request/update command in formated form */
  DOMOTICZ_MQTT_COMMAND getCommand();
  /* Classfies and invokes code for processing the request */
  void processRequest();

  /* Checks if IDX can be proccesed. It's checked against bypassing IDX */
  boolean idxForProcessing(uint32_t idx);

  /* Returns RSSI level in DOmoticz Range */
  uint8_t getRSSI();

public:
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTDomoticz();
  void begin(AFEDataAccess *, AFEDevice *);
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *Led);
#endif

  /* Subscribe domoticz topic in MQTT Broker */
  void subscribe();

  /* Synchronize values of device items after connection to MQTT Broker is
   * established */
  void synchronize();

  /* Listens for MQTT messages */
  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
  virtual void addClass(AFERelay *);
  boolean publishRelayState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  virtual void addClass(AFESwitch *);
  boolean publishSwitchState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  virtual void addClass(AFEAnalogInput *);
  void publishADCValues();
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  void publishBatteryMeterValues();
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

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
#endif //  AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  virtual void addClass(AFESensorAS3935 *);
  boolean publishAS3935SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  virtual void addClass(AFESensorAnemometer *);
  void publishAnemometerSensorData();
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  virtual void addClass(AFESensorRainmeter *);
  void publishRainSensorData();
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_GATE
  virtual void addClass(AFEGate *);
  boolean publishGateState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  virtual void addClass(AFEContactron *);
  boolean publishContactronState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  virtual void addClass(AFESensorDS18B20 *);
  boolean publishDS18B20SensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  virtual void addClass(AFERegulator *);
  boolean publishRegulatorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  virtual void addClass(AFEThermalProtector *);
  boolean publishThermalProtectorState(uint8_t id);
#endif
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif //_AFE_API_MQTT_DOMOTICZ_h
