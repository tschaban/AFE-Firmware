#include "AFE-Event.h"

AFEEvent::AFEEvent() {}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEEvent::begin(AFEFirmware *_Firmware, AFEAPIMQTTDomoticz *_MqttAPI,
                     AFEAPIHTTPDomoticz *_HttpDomoticzAPI) {
  MqttAPI = _MqttAPI;
  HttpDomoticzAPI = _HttpDomoticzAPI;
  begin(_Firmware);
}
#else
void AFEEvent::begin(AFEFirmware *_Firmware, AFEAPIMQTTStandard *_MqttAPI) {
  MqttAPI = _MqttAPI;
  begin(_Firmware);
}
#endif

void AFEEvent::begin(AFEFirmware *_Firmware) { Firmware = _Firmware; }

void AFEEvent::listener(void) {
  /* Events triggered every 1m  */
  if (millis() - Firmware->timer->miliseconds >= AFE_TIMER_ONE_MINUTE) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 1m");
#endif
    Firmware->timer->miliseconds = millis();
    Firmware->timer->minutes++;
  }

  /* Events triggered every 1h  */
  if (Firmware->timer->minutes > AFE_TIMER_ONE_HOUR) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 1hr");
#endif
    Firmware->timer->minutes = 0;
    Firmware->timer->hours++;
  }

  /* Events triggered every 24hrs  */
  if (Firmware->timer->hours > AFE_TIMER_ONE_DAY) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 24hr");
#endif

    if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
      /* Check if firmware up2date */
      if (Firmware->API->Network->connected()) {
        Firmware->checkFirmwareVersion();
      }
      /* Publish Firmware version to MQTT Broker */
      publishFirmwareVersion();
      /* Checking if AFE Key is valid */
      Firmware->validateProVersion();
    }

    Firmware->timer->hours = 0;
    Firmware->timer->days++;
  }

  /* Events triggered every 1 month  */
  if (Firmware->timer->days > AFE_TIMER_ONE_MONTH) {
#ifdef DEBUG
    Serial << endl << F("INFO: EVENT: Trigger: 30d");
#endif

    Firmware->timer->days = 0;
    Firmware->timer->months++;
  }

  /* Instant triggered events */

  if (Firmware->API->Network->eventConnected()) {
    conenctedToNetwork();
    /* Checking if AFE Key is valid */
    Firmware->validateProVersion();
  }

  if (Firmware->API->Network->eventDisconnected()) {
    disconnectedFromNetwork();
  }

  if (Firmware->API->Network->connected()) {
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
  if (Firmware->Device->getMode() == AFE_MODE_NORMAL) {
    if (Firmware->Device->configuration.api.domoticz) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Domoticz HTTP API boot actions triggering");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Firmware->Hardware->SystemLed->on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current gate state to Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfGates; i++) {
        _HttpDomoticzAPI->publishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of contactrons to "
                  "Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfContactrons;
           i++) {
        _HttpDomoticzAPI->publishContactronState(i);
        lastPublishedContactronState[i] = Contactron[i]->get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
      Serial
          << endl
          << F("INFO: EVENTS: Sending current state of switches to Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfSwitches;
           i++) {
        _HttpDomoticzAPI->publishSwitchState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of binary sensors to "
                  "Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfBinarySensors;
           i++) {
        _HttpDomoticzAPI->publishBinarySensorState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of RGB LEDs "
                  "Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfCLEDs; i++) {
        _HttpDomoticzAPI->publishCLEDState(i);
        _HttpDomoticzAPI->publishCLEDEffectState(i);
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of regulator to "
                  "Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfRegulators;
           i++) {
        _HttpDomoticzAPI->publishRegulatorState(i);
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of regulator to "
                  "Domoticz");
#endif
      for (uint8_t i = 0;
           i < Firmware->Device->configuration.noOfThermalProtectors; i++) {
        _HttpDomoticzAPI->publishThermalProtectorState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
      Serial << endl
             << F("INFO: EVENTS: Sending current state of relays to Domoticz");
#endif
      for (uint8_t i = 0; i < Firmware->Device->configuration.noOfRelays; i++) {
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (Relay[i]->gateId == AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif
          _HttpDomoticzAPI->publishRelayState(i);
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* Closing the condition for skipping relay if assigned to a gate */
        }
#ifdef DEBUG
        else {
          Serial << endl
                 << F("INFO: EVENTS: Excluding relay: ") << i
                 << F(" as it's assigned to a Gate: ") << Relay[i]->gateId;
        }
#endif
#endif
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Firmware->Hardware->SystemLed->off();
#endif
    }

#ifdef DEBUG
    Serial << endl << F("INFO: EVENTS: Post WiFi Connection actions completed");
#endif
  }
#endif
  // Checing access to WAN
  Firmware->API->REST->checkAccessToWAN();
}

void AFEEvent::disconnectedFromNetwork(void) {
#ifdef DEBUG
  Serial << endl << F("INFO: EVENT: Network disconnected: triggered");
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->Hardware->SystemLed->on();
#endif
  Firmware->API->REST->setNoWANAccess();
#ifdef AFE_CONFIG_HARDWARE_LED
  Firmware->Hardware->SystemLed->off();
#endif
}

void AFEEvent::connectedToMQTTBroker(void) {
  /* Event: connected to MQTT API */
  if (Firmware->Device->getMode() == AFE_MODE_NORMAL &&
      Firmware->Device->configuration.api.mqtt) {

    if (MqttAPI->Mqtt->connectedEvent()) {
#ifdef DEBUG
      Serial << endl << F("INFO: EVENT: MQTT Connected: triggered");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      Firmware->Hardware->SystemLed->on();
#endif
      MqttAPI->subscribe();

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
      publishMQTTHADiscoveryConfiguration();
#endif

      MqttAPI->synchronize();

    /* Publish Firmware version to MQTT Broker */
    publishFirmwareVersion();
  }

#ifdef AFE_CONFIG_HARDWARE_LED
      Firmware->Hardware->SystemLed->off();
#endif
    }
}

void AFEEvent::disconnectedFromMQTTBroker(void) {
  if (MqttAPI->Mqtt->disconnectedEvent()) {
#ifdef DEBUG
    Serial << endl
           << F("INFO: EVENT: MQTT Disconnected: triggered") << endl
           << F("Forcing disconnection with MQTT Broker in the object only");
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
    Firmware->Hardware->SystemLed->on();
#endif
    MqttAPI->Mqtt->_Broker->disconnect(true);

#ifdef AFE_CONFIG_HARDWARE_LED
    Firmware->Hardware->SystemLed->off();
#endif
  }
}

void AFEEvent::publishFirmwareVersion(void) {
  if (strlen(MqttAPI->Mqtt->configuration->status.topic) > 0) {
    String _json;
    Firmware->API->REST->sent(
        _json, AFE_CONFIG_JSONRPC_REST_METHOD_GET_JSON_LATEST_FIRMWARE_VERSION);
    if (_json.length() > 0) {
      char _topic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(_topic, "%s/state", MqttAPI->Mqtt->configuration->status.topic);
      char _message[_json.length() + 50];
      _json.toCharArray(_message, _json.length());
      MqttAPI->Mqtt->publish(_topic, _message);
    }
  }
}

/**
 * @brief Publishing device configuration to Home Assistant
 *
 */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
void AFEEvent::publishMQTTHADiscoveryConfiguration(void) {

  AFEAPIHomeAssistantIntegration *HomeAssistantDiscoveryAPI =
      new AFEAPIHomeAssistantIntegration(Firmware, MqttAPI);
  HomeAssistantDiscoveryAPI->publish();
  delete HomeAssistantDiscoveryAPI;
  HomeAssistantDiscoveryAPI = NULL;
}
#endif
