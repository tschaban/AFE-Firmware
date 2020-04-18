
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
#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
    processRequest();
#endif
  }
}

void AFEAPIMQTTStandard::synchronize() {

#ifdef DEBUG
  Serial << endl << "INFO: Sending current device state to MQTT Broker  ...";
#endif

  Mqtt.publish(Mqtt.configuration.lwt.topic, "connected");

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    if (!_Relay[i]->setRelayAfterRestoringMQTTConnection()) {
      /* Requesting state from MQTT Broker / service */
      Mqtt.publish(_Relay[i]->mqttStateTopic, "get");
    } else {
      /* Updating relay state after setting default value after MQTT connected
       */
      publishRelayState(i);
    }
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    publishSwitchState(i);
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
}

void AFEAPIMQTTStandard::subscribe() {

#ifdef DEBUG
  Serial << endl << "INFO: Subsribing to MQTT Topics ...";
#endif

/* Subscribe: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    Mqtt.subscribe(_Relay[i]->mqttCommandTopic);
    if (strlen(_Relay[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _Relay[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_RELAY;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    Mqtt.subscribe(_Switch[i]->mqttCommandTopic);
    if (strlen(_Switch[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _Switch[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_SWITCH;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: ADC */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  if (_Device->configuration.isAnalogInput) {
    Mqtt.subscribe(_AnalogInput->mqttCommandTopic);
    if (strlen(_AnalogInput->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _AnalogInput->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_ADC;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: BMx80 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
  for (uint8_t i = 0; i < _Device->configuration.noOfBMEX80s; i++) {
    Mqtt.subscribe(_BMx80Sensor[i]->mqttCommandTopic);
    if (strlen(_BMx80Sensor[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _BMx80Sensor[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_BMX80;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: BH1750 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
  for (uint8_t i = 0; i < _Device->configuration.noOfBH1750s; i++) {
    Mqtt.subscribe(_BH1750Sensor[i]->mqttCommandTopic);
    if (strlen(_BH1750Sensor[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _BH1750Sensor[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_BH1750;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: AS3935 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
  for (uint8_t i = 0; i < _Device->configuration.noOfAS3935s; i++) {
    Mqtt.subscribe(_AS3935Sensor[i]->mqttCommandTopic);
    if (strlen(_AS3935Sensor[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _AS3935Sensor[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_AS3935;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: HPMA115S0 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  for (uint8_t i = 0; i < _Device->configuration.noOfHPMA115S0s; i++) {
    Mqtt.subscribe(_HPMA115S0Sensor[i]->mqttCommandTopic);
    if (strlen(_HPMA115S0Sensor[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _HPMA115S0Sensor[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_HPMA115S0;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Device->configuration.noOfContactrons; i++) {
    Mqtt.subscribe(_Contactron[i]->mqttCommandTopic);
    if (strlen(_Contactron[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _Contactron[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_CONTACTRON;
      currentCacheSize++;
    }
  }
#endif

/* Subscribe: Contactron */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Device->configuration.noOfGates; i++) {
    Mqtt.subscribe(_Gate[i]->mqttCommandTopic);
    if (strlen(_Gate[i]->mqttCommandTopic) > 0) {
      sprintf(mqttTopicsCache[currentCacheSize].message.topic,
              _Gate[i]->mqttCommandTopic);
      mqttTopicsCache[currentCacheSize].id = i;
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_GATE;
      currentCacheSize++;
    }
  }
#endif
}

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
void AFEAPIMQTTStandard::processRequest() {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Got message: " << Mqtt.message.topic << " | ";
  for (uint8_t i = 0; i < Mqtt.message.length; i++) {
    Serial << (char)Mqtt.message.content[i];
  }
#endif
  for (uint8_t i = 0; i < currentCacheSize; i++) {
    if (strcmp(Mqtt.message.topic, mqttTopicsCache[i].message.topic) == 0) {
#ifdef DEBUG
      Serial << endl
             << "INFO: MQTT: Found topic in cache: Device Type="
             << mqttTopicsCache[i].type;
#endif
      switch (mqttTopicsCache[i].type) {
#ifdef AFE_CONFIG_HARDWARE_RELAY
      case AFE_MQTT_DEVICE_RELAY: // RELAY
        processRelay(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_SWITCH
      case AFE_MQTT_DEVICE_SWITCH:
        processSwitch(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      case AFE_MQTT_DEVICE_ADC: // ADC
        processADC();
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
      case AFE_MQTT_DEVICE_BH1750:
        processBH1750(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
      case AFE_MQTT_DEVICE_BMX80:
        processBMEX80(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_AS3935
      case AFE_MQTT_DEVICE_AS3935:
        processAS3935(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
      case AFE_MQTT_DEVICE_HPMA115S0:
        processHPMA115S0(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
      case AFE_MQTT_DEVICE_GATE:
        processGate(&mqttTopicsCache[i].id);
        break;
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
      case AFE_MQTT_DEVICE_CONTACTRON:
        processContactron(&mqttTopicsCache[i].id);
        break;
#endif
      default:
#ifdef DEBUG
        Serial << endl
               << "ERROR: Device type " << mqttTopicsCache[i].type
               << " not found";
#endif
        break;
      }

      break;
    }
  }
}
#endif // AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTStandard::publishRelayState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    publishStatus =
        Mqtt.publish(_Relay[id]->mqttStateTopic,
                     _Relay[id]->get() == AFE_RELAY_ON ? "on" : "off");
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#if defined(AFE_CONFIG_HARDWARE_RELAY) &&                                      \
    defined(AFE_CONFIG_API_PROCESS_REQUESTS)
void AFEAPIMQTTStandard::processRelay(uint8_t *id) {
  boolean publishState = true;

#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing Relay ID: " << *id;
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
    Serial << endl << "WARN: MQTT: Command not implemente";
#endif
  }
  if (publishState) {
    publishRelayState(*id);
  }
}
#endif // AFE_CONFIG_HARDWARE_RELAY && AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIMQTTStandard::processSwitch(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing Switch ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishSwitchState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIMQTTStandard::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    publishStatus =
        Mqtt.publish(_Switch[id]->mqttStateTopic,
                     _Switch[id]->getPhisicalState() ? "open" : "closed");
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
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#if defined(AFE_CONFIG_HARDWARE_ADC_VCC) &&                                    \
    defined(AFE_CONFIG_API_PROCESS_REQUESTS)
void AFEAPIMQTTStandard::processADC() {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing ADC: ";
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishADCValues();
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
  }
#endif
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC && AFE_CONFIG_API_PROCESS_REQUESTS

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIMQTTStandard::processBMEX80(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing BMX80 ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishBMx80SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
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
  Serial << endl << "INFO: MQTT: Processing HPMA115S0 ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishHPMA115S0SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
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
  Serial << endl << "INFO: MQTT: Processing BH1750 ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishBH1750SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
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
  Serial << endl << "INFO: MQTT: Processing AS3935 ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishAS3935SensorData(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
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

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIMQTTStandard::processGate(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing Gate ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 't' && Mqtt.message.length == 6) {
    _Gate[*id]->toggle();
  } else if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishGateState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
  }
#endif
}

boolean AFEAPIMQTTStandard::publishGateState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    uint8_t _state = _Gate[id]->get();
    publishStatus = Mqtt.publish(_Gate[id]->mqttStateTopic,
                                 _state == AFE_GATE_OPEN
                                     ? AFE_MQTT_GATE_OPEN
                                     : _state == AFE_GATE_CLOSED
                                           ? AFE_MQTT_GATE_CLOSED
                                           : _state == AFE_GATE_PARTIALLY_OPEN
                                                 ? AFE_MQTT_GATE_PARTIALLY_OPEN
                                                 : AFE_MQTT_GATE_UNKNOWN);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIMQTTStandard::processContactron(uint8_t *id) {
#ifdef DEBUG
  Serial << endl << "INFO: MQTT: Processing Contactron ID: " << *id;
#endif
  if ((char)Mqtt.message.content[0] == 'g' && Mqtt.message.length == 3) {
    publishContactronState(*id);
  }
#ifdef DEBUG
  else {
    Serial << endl << "WARN: MQTT: Command not implemente";
  }
#endif
}

boolean AFEAPIMQTTStandard::publishContactronState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled) {
    publishStatus = Mqtt.publish(_Contactron[id]->mqttStateTopic,
                                 _Contactron[id]->get() == AFE_CONTACTRON_OPEN
                                     ? AFE_MQTT_CONTACTRON_OPEN
                                     : AFE_MQTT_CONTACTRON_CLOSED);
  }
  return publishStatus;
}
#endif //  AFE_CONFIG_HARDWARE_CONTACTRON

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED