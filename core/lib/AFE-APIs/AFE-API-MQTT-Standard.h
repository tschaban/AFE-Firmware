/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_API_MQTT_STANDARD_h
#define _AFE_API_MQTT_STANDARD_h

#include <AFE-Configuration.h>

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ

#include <AFE-Firmware.h>
#include <AFE-Hardware.h>
#include <AFE-ASYNC-MQTT.h>
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <ArduinoJson.h>
#include <hardwares/AFE-RGB-LED.h>

struct CLED_COMMAND {
  char command[10];
  uint32_t color;
  uint8_t brightness;
};

#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPIMQTTStandard {

private:
  AFEFirmware *_Firmware;
  AFEHardware *_Hardware;

  /* Is API enabled, set in begin() */
  boolean enabled = false;

  /**
   * @brief formats MQTT topic and subscribes to MQTT Broker for command
   * messages
   *
   * @param  topic            device items topic (for state and command)
   * @param  topicId          if of a topic type:
   * afe_mqtt_standard_device_type_t
   * @param  index            device Item Id, default 0 => If there is only one
   * instance of an item (eg one sensor)
   */
  void subscribeToCommand(const char *topic);

  /**
   * @brief Processing common ON / OFF commands
   *
   * @param  command          byte array of chars that should contain a command
   * On,Off
   * @param  length           length of command array
   * @return uint8_t          Returns AFE_ON, AFE_OFF, AFE_NONE
   */
  uint8_t processOnOffCommand(byte *command, uint16_t *length);

  /**
   * @brief Publishes MQTT item state ON/OFF message
   *
   * @param  topic            MQTT topic used to send state update
   * @param  state            AFE_ON, AFE_OFF, AFE_NONE
   * @param  sendAsOpenClosed if false sends as ON/OFF if true OPEN/CLOSED
   * @return boolean          true if success, false: mqtt not enabled, AFE_NONE
   * or XYZ problem with MQTT Broker
   */
  boolean publishOnOffState(const char *topic, uint8_t state,
                            boolean sendAsOpenClosed = false);

  /**
   * @brief stores prepared mqtt message to compare with incoming one
   *
   */
  char mqttCommandTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];

public:

  AFEAsyncMQTTClient *Mqtt = new AFEAsyncMQTTClient();

  /**
   * @brief Construct a new AFEAPIMQTTStandard object
   *
   */
  AFEAPIMQTTStandard();

  void begin(AFEFirmware *, AFEHardware *);

  /**
   * @brief Takes care of subscription to all MQTT commands controlling device
   * items
   *
   */
  void subscribe();

  /**
   * @brief Synchronize device's items values after connection to MQTT Broker is
   * established
   *
   */
  void synchronize();

  /**
   * @brief Listens for MQTT Messages and dispaches them for processing
   *
   */
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

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  void publishADCValues(uint8_t id);
  void processADC(uint8_t *id);
#else
  void publishADCValues();
  void processADC();
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
  void processBatteryMeter(uint8_t *id);
  boolean publishBatteryMeterValues(uint8_t id);
#else
  void processBatteryMeter();
  boolean publishBatteryMeterValues();
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void processBMEX80(uint8_t *id);
  boolean publishBoschBMSensorData(uint8_t id);
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

#ifdef AFE_CONFIG_HARDWARE_CLED
  void processCLED(uint8_t *id);
  boolean publishCLEDState(uint8_t id);
  void processCLEDEffect(uint8_t *id);
  boolean publishCLEDEffectsState(uint8_t id);
  void processCLEDBrigtness(uint8_t *id);
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  void processTSL2561(uint8_t *id);
  boolean publishTSL2561SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_FS3000
  void processFS3000(uint8_t *id);
  boolean publishFS3000SensorData(uint8_t id);
#endif // AFE_CONFIG_HARDWARE_FS3000


};

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif // _AFE_API_MQTT_STANDARD_h