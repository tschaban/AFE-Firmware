
/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Standard.h"

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ

AFEAPIMQTTStandard::AFEAPIMQTTStandard() : AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAPIMQTTStandard::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  AFEAPI::begin(Data, Device, Led);
}
#else
void AFEAPIMQTTStandard::begin(AFEDataAccess *Data, AFEDevice *Device) {
  AFEAPI::begin(Data, Device);
}
#endif

void AFEAPIMQTTStandard::synchronize() {

#ifdef DEBUG
  Serial << endl << F("INFO: Sending current device state to MQTT Broker  ...");
#endif

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (strlen(_Relay[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttStateTopic, "%s/state", _Relay[i]->configuration.mqtt.topic);
      if (!_Relay[i]->setRelayAfterRestoringMQTTConnection()) {
        /* Requesting state from MQTT Broker / service */
        Mqtt.publish(mqttStateTopic, "get");
      } else {
        /* Updating relay state after setting default value after MQTT connected
         */
        publishRelayState(i);
      }
    }
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    publishSwitchState(i);
  }
#endif

/* Synchronize: Binary sensor */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    publishBinarySensorState(i);
  }
#endif

/* Synchronize: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    publishGateState(i);
  }
#endif

/* Synchronize: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    publishContactronState(i);
  }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    publishRegulatorState(i);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    publishThermalProtectorState(i);
  }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {
    publishCLEDState(i);
    publishCLEDEffectsState(i);
  }
#endif // AFE_CONFIG_HARDWARE_CLED
}

void AFEAPIMQTTStandard::subscribe() {

#ifdef DEBUG
  Serial << endl << F("INFO: Subscribing to MQTT Topics ...");
#endif

/* Subscribe: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY

  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    subscribeToCommand(_Relay[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_RELAY, i);
  }
#endif

/* Subscribe: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    subscribeToCommand(_Switch[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_SWITCH, i);
  }
#endif

/* Subscribe: Binary sensors */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    subscribeToCommand(_BinarySensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_BINARY_SENSOR, i);
  }
#endif

/* Subscribe: ADC */
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  for (uint8_t i = 0; i < _Device->configuration.noOfAnalogInputs; i++) {
    subscribeToCommand(_AnalogInput[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_ADC, i);
  }
#else
  if (_Device->configuration.isAnalogInput) {
    subscribeToCommand(_AnalogInput->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_ADC);
  }
#endif // AFE_ESP32
#endif

/* Subscribe: BMx80 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    subscribeToCommand(_BMx80Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_BMX80, i);
  }
#endif

/* Subscribe: BH1750 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    subscribeToCommand(_BH1750Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_BH1750, i);
  }
#endif

/* Subscribe: TSL2561 */
#ifdef AFE_CONFIG_HARDWARE_TSL2561
  for (uint8_t i = 0; i < _Device->configuration.noOfTSL2561s; i++) {
    subscribeToCommand(_TSL2561Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_TSL2561, i);
  }
#endif

/* Subscribe: AS3935 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    subscribeToCommand(_AS3935Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_AS3935, i);
  }
#endif

/* Subscribe: HPMA115S0 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    subscribeToCommand(_HPMA115S0Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_HPMA115S0, i);
  }
#endif

/* Subscribe: ANEMOMETER */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  if (_Device->configuration.noOfAnemometerSensors > 0) {
    subscribeToCommand(_AnemometerSensor->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_ANEMOMETER);
  }
#endif

/* Subscribe: RAIN */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  if (_Device->configuration.noOfRainmeterSensors > 0) {
    subscribeToCommand(_RainmeterSensor->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_RAINMETER);
  }
#endif

/* Subscribe: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    subscribeToCommand(_Contactron[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_CONTACTRON, i);
  }
#endif

/* Subscribe: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    subscribeToCommand(_Gate[i]->configuration.mqtt.topic, AFE_MQTT_DEVICE_GATE,
                       i);
  }
#endif

/* Subscribe: DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  for (uint8_t i = 0; i < _Device->configuration.noOfDS18B20s; i++) {
    subscribeToCommand(_DS18B20Sensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_DS18B20, i);
  }
#endif

/* Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    subscribeToCommand(_Regulator[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_REGULATOR, i);
  }
#endif

/* Thermal protector */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    subscribeToCommand(_ThermalProtector[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_THERMAL_PROTECTOR, i);
  }
#endif

/* Subscribe: DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
  for (uint8_t i = 0; i < _Device->configuration.noOfDHTs; i++) {
    subscribeToCommand(_DHTSensor[i]->configuration.mqtt.topic,
                       AFE_MQTT_DEVICE_DHT, i);
  }
#endif

/* Subscribe: CLED */
#ifdef AFE_CONFIG_HARDWARE_CLED

  char _brightnessTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
  for (uint8_t i = 0; i < _Device->configuration.noOfCLEDs; i++) {

    subscribeToCommand(_CLED->configuration[i].cled.topic, AFE_MQTT_DEVICE_CLED,
                       i);

    sprintf(_brightnessTopic, "%s%s", _CLED->configuration[i].cled.topic,
            AFE_CONFIG_HARDWARE_CLED_BRIGHTNESS_CMD_TOPIC_SUFIX);

    subscribeToCommand(_brightnessTopic, AFE_MQTT_DEVICE_CLED_BRIGHTNESS, i);

    subscribeToCommand(_CLED->configuration[i].effect.topic,
                       AFE_MQTT_DEVICE_CLED_EFFECTS, i);
  }

#endif

#ifdef DEBUG
  if (currentCacheSize > 0 && sizeof(mqttTopicsCache) < currentCacheSize - 1) {
    Serial << endl
           << F("ERROR: MQTT Topics cache[") << sizeof(mqttTopicsCache)
           << F("] is too small : ") << currentCacheSize - 1;
  }
#endif
}

void AFEAPIMQTTStandard::listener() {
  if (Mqtt.listener()) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: MQTT: Got message: ") << Mqtt.message.topic << F(" | ")
           << Mqtt.message.content;
/*
    for (uint8_t i = 0; i < Mqtt.message.length; i++) {
      Serial << (char)Mqtt.message.content[i];
    }
*/
#endif
    for (uint8_t i = 0; i < currentCacheSize; i++) {
      if (strcmp(Mqtt.message.topic, mqttTopicsCache[i].message.topic) == 0) {
#ifdef DEBUG
        Serial << endl
               << F("INFO: MQTT: Found topic in cache: Device Type=")
               << mqttTopicsCache[i].type;
#endif
        switch (mqttTopicsCache[i].type) {
#ifdef AFE_CONFIG_HARDWARE_RELAY
        case AFE_MQTT_DEVICE_RELAY: // RELAY
          processRelay(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_RELAY
#ifdef AFE_CONFIG_HARDWARE_SWITCH
        case AFE_MQTT_DEVICE_SWITCH:
          processSwitch(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_SWITCH
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
        case AFE_MQTT_DEVICE_BINARY_SENSOR:
          processBinarySensor(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_SWITCH
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
        case AFE_MQTT_DEVICE_ADC: // ADC
#ifdef AFE_ESP32
          processADC(&mqttTopicsCache[i].id);
#else
          processADC();
#endif
          break;

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_CONFIG_HARDWARE_BH1750
        case AFE_MQTT_DEVICE_BH1750:
          processBH1750(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_BH1750
#ifdef AFE_CONFIG_HARDWARE_BMEX80
        case AFE_MQTT_DEVICE_BMX80:
          processBMEX80(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_BMEX80
#ifdef AFE_CONFIG_HARDWARE_AS3935
        case AFE_MQTT_DEVICE_AS3935:
          processAS3935(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_AS3935
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
        case AFE_MQTT_DEVICE_ANEMOMETER:
          processAnemometerSensor();
          break;
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
        case AFE_MQTT_DEVICE_RAINMETER:
          processRainSensor();
          break;
#endif // AFE_CONFIG_HARDWARE_RAINMETER
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
        case AFE_MQTT_DEVICE_HPMA115S0:
          processHPMA115S0(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_HPMA115S0
#ifdef AFE_CONFIG_HARDWARE_GATE
        case AFE_MQTT_DEVICE_GATE:
          processGate(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_GATE
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
        case AFE_MQTT_DEVICE_CONTACTRON:
          processContactron(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef AFE_CONFIG_HARDWARE_DS18B20
        case AFE_MQTT_DEVICE_DS18B20:
          processDS18B20(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_DS18B20
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
        case AFE_MQTT_DEVICE_REGULATOR:
          processRegulator(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
        case AFE_MQTT_DEVICE_THERMAL_PROTECTOR:
          processThermalProtector(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef AFE_CONFIG_HARDWARE_DHT
        case AFE_MQTT_DEVICE_DHT:
          processDHT(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_DHT
#ifdef AFE_CONFIG_HARDWARE_CLED
        case AFE_MQTT_DEVICE_CLED:
          processCLED(&mqttTopicsCache[i].id);
          break;
        case AFE_MQTT_DEVICE_CLED_EFFECTS:
          processCLEDEffect(&mqttTopicsCache[i].id);
          break;
        case AFE_MQTT_DEVICE_CLED_BRIGHTNESS:
          processCLEDBrigtness(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_CLED
#ifdef AFE_CONFIG_HARDWARE_TSL2561
        case AFE_MQTT_DEVICE_TSL2561:
          processTSL2561(&mqttTopicsCache[i].id);
          break;
#endif // AFE_CONFIG_HARDWARE_TSL2561
        default:
#ifdef DEBUG
          Serial << endl
                 << F("ERROR: Device type ") << mqttTopicsCache[i].type
                 << F(" not found");
#endif
          break;
        }

        break;
      }
    }
  }
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTStandard::publishRelayState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Relay[id]->configuration.mqtt.topic,
                       _Relay[id]->get() == AFE_RELAY_ON ? AFE_ON : AFE_OFF)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIMQTTStandard::processRelay(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Relay ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _Relay[*id]->on();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _Relay[*id]->off();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _Relay[*id]->toggle();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG
    Serial << endl << F("WARN: MQTT: Command not implemented");
#endif
  }
  if (publishState) {
    publishRelayState(*id);
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTStandard::processSwitch(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Switch ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishSwitchState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishSwitchState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Switch[id]->configuration.mqtt.topic,
                       _Switch[id]->getPhisicalState() ? AFE_OPEN : AFE_CLOSED,
                       true)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
void AFEAPIMQTTStandard::publishADCValues(uint8_t id) {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _AnalogInput[id]->getJSON(message);
    Mqtt.publish(_AnalogInput[id]->configuration.mqtt.topic, message);
  }
}
void AFEAPIMQTTStandard::processADC(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing ADC: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishADCValues(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
#else // AFE_ESP8266
void AFEAPIMQTTStandard::publishADCValues() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ADC_DATA_LENGTH];
    _AnalogInput->getJSON(message);
    Mqtt.publish(_AnalogInput->configuration.mqtt.topic, message);
  }
}
void AFEAPIMQTTStandard::processADC() {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing ADC: ");
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishADCValues();
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
void AFEAPIMQTTStandard::processBatteryMeter(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BatteryMeter: ");
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBatteryMeterValues(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBatteryMeterValues(uint8_t id) {
  boolean _ret = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _AnalogInput[id]->getBatteryMeterJSON(message);
    _ret =
        Mqtt.publish(_AnalogInput[id]->configuration.battery.mqtt.topic, message);
  }
  return _ret;
}

#else  // AFE_ESP8266
void AFEAPIMQTTStandard::processBatteryMeter() {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BatteryMeter: ");
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBatteryMeterValues();
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBatteryMeterValues() {
  boolean _ret = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BATTERYMETER_DATA_LENGTH];
    _AnalogInput->getBatteryMeterJSON(message);
    _ret =
        Mqtt.publish(_AnalogInput->configuration.battery.mqtt.topic, message);
  }
  return _ret;
}
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIMQTTStandard::processBMEX80(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BMX80 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBMx80SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishBMx80SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH];
    _BMx80Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_BMx80Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
void AFEAPIMQTTStandard::processHPMA115S0(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing HPMA115S0 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishHPMA115S0SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishHPMA115S0SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_HPMA115S0_DATA_LENGTH];
    _HPMA115S0Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_HPMA115S0Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIMQTTStandard::processBH1750(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BH1750 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBH1750SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishBH1750SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_BH1750_DATA_LENGTH];
    _BH1750Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_BH1750Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEAPIMQTTStandard::processTSL2561(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing TSL2561 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishTSL2561SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishTSL2561SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_TSL2561_DATA_LENGTH];
    _TSL2561Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_TSL2561Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIMQTTStandard::processAS3935(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing AS3935 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishAS3935SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishAS3935SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_AS3935_DATA_LENGTH];
    _AS3935Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_AS3935Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIMQTTStandard::publishAnemometerSensorData() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_ANEMOMETER_DATA_LENGTH];
    _AnemometerSensor->getJSON(message);
    Mqtt.publish(_AnemometerSensor->configuration.mqtt.topic, message);
  }
}

void AFEAPIMQTTStandard::processAnemometerSensor() {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Anemometer: ");
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishAnemometerSensorData();
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIMQTTStandard::publishRainSensorData() {
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_RAINMETER_DATA_LENGTH];
    _RainmeterSensor->getJSON(message);
    Mqtt.publish(_RainmeterSensor->configuration.mqtt.topic, message);
  }
}

void AFEAPIMQTTStandard::processRainSensor() {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Rain: ");
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishRainSensorData();
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIMQTTStandard::processGate(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Gate ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 't' && Mqtt.message.length == 6) {
    _Gate[*id]->toggle();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishGateState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishGateState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Gate[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state", _Gate[id]->configuration.mqtt.topic);
      uint8_t _state = _Gate[id]->get();
      publishStatus = Mqtt.publish(
          mqttStateTopic, _state == AFE_GATE_OPEN
                              ? AFE_MQTT_GATE_OPEN
                              : _state == AFE_GATE_CLOSED
                                    ? AFE_MQTT_GATE_CLOSED
                                    : _state == AFE_GATE_PARTIALLY_OPEN
                                          ? AFE_MQTT_GATE_PARTIALLY_OPEN
                                          : AFE_MQTT_GATE_UNKNOWN);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIMQTTStandard::processContactron(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Contactron ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishContactronState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishContactronState(uint8_t id) {
  // TODO T5 - check if works as in previous version
  return enabled ? publishOnOffState(_Contactron[id]->configuration.mqtt.topic,
                                     _Contactron[id]->get(), true)
                 : false;
}
#endif //  AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIMQTTStandard::processDS18B20(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing DS18B20 ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishDS18B20SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishDS18B20SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_DS18B20_DATA_LENGTH];
    _DS18B20Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_DS18B20Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
boolean AFEAPIMQTTStandard::publishRegulatorState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _Regulator[id]->configuration.mqtt.topic,
                       _Regulator[id]->configuration.enabled ? AFE_ON : AFE_OFF)
                 : false;
}

void AFEAPIMQTTStandard::processRegulator(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Regulator ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _Regulator[*id]->on();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _Regulator[*id]->off();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _Regulator[*id]->toggle();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG
    Serial << endl << F("WARN: MQTT: Command not implemented");
#endif
  }
  if (publishState) {
    publishRegulatorState(*id);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
boolean AFEAPIMQTTStandard::publishThermalProtectorState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _ThermalProtector[id]->configuration.mqtt.topic,
                       _ThermalProtector[id]->configuration.enabled ? AFE_ON
                                                                    : AFE_OFF)
                 : false;
}

void AFEAPIMQTTStandard::processThermalProtector(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Themrnal Protector ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_ON) == 0) {
    _ThermalProtector[*id]->on();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_OFF) == 0) {
    _ThermalProtector[*id]->off();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_TOGGLE) ==
             0) {
    _ThermalProtector[*id]->toggle();
  } else if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
  } else {
    publishState = false;
#ifdef DEBUG
    Serial << endl << F("WARN: MQTT: Command not implemented");
#endif
  }
  if (publishState) {
    publishThermalProtectorState(*id);
  }
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPIMQTTStandard::processDHT(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing DHT ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishDHTSensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishDHTSensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_DHT_DATA_LENGTH];
    _DHTSensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_DHTSensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIMQTTStandard::processBinarySensor(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Binary Sensor ID: ") << *id;
#endif
  if (strcmp(Mqtt.message.content, AFE_CONFIG_MQTT_COMMAND_GET) == 0) {
    publishBinarySensorState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishBinarySensorState(uint8_t id) {
  return enabled ? publishOnOffState(
                       _BinarySensor[id]->configuration.mqtt.topic,
                       _BinarySensor[id]->get() == AFE_BINARY_SENSOR_OPEN
                           ? AFE_OPEN
                           : AFE_CLOSED,
                       !_BinarySensor[id]->configuration.sendAsSwitch)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
boolean AFEAPIMQTTStandard::publishPN532SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_PN532Sensor[id]->configuration.mqtt.topic) > 0) {
      char message[AFE_CONFIG_API_JSON_PN582_DATA_LENGTH];
      _PN532Sensor[id]->getJSON(message);
      publishStatus =
          Mqtt.publish(_PN532Sensor[id]->configuration.mqtt.topic, message);
    }
  }
  return publishStatus;
}
boolean AFEAPIMQTTStandard::publishMiFareCardState(uint8_t id, uint8_t state) {
  return enabled
             ? publishOnOffState(_MiFareCard[id]->configuration.mqtt.topic,
                                 state == AFE_HARDWARE_MIFARE_CARD_ACTION_ON
                                     ? AFE_ON
                                     : AFE_OFF,
                                 !_MiFareCard[id]->configuration.sendAsSwitch)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIMQTTStandard::processCLED(uint8_t *id) {
  boolean _success = true;
#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT: Processing CLED: ") << *id << F(" : command: ");
#endif

  char _command[strlen(Mqtt.message.content) + 1];

  sprintf(_command, "%s", Mqtt.message.content);
/*
  for (uint16_t i = 0; i < Mqtt.message.length; i++) {
    _command[i] = (char)Mqtt.message.content[i];
  }
  _command[Mqtt.message.length] = AFE_EMPTY_STRING;
*/

#ifdef DEBUG
  Serial << _command;
#endif

  StaticJsonBuffer<AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(_command);
#ifdef DEBUG
  Serial << endl
         << F("INFO: API REST: JSON Buffer size: ")
         << AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH
         << F(", actual JSON size: ") << jsonBuffer.size();
  if (AFE_CONFIG_HARDWARE_CLED_CMD_JSON_LENGTH < jsonBuffer.size() + 10) {
    Serial << endl << F("WARN: API REST: Too small buffer size");
  }
#endif
  if (!root.success()) {
#ifdef DEBUG
    Serial << endl << F("ERROR: API REST: JSON Parsing error");
#endif
  } else {

    CLED_PARAMETERS _color;
    char _state[4];

    sprintf(_state, root["command"] | "");

    if (strcmp(_state, "on") == 0) {
#ifdef DEBUG
      Serial << endl << F("INFO: CLED: Processing ON");
#endif
      _CLED->on(*id, true);
    } else if (strcmp(_state, "off") == 0) {
#ifdef DEBUG
      Serial << endl << F("INFO: CLED: Processing OFF");
#endif
      _CLED->off(*id, false);
    } else {
      _color.color.blue = root["color"]["blue"];
      _color.color.green = root["color"]["green"];
      _color.color.red = root["color"]["red"];
      _color.brightness =
          root["brightness"] | _CLED->currentState[*id].on.brightness;

#ifdef DEBUG
      Serial << endl
             << F("INFO: CLED: Changing color: RGB[") << _color.color.red
             << F(",") << _color.color.green << F(",") << _color.color.blue
             << F("], Brightness: ") << _color.brightness;
#endif

      /**
       * @brief if color is set to RGB=0 then turning OFF LED strip, otherwise
       * turn it ON
       *
       */

      if (_color.color.blue == 0 && _color.color.green == 0 &&
          _color.color.red == 0) {
        _CLED->off(*id, false);
      } else {
        _CLED->on(*id, _color, true, true);
      }
    }
  }

  if (_success) {
    if (_CLED->isStateUpdated(*id)) {
      publishCLEDState(*id);
    }
    if (_CLED->isEffectStateUpdated(*id)) {
      publishCLEDEffectsState(*id);
    }
  }
}

void AFEAPIMQTTStandard::processCLEDEffect(uint8_t *id) {
  boolean _success = true;
#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT: Processing CLED: ") << *id << F(" : effect: ");
#endif

  char _command[strlen(Mqtt.message.content) + 1];
  sprintf(_command, "%s", Mqtt.message.content);
/*
for (uint16_t i = 0; i < Mqtt.message.length; i++) {
  _command[i] = (char)Mqtt.message.content[i];
}
_command[Mqtt.message.length] = AFE_EMPTY_STRING;
*/
#ifdef DEBUG
  Serial << _command;
#endif

  if (strcmp(_command, AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF) == 0) {
    _CLED->deactivateEffect(*id);
  } else if (strcmp(_command, _CLED->configurationEffectBlinking[*id].name) ==
             0) {
    _CLED->activateEffect(*id, AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING);
  } else if (strcmp(_command, _CLED->configurationEffectFadeInOut[*id].name) ==
             0) {
    _CLED->activateEffect(*id, AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT);
  } else if (strcmp(_command, _CLED->configurationEffectWave[*id].name) == 0) {
    _CLED->activateEffect(*id, AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE);
  } else {
    _success = false;
  }

  if (_success) {
    if (_CLED->isEffectStateUpdated(*id)) {
      publishCLEDEffectsState(*id);
    }
    if (_CLED->isStateUpdated(*id)) {
      publishCLEDState(*id);
    }
  }
}

void AFEAPIMQTTStandard::processCLEDBrigtness(uint8_t *id) {
#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT: Processing CLED: ") << *id << F(" : brightness : ");
#endif

  char _command[strlen(Mqtt.message.content) + 1];
  sprintf(_command, "%s", Mqtt.message.content);

/*
for (uint16_t i = 0; i < Mqtt.message.length; i++) {
 _command[i] = (char)Mqtt.message.content[i];
}
_command[Mqtt.message.length] = AFE_EMPTY_STRING;
*/
#ifdef DEBUG
  Serial << _command;
#endif
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
  uint8_t _brightness = _CLED->convertBrightnessFromAPI(*id, atof(_command));
#else
  uint8_t _brightness = atoi(_command);
#endif // AFE_FIRMWARE_API_STANDARD

  if (_brightness > 0) {
    CLED_PARAMETERS _color = _CLED->currentState[*id].on;
    _color.brightness = _brightness;
    _CLED->on(*id, _color, true, true);
  } else {
    _CLED->off(*id, true);
  }

  if (_CLED->isStateUpdated(*id)) {
    publishCLEDState(*id);
  }
  if (_CLED->isEffectStateUpdated(*id)) {
    publishCLEDEffectsState(*id);
  }
}

boolean AFEAPIMQTTStandard::publishCLEDEffectsState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_CLED->configuration[id].effect.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _CLED->configuration[id].effect.topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic,
          _CLED->currentState[id].effect.id ==
                  AFE_CONFIG_HARDWARE_CLED_EFFECT_BINKING
              ? _CLED->configurationEffectBlinking[id].name
              : _CLED->currentState[id].effect.id ==
                        AFE_CONFIG_HARDWARE_CLED_EFFECT_FADE_IN_OUT
                    ? _CLED->configurationEffectFadeInOut[id].name
                    : _CLED->currentState[id].effect.id ==
                              AFE_CONFIG_HARDWARE_CLED_EFFECT_WAVE
                          ? _CLED->configurationEffectWave[id].name
                          : AFE_CONFIG_HARDWARE_CLED_EFFECT_CMD_OFF);
    }
  }
  return publishStatus;
}

boolean AFEAPIMQTTStandard::publishCLEDState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_CLED->configuration[id].cled.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      char _message[AFE_CONFIG_HARDWARE_CLED_STATE_JSON_LENGTH];

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_STANDARD
      sprintf(_message, "{\"state\":\"%s\",\"color\":{\"red\":%d,\"green\":%d,"
                        "\"blue\":%d},\"brightness\":%.2f}",
              _CLED->currentState[id].state ? "on" : "off",
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.red
                  : _CLED->currentState[id].off.color.red,
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.green
                  : _CLED->currentState[id].off.color.green,
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.blue
                  : _CLED->currentState[id].off.color.blue,
              _CLED->convertBrigtnessToAPI(
                  id, _CLED->currentState[id].config.brightness));
#else
      sprintf(_message, "{\"state\":\"%s\",\"color\":{\"red\":%d,\"green\":%d,"
                        "\"blue\":%d},\"brightness\":%d}",
              _CLED->currentState[id].state ? "on" : "off",
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.red
                  : _CLED->currentState[id].off.color.red,
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.green
                  : _CLED->currentState[id].off.color.green,
              _CLED->currentState[id].state
                  ? _CLED->currentState[id].on.color.blue
                  : _CLED->currentState[id].off.color.blue,
              _CLED->currentState[id].config.brightness);
#endif
      sprintf(mqttStateTopic, "%s/state", _CLED->configuration[id].cled.topic);
      publishStatus = Mqtt.publish(mqttStateTopic, _message);
    }
  }
  return publishStatus;
}

#endif // AFE_CONFIG_HARDWARE_CLED

/**** Private methods ****/

void AFEAPIMQTTStandard::subscribeToCommand(
    const char *topic, afe_mqtt_standard_device_type_t topicId, uint8_t index) {
  if (strlen(topic) > 0) {
    char mqttCommandTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];
    sprintf(mqttCommandTopic, "%s/cmd", topic);
    Mqtt.subscribe(mqttCommandTopic);
    sprintf(mqttTopicsCache[currentCacheSize].message.topic, mqttCommandTopic);
    mqttTopicsCache[currentCacheSize].id = index;
    mqttTopicsCache[currentCacheSize].type = topicId;
    currentCacheSize++;
  }
}

boolean AFEAPIMQTTStandard::publishOnOffState(const char *topic, uint8_t state,
                                              boolean sendAsOpenClosed) {
  boolean publishStatus = false;

  if (enabled && (state == AFE_ON || state == AFE_OFF)) {
    if (strlen(topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state", topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic,
          state == AFE_ON ? sendAsOpenClosed ? AFE_CLOSED_LABEL : AFE_ON_LABEL
                          : sendAsOpenClosed ? AFE_OPEN_LABEL : AFE_OFF_LABEL);
    }
  }
  return publishStatus;
}

uint8_t AFEAPIMQTTStandard::processOnOffCommand(byte *command,
                                                uint16_t *length) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing ON/OFF command: ");
#endif
  uint8_t _return = AFE_NONE;
  if ((char)command[1] == 'n' && *length == 2) { /* ON */
    _return = AFE_ON;
  } else if ((char)command[1] == 'f' && *length == 3) { /* OFF */
    _return = AFE_OFF;
#ifdef DEBUG
  } else {
    Serial << endl << F("WARN: MQTT: Command: not implemented");
#endif
  }
  return _return;
}

#endif