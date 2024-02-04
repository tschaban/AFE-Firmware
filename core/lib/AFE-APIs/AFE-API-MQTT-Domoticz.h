/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_DOMOTICZ_h
#define _AFE_API_MQTT_DOMOTICZ_h

#include <AFE-Configuration.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

#include <AFE-Firmware.h>
#include <AFE-Hardware.h>
#include <ArduinoJson.h>
#include <AFE-ASYNC-MQTT.h>

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

class AFEAPIMQTTDomoticz
{

private:
  AFEFirmware *_Firmware;
  AFEHardware *_Hardware;
  /* Is API enabled, set in begin() */
  boolean enabled = false;
  /* Generates switch update JSON */

  void generateSwitchMessage(char *json, uint32_t idx, boolean relayState);

  /* Generates custome sensor JSON */
  void generateDeviceValue(char *json, uint32_t idx, const char *svalue,
                           uint16_t nvalue = 0);

  /* It stories IDX of a device that should be excluded from processing */
  DOMOTICZ_BASIC_CONFIG bypassProcessing;

  /**
   * @brief Cache that stories IDXs for devices that are controlled by Domoticz
   *
   */
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
#ifdef AFE_CONFIG_HARDWARE_CLED
                              + (2 * AFE_CONFIG_HARDWARE_NUMBER_OF_CLED_STRIPS)
#endif
  ];

  void addIdxToCache(uint8_t id, afe_domoticz_device_type_t type, uint32_t idx);

  /* Get domoticz request/update command in formated form */
  DOMOTICZ_MQTT_COMMAND getCommand();

  /* Classfies and invokes code for processing the request */
  void processRequest();

  boolean publishSwitchMessage(uint32_t *idx, boolean state);

#ifdef AFE_CONFIG_HARDWARE_CLED
  boolean publishSetLevelMessage(uint32_t *idx, uint8_t *level);
  boolean publishSetColorMessage(uint32_t *idx, CLED_PARAMETERS *led);
#endif

#ifdef DEBUG
  void printSameStateWarningMessage();
  void printFoundDeviceItem(const __FlashStringHelper *deviceName, uint8_t id);
  void printSynchronzingDeviceItem(const __FlashStringHelper *deviceName, uint8_t id);
  void printCacheingDeviceItem(const __FlashStringHelper *deviceName, uint8_t id, uint32_t idx);
#endif

public:
  AFEAsyncMQTTClient *Mqtt = new AFEAsyncMQTTClient();
  /* Constructor: it sets all necessary parameters */
  AFEAPIMQTTDomoticz();
  void begin(AFEFirmware *, AFEHardware *);

  /* Subscribe domoticz topic in MQTT Broker */
  void subscribe();

  /* Synchronize values of device items after connection to MQTT Broker is
   * established */
  void synchronize();

  /* Listens for MQTT messages */
  void listener();

#ifdef AFE_CONFIG_HARDWARE_RELAY
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

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
  void publishBatteryMeterValues(uint8_t id);
#else
  void publishBatteryMeterValues();
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  boolean publishBoschBMSensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  boolean publishHPMA115S0SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
  boolean publishBH1750SensorData(uint8_t id);
#endif //  AFE_CONFIG_HARDWARE_BH1750

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
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  boolean publishContactronState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  boolean publishDS18B20SensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  boolean publishRegulatorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  boolean publishThermalProtectorState(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  boolean publishDHTSensorData(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  boolean publishBinarySensorState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  boolean publishMiFareCardState(uint8_t id, uint8_t tagId, uint8_t state,
                                 const char *user);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  boolean publishTSL2561SensorData(uint8_t id);
#endif //  AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_CLED
  boolean publishCLEDState(uint8_t id);
  boolean publishCLEDColorState(uint8_t id);
  boolean publishCLEDEffectsState(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_FS3000
  boolean publishFS3000SensorData(uint8_t id);
#endif //  AFE_CONFIG_HARDWARE_FS3000
};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif //_AFE_API_MQTT_DOMOTICZ_h
