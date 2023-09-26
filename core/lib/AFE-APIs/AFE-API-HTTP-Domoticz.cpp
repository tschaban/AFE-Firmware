/* AFE _Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP-Domoticz.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

AFEAPIHTTPDomoticz::AFEAPIHTTPDomoticz(){};

void AFEAPIHTTPDomoticz::begin(AFEFirmware *Firmware, AFEHardware *Hardware) {
  _Firmware = Firmware;
  _Hardware = Hardware;

  DOMOTICZ *configuration = new DOMOTICZ;
  _Firmware->API->Flash->getConfiguration(configuration);

  char _user[45] = {0}; // base64 conversion takes ceil(n/3)*4 size of mem
  char _pass[45] = {0};
  char authorization[20 + sizeof(_user) + sizeof(_pass) + 1] = {0};

  if (configuration->user[0] != AFE_EMPTY_STRING &&
      configuration->password[0] != AFE_EMPTY_STRING) {
    rbase64.encode(configuration->user);
    sprintf(_user, rbase64.result());
    rbase64.encode(configuration->password);
    sprintf(_pass, rbase64.result());
    sprintf(authorization, "&username=%s&password=%s", _user, _pass);
  }

  http->setTimeout(AFE_CONFIG_API_HTTP_TIMEOUT);

  if (strlen(configuration->host) > 0) {
    _initialized = true;
    sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
            configuration->protocol == 0 ? "http://" : "https://",
            configuration->host, configuration->port, authorization);
  } else {
    serverURL[0] = AFE_EMPTY_STRING;
  }
}

const String AFEAPIHTTPDomoticz::getApiCall(const char *param,
                                            unsigned int idx) {
  char url[sizeof(serverURL) + 18 + strlen(param)];
  sprintf(url, "%s&param=%s&idx=%u", serverURL, param, idx);
  return url;
}

boolean AFEAPIHTTPDomoticz::callURL(const String url) {
  boolean _return;
#ifdef AFE_CONFIG_HARDWARE_LED
  _Firmware->Hardware->SystemLed->on();
#endif

#ifdef DEBUG
  _Firmware->Debugger->printInformation(F("Publishing: "),F("HTTP-Domoticz"));
  Serial << url;
#endif
  http->begin(client, url);
  _return = http->GET() == 200 ? true : false;
  http->end();
  delay(10);
#ifdef AFE_CONFIG_HARDWARE_LED
  _Firmware->Hardware->SystemLed->off();
#endif
  return _return;
}

/*
void AFEAPIHTTPDomoticz::replaceSpaceinUrl(const char *inputString,
                                           const char &outputString); {}
*/

boolean AFEAPIHTTPDomoticz::sendSwitchCommand(unsigned int idx, boolean state) {
  boolean _return = false;
  // @TODO T7  - problem with updates in DOmoticz if changed by browser
  if (_initialized && idx > 0) {
    String call = getApiCall("switchlight", idx);
    call += "&switchcmd=";
    call += state ? "On" : "Off";
    _return = callURL(call);
  }
  return _return;
}

boolean AFEAPIHTTPDomoticz::sendCustomSensorCommand(unsigned int idx,
                                                    const char *value,
                                                    uint16_t nvalue) {
  boolean _return = false;
  if (_initialized) {
    String call = getApiCall("udevice", idx);
    call += F("&nvalue=");
    call += nvalue;
    call += F("&svalue=");
    call += value;
    call.replace(F(" "), F("%20"));
    _return = callURL(call);
  }
  return _return;
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIHTTPDomoticz::publishRelayState(uint8_t id) {
  return _initialized ? sendSwitchCommand(
                            _Hardware->Relay[id]->configuration->domoticz.idx,
                            _Hardware->Relay[id]->get() == AFE_RELAY_ON)
                      : false;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIHTTPDomoticz::publishSwitchState(uint8_t id) {
  return _initialized ? sendSwitchCommand(
                            _Hardware->Switch[id]->configuration->domoticz.idx,
                            !_Hardware->Switch[id]->getPhisicalState())
                      : false;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
void AFEAPIHTTPDomoticz::publishADCValues(uint8_t id) {
  if (_initialized) {
    char value[20];
    if (_Hardware->AnalogInput[id]->configuration->domoticz.percent > 0) {
      sprintf(value, "%-.2f", _Hardware->AnalogInput[id]->data->percent);
      sendCustomSensorCommand(
          _Hardware->AnalogInput[id]->configuration->domoticz.percent, value);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _Hardware->AnalogInput[id]->data->voltage);
      sendCustomSensorCommand(
          _Hardware->AnalogInput[id]->configuration->domoticz.voltage, value);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.voltageCalculated >
        0) {
      sprintf(value, "%-.4f",
              _Hardware->AnalogInput[id]->data->voltageCalculated);
      sendCustomSensorCommand(
          _Hardware->AnalogInput[id]->configuration->domoticz.voltageCalculated,
          value);
    }
    if (_Hardware->AnalogInput[id]->configuration->domoticz.raw > 0) {
      sprintf(value, "%-d", _Hardware->AnalogInput[id]->data->raw);
      sendCustomSensorCommand(
          _Hardware->AnalogInput[id]->configuration->domoticz.raw, value);
    }
  }
}
#else  // ESP8266
void AFEAPIHTTPDomoticz::publishADCValues() {
  if (_initialized) {
    char value[20];
    if (_Hardware->AnalogInput->configuration->domoticz.percent > 0) {
      sprintf(value, "%-.2f", _Hardware->AnalogInput->data->percent);
      sendCustomSensorCommand(
          _Hardware->AnalogInput->configuration->domoticz.percent, value);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _Hardware->AnalogInput->data->voltage);
      sendCustomSensorCommand(
          _Hardware->AnalogInput->configuration->domoticz.voltage, value);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _Hardware->AnalogInput->data->voltageCalculated);
      sendCustomSensorCommand(
          _Hardware->AnalogInput->configuration->domoticz.voltageCalculated,
          value);
    }
    if (_Hardware->AnalogInput->configuration->domoticz.raw > 0) {
      sprintf(value, "%-d", _Hardware->AnalogInput->data->raw);
      sendCustomSensorCommand(
          _Hardware->AnalogInput->configuration->domoticz.raw, value);
    }
  }
}
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#ifdef AFE_ESP32
boolean AFEAPIHTTPDomoticz::publishBatteryMeterValues(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    char value[8];
    if (_Hardware->AnalogInput[id]->configuration->battery.domoticz.idx > 0) {
      sprintf(value, "%-.3f", _Hardware->AnalogInput[id]->batteryPercentage);
      _ret = sendCustomSensorCommand(
          _Hardware->AnalogInput[id]->configuration->battery.domoticz.idx,
          value);
    }
  }
  return _ret;
}

#else

boolean AFEAPIHTTPDomoticz::publishBatteryMeterValues() {
  boolean _ret = false;
  if (_initialized) {
    char value[8];
    if (_Hardware->AnalogInput->configuration->battery.domoticz.idx > 0) {
      sprintf(value, "%-.3f", _Hardware->AnalogInput->batteryPercentage);
      _ret = sendCustomSensorCommand(
          _Hardware->AnalogInput->configuration->battery.domoticz.idx, value);
    }
  }
  return _ret;
}
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
boolean AFEAPIHTTPDomoticz::publishBoschBMSensorData(uint8_t id) {

  boolean _ret = false;
  if (_initialized) {
    char value[20];
    if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.temperature.idx >
        0) {
      sprintf(value, "%-.2f",
              _Hardware->BMEX80Sensor[id]->data->temperature.value);
      sendCustomSensorCommand(
          _Hardware->BMEX80Sensor[id]->configuration->domoticz.temperature.idx,
          value);
    }

    if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.pressure.idx > 0) {
      sprintf(value, "%-.2f;0",
              _Hardware->BMEX80Sensor[id]->data->pressure.value);
      sendCustomSensorCommand(
          _Hardware->BMEX80Sensor[id]->configuration->domoticz.pressure.idx,
          value);
    }
    if (_Hardware->BMEX80Sensor[id]
            ->configuration->domoticz.relativePressure.idx > 0) {
      sprintf(value, "%-.2f;0",
              _Hardware->BMEX80Sensor[id]->data->relativePressure.value);
      sendCustomSensorCommand(
          _Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.relativePressure.idx,
          value);
    }

    if (_Hardware->BMEX80Sensor[id]->configuration->type != AFE_BMP180_SENSOR) {

      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.temperatureHumidity.idx > 0) {
        sprintf(value, "%-.2f;%-.2f;%-d",
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]
                ->configuration->domoticz.temperatureHumidity.idx,
            value);
      }

      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.temperatureHumidityPressure.idx > 0) {
        sprintf(value, "%-.2f;%-.2f;%-d;%-.2f;0",
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value),
                _Hardware->BMEX80Sensor[id]->data->pressure.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]
                ->configuration->domoticz.temperatureHumidityPressure.idx,
            value);
      }

      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.humidity.idx >
          0) {
        sprintf(value, "%d",
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.humidity.idx,
            value, (uint8_t)_Hardware->BMEX80Sensor[id]->data->humidity.value);
      }

      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.absoluteHumidity.idx > 0) {
        sprintf(value, "%d",
                _Hardware->BMEX80Sensor[id]->convertHumidyStatusDomoticz(
                    _Hardware->BMEX80Sensor[id]->data->humidity.value));
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]
                ->configuration->domoticz.absoluteHumidity.idx,
            value,
            (uint8_t)_Hardware->BMEX80Sensor[id]->absoluteHumidity(
                _Hardware->BMEX80Sensor[id]->data->temperature.value,
                _Hardware->BMEX80Sensor[id]->data->humidity.value,
                _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                    AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      }

      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.dewPoint.idx >
          0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->dewPoint.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.dewPoint.idx,
            value);
      }
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx >
          0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->heatIndex.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx,
            value);
      }

      /* Perception */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.perception.idx >
          0) {
        char _perception[22]; // Max size of Perception from lang.pack
        byte _perceptionId = _Hardware->BMEX80Sensor[id]->perception(
            _Hardware->BMEX80Sensor[id]->data->temperature.value,
            _Hardware->BMEX80Sensor[id]->data->humidity.value,
            _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_perception,
                 (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));

        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx,
            _perception, _Hardware->BMEX80Sensor[id]->convertPerceptionDomoticz(
                             _perceptionId));
      }

      /* Comfort */
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.comfort.idx >
          0) {
        char _comfort[80]; // Max size of Comfort from lang.pack
        ComfortState comfortStatus;
        _Hardware->BMEX80Sensor[id]->comfort(
            comfortStatus, _Hardware->BMEX80Sensor[id]->data->temperature.value,
            _Hardware->BMEX80Sensor[id]->data->humidity.value,
            _Hardware->BMEX80Sensor[id]->configuration->temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.heatIndex.idx,
            _comfort,
            _Hardware->BMEX80Sensor[id]->convertComfortDomoticz(comfortStatus));
      }
    }

    if (_Hardware->BMEX80Sensor[id]->configuration->type == AFE_BME680_SENSOR) {
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.gasResistance.idx > 0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->gasResistance.value);
        sendCustomSensorCommand(_Hardware->BMEX80Sensor[id]
                                    ->configuration->domoticz.gasResistance.idx,
                                value);
      }
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.iaq.idx > 0) {
        sprintf(value, "%-.2f", _Hardware->BMEX80Sensor[id]->data->iaq.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.iaq.idx,
            value);
      }
      if (_Hardware->BMEX80Sensor[id]->configuration->domoticz.staticIaq.idx >
          0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->staticIaq.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]->configuration->domoticz.staticIaq.idx,
            value);
      }
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.co2Equivalent.idx > 0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->co2Equivalent.value);
        sendCustomSensorCommand(_Hardware->BMEX80Sensor[id]
                                    ->configuration->domoticz.co2Equivalent.idx,
                                value);
      }
      if (_Hardware->BMEX80Sensor[id]
              ->configuration->domoticz.breathVocEquivalent.idx > 0) {
        sprintf(value, "%-.2f",
                _Hardware->BMEX80Sensor[id]->data->breathVocEquivalent.value);
        sendCustomSensorCommand(
            _Hardware->BMEX80Sensor[id]
                ->configuration->domoticz.breathVocEquivalent.idx,
            value);
      }
    }
    /* ADD SENDING THESE TWO
      DOMOTICZ_BASIC_CONFIG temperatureHumidity;
  DOMOTICZ_BASIC_CONFIG temperatureHumidityPressure;
  */

    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
boolean AFEAPIHTTPDomoticz::publishHPMA115S0SensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    char value[5];
    if (_Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm10.idx > 0) {
      sprintf(value, "%-.1f", _Hardware->HPMA115S0Sensor[id]->data->pm10);
      sendCustomSensorCommand(
          _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm10.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->pm10);
    }
    if (_Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm25.idx > 0) {
      sprintf(value, "%-.1f", _Hardware->HPMA115S0Sensor[id]->data->pm25);
      sendCustomSensorCommand(
          _Hardware->HPMA115S0Sensor[id]->configuration->domoticz.pm25.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->pm25);
    }
    if (_Hardware->HPMA115S0Sensor[id]
            ->configuration->domoticz.whoPM10Norm.idx > 0) {
      sprintf(value, "%-.1f",
              _Hardware->HPMA115S0Sensor[id]->data->whoPM10Norm);
      sendCustomSensorCommand(
          _Hardware->HPMA115S0Sensor[id]
              ->configuration->domoticz.whoPM10Norm.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->whoPM10Norm);
    }
    if (_Hardware->HPMA115S0Sensor[id]
            ->configuration->domoticz.whoPM25Norm.idx > 0) {
      sprintf(value, "%-.1f",
              _Hardware->HPMA115S0Sensor[id]->data->whoPM25Norm);
      sendCustomSensorCommand(
          _Hardware->HPMA115S0Sensor[id]
              ->configuration->domoticz.whoPM25Norm.idx,
          value, _Hardware->HPMA115S0Sensor[id]->data->whoPM25Norm);
    }
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
boolean AFEAPIHTTPDomoticz::publishBH1750SensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    if (_Hardware->BH1750Sensor[id]->configuration->domoticz.idx > 0) {
      char value[10];
      sprintf(value, "%-.2f", _Hardware->BH1750Sensor[id]->data);
      sendCustomSensorCommand(
          _Hardware->BH1750Sensor[id]->configuration->domoticz.idx, value);
      _ret = true;
    }
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
boolean AFEAPIHTTPDomoticz::publishTSL2561SensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    char value[6]; // max 65536
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.illuminance.idx >
        0) {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->illuminance);
      sendCustomSensorCommand(
          _Hardware->TSL2561Sensor[id]->configuration->domoticz.illuminance.idx,
          value);
      _ret = true;
    }
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.ir.idx > 0) {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->ir);
      sendCustomSensorCommand(
          _Hardware->TSL2561Sensor[id]->configuration->domoticz.ir.idx, value);
      _ret = true;
    }
    if (_Hardware->TSL2561Sensor[id]->configuration->domoticz.broadband.idx >
        0) {
      sprintf(value, "%d", _Hardware->TSL2561Sensor[id]->broadband);
      sendCustomSensorCommand(
          _Hardware->TSL2561Sensor[id]->configuration->domoticz.broadband.idx,
          value);
      _ret = true;
    }
  }

  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
boolean AFEAPIHTTPDomoticz::publishAS3935SensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized &&
      _Hardware->AS3935Sensorr[id]->configuration.domoticz.idx > 0) {
    char value[20]; // @TODO T6 CHeck the max size
    sprintf(value, "%-d", _Hardware->AS3935Sensorr[id]->distance);
    sendCustomSensorCommand(
        _Hardware->AS3935Sensorr[id]->configuration.domoticz.idx, value);
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIHTTPDomoticz::publishAnemometerSensorData() {
  if (_initialized) {
    char value[20];
    if (_Hardware->AnemometerSensor->configuration->domoticz.idx > 0) {
      sprintf(value, "0;N;%-.2f;0;?;?",
              10 * _Hardware->AnemometerSensor->lastSpeedMS);
      sendCustomSensorCommand(
          _Hardware->AnemometerSensor->configuration->domoticz.idx, value);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIHTTPDomoticz::publishRainSensorData() {
  if (_initialized) {
    char value[20]; // 999999.00;999999.00
    if (_Hardware->RainmeterSensor->configuration->domoticz.idx > 0) {
      sprintf(value, "%-.2f;%-.2f",
              _Hardware->RainmeterSensor->rainLevelLastHour * 100,
              _Hardware->RainmeterSensor->current->counter);
      sendCustomSensorCommand(
          _Hardware->RainmeterSensor->configuration->domoticz.idx, value);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
boolean AFEAPIHTTPDomoticz::publishGateState(uint8_t id) {
  return _initialized ? sendSwitchCommand(
                            _Hardware->Gate[id]->configuration->domoticz.idx,
                            _Hardware->Gate[id]->get() == AFE_GATE_OPEN)
                      : false;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
boolean AFEAPIHTTPDomoticz::publishContactronState(uint8_t id) {
  return _initialized
             ? sendSwitchCommand(
                   _Hardware->Contactron[id]->configuration->domoticz.idx,
                   _Hardware->Contactron[id]->get() == AFE_CONTACTRON_OPEN)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
boolean AFEAPIHTTPDomoticz::publishDS18B20SensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    if (_Hardware->DS18B20Sensor[id]->configuration->domoticz.idx > 0) {
      char value[9]; // Max size: -999.999
      sprintf(value, "%-.3f", _Hardware->DS18B20Sensor[id]->getTemperature());
      sendCustomSensorCommand(
          _Hardware->DS18B20Sensor[id]->configuration->domoticz.idx, value);
      _ret = true;
    }
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
boolean AFEAPIHTTPDomoticz::publishRegulatorState(uint8_t id) {
  return _initialized
             ? sendSwitchCommand(
                   _Hardware->Regulator[id]->configuration->domoticz.idx,
                   _Hardware->Regulator[id]->configuration->enabled)
             : false;
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
boolean AFEAPIHTTPDomoticz::publishThermalProtectorState(uint8_t id) {
  return _initialized
             ? sendSwitchCommand(
                   _Hardware->ThermalProtector[id]->configuration->domoticz.idx,
                   _Hardware->ThermalProtector[id]->configuration->enabled)
             : false;
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
boolean AFEAPIHTTPDomoticz::publishDHTSensorData(uint8_t id) {
  boolean _ret = false;
  if (_initialized) {
    char value[15];
    /* Temperature */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.temperature.idx > 0) {
      sprintf(value, "%-.1f", _Hardware->DHTSensor[id]->currentTemperature);
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.temperature.idx,
          value);
      _ret = true;
    }

    /* Humidity */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.humidity.idx > 0) {
      sprintf(value, "%d",
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.humidity.idx, value,
          (uint8_t)_Hardware->DHTSensor[id]->currentHumidity);
      _ret = true;
    }

    /* Absolute Humidity */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.absoluteHumidity.idx >
        0) {
      sprintf(value, "%d",
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]
              ->configuration->domoticz.absoluteHumidity.idx,
          value,
          (uint8_t)_Hardware->DHTSensor[id]->absoluteHumidity(
              _Hardware->DHTSensor[id]->currentTemperature,
              _Hardware->DHTSensor[id]->currentHumidity,
              _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                  AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      _ret = true;
    }

    /* DewPoint */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.dewPoint.idx > 0) {
      sprintf(value, "%-.2f",
              _Hardware->DHTSensor[id]->dewPoint(
                  _Hardware->DHTSensor[id]->currentTemperature,
                  _Hardware->DHTSensor[id]->currentHumidity,
                  _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.dewPoint.idx,
          value);
      _ret = true;
    }

    /* HeatIndex */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.heatIndex.idx > 0) {
      sprintf(value, "%-.2f",
              _Hardware->DHTSensor[id]->heatIndex(
                  _Hardware->DHTSensor[id]->currentTemperature,
                  _Hardware->DHTSensor[id]->currentHumidity,
                  _Hardware->DHTSensor[id]->configuration->temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.heatIndex.idx,
          value);
      _ret = true;
    }

    /* Temperature & Humidity */
    if (_Hardware->DHTSensor[id]
            ->configuration->domoticz.temperatureHumidity.idx > 0) {
      sprintf(value, "%-.1f;%-.1f;%-d",
              _Hardware->DHTSensor[id]->currentTemperature,
              _Hardware->DHTSensor[id]->currentHumidity,
              _Hardware->DHTSensor[id]->convertHumidyStatusDomoticz(
                  _Hardware->DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]
              ->configuration->domoticz.temperatureHumidity.idx,
          value);
      _ret = true;
    }

    /* Comfort */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.comfort.idx > 0) {
      ComfortState comfortStatus;
      char _charText[100]; // Max size of Comfort,Perception from lang.pack +
                           // %20 instead of space
      String _stringText;
      _Hardware->DHTSensor[id]->comfort(
          comfortStatus, _Hardware->DHTSensor[id]->currentTemperature,
          _Hardware->DHTSensor[id]->currentHumidity,
          _Hardware->DHTSensor[id]->configuration->temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_charText, (char *)pgm_read_dword(&(Comfort[comfortStatus])));

      // @TODO T2 is there a better one?
      _stringText = _charText;
      _stringText.replace(F(" "), F("%20"));
      _stringText.toCharArray(_charText, sizeof(_charText));

      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.comfort.idx,
          _charText,
          _Hardware->DHTSensor[id]->convertComfortDomoticz(comfortStatus));
      _ret = true;
    }

    /* Perception */
    if (_Hardware->DHTSensor[id]->configuration->domoticz.perception.idx > 0) {
      char _charText[100]; // Max size of Comfort,Perception from lang.pack +
                           // %20 instead of space
      String _stringText;
      byte _perceptionId = _Hardware->DHTSensor[id]->perception(
          _Hardware->DHTSensor[id]->currentTemperature,
          _Hardware->DHTSensor[id]->currentHumidity,
          _Hardware->DHTSensor[id]->configuration->temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_charText,
               (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));

      // @TODO T2 is there a better one?
      _stringText = _charText;
      _stringText.replace(F(" "), F("%20"));
      _stringText.toCharArray(_charText, sizeof(_charText));

      sendCustomSensorCommand(
          _Hardware->DHTSensor[id]->configuration->domoticz.perception.idx,
          _charText,
          _Hardware->DHTSensor[id]->convertPerceptionDomoticz(_perceptionId));
      _ret = true;
    }
  }

  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
boolean AFEAPIHTTPDomoticz::publishBinarySensorState(uint8_t id) {
  return _initialized
             ? sendSwitchCommand(
                   _Hardware->BinarySensor[id]->configuration->domoticz.idx,
                   _Hardware->BinarySensor[id]->get() == 0)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
boolean AFEAPIHTTPDomoticz::publishMiFareCardState(uint8_t id, uint8_t tagId,
                                                   uint8_t state,
                                                   const char *user) {
  boolean publishStatus = false;
  if (_initialized) {
    if (_Hardware->MiFareCard[id]->configuration.domoticz[tagId].idx > 0) {
      publishStatus = sendCustomSensorCommand(
          _Hardware->MiFareCard[id]->configuration.domoticz[tagId].idx, user,
          state);
    }
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_FS3000
boolean AFEAPIHTTPDomoticz::publishFS3000SensorData(uint8_t id) {
  if (_initialized) {
    char value[20];
    if (_Hardware->FS3000Sensor[id]
            ->configuration->domoticz.meterPerSecond.idx > 0) {
      sprintf(value, "0;N;%-.3f;0;?;?",
              10 * _Hardware->FS3000Sensor[id]->data->metersPerSecond);
      sendCustomSensorCommand(_Hardware->FS3000Sensor[id]
                                  ->configuration->domoticz.meterPerSecond.idx,
                              value);
    }
    if (_Hardware->FS3000Sensor[id]->configuration->domoticz.raw.idx > 0) {
      sprintf(value, "%d", _Hardware->FS3000Sensor[id]->data->raw);
      sendCustomSensorCommand(
          _Hardware->FS3000Sensor[id]->configuration->domoticz.raw.idx, value);
    }
    if (_Hardware->FS3000Sensor[id]->configuration->domoticz.milesPerHour.idx >
        0) {
      sprintf(value, "%-.3f", _Hardware->FS3000Sensor[id]->data->milesPerHour);
      sendCustomSensorCommand(
          _Hardware->FS3000Sensor[id]->configuration->domoticz.milesPerHour.idx,
          value);
    }
    if (_Hardware->FS3000Sensor[id]
            ->configuration->domoticz.meters3PerHour.idx > 0) {
      sprintf(value, "%-.3f", _Hardware->FS3000Sensor[id]->data->meters3PerHour);
      sendCustomSensorCommand(_Hardware->FS3000Sensor[id]
                                  ->configuration->domoticz.meters3PerHour.idx,
                              value);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_FS3000

/* RGB LED are not supported in HTTP Domoticz API
#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHTTPDomoticz::addClass(AFECLED *CLed) { AFEAPI::addClass(CLed); }

boolean AFEAPIHTTPDomoticz::publishCLEDState(uint8_t id) {
  return _initialized ? sendSwitchCommand(_CLED->configuration[id].domoticz.idx,
                                     _CLED->currentState[id].state == AFE_ON)
                 : false;
}

boolean AFEAPIHTTPDomoticz::publishCLEDEffectState(uint8_t id) {
  if (_initialized) {
    sendSwitchCommand(_CLED->configurationEffectBlinking[id].domoticz.idx,
                      _CLED->currentState[id].effect.id ==
                              AFE_DOMOTICZ_DEVICE_CLED_EFFECT_BLINKING
                          ? AFE_ON
                          : AFE_OFF);
    sendSwitchCommand(_CLED->configurationEffectFadeInOut[id].domoticz.idx,
                      _CLED->currentState[id].effect.id ==
                              AFE_DOMOTICZ_DEVICE_CLED_EFFECT_FADE_IN_OUT
                          ? AFE_ON
                          : AFE_OFF);

    return sendSwitchCommand(_CLED->configurationEffectWave[id].domoticz.idx,
                             _CLED->currentState[id].effect.id ==
                                     AFE_DOMOTICZ_DEVICE_CLED_EFFECT_WAVE
                                 ? AFE_ON
                                 : AFE_OFF);
  } else {
    return false;
  }
}
#endif // AFE_CONFIG_HARDWARE_CLED
*/
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED