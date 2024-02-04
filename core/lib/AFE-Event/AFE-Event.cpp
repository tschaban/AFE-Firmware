#include "AFE-Event.h"

AFEEvent::AFEEvent() {}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
void AFEEvent::begin(AFEFirmware *Firmware, AFEHardware *Hardware,
                     AFEAPIMQTTDomoticz *MqttAPI,
                     AFEAPIHTTPDomoticz *HttpDomoticzAPI)
{
  _MqttAPI = MqttAPI;
  _HttpDomoticzAPI = HttpDomoticzAPI;
  begin(Firmware, Hardware);
}
#else
void AFEEvent::begin(AFEFirmware *Firmware, AFEHardware *Hardware,
                     AFEAPIMQTTStandard *MqttAPI)
{
  _MqttAPI = MqttAPI;
  begin(Firmware, Hardware);
}
#endif

void AFEEvent::begin(AFEFirmware *Firmware, AFEHardware *Hardware)
{
  _Firmware = Firmware;
  _Hardware = Hardware;
}

void AFEEvent::listener(void)
{
  /* Events triggered every 1m  */
  if (millis() - _Firmware->timer->miliseconds >= AFE_TIMER_ONE_MINUTE)
  {
#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Trigger: 1m"), F("EVENT"));
#endif
    _Firmware->timer->miliseconds = millis();
    _Firmware->timer->minutes++;

/**
 * @brief Debug information
 *
 */
#ifdef DEBUG
    _Firmware->Debugger->getFirmwareAllDebugInformation();
#endif // DEBUG
  }

  /* Events triggered every 1h  */
  if (_Firmware->timer->minutes > AFE_TIMER_ONE_HOUR)
  {
#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Trigger: 1hr"), F("EVENT"));
#endif
    _Firmware->timer->minutes = 0;
    _Firmware->timer->hours++;

    if (_Firmware->Device->getMode() == AFE_MODE_NORMAL)
    {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Checking access to WAN"), F("EVENT"));
#endif
      if (!_Firmware->API->REST->accessToWAN())
      {
        _Firmware->API->REST->checkAccessToWAN();
#ifdef DEBUG
      }
      else
      {
        Serial << F(" ... already connected");
#endif
      }
    }
  }

  /* Events triggered every 24hrs  */
  if (_Firmware->timer->hours > AFE_TIMER_ONE_DAY)
  {
#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Trigger: 24hr"), F("EVENT"));
#endif

    if (_Firmware->Device->getMode() == AFE_MODE_NORMAL)
    {
      /* Check if firmware up2date */
      if (_Firmware->API->Network->connected())
      {
        _Firmware->checkFirmwareVersion();
      }
      /* Publish _Firmware version to MQTT Broker */
      publishFirmwareVersion();
      /* Checking if AFE Key is valid */
      _Firmware->validateProVersion();
    }

    _Firmware->timer->hours = 0;
    _Firmware->timer->days++;
  }

  /* Events triggered every 1 month  */
  if (_Firmware->timer->days > AFE_TIMER_ONE_MONTH)
  {
#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Trigger: 30d"), F("EVENT"));
#endif

    _Firmware->timer->days = 0;
    _Firmware->timer->months++;
  }

  /* Instant triggered events */

  if (_Firmware->API->Network->eventConnected())
  {
    conenctedToNetwork();
  }

  /* Checking if got disconnected from Network */
  if (_Firmware->API->Network->eventDisconnected())
  {
    disconnectedFromNetwork();
  }

  if (_Firmware->API->Network->connected())
  {
    connectedToMQTTBroker();
  }

  disconnectedFromMQTTBroker();
}

/**
 * @brief Events
 *
 */

void AFEEvent::conenctedToNetwork(void)
{

// Update HTTP states for Domoticz HTTP API
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL)
  {
    if (_Firmware->Device->configuration.api.domoticz)
    {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Domoticz HTTP API boot actions triggering"), F("EVENT"));
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      _Hardware->SystemLed->on();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current gate state to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++)
      {
        _HttpDomoticzAPI->publishGateState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of contactrons to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
           i++)
      {
        _HttpDomoticzAPI->publishContactronState(i);
        _Hardware->lastPublishedContactronState[i] =
            _Hardware->Contactron[i]->get();
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of switches to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches;
           i++)
      {
        _HttpDomoticzAPI->publishSwitchState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of binary sensors to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0;
           i < _Firmware->Device->configuration.noOfBinarySensors; i++)
      {
        _HttpDomoticzAPI->publishBinarySensorState(i);
      }
#endif

      /* Not implemented for Dmoticz HTTP
      #ifdef AFE_CONFIG_HARDWARE_CLED

      #ifdef DEBUG
            Serial << endl
                   << F("INFO: EVENTS: Sending current state of RGB LEDs "
                        "Domoticz");
      #endif
            for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++) {
              _HttpDomoticzAPI->publishCLEDState(i);
              _HttpDomoticzAPI->publishCLEDEffectState(i);
            }
      #endif
      */

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of regulator to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
           i++)
      {
        _HttpDomoticzAPI->publishRegulatorState(i);
      }
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of regulator to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0;
           i < _Firmware->Device->configuration.noOfThermalProtectors; i++)
      {
        _HttpDomoticzAPI->publishThermalProtectorState(i);
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("Sending current state of relays to Domoticz"), F("EVENT"));
#endif
      for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays;
           i++)
      {
#ifdef AFE_CONFIG_HARDWARE_GATE
        /* For the Relay assigned to a gate code below is not needed for
         * execution
         */
        if (_Hardware->Relay[i]->gateId == AFE_HARDWARE_ITEM_NOT_EXIST)
        {
#endif
          _HttpDomoticzAPI->publishRelayState(i);
#ifdef AFE_CONFIG_HARDWARE_GATE
          /* Closing the condition for skipping relay if assigned to a gate */
        }
#ifdef DEBUG
        else
        {
          _Firmware->Debugger->printInformation(F("Excluding relay: "), F("EVENT"));
          Serial << F(" as it's assigned to a Gate: ")
                 << _Hardware->Relay[i]->gateId;
        }
#endif
#endif
      }
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      _Hardware->SystemLed->off();
#endif
    }

#ifdef DEBUG
    _Firmware->Debugger->printInformation(F("Post WiFi Connection actions completed"), F("EVENT"));
#endif
  }
#endif
  // Checing access to WAN
  _Firmware->API->REST->checkAccessToWAN();
}

void AFEEvent::disconnectedFromNetwork(void)
{
#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Network disconnected: triggered"), F("EVENT"));
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
  _Hardware->SystemLed->on();
#endif
  _Firmware->API->REST->setNoWANAccess();

  /* Forcing disconnection from MQTT Brokere */
  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL &&
      _Firmware->Device->configuration.api.mqtt)
  {

    AFEAsyncMQTTClient::eventDisconnected = true;
  }

#ifdef AFE_CONFIG_HARDWARE_LED
  _Hardware->SystemLed->off();
#endif
}

void AFEEvent::connectedToMQTTBroker(void)
{
  /* Event: connected to MQTT API */
  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL &&
      _Firmware->Device->configuration.api.mqtt)
  {

    if (_MqttAPI->Mqtt->connectedEvent())
    {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("MQTT Connected: triggered"), F("EVENT"));
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
      _Hardware->SystemLed->on();
#endif
      _MqttAPI->subscribe();

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
      publishMQTTHADiscoveryConfiguration();
#endif

      _MqttAPI->synchronize();

      /* Publish _Firmware version to MQTT Broker */
      publishFirmwareVersion();
    }

#ifdef AFE_CONFIG_HARDWARE_LED
    _Hardware->SystemLed->off();
#endif
  }
}

void AFEEvent::disconnectedFromMQTTBroker(void)
{
  if (_Firmware->Device->getMode() == AFE_MODE_NORMAL &&
      _Firmware->Device->configuration.api.mqtt)
  {

    if (_MqttAPI->Mqtt->disconnectedEvent())
    {
#ifdef DEBUG
      _Firmware->Debugger->printInformation(F("MQTT Disconnected: triggered"), F("EVENT"));

#endif
    }
  }
}

void AFEEvent::publishFirmwareVersion(void)
{
  if (strlen(_MqttAPI->Mqtt->configuration->status.topic) > 0)
  {
    String _json;
    _Firmware->API->REST->sent(
        _json, AFE_CONFIG_JSONRPC_REST_METHOD_GET_JSON_LATEST_FIRMWARE_VERSION);
    if (_json.length() > 0)
    {
      char _topic[AFE_CONFIG_MQTT_TOPIC_STATE_LENGTH];
      sprintf(_topic, "%s/state", _MqttAPI->Mqtt->configuration->status.topic);
      char _message[_json.length() + 50];
      _json.toCharArray(_message, _json.length());
      _MqttAPI->Mqtt->publish(_topic, _message);
    }
  }
}

/**
 * @brief Publishing device configuration to Home Assistant
 *
 */
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
void AFEEvent::publishMQTTHADiscoveryConfiguration(void)
{

  AFEAPIHomeAssistantIntegration *HomeAssistantDiscoveryAPI =
      new AFEAPIHomeAssistantIntegration(_Firmware, _MqttAPI);
  HomeAssistantDiscoveryAPI->publish();
  delete HomeAssistantDiscoveryAPI;
  HomeAssistantDiscoveryAPI = NULL;
}
#endif
