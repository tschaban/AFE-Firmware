
/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Standard.h"

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED

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

void AFEAPIMQTTStandard::listener() {
  if (Mqtt.listener()) {
    processRequest();
  }
}

void AFEAPIMQTTStandard::synchronize() {

#ifdef DEBUG
  Serial << endl << F("INFO: Sending current device state to MQTT Broker  ...");
#endif

  // workaround for LWT retain flag
  boolean _retainAll = Mqtt.configuration.retainAll;
  Mqtt.configuration.retainAll = Mqtt.configuration.retainLWT;
  Mqtt.publish(Mqtt.configuration.lwt.topic, "connected");
  Mqtt.configuration.retainAll = _retainAll;

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
}

void AFEAPIMQTTStandard::subscribe() {

#ifdef DEBUG
  Serial << endl << F("INFO: Subscribing to MQTT Topics ...");
#endif

  char mqttCommandTopic[AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH];

/* Subscribe: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY

  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (strlen(_Relay[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd", _Relay[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_RELAY;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    if (strlen(_Switch[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd", _Switch[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_SWITCH;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Binary sensors */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Device->configuration.noOfBinarySensors; i++) {
    if (strlen(_BinarySensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _BinarySensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_BINARY_SENSOR;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: ADC */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (_Device->configuration.isAnalogInput) {
    if (strlen(_AnalogInput->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _AnalogInput->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_ADC;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: BMx80 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    if (strlen(_BMx80Sensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _BMx80Sensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_BMX80;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: BH1750 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    if (strlen(_BH1750Sensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _BH1750Sensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_BH1750;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: AS3935 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    if (strlen(_AS3935Sensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _AS3935Sensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_AS3935;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: HPMA115S0 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    if (strlen(_HPMA115S0Sensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _HPMA115S0Sensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_HPMA115S0;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: ANEMOMETER */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  if (_Device->configuration.noOfAnemometerSensors > 0) {
    if (strlen(_AnemometerSensor->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _AnemometerSensor->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_ANEMOMETER;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: RAIN */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  if (_Device->configuration.noOfRainmeterSensors > 0) {

    if (strlen(_RainmeterSensor->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _RainmeterSensor->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_RAINMETER;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    if (strlen(_Contactron[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _Contactron[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_CONTACTRON;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    if (strlen(_Gate[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd", _Gate[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_GATE;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: DS18B20 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
  for (uint8_t i = 0; i < _Device->configuration.noOfDS18B20s; i++) {
    if (strlen(_DS18B20Sensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _DS18B20Sensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_DS18B20;
      currentCacheSize++;
    }
  }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Device->configuration.noOfRegulators; i++) {
    if (strlen(_Regulator[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _Regulator[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_REGULATOR;
      currentCacheSize++;
    }
  }
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0; i < _Device->configuration.noOfThermalProtectors; i++) {
    if (strlen(_ThermalProtector[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _ThermalProtector[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type =
          AFE_MQTT_DEVICE_THERMAL_PROTECTOR;
      currentCacheSize++;
    }
  }
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

/* Subscribe: DHT */
#ifdef AFE_CONFIG_HARDWARE_DHT
  for (uint8_t i = 0; i < _Device->configuration.noOfDHTs; i++) {
    if (strlen(_DHTSensor[i]->configuration.mqtt.topic) > 0) {
      sprintf(mqttCommandTopic, "%s/cmd",
              _DHTSensor[i]->configuration.mqtt.topic);
      Mqtt.subscribe(mqttCommandTopic);
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_DHT;
      currentCacheSize++;
    }
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

void AFEAPIMQTTStandard::processRequest() {
#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT: Got message: ") << Mqtt.message.topic << F(" | ");
  for (uint8_t i = 0; i < Mqtt.message.length; i++) {
    Serial << (char)Mqtt.message.content[i];
  }
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
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      case AFE_MQTT_DEVICE_ADC: // ADC
        processADC();
        break;
#endif // AFE_CONFIG_HARDWARE_ADC_VCC
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

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTStandard::publishRelayState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Relay[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state", _Relay[id]->configuration.mqtt.topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic, _Relay[id]->get() == AFE_RELAY_ON ? "on" : "off");
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIMQTTStandard::processRelay(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Relay ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 2) {
    _Relay[*id]->on();
  } else if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 3) {
    _Relay[*id]->off();
  } else if ((char)Mqtt.message.content[0] == 't' && Mqtt.message.length == 6) {
    _Relay[*id]->toggle();
  } else if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishSwitchState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
boolean AFEAPIMQTTStandard::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Switch[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _Switch[id]->configuration.mqtt.topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic, _Switch[id]->getPhisicalState() ? "open" : "closed");
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishADCValues();
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
void AFEAPIMQTTStandard::processBatteryMeter() {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BatteryMeter: ");
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIMQTTStandard::processBMEX80(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing BMX80 ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIMQTTStandard::processAS3935(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing AS3935 ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  } else if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishContactronState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishContactronState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Contactron[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _Contactron[id]->configuration.mqtt.topic);
      publishStatus = Mqtt.publish(mqttStateTopic,
                                   _Contactron[id]->get() == AFE_CONTACTRON_OPEN
                                       ? AFE_MQTT_CONTACTRON_OPEN
                                       : AFE_MQTT_CONTACTRON_CLOSED);
    }
  }
  return publishStatus;
}
#endif //  AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIMQTTStandard::processDS18B20(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing DS18B20 ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_Regulator[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _Regulator[id]->configuration.mqtt.topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic, _Regulator[id]->configuration.enabled ? "on" : "off");
    }
  }
  return publishStatus;
}

void AFEAPIMQTTStandard::processRegulator(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Regulator ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 2) {
    _Regulator[*id]->on();
  } else if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 3) {
    _Regulator[*id]->off();
  } else if ((char)Mqtt.message.content[0] == 't' && Mqtt.message.length == 6) {
    _Regulator[*id]->toggle();
  } else if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  boolean publishStatus = false;
  if (enabled) {

    if (strlen(_ThermalProtector[id]->configuration.mqtt.topic) > 0) {
      char mqttStateTopic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(mqttStateTopic, "%s/state",
              _ThermalProtector[id]->configuration.mqtt.topic);
      publishStatus = Mqtt.publish(
          mqttStateTopic,
          _ThermalProtector[id]->configuration.enabled ? "on" : "off");
    }
  }
  return publishStatus;
}

void AFEAPIMQTTStandard::processThermalProtector(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Processing Themrnal Protector ID: ") << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 2) {
    _ThermalProtector[*id]->on();
  } else if ((char)Mqtt.message.content[0] == 'o' && Mqtt.message.length == 3) {
    _ThermalProtector[*id]->off();
  } else if ((char)Mqtt.message.content[0] == 't' && Mqtt.message.length == 6) {
    _ThermalProtector[*id]->toggle();
  } else if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
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
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishBinarySensorState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << F("WARN: MQTT: Command not implemented");
  }
#endif
}

boolean AFEAPIMQTTStandard::publishBinarySensorState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    if (strlen(_BinarySensor[id]->configuration.mqtt.topic) > 0) {
      publishStatus = Mqtt.publish(
          _BinarySensor[id]->configuration.mqtt.topic,
          _BinarySensor[id]->get() == 1
              ? _BinarySensor[id]->configuration.sendAsSwitch ? "off" : "open"
              : _BinarySensor[id]->configuration.sendAsSwitch ? "on"
                                                              : "closed");
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
boolean AFEAPIMQTTStandard::publishPN532SensorData(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    char message[AFE_CONFIG_API_JSON_PN582_DATA_LENGTH];
    _PN532Sensor[id]->getJSON(message);
    publishStatus =
        Mqtt.publish(_PN532Sensor[id]->configuration.mqtt.topic, message);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#endif // #ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED