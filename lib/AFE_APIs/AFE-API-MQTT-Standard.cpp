
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

#ifdef AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
  Mqtt.publish(AFEAPI::Mqtt.getLWTTopic(), "connected");
#endif

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
      mqttTopicsCache[currentCacheSize].type = AFE_MQTT_DEVICE_RELAY;
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
      case AFE_MQTT_DEVICE_RELAY: // RELAY
        processRelay(&mqttTopicsCache[i].id);
        break;
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
      case AFE_MQTT_DEVICE_ADC: // ADC
        processADC();
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

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED