#include "AFE-Event.h"

AFEEvent::AFEEvent() {}

void AFEEvent::begin(AFEGlobalObject *_Object) {
  Object = _Object;
  timer->oneMinute = millis();
  timer->oneHour = 0;
  timer->oneDay = 0;
}

void AFEEvent::listener(void) {
  /* Events triggered every 1m  */
  if (millis() - timer->oneMinute >= AFE_EVENTS_ONE_MINUTE) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 1m");
#endif
    timer->oneMinute = millis();
    timer->oneHour++;

    if (Object->Core->Device->getMode() == AFE_MODE_NORMAL) {
      if (Object->Core->Network->connected()) {
        checkingFirmwareVersion();
      }
    }
  }

  /* Events triggered every 1h  */
  if (timer->oneHour > AFE_EVENTS_ONE_HOUR) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 1hr");
#endif
    timer->oneHour = 0;
    timer->oneDay++;
  }

  /* Events triggered every 1h  */
  if (timer->oneDay > AFE_EVENTS_ONE_DAY) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 24hr");
#endif
    timer->oneDay = 0;
  }

  /* Instant triggered events */

  if (Object->Core->Network->eventDisconnected()) {
    disconnectedFromNetwork();
  }

  if (Object->Core->Network->connected()) {
    connectedToMQTTBroker();
  }

  disconnectedFromMQTTBroker();
}

/**
 * @brief Events
 *
 */

void AFEEvent::conenctedToNetwork(void) {

// Update HTTP states for Domoticz HTTP API
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (Object->Core->Device->getMode() == AFE_MODE_NORMAL) {
    if (Object->Core->Device->configuration.api.domoticz) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Domoticz HTTP API boot actions triggering");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Object->Core->SystemLed->on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current gate state to Domoticz");
#endif
      for (uint8_t i = 0; i < Object->Core->Device->configuration.noOfGates;
           i++) {
        Object->Core->HttpDomoticzAPI->publishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of contactrons to "
                  "Domoticz");
#endif
      for (uint8_t i = 0;
           i < Object->Core->Device->configuration.noOfContactrons; i++) {
        Object->Core->HttpDomoticzAPI->publishContactronState(i);
        lastPublishedContactronState[i] = Object->Hardware->Contactron[i].get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
      Serial
          << endl
          << F("INFO: EVENTS: Sending current state of switches to Domoticz");
#endif
      for (uint8_t i = 0; i < Object->Core->Device->configuration.noOfSwitches;
           i++) {
        Object->Core->HttpDomoticzAPI->publishSwitchState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of binary sensors to "
                  "Domoticz");
#endif
      for (uint8_t i = 0;
           i < Object->Core->Device->configuration.noOfBinarySensors; i++) {
        Object->Core->HttpDomoticzAPI->publishBinarySensorState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of RGB LEDs "
                  "Domoticz");
#endif
      for (uint8_t i = 0; i < Object->Core->Device->configuration.noOfCLEDs;
           i++) {
        //     Object->Core->HttpDomoticzAPI->publishCLEDState(i);
        //     Object->Core->HttpDomoticzAPI->publishCLEDEffectState(i);
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of regulator to "
                  "Domoticz");
#endif
      for (uint8_t i = 0;
           i < Object->Core->Device->configuration.noOfRegulators; i++) {
        Object->Core->HttpDomoticzAPI->publishRegulatorState(i);
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of regulator to "
                  "Domoticz");
#endif
      for (uint8_t i = 0;
           i < Object->Core->Device->configuration.noOfThermalProtectors; i++) {
        Object->Core->HttpDomoticzAPI->publishThermalProtectorState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of relays to Domoticz");
#endif
      for (uint8_t i = 0; i < Object->Core->Device->configuration.noOfRelays;
           i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (Object->Hardware->Relay[i].gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
          Object->Core->HttpDomoticzAPI->publishRelayState(i);
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* Closing the condition for skipping relay if assigned to a gate */
        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("INFO: EVENTS: Excluding relay: ") << i
                 << F(" as it's assigned to a Gate: ")
                 << Object->Hardware->Relay[i].gateId;
        }
#endif
#endif
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Object->Core->SystemLed->off();
#endif
    }

#ifdef DEBUG
    Serial << endl << F("INFO: EVENTS: Post WiFi Connection actions completed");
#endif
  }
#endif
  // Checing access to WAN
  Object->Core->RestAPI->checkAccessToWAN();
}

void AFEEvent::disconnectedFromNetwork(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: EVENT: Network disconnected: triggered");
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  Object->Hardware->SystemLed->on();
#endif
  Object->Core->RestAPI->setNoWANAccess();
#ifdef AFE_CONFIG_HARDWARE_LED
  Object->Hardware->SystemLed->off();
#endif
}

void AFEEvent::connectedToMQTTBroker(void) {
  /* Event: connected to MQTT API */
  if (Object->Core->Device->getMode() == AFE_MODE_NORMAL &&
      Object->Core->Device->configuration.api.mqtt) {
    if (Object->Core->MqttAPI->Mqtt->connectedEvent()) {
#ifdef DEBUG
      Serial << endl << F("INFO: EVENT: MQTT Connected: triggered");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Object->Hardware->SystemLed->on();
#endif
      Object->Core->MqttAPI->subscribe();

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
      publishMQTTHADiscoveryConfiguration();
#endif

      Object->Core->MqttAPI->synchronize();
#ifdef AFE_CONFIG_HARDWARE_LED
      Object->Hardware->SystemLed->off();
#endif
    }
  }
}

void AFEEvent::disconnectedFromMQTTBroker(void) {
  if (Object->Core->MqttAPI->Mqtt->disconnectedEvent()) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: EVENT: MQTT Disconnected: triggered") << endl
           << F("Forcing disconnection with MQTT Broker in the object only");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    Object->Hardware->SystemLed->on();
#endif
    Object->Core->MqttAPI->Mqtt->_Broker->disconnect(true);

#ifdef AFE_CONFIG_HARDWARE_LED
    Object->Hardware->SystemLed->off();
#endif
  }
}

/**
 * @brief Publishing device configuration to Home Assistant
 *
 */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
void AFEEvent::publishMQTTHADiscoveryConfiguration(void) {
  AFEAPIHomeAssistantIntegration *HomeAssistantDiscoveryAPI =
      new AFEAPIHomeAssistantIntegration(
          Object->Core->Data, Object->Core->Device, Object->Core->MqttAPI,
          Object->Core->Firmware);
  HomeAssistantDiscoveryAPI->publish();
  delete HomeAssistantDiscoveryAPI;
  HomeAssistantDiscoveryAPI = NULL;
}
#endif

void AFEEvent::checkingFirmwareVersion(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: EVENT: Checking firmware version");
#endif

  if (strlen(Object->Core->MqttAPI->Mqtt->configuration->status.topic) > 0) {
    String _json;
    Object->Core->RestAPI->sent(
        _json, AFE_CONFIG_JSONRPC_REST_METHOD_GET_JSON_LATEST_FIRMWARE_VERSION);
    if (_json.length() > 0) {
      char _topic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(_topic, "%s/state",
              Object->Core->MqttAPI->Mqtt->configuration->status.topic);
      char _message[_json.length() + 50];
      _json.toCharArray(_message, _json.length());
      Object->Core->MqttAPI->Mqtt->publish(_topic, _message);
    }
  }
}
