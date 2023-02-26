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

void AFEEvent::conenctedToNetwork(void) {}

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
