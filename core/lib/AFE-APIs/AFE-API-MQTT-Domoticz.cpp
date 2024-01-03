/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-MQTT-Domoticz.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

AFEAPIMQTTDomoticz::AFEAPIMQTTDomoticz(){};

#ifdef DEBUG
void AFEAPIMQTTDomoticz::printSameStateWarningMessage()
{
  _Firmware->Debugger->printInformation(F("Same state. No change needed"), F("Domoticz"));
}

void AFEAPIMQTTDomoticz::printFoundDeviceItem(const __FlashStringHelper *deviceName, uint8_t id)
{
  _Firmware->Debugger->printInformation(F("Found "), F("Domoticz"));
  Serial << deviceName << F(", Id: ") << id;
}

void AFEAPIMQTTDomoticz::printSynchronzingDeviceItem(const __FlashStringHelper *deviceName, uint8_t id)
{
  _Firmware->Debugger->printInformation(F("Synchronizing: "), F("Domoticz"));
  Serial << deviceName << F(", Id: ") << id;
}

void AFEAPIMQTTDomoticz::printCacheingDeviceItem(const __FlashStringHelper *deviceName, uint8_t id, uint32_t idx)
{
  _Firmware->Debugger->printInformation(F("Caching IDX for "), F("Domoticz"));
  Serial << deviceName << F("[") << id << F("], IDX: ") << idx;
}
#endif

void AFEAPIMQTTDomoticz::begin(AFEFirmware *Firmware, AFEHardware *Hardware)
{
  _Firmware = Firmware;
  _Hardware = Hardware;
#ifdef DEBUG
  Mqtt->addReference(_Firmware->Debugger);
#endif
  if (_Firmware->Device->configuration.api.mqtt)
  {
#ifdef AFE_CONFIG_HARDWARE_LED
    enabled = Mqtt->begin(_Firmware->API->Flash, _Firmware->Device,
                          _Firmware->Hardware->SystemLed);
#else
    enabled = Mqtt->begin(_Firmware->API->Flash, _Firmware->Device);
#endif
  }

#ifdef AFE_CONFIG_HARDWARE_RELAY

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++)
  {
#ifdef DEBUG
    printCacheingDeviceItem(F("Relay"), i, _Hardware->Relay[i]->configuration->domoticz.idx);
#endif

    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_RELAY, _Hardware->Relay[i]->configuration->domoticz.idx);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_REGULATOR

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
       i++)
  {
#ifdef DEBUG
    printCacheingDeviceItem(F("Regulator"), i, _Regulator[i]->configuration->domoticz.idx);
#endif
    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_REGULATOR,
                  _Regulator[i]->configuration->domoticz.idx);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++)
  {
#ifdef DEBUG
    printCacheingDeviceItem(F("Gate"), i, _Hardware->Gate[i]->configuration->domoticz.idx);
#endif

    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_GATE,
                  _Hardware->Gate[i]->configuration->domoticz.idx);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_THERMAL_PROTECTOR

  for (uint8_t i = 0;
       i < _Firmware->Device->configuration.noOfThermalProtectors; i++)
  {
#ifdef DEBUG
    printCacheingDeviceItem(F("Thermal Protector"), i, _Hardware->ThermalProtector[i]->configuration->domoticz.idx);
    ;
#endif
    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_THERMAL_PROTECTOR,
                  _Hardware->ThermalProtector[i]->configuration->domoticz.idx);
  }
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED

  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++)
  {
#ifdef DEBUG
    printCacheingDeviceItem(F("RGB LED"), i, _Hardware->RGBLEDStrip->configuration[i].cled.idx);
#endif
    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_CLED,
                  _Hardware->RGBLEDStrip->configuration[i].cled.idx);
#ifdef DEBUG
    printCacheingDeviceItem(F("RGB LED Effect"), i, _Hardware->RGBLEDStrip->configuration[i].effect.idx);
#endif
    addIdxToCache(i, AFE_DOMOTICZ_DEVICE_CLED_EFFECT,
                  _Hardware->RGBLEDStrip->configuration[i].effect.idx);
  }
#endif
}

void AFEAPIMQTTDomoticz::listener()
{
  if (Mqtt->listener())
  {
    processRequest();
  }
}

void AFEAPIMQTTDomoticz::synchronize()
{
#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Sending current device state to MQTT Broker"), F("Domoticz"));
#endif

/* Synchronize: Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRelays; i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Relay"), i);
#endif
    publishRelayState(i);
  }
#endif

/* Synchronize: Switch */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfSwitches; i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Switch"), i);
#endif
    publishSwitchState(i);
  }
#endif

/* Synchronize: Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfContactrons;
       i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Contactron"), i);
#endif
    publishContactronState(i);
  }
#endif

/* Synchronize: Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfGates; i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Gate"), i);
#endif
    publishGateState(i);
  }
#endif

/* Synchronize: Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfRegulators;
       i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Regulator"), i);
#endif
    publishRegulatorState(i);
  }
#endif

/* Synchronize: Regulator */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  for (uint8_t i = 0;
       i < _Firmware->Device->configuration.noOfThermalProtectors; i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Thermal Protector"), i);
#endif
    publishThermalProtectorState(i);
  }
#endif

/* Synchronize: Binary sensor */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfBinarySensors;
       i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("Binary sensor"), i);
#endif
    publishBinarySensorState(i);
  }
#endif

/* Synchronize: RGB LED */
#ifdef AFE_CONFIG_HARDWARE_CLED
  for (uint8_t i = 0; i < _Firmware->Device->configuration.noOfCLEDs; i++)
  {
#ifdef DEBUG
    printSynchronzingDeviceItem(F("RGB LED Color"), i);
#endif

    publishSetColorMessage(&_Hardware->RGBLEDStrip->configuration[i].cled.idx,
                           &_Hardware->RGBLEDStrip->currentState[i].on);
#ifdef DEBUG
    printSynchronzingDeviceItem(F("RGB LED State"), i);
#endif
    publishCLEDState(i);
#ifdef DEBUG
    printSynchronzingDeviceItem(F("RGB LED Effect"), i);
#endif
    publishCLEDEffectsState(i);
  }
#endif
}

void AFEAPIMQTTDomoticz::subscribe()
{
#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Subsribing to Domoticz MQTT Topic"), F("MQTT"));
#endif
  Mqtt->subscribe(AFE_CONFIG_API_DOMOTICZ_TOPIC_OUT);
}

DOMOTICZ_MQTT_COMMAND AFEAPIMQTTDomoticz::getCommand()
{
  DOMOTICZ_MQTT_COMMAND command;

  command.domoticz.idx = Mqtt->message.command.domoticz.idx;
  command.nvalue = Mqtt->message.command.nvalue;
  sprintf(command.svalue, Mqtt->message.command.svalue);

#ifdef AFE_CONFIG_HARDWARE_CLED
  command.led.brightness = Mqtt->message.command.led.brightness;
  command.led.color.blue = Mqtt->message.command.led.color.blue;
  command.led.color.red = Mqtt->message.command.led.color.red;
  command.led.color.green = Mqtt->message.command.led.color.green;
#endif

  return command;
}

void AFEAPIMQTTDomoticz::processRequest()
{
  DOMOTICZ_MQTT_COMMAND command = getCommand();
  if (command.domoticz.idx > 0)
  {
#ifdef DEBUG
    uint8_t _found = false;
#endif
    for (uint8_t i = 0; i < lastIDXChacheIndex; i++)
    {
      if (idxCache[i].domoticz.idx == command.domoticz.idx)
      {
        switch (idxCache[i].type)
        {
/* Processing Relay command*/
#ifdef AFE_CONFIG_HARDWARE_RELAY
        case AFE_DOMOTICZ_DEVICE_RELAY:
#ifdef DEBUG
          printFoundDeviceItem(F("Relay"), idxCache[i].id);

          _found = true;
#endif
          if (_Hardware->Relay[idxCache[i].id]->get() != (byte)command.nvalue)
          {
            if (command.nvalue == AFE_SWITCH_OFF)
            {
              _Hardware->Relay[idxCache[i].id]->off();
            }
            else
            {
              _Hardware->Relay[idxCache[i].id]->on();
            }
            publishRelayState(idxCache[i].id);
          }
#ifdef DEBUG
          else
          {
            printSameStateWarningMessage();
          }
#endif
          break;
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_GATE
        /* Processing Gate command*/
        case AFE_DOMOTICZ_DEVICE_GATE:
#ifdef DEBUG
          printFoundDeviceItem(F("Gate"), idxCache[i].id);

          _found = true;
#endif

          if (command.nvalue == AFE_SWITCH_ON)
          {
            _Hardware->Gate[idxCache[i].id]->toggle();
          }
#ifdef DEBUG
          else
          {
            _Firmware->Debugger->printInformation(F("OFF Command, skipping"), F("Domoticz"));
          }
#endif
          break;
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
        case AFE_DOMOTICZ_DEVICE_REGULATOR:
#ifdef DEBUG
          printFoundDeviceItem(F("Regulator"), idxCache[i].id);

          _found = true;
#endif
          if (_Regulator[idxCache[i].id]->configuration->enabled !=
              (byte)command.nvalue)
          {
            if (command.nvalue == AFE_SWITCH_OFF)
            {
              _Regulator[idxCache[i].id]->off();
            }
            else
            {
              _Regulator[idxCache[i].id]->on();
            }
            publishRegulatorState(idxCache[i].id);
          }
#ifdef DEBUG
          else
          {
            printSameStateWarningMessage();
          }
#endif
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
        case AFE_DOMOTICZ_DEVICE_THERMAL_PROTECTOR:
#ifdef DEBUG
          printFoundDeviceItem(F("Thermal Protector"), idxCache[i].id);

          _found = true;
#endif
          if (_Hardware->ThermalProtector[idxCache[i].id]
                  ->configuration->enabled != (byte)command.nvalue)
          {
            if (command.nvalue == AFE_SWITCH_OFF)
            {
              _Hardware->ThermalProtector[idxCache[i].id]->off();
            }
            else
            {
              _Hardware->ThermalProtector[idxCache[i].id]->on();
            }
            publishThermalProtectorState(idxCache[i].id);
          }
#ifdef DEBUG
          else
          {
            printSameStateWarningMessage();
          }
#endif
          break;
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_CLED
        case AFE_DOMOTICZ_DEVICE_CLED:
#ifdef DEBUG
          printFoundDeviceItem(F("RBG LED"), idxCache[i].id);

          _found = true;
#endif

          if (_Hardware->RGBLEDStrip->currentState[idxCache[i].id].state !=
              (byte)command.nvalue)
          {

            boolean _success = true;
            /**
             * @brief Processing domoticz command
             *
             */
            switch (command.nvalue)
            {
            /* Turn OFF */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_OFF:
              _Hardware->RGBLEDStrip->off(idxCache[i].id, true);
              break;
            /* Turn ON */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_ON:
              _Hardware->RGBLEDStrip->on(idxCache[i].id, true);
              break;
            /* Turn ON: full brightness */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_FULL_LIGHT:
              command.led.brightness =
                  AFE_CONFIG_HARDWARE_CLED_LIGHT_LEVEL_FULL_LIGHT;
              command.led.color =
                  _Hardware->RGBLEDStrip->currentState[idxCache[i].id].on.color;
              _Hardware->RGBLEDStrip->on(idxCache[i].id, command.led, true, true);
              break;
            /* Turn ON: night brightness */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_NIGHT_LIGHT:
              command.led.brightness =
                  AFE_CONFIG_HARDWARE_CLED_LIGHT_LEVEL_NIGHT_LIGHT;
              command.led.color =
                  _Hardware->RGBLEDStrip->currentState[idxCache[i].id].on.color;
              _Hardware->RGBLEDStrip->on(idxCache[i].id, command.led, true, true);
              break;
            /* Turn ON or OFF depending on new brightness level */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_BRIGHTNESS:
              if (command.led.brightness > 0)
              {
                command.led.brightness *=
                    AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS / 100;
                command.led.color =
                    _Hardware->RGBLEDStrip->currentState[idxCache[i].id].on.color;
                _Hardware->RGBLEDStrip->on(idxCache[i].id, command.led, true, true);
              }
              else
              {
                _Hardware->RGBLEDStrip->off(idxCache[i].id, false);
              }
              break;
            /* Turn ON or OFF depending on brightness level and changing the
             * color */
            case AFE_CONFIG_HARDWARE_CLED_DOMOTICZ_NVALUE_COLOR:
              if (command.led.brightness > 0)
              {
                command.led.brightness *=
                    AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS / 100;
                _Hardware->RGBLEDStrip->on(idxCache[i].id, command.led, true, true);
              }
              else
              {
                _Hardware->RGBLEDStrip->off(idxCache[i].id, false);
              }
              break;
            /* Command not recognized */
            default:
              _success = false;
#ifdef DEBUG
              _Firmware->Debugger->printWarning(F("Not recognized command"), F("RGB LED"));
#endif
              break;
            }

            /**
             * @brief Updating states (only if authorized command recieved)
             *
             */
            if (_success)
            {
              if (_Hardware->RGBLEDStrip->isStateUpdated(idxCache[i].id))
              {
                publishCLEDColorState(idxCache[i].id);
              }
              if (_Hardware->RGBLEDStrip->isEffectStateUpdated(idxCache[i].id))
              {
                publishCLEDEffectsState(idxCache[i].id);
              }
            }
          }
#ifdef DEBUG
          else
          {
            printSameStateWarningMessage();
          }
#endif
          break;
        case AFE_DOMOTICZ_DEVICE_CLED_EFFECT:

#ifdef DEBUG
          printFoundDeviceItem(F("RGB LED Effect"), idxCache[i].id);
          _found = true;
#endif
          _Hardware->RGBLEDStrip->activateEffect(idxCache[i].id, atoi(command.svalue));
          publishCLEDEffectsState(idxCache[i].id);
          if (atoi(command.svalue) != AFE_CONFIG_HARDWARE_CLED_EFFECT_NONE)
          {
            publishCLEDState(idxCache[i].id);
          }
          break;

#endif // AFE_CONFIG_HARDWARE_CLED

        /* Processing Unknown command*/
        default:
#ifdef DEBUG
          _Firmware->Debugger->printError(F("Device type not handled. Type: "), F("Domoticz"));
          Serial
              << idxCache[i].type;
#endif
          break;
        }
      }
    }
#ifdef DEBUG
    if (!_found)
    {
      _Firmware->Debugger->printWarning(F("No item found with IDX: "), F("Domoticz"));
      Serial
          << command.domoticz.idx;
    }
#endif
  }
#ifdef DEBUG
  else
  {
    _Firmware->Debugger->printInformation((command.domoticz.idx > 0 ? F("Bypassing")
                                                                      : F("No")),
                                          F("Domoticz"));
    Serial << F(" IDX: ") << command.domoticz.idx;
  }
#endif

#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Request processing finished"), F("Domoticz"));

#endif
}

void AFEAPIMQTTDomoticz::addIdxToCache(uint8_t id,
                                       afe_domoticz_device_type_t type,
                                       uint32_t idx)
{
  if (idx > 0)
  {
    idxCache[lastIDXChacheIndex].domoticz.idx = idx;
    idxCache[lastIDXChacheIndex].id = id;
    idxCache[lastIDXChacheIndex].type = type;
#ifdef DEBUG
    _Firmware->Debugger->printBulletPoint(F("added IDX["));
    Serial << lastIDXChacheIndex << F("][")
           << idxCache[lastIDXChacheIndex].type << F("][")
           << idxCache[lastIDXChacheIndex].id << F(")]: ") << idx;
#endif
    lastIDXChacheIndex++;

#ifdef DEBUG
  }
  else
  {
    _Firmware->Debugger->printBulletPoint(F("IDX not set"));

#endif
  }
}

boolean AFEAPIMQTTDomoticz::publishSwitchMessage(uint32_t *idx, boolean state)
{
  boolean publishStatus = false;
  if (enabled)
  {

    if (*idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];
      generateSwitchMessage(json, *idx, state ? AFE_ON : AFE_OFF);
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      bypassProcessing.idx = *idx;
    }
  }
  return publishStatus;
}

#ifdef AFE_CONFIG_HARDWARE_CLED
boolean AFEAPIMQTTDomoticz::publishSetLevelMessage(uint32_t *idx,
                                                   uint8_t *level)
{
  boolean publishStatus = false;
  if (enabled)
  {
    if (*idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_SET_LEVEL_COMMAND_LENGTH];
      sprintf(json,
              "{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"Set "
              "Level\",\"level\":%d}",
              *idx, *level);
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      bypassProcessing.idx = *idx;
    }
  }
  return publishStatus;
}

boolean AFEAPIMQTTDomoticz::publishSetColorMessage(uint32_t *idx,
                                                   CLED_PARAMETERS *led)
{
  boolean publishStatus = false;
  if (enabled)
  {
    if (*idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_SET_COLOR_COMMAND_LENGTH];
      sprintf(
          json, "{\"command\":\"setcolbrightnessvalue\",\"idx\":%d,\"color\":"
                "{\"m\":3,\"t\":0,\"r\":%d,\"g\":%d,\"b\":%d,\"cw\":0,\"ww\":"
                "0},\"brightness\":%d}",
          *idx, led->color.red, led->color.green, led->color.blue,
          led->brightness / (AFE_CONFIG_HARDWARE_CLED_MAX_BRIGHTNESS / 100));
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      bypassProcessing.idx = *idx;
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIMQTTDomoticz::publishRelayState(uint8_t id)
{
  return enabled ? publishSwitchMessage(
                       &_Hardware->Relay[id]->configuration->domoticz.idx,
                       _Hardware->Relay[id]->get())
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIMQTTDomoticz::publishSwitchState(uint8_t id)
{
  boolean publishStatus = false;
  if (enabled)
  {
    if (_Hardware->Switch[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_SWITCH_COMMAND_LENGTH];

      generateSwitchMessage(
          json, _Hardware->Switch[id]->configuration->domoticz.idx,
          _Hardware->Switch[id]->getPhisicalState() == 1 ? AFE_SWITCH_OFF
                                                         : AFE_SWITCH_ON);
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
void AFEAPIMQTTDomoticz::publishADCValues(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[20];
    if (_Hardware->AnalogInput[id]->configuration->domoticz.percent > 0)
    {
      sprintf(value, "%-.2f", _Hardware->AnalogInput[id]->data->percent);
      generateDeviceValue(
          json, _Hardware->AnalogInput[id]->configuration->domoticz.percent,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.voltage > 0)
    {
      sprintf(value, "%-.4f", _Hardware->AnalogInput[id]->data->voltage);
      generateDeviceValue(
          json, _Hardware->AnalogInput[id]->configuration->domoticz.voltage,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.voltageCalculated >
        0)
    {
      sprintf(value, "%-.4f",
              _Hardware->AnalogInput[id]->data->voltageCalculated);
      generateDeviceValue(
          json,
          _Hardware->AnalogInput[id]->configuration->domoticz.voltageCalculated,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.raw > 0)
    {
      sprintf(value, "%-d", _Hardware->AnalogInput[id]->data->raw);
      generateDeviceValue(
          json, _Hardware->AnalogInput[id]->configuration->domoticz.raw, value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#else  // ESP8266
void AFEAPIMQTTDomoticz::publishADCValues()
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[20];
    if (_Hardware->AnalogInput->configuration->domoticz.percent > 0)
    {
      sprintf(value, "%-.2f", _Hardware->AnalogInput->data->percent);
      generateDeviceValue(
          json, _Hardware->AnalogInput->configuration->domoticz.percent, value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.voltage > 0)
    {
      sprintf(value, "%-.4f", _Hardware->AnalogInput->data->voltage);
      generateDeviceValue(
          json, _Hardware->AnalogInput->configuration->domoticz.voltage, value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.voltageCalculated > 0)
    {
      sprintf(value, "%-.4f", _Hardware->AnalogInput->data->voltageCalculated);
      generateDeviceValue(
          json,
          _Hardware->AnalogInput->configuration->domoticz.voltageCalculated,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.raw > 0)
    {
      sprintf(value, "%-d", _Hardware->AnalogInput->data->raw);
      generateDeviceValue(
          json, _Hardware->AnalogInput->configuration->domoticz.raw, value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifdef AFE_ESP32
void AFEAPIMQTTDomoticz::publishBatteryMeterValues(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_BATTERYMETER_COMMAND_LENGTH];
    char value[8];
    if (_Hardware->AnalogInput[id]->configuration->battery.domoticz.idx > 0)
    {
      sprintf(value, "%-.3f", _Hardware->AnalogInput[id]->batteryPercentage);
      generateDeviceValue(
          json, _Hardware->AnalogInput[id]->configuration->battery.domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#else
void AFEAPIMQTTDomoticz::publishBatteryMeterValues()
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_BATTERYMETER_COMMAND_LENGTH];
    char value[8];
    if (_Hardware->AnalogInput->configuration->battery.domoticz.idx > 0)
    {
      sprintf(value, "%-.3f", _Hardware->AnalogInput->batteryPercentage);
      generateDeviceValue(
          json, _Hardware->AnalogInput->configuration->battery.domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

void AFEAPIMQTTDomoticz::generateSwitchMessage(char *json, uint32_t idx,
                                               boolean state)
{
  sprintf(json, "{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}",
          idx, state ? "On" : "Off");
}

void AFEAPIMQTTDomoticz::generateDeviceValue(char *json, uint32_t idx,
                                             const char *svalue,
                                             uint16_t nvalue)
{

  /**
   * @brief {"command":"udevice","idx":999999,"nvalue":,"svalue":""}
   *
   */

  sprintf(
      json,
      "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}",
      idx, nvalue, svalue);
}

#ifdef AFE_CONFIG_HARDWARE_BMEX80
boolean AFEAPIMQTTDomoticz::publishBoschBMSensorData(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[50];
    /* Temperatura */
    if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.temperature.idx >
        0)
    {
      sprintf(value, "%-.2f",
              _Hardware->BMEX80Sensor[id]->data->temperature.value);
      generateDeviceValue(
          json,
          _Hardware->BMEX80Sensor[id]->configuration->domoticz.temperature.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Pressure */
    if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.pressure.idx > 0)
    {
      sprintf(value, "%-.2f;0",
              _Hardware->BMEX80Sensor[id]->data->pressure.value);
      generateDeviceValue(
          json,
          _Hardware->BMEX80Sensor[id]->configuration->domoticz.pressure.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Relative pressure */
    if (_Hardware->BMEX80Sensor[id]
            ->configuration->domoticz.relativePressure.idx > 0)
    {
      sprintf(value, "%-.2f;0",
              _Hardware->BMEX80Sensor[id]->data->relativePressure.value);
      generateDeviceValue(json,
                          _Hardware->BMEX80Sensor[id]
                              ->configuration->domoticz.relativePressure.idx,
                          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_Hardware->BMEX80Sensor[id]->configuration->type != AFE_BMP180_SENSOR)
    {
      /* Temp and Hum */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.temperatureHumidity.idx > 0)
      {
        sprintf(value, "%-.2f;%-.2f;%-d",
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        generateDeviceValue(
            json, _Hardware->BMEX80Sensor[id]->configuration->domoticz.temperatureHumidity.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Temp + Hum + Press */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.temperatureHumidityPressure.idx > 0)
      {
        sprintf(value, "%-.2f;%-.2f;%-d;%-.2f;0",
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value),
                _Hardware->BMEX80Sensor[id]->data->pressure.value);
        generateDeviceValue(
            json, _Hardware->BMEX80Sensor[id]->configuration->domoticz.temperatureHumidityPressure.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Humididty */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.humidity.idx >
          0)
      {
        sprintf(value, "%d",
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.humidity.idx,
            value, (uint8_t)_Hardware->BMEX80Sensor[id]->data->humidity.value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Absolute Humidity */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.absoluteHumidity.idx > 0)
      {
        sprintf(value, "%d",
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        generateDeviceValue(
            json, _Hardware->BMEX80Sensor[id]->configuration->domoticz.absoluteHumidity.idx,
            value,
            (uint8_t)_Hardware->BMEX80Sensor[id]->absoluteHumidity(
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                    AFE_TEMPERATURE_UNIT_FAHRENHEIT));
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* DewPoint */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.dewPoint.idx >
          0)
      {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->dewPoint.value);
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.dewPoint.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Heat Index */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx >
          0)
      {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->heatIndex.value);
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Perception */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.perception.idx >
          0)
      {
        char _perception[22]; // Max size of Perception from lang.pack
        byte _perceptionId = _Hardware->BMEX80Sensor[id]->perception(
            _Hardware->BMEX80Sensor[id]->data->temperature.value,
            _Hardware->BMEX80Sensor[id]->data->humidity.value,
            _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_perception,
                 (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.perception.idx,
            _perception, _Hardware->BMEX80Sensor[id]->convertPerceptionDomoticz(_perceptionId));
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }

      /* Comfort */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.comfort.idx >
          0)
      {
        char _comfort[80]; // Max size of Comfort from lang.pack
        ComfortState comfortStatus;
        _Hardware->BMEX80Sensor[id]->comfort(
            comfortStatus, _Hardware->BMEX80Sensor[id]->data->temperature.value,
            _Hardware->BMEX80Sensor[id]->data->humidity.value,
            _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.comfort.idx,
            _comfort,
            _Hardware->BMEX80Sensor[id]->convertComfortDomoticz(comfortStatus));
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
    }
    if (_Hardware->BMEX80Sensor[id]->configuration->type == AFE_BME680_SENSOR)
    {

      /* Gas Sensor */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.gasResistance.idx > 0)
      {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->gasResistance.value);
        generateDeviceValue(json,
                            _Hardware->BMEX80Sensor[id]
                                ->configuration->domoticz.gasResistance.idx,
                            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* IAQ */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.iaq.idx > 0)
      {
        sprintf(value, "%-.0f", _Hardware->BMEX80Sensor[id]->data->iaq.value);
        generateDeviceValue(
            json, _Hardware->BMEX80Sensor[id]->configuration->domoticz.iaq.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* Static IAQ */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.staticIaq.idx >
          0)
      {
        sprintf(value, "%-.0f",
                _Hardware->BMEX80Sensor[id]->data->staticIaq.value);
        generateDeviceValue(
            json,
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.staticIaq.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* CO2 */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.co2Equivalent.idx > 0)
      {
        sprintf(value, "%-.0f",
                _Hardware->BMEX80Sensor[id]->data->co2Equivalent.value);
        generateDeviceValue(json,
                            _Hardware->BMEX80Sensor[id]
                                ->configuration->domoticz.co2Equivalent.idx,
                            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
      /* BVOC */
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.breathVocEquivalent.idx > 0)
      {
        sprintf(value, "%-.1f",
                _Hardware->BMEX80Sensor[id]->data->breathVocEquivalent.value);
        generateDeviceValue(
            json, _Hardware->BMEX80Sensor[id]->configuration->domoticz.breathVocEquivalent.idx,
            value);
        Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
      }
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
boolean AFEAPIMQTTDomoticz::publishHPMA115S0SensorData(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[5];

    if (_Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm10.idx > 0)
    {

      sprintf(value, "%-.1f", _Hardware->HPMA115S0Sensor[id]->data->pm10);
      generateDeviceValue(
          json,
          _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm10.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->pm10);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm25.idx > 0)
    {
      sprintf(value, "%-.1f", _Hardware->HPMA115S0Sensor[id]->data->pm25);
      generateDeviceValue(
          json,
          _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm25.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->pm25);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->HPMA115S0Sensor[id]
            ->configuration->domoticz.whoPM10Norm.idx > 0)
    {

      sprintf(value, "%-.1f",
              _Hardware->HPMA115S0Sensor[id]->data->whoPM10Norm);
      generateDeviceValue(json, _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.whoPM10Norm.idx,
                          value,
                          _Hardware->HPMA115S0Sensor[id]->data->whoPM10Norm);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    if (_Hardware->HPMA115S0Sensor[id]
            ->configuration->domoticz.whoPM25Norm.idx > 0)
    {
      sprintf(value, "%-.1f",
              _Hardware->HPMA115S0Sensor[id]->data->whoPM25Norm);
      generateDeviceValue(json, _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.whoPM25Norm.idx,
                          value,
                          _Hardware->HPMA115S0Sensor[id]->data->whoPM25Norm);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
boolean AFEAPIMQTTDomoticz::publishBH1750SensorData(uint8_t id)
{
  if (enabled)
  {
    if (_Hardware->BH1750Sensor[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
      char value[6];
      sprintf(value, "%-.0f", _Hardware->BH1750Sensor[id]->data);
      generateDeviceValue(
          json, _Hardware->BH1750Sensor[id]->configuration->domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
boolean AFEAPIMQTTDomoticz::publishTSL2561SensorData(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
    char value[6];
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.ir.idx > 0)
    {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->ir);
      generateDeviceValue(
          json, _Hardware->TSL2561Sensor[id]->configuration->domoticz.ir.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.illuminance.idx >
        0)
    {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->illuminance);
      generateDeviceValue(
          json,
          _Hardware->TSL2561Sensor[id]->configuration->domoticz.illuminance.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.broadband.idx >
        0)
    {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->broadband);
      generateDeviceValue(
          json,
          _Hardware->TSL2561Sensor[id]->configuration->domoticz.broadband.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
boolean AFEAPIMQTTDomoticz::publishAS3935SensorData(uint8_t id)
{
  if (enabled)
  {
    if (_Hardware->AS3935Sensor[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_DEVICE_COMMAND_LENGTH];
      char value[4];
      sprintf(value, "%-d", _Hardware->AS3935Sensor[id]->distance);
      generateDeviceValue(
          json, _Hardware->AS3935Sensor[id]->configuration->domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIMQTTDomoticz::publishAnemometerSensorData()
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_ANEMOMETER_COMMAND_LENGTH];
    char value[20]; // 0;N;999999.99;0;?;?
    if (_Hardware->AnemometerSensor->configuration->domoticz.idx > 0)
    {
      sprintf(value, "0;N;%-.2f;0;?;?",
              10 * _Hardware->AnemometerSensor->lastSpeedMS);
      generateDeviceValue(
          json, _Hardware->AnemometerSensor->configuration->domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIMQTTDomoticz::publishRainSensorData()
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_RAINMETER_COMMAND_LENGTH];
    char value[20]; // 999999.00;999999.00
    if (_Hardware->RainmeterSensor->configuration->domoticz.idx > 0)
    {
      sprintf(value, "%-.2f;%-.2f",
              _Hardware->RainmeterSensor->rainLevelLastHour * 100,
              _Hardware->RainmeterSensor->current->counter);
      generateDeviceValue(
          json, _Hardware->RainmeterSensor->configuration->domoticz.idx, value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
boolean AFEAPIMQTTDomoticz::publishGateState(uint8_t id)
{
  return enabled
             ? publishSwitchMessage(
                   &_Hardware->Gate[id]->configuration->domoticz.idx,
                   _Hardware->Gate[id]->get() == AFE_GATE_CLOSED ? false : true)
             : false;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
boolean AFEAPIMQTTDomoticz::publishContactronState(uint8_t id)
{
  if (enabled)
  {
    if (_Hardware->Contactron[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_CONTACTRON_COMMAND_LENGTH];
      generateSwitchMessage(
          json, _Hardware->Contactron[id]->configuration->domoticz.idx,
          _Hardware->Contactron[id]->get() == AFE_CONTACTRON_OPEN ? true
                                                                  : false);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
boolean AFEAPIMQTTDomoticz::publishDS18B20SensorData(uint8_t id)
{
  if (enabled)
  {
    if (_Hardware->DS18B20Sensor[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_DS18B20_COMMAND_LENGTH];
      char value[9];
      sprintf(value, "%-.3f", _Hardware->DS18B20Sensor[id]->getTemperature());
      generateDeviceValue(
          json, _Hardware->DS18B20Sensor[id]->configuration->domoticz.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
boolean AFEAPIMQTTDomoticz::publishRegulatorState(uint8_t id)
{
  return enabled ? publishSwitchMessage(
                       &_Regulator[id]->configuration->domoticz.idx,
                       _Regulator[id]->configuration->enabled)
                 : false;
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
boolean AFEAPIMQTTDomoticz::publishThermalProtectorState(uint8_t id)
{
  return publishSwitchMessage(
      &_Hardware->ThermalProtector[id]->configuration->domoticz.idx,
      _Hardware->ThermalProtector[id]->configuration->enabled);
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
boolean AFEAPIMQTTDomoticz::publishDHTSensorData(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_DHT_COMMAND_LENGTH];
    char value[15];
    /* Temperature */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.temperature.idx > 0)
    {
      sprintf(value, "%-.1f", _Hardware->DHTSensor[id]->currentTemperature);
      generateDeviceValue(
          json,
          _Hardware->DHTSensor[id]->configuration->domoticz.temperature.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    /* Humidity */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.humidity.idx > 0)
    {
      sprintf(value, "%d",
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      generateDeviceValue(
          json, _Hardware->DHTSensor[id]->configuration->domoticz.humidity.idx,
          value, (uint8_t)_Hardware->DHTSensor[id]->currentHumidity);

      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Temperature and Humidity */
    if (_Hardware->DHTSensor[id]
            ->configuration->domoticz.temperatureHumidity.idx > 0)
    {
      sprintf(value, "%-.1f;%-.1f;%-d",
              _Hardware->DHTSensor[id]->currentTemperature,
              _Hardware->DHTSensor[id]->currentHumidity,
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      generateDeviceValue(json,
                          _Hardware->DHTSensor[id]
                              ->configuration->domoticz.temperatureHumidity.idx,
                          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Absolute Humidity */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.absoluteHumidity.idx >
        0)
    {
      sprintf(value, "%d",
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      generateDeviceValue(
          json, _Hardware->DHTSensor[id]->configuration->domoticz.absoluteHumidity.idx,
          value,
          (uint8_t)_Hardware->DHTSensor[id]->absoluteHumidity(
              _Hardware->DHTSensor[id]->currentTemperature,
              _Hardware->DHTSensor[id]->currentHumidity,
              _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                  AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* HeatIndex */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.heatIndex.idx > 0)
    {
      sprintf(value, "%-.2f",
              _Hardware->DHTSensor[id]->heatIndex(
                  _Hardware->DHTSensor[id]->currentTemperature,
                  _Hardware->DHTSensor[id]->currentHumidity,
                  _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      generateDeviceValue(
          json, _Hardware->DHTSensor[id]->configuration->domoticz.heatIndex.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* DewPoint */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.dewPoint.idx > 0)
    {
      sprintf(value, "%-.2f",
              _Hardware->DHTSensor[id]->dewPoint(
                  _Hardware->DHTSensor[id]->currentTemperature,
                  _Hardware->DHTSensor[id]->currentHumidity,
                  _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      generateDeviceValue(
          json, _Hardware->DHTSensor[id]->configuration->domoticz.dewPoint.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Perception */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.perception.idx > 0)
    {
      char _perception[22]; // Max size of Perception from lang.pack
      byte _perceptionId = _Hardware->DHTSensor[id]->perception(
          _Hardware->DHTSensor[id]->currentTemperature,
          _Hardware->DHTSensor[id]->currentHumidity,
          _Hardware->DHTSensor[id]->configuration->temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_perception,
               (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));
      generateDeviceValue(
          json,
          _Hardware->DHTSensor[id]->configuration->domoticz.perception.idx,
          _perception,
          _Hardware->DHTSensor[id]->convertPerceptionDomoticz(_perceptionId));
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }

    /* Comfort */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.comfort.idx > 0)
    {
      char _comfort[80]; // Max size of Comfort from lang.pack
      ComfortState comfortStatus;
      _Hardware->DHTSensor[id]->comfort(
          comfortStatus, _Hardware->DHTSensor[id]->currentTemperature,
          _Hardware->DHTSensor[id]->currentHumidity,
          _Hardware->DHTSensor[id]->configuration->temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
      generateDeviceValue(
          json, _Hardware->DHTSensor[id]->configuration->domoticz.comfort.idx,
          _comfort,
          _Hardware->DHTSensor[id]->convertComfortDomoticz(comfortStatus));
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return true;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
boolean AFEAPIMQTTDomoticz::publishBinarySensorState(uint8_t id)
{
  boolean publishStatus = false;
  if (enabled)
  {
    if (_Hardware->BinarySensor[id]->configuration->domoticz.idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_BINARY_SENSOR_COMMAND_LENGTH];

      generateSwitchMessage(
          json, _Hardware->BinarySensor[id]->configuration->domoticz.idx,
          _Hardware->BinarySensor[id]->get() == 1 ? AFE_SWITCH_OFF
                                                  : AFE_SWITCH_ON);
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
boolean AFEAPIMQTTDomoticz::publishMiFareCardState(uint8_t id, uint8_t tagId,
                                                   uint8_t state,
                                                   const char *user)
{
  boolean publishStatus = false;
  if (enabled)
  {
    if (_Hardware->MiFareCard[id]->configuration->domoticz[tagId].idx > 0)
    {
      char json[AFE_CONFIG_API_JSON_MIFARE_CARD_COMMAND_LENGTH];

      generateDeviceValue(
          json, _Hardware->MiFareCard[id]->configuration->domoticz[tagId].idx,
          user, state);
      publishStatus = Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
boolean AFEAPIMQTTDomoticz::publishCLEDState(uint8_t id)
{
  return enabled ? publishSwitchMessage(
                       &_Hardware->RGBLEDStrip->configuration[id].cled.idx,
                       _Hardware->RGBLEDStrip->currentState[id].state)
                 : false;
}

boolean AFEAPIMQTTDomoticz::publishCLEDColorState(uint8_t id)
{
  return enabled ? publishSetColorMessage(
                       &_Hardware->RGBLEDStrip->configuration[id].cled.idx,
                       _Hardware->RGBLEDStrip->currentState[id].state
                           ? &_Hardware->RGBLEDStrip->currentState[id].on
                           : &_Hardware->RGBLEDStrip->currentState[id].off)
                 : false;
}

boolean AFEAPIMQTTDomoticz::publishCLEDEffectsState(uint8_t id)
{
  return enabled ? publishSetLevelMessage(
                       &_Hardware->RGBLEDStrip->configuration[id].effect.idx,
                       &_Hardware->RGBLEDStrip->currentState[id].effect.id)
                 : false;
}

#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_FS3000
boolean AFEAPIMQTTDomoticz::publishFS3000SensorData(uint8_t id)
{
  if (enabled)
  {
    char json[AFE_CONFIG_API_JSON_FS3000_COMMAND_LENGTH];
    char value[20]; // 0;N;999999.99;0;?;?
    if (_Hardware->FS3000Sensor[id]
            ->configuration->domoticz.meterPerSecond.idx > 0)
    {
      sprintf(value, "0;N;%-.3f;0;?;?",
              10 * _Hardware->FS3000Sensor[id]->data->metersPerSecond);
      generateDeviceValue(json,
                          _Hardware->FS3000Sensor[id]
                              ->configuration->domoticz.meterPerSecond.idx,
                          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->FS3000Sensor[id]->configuration->domoticz.raw.idx > 0)
    {
      sprintf(value, "%d", _Hardware->FS3000Sensor[id]->data->raw);
      generateDeviceValue(
          json, _Hardware->FS3000Sensor[id]->configuration->domoticz.raw.idx,
          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->FS3000Sensor[id]
            ->configuration->domoticz.meters3PerHour.idx > 0)
    {
      sprintf(value, "%-.3f",
              _Hardware->FS3000Sensor[id]->data->meters3PerHour);
      generateDeviceValue(json,
                          _Hardware->FS3000Sensor[id]
                              ->configuration->domoticz.meters3PerHour.idx,
                          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
    if (_Hardware->FS3000Sensor[id]
            ->configuration->domoticz.milesPerHour.idx > 0)
    {
      sprintf(value, "%-.3f",
              _Hardware->FS3000Sensor[id]->data->milesPerHour);
      generateDeviceValue(json,
                          _Hardware->FS3000Sensor[id]
                              ->configuration->domoticz.milesPerHour.idx,
                          value);
      Mqtt->publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, json);
    }
  }
}
#endif //  AFE_CONFIG_HARDWARE_FS3000

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
