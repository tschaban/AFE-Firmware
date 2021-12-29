/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP-Domoticz.h"

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

AFEAPIHTTPDomoticz::AFEAPIHTTPDomoticz() : AFEAPI(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAPIHTTPDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  AFEAPI::begin(Data, Device, Led);
  init();
}
#else
void AFEAPIHTTPDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  AFEAPI::begin(Data, Device);
  init();
}
#endif // AFE_CONFIG_HARDWARE_LED

void AFEAPIHTTPDomoticz::init() {
  _Data->getConfiguration(&configuration);

  char _user[45] = {0}; // base64 conversion takes ceil(n/3)*4 size of mem
  char _pass[45] = {0};
  char authorization[20 + sizeof(_user) + sizeof(_pass) + 1] = {0};

  if (configuration.user[0] != AFE_EMPTY_STRING &&
      configuration.password[0] != AFE_EMPTY_STRING) {
    rbase64.encode(configuration.user);
    sprintf(_user, rbase64.result());
    rbase64.encode(configuration.password);
    sprintf(_pass, rbase64.result());
    sprintf(authorization, "&username=%s&password=%s", _user, _pass);
  }

  http.setTimeout(AFE_CONFIG_API_HTTP_TIMEOUT);

  sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port, authorization);
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
  _Led->on();
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: Publishing to Domoticz: ") << url;
#endif
  http.begin(client, url);
  _return = http.GET() == 200 ? true : false;
  http.end();
  delay(10);
#ifdef AFE_CONFIG_HARDWARE_LED
  _Led->off();
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
  if (enabled && idx > 0) {
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
  if (enabled) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=";
    call += nvalue;
    call += "&svalue=";
    call += value;
    call.replace(" ", "%20");
    _return = callURL(call);
  }
  return _return;
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHTTPDomoticz::addClass(AFERelay *Relay) { AFEAPI::addClass(Relay); }
boolean AFEAPIHTTPDomoticz::publishRelayState(uint8_t id) {
  return enabled ? sendSwitchCommand(_Relay[id]->configuration.domoticz.idx,
                                     _Relay[id]->get() == AFE_RELAY_ON)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHTTPDomoticz::addClass(AFESwitch *Switch) {
  AFEAPI::addClass(Switch);
}
boolean AFEAPIHTTPDomoticz::publishSwitchState(uint8_t id) {
  return enabled ? sendSwitchCommand(_Switch[id]->configuration.domoticz.idx,
                                     !_Switch[id]->getPhisicalState())
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
void AFEAPIHTTPDomoticz::addClass(AFEAnalogInput *Analog) {
  AFEAPI::addClass(Analog);
}
#ifdef AFE_ESP32
void AFEAPIHTTPDomoticz::publishADCValues(uint8_t id) {
  if (enabled) {
    char value[20];
    if (_AnalogInput[id]->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _AnalogInput[id]->data.percent);
      sendCustomSensorCommand(_AnalogInput[id]->configuration.domoticz.percent,
                              value);
    }
    if (_AnalogInput[id]->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _AnalogInput[id]->data.voltage);
      sendCustomSensorCommand(_AnalogInput[id]->configuration.domoticz.voltage,
                              value);
    }
    if (_AnalogInput[id]->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _AnalogInput[id]->data.voltageCalculated);
      sendCustomSensorCommand(
          _AnalogInput[id]->configuration.domoticz.voltageCalculated, value);
    }
    if (_AnalogInput[id]->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _AnalogInput[id]->data.raw);
      sendCustomSensorCommand(_AnalogInput[id]->configuration.domoticz.raw,
                              value);
    }
  }
}
#else  // ESP8266
void AFEAPIHTTPDomoticz::publishADCValues() {
  if (enabled) {
    char value[20];
    if (_AnalogInput->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _AnalogInput->data.percent);
      sendCustomSensorCommand(_AnalogInput->configuration.domoticz.percent,
                              value);
    }
    if (_AnalogInput->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _AnalogInput->data.voltage);
      sendCustomSensorCommand(_AnalogInput->configuration.domoticz.voltage,
                              value);
    }
    if (_AnalogInput->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _AnalogInput->data.voltageCalculated);
      sendCustomSensorCommand(
          _AnalogInput->configuration.domoticz.voltageCalculated, value);
    }
    if (_AnalogInput->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _AnalogInput->data.raw);
      sendCustomSensorCommand(_AnalogInput->configuration.domoticz.raw, value);
    }
  }
}
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
boolean AFEAPIHTTPDomoticz::publishBatteryMeterValues() {
  boolean _ret = false;
  if (enabled) {
    char value[8];
    if (_AnalogInput->configuration.battery.domoticz.idx > 0) {
      sprintf(value, "%-.3f", _AnalogInput->batteryPercentage);
      _ret = sendCustomSensorCommand(
          _AnalogInput->configuration.battery.domoticz.idx, value);
    }
  }
  return _ret;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
void AFEAPIHTTPDomoticz::addClass(AFESensorBMEX80 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishBMx80SensorData(uint8_t id) {

  boolean _ret = false;
  if (enabled) {
    char value[20]; // @TODO CHeck the max size
    if (_BMx80Sensor[id]->configuration.domoticz.temperature.idx > 0) {
      sprintf(value, "%-.2f", _BMx80Sensor[id]->data.temperature.value);
      sendCustomSensorCommand(
          _BMx80Sensor[id]->configuration.domoticz.temperature.idx, value);
    }

    if (_BMx80Sensor[id]->configuration.domoticz.pressure.idx > 0) {
      sprintf(value, "%-.2f;0", _BMx80Sensor[id]->data.pressure.value);
      sendCustomSensorCommand(
          _BMx80Sensor[id]->configuration.domoticz.pressure.idx, value);
    }
    if (_BMx80Sensor[id]->configuration.domoticz.relativePressure.idx > 0) {
      sprintf(value, "%-.2f;0", _BMx80Sensor[id]->data.relativePressure.value);
      sendCustomSensorCommand(
          _BMx80Sensor[id]->configuration.domoticz.relativePressure.idx, value);
    }

    if (_BMx80Sensor[id]->configuration.type != AFE_BMP180_SENSOR) {

      if (_BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx >
          0) {
        sprintf(value, "%-.2f;%-.2f;%-d",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                    _BMx80Sensor[id]->data.humidity.value));
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.temperatureHumidity.idx,
            value);
      }

      if (_BMx80Sensor[id]
              ->configuration.domoticz.temperatureHumidityPressure.idx > 0) {
        sprintf(value, "%-.2f;%-.2f;%-d;%-.2f;0",
                _BMx80Sensor[id]->data.temperature.value,
                _BMx80Sensor[id]->data.humidity.value,
                _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                    _BMx80Sensor[id]->data.humidity.value),
                _BMx80Sensor[id]->data.pressure.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]
                ->configuration.domoticz.temperatureHumidityPressure.idx,
            value);
      }

      if (_BMx80Sensor[id]->configuration.domoticz.humidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                                 _BMx80Sensor[id]->data.humidity.value));
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.humidity.idx, value,
            (uint8_t)_BMx80Sensor[id]->data.humidity.value);
      }

      if (_BMx80Sensor[id]->configuration.domoticz.absoluteHumidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->convertHumidyStatusDomoticz(
                                 _BMx80Sensor[id]->data.humidity.value));
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.absoluteHumidity.idx,
            value, (uint8_t)_BMx80Sensor[id]->absoluteHumidity(
                       _BMx80Sensor[id]->data.temperature.value,
                       _BMx80Sensor[id]->data.humidity.value,
                       _BMx80Sensor[id]->configuration.temperature.unit ==
                           AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      }

      if (_BMx80Sensor[id]->configuration.domoticz.dewPoint.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.dewPoint.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.dewPoint.idx, value);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.heatIndex.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.heatIndex.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.heatIndex.idx, value);
      }

      /* Perception */
      if (_BMx80Sensor[id]->configuration.domoticz.perception.idx > 0) {
        char _perception[22]; // Max size of Perception from lang.pack
        byte _perceptionId = _BMx80Sensor[id]->perception(
            _BMx80Sensor[id]->data.temperature.value,
            _BMx80Sensor[id]->data.humidity.value,
            _BMx80Sensor[id]->configuration.temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_perception,
                 (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));

        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.heatIndex.idx, _perception,
            _BMx80Sensor[id]->convertPerceptionDomoticz(_perceptionId));
      }

      /* Comfort */
      if (_BMx80Sensor[id]->configuration.domoticz.comfort.idx > 0) {
        char _comfort[80]; // Max size of Comfort from lang.pack
        ComfortState comfortStatus;
        _BMx80Sensor[id]->comfort(
            comfortStatus, _BMx80Sensor[id]->data.temperature.value,
            _BMx80Sensor[id]->data.humidity.value,
            _BMx80Sensor[id]->configuration.temperature.unit ==
                AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.heatIndex.idx, _comfort,
            _BMx80Sensor[id]->convertComfortDomoticz(comfortStatus));
      }
    }

    if (_BMx80Sensor[id]->configuration.type == AFE_BME680_SENSOR) {
      if (_BMx80Sensor[id]->configuration.domoticz.gasResistance.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.gasResistance.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.gasResistance.idx, value);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.iaq.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.iaq.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.iaq.idx, value);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.staticIaq.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.staticIaq.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.staticIaq.idx, value);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx > 0) {
        sprintf(value, "%-.2f", _BMx80Sensor[id]->data.co2Equivalent.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.co2Equivalent.idx, value);
      }
      if (_BMx80Sensor[id]->configuration.domoticz.breathVocEquivalent.idx >
          0) {
        sprintf(value, "%-.2f",
                _BMx80Sensor[id]->data.breathVocEquivalent.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.breathVocEquivalent.idx,
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
void AFEAPIHTTPDomoticz::addClass(AFESensorHPMA115S0 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishHPMA115S0SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled) {
    char value[5];
    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.pm10);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx, value,
          _HPMA115S0Sensor[id]->data.pm10);
    }
    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.pm25);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx, value,
          _HPMA115S0Sensor[id]->data.pm25);
    }
    if (_HPMA115S0Sensor[id]->configuration.domoticz.whoPM10Norm.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.whoPM10Norm);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.whoPM10Norm.idx, value,
          _HPMA115S0Sensor[id]->data.whoPM10Norm);
    }
    if (_HPMA115S0Sensor[id]->configuration.domoticz.whoPM25Norm.idx > 0) {
      sprintf(value, "%-.1f", _HPMA115S0Sensor[id]->data.whoPM25Norm);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.whoPM25Norm.idx, value,
          _HPMA115S0Sensor[id]->data.whoPM25Norm);
    }
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BH1750
void AFEAPIHTTPDomoticz::addClass(AFESensorBH1750 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishBH1750SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled && _BH1750Sensor[id]->configuration.domoticz.idx > 0) {
    char value[20]; // @TODO CHeck the max size
    sprintf(value, "%-.2f", _BH1750Sensor[id]->data);
    sendCustomSensorCommand(_BH1750Sensor[id]->configuration.domoticz.idx,
                            value);
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_TSL2561
void AFEAPIHTTPDomoticz::addClass(AFESensorTSL2561 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishTSL2561SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled) {
    char value[6]; // max 65536
    if (_TSL2561Sensor[id]->configuration.domoticz.illuminance.idx > 0) {
      sprintf(value, "%d", _TSL2561Sensor[id]->illuminance);
      sendCustomSensorCommand(
          _TSL2561Sensor[id]->configuration.domoticz.illuminance.idx, value);
      _ret = true;
    }
    if (_TSL2561Sensor[id]->configuration.domoticz.ir.idx > 0) {
      sprintf(value, "%d", _TSL2561Sensor[id]->ir);
      sendCustomSensorCommand(_TSL2561Sensor[id]->configuration.domoticz.ir.idx,
                              value);
      _ret = true;
    }
    if (_TSL2561Sensor[id]->configuration.domoticz.broadband.idx > 0) {
      sprintf(value, "%d", _TSL2561Sensor[id]->broadband);
      sendCustomSensorCommand(
          _TSL2561Sensor[id]->configuration.domoticz.broadband.idx, value);
      _ret = true;
    }
  }

  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_AS3935
void AFEAPIHTTPDomoticz::addClass(AFESensorAS3935 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishAS3935SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled && _AS3935Sensor[id]->configuration.domoticz.idx > 0) {
    char value[20]; // @TODO CHeck the max size
    sprintf(value, "%-d", _AS3935Sensor[id]->distance);
    sendCustomSensorCommand(_AS3935Sensor[id]->configuration.domoticz.idx,
                            value);
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
void AFEAPIHTTPDomoticz::addClass(AFEAnemometer *Sensor) {
  AFEAPI::addClass(Sensor);
}

void AFEAPIHTTPDomoticz::publishAnemometerSensorData() {
  if (enabled) {
    char value[20];
    if (_AnemometerSensor->configuration.domoticz.idx > 0) {
      sprintf(value, "0;N;%-.2f;0;?;?", 10 * _AnemometerSensor->lastSpeedMS);
      sendCustomSensorCommand(_AnemometerSensor->configuration.domoticz.idx,
                              value);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
void AFEAPIHTTPDomoticz::addClass(AFERainmeter *Sensor) {
  AFEAPI::addClass(Sensor);
}

void AFEAPIHTTPDomoticz::publishRainSensorData() {
  if (enabled) {
    char value[20];
    if (_RainmeterSensor->configuration.domoticz.idx > 0) {
      sprintf(value, "%-.2f;%-.2f", _RainmeterSensor->rainLevelLastHour * 100,
              _RainmeterSensor->current.counter);
      sendCustomSensorCommand(_RainmeterSensor->configuration.domoticz.idx,
                              value);
    }
  }
}
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIHTTPDomoticz::addClass(AFEGate *Item) { AFEAPI::addClass(Item); }

boolean AFEAPIHTTPDomoticz::publishGateState(uint8_t id) {
  return enabled ? sendSwitchCommand(_Gate[id]->configuration.domoticz.idx,
                                     _Gate[id]->get() == AFE_GATE_OPEN)
                 : false;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIHTTPDomoticz::addClass(AFEContactron *Item) {
  AFEAPI::addClass(Item);
}

boolean AFEAPIHTTPDomoticz::publishContactronState(uint8_t id) {
  return enabled
             ? sendSwitchCommand(_Contactron[id]->configuration.domoticz.idx,
                                 _Contactron[id]->get() == AFE_CONTACTRON_OPEN)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHTTPDomoticz::addClass(AFESensorDS18B20 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishDS18B20SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled && _DS18B20Sensor[id]->configuration.domoticz.idx > 0) {
    char value[9]; // Max size: -999.999
    sprintf(value, "%-.3f", _DS18B20Sensor[id]->getTemperature());
    sendCustomSensorCommand(_DS18B20Sensor[id]->configuration.domoticz.idx,
                            value);
    _ret = true;
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
void AFEAPIHTTPDomoticz::addClass(AFERegulator *Regulator) {
  AFEAPI::addClass(Regulator);
}
boolean AFEAPIHTTPDomoticz::publishRegulatorState(uint8_t id) {
  return enabled ? sendSwitchCommand(_Regulator[id]->configuration.domoticz.idx,
                                     _Regulator[id]->configuration.enabled)
                 : false;
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIHTTPDomoticz::addClass(AFEThermalProtector *Protector) {
  AFEAPI::addClass(Protector);
}
boolean AFEAPIHTTPDomoticz::publishThermalProtectorState(uint8_t id) {
  return enabled ? sendSwitchCommand(
                       _ThermalProtector[id]->configuration.domoticz.idx,
                       _ThermalProtector[id]->configuration.enable)
                 : false;
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#ifdef AFE_CONFIG_HARDWARE_DHT
void AFEAPIHTTPDomoticz::addClass(AFESensorDHT *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishDHTSensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled) {
    char value[15];
    /* Temperature */
    if (_DHTSensor[id]->configuration.domoticz.temperature.idx > 0) {
      sprintf(value, "%-.1f", _DHTSensor[id]->currentTemperature);
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.temperature.idx, value);
      _ret = true;
    }

    /* Humidity */
    if (_DHTSensor[id]->configuration.domoticz.humidity.idx > 0) {
      sprintf(value, "%d", _DHTSensor[id]->convertHumidyStatusDomoticz(
                               _DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.humidity.idx, value,
          (uint8_t)_DHTSensor[id]->currentHumidity);
      _ret = true;
    }

    /* Absolute Humidity */
    if (_DHTSensor[id]->configuration.domoticz.absoluteHumidity.idx > 0) {
      sprintf(value, "%d", _DHTSensor[id]->convertHumidyStatusDomoticz(
                               _DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.absoluteHumidity.idx, value,
          (uint8_t)_DHTSensor[id]->absoluteHumidity(
              _DHTSensor[id]->currentTemperature,
              _DHTSensor[id]->currentHumidity,
              _DHTSensor[id]->configuration.temperature.unit ==
                  AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      _ret = true;
    }

    /* DewPoint */
    if (_DHTSensor[id]->configuration.domoticz.dewPoint.idx > 0) {
      sprintf(value, "%-.2f",
              _DHTSensor[id]->dewPoint(
                  _DHTSensor[id]->currentTemperature,
                  _DHTSensor[id]->currentHumidity,
                  _DHTSensor[id]->configuration.temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.dewPoint.idx, value);
      _ret = true;
    }

    /* HeatIndex */
    if (_DHTSensor[id]->configuration.domoticz.heatIndex.idx > 0) {
      sprintf(value, "%-.2f",
              _DHTSensor[id]->heatIndex(
                  _DHTSensor[id]->currentTemperature,
                  _DHTSensor[id]->currentHumidity,
                  _DHTSensor[id]->configuration.temperature.unit ==
                      AFE_TEMPERATURE_UNIT_FAHRENHEIT));
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.heatIndex.idx, value);
      _ret = true;
    }

    /* Temperature & Humidity */
    if (_DHTSensor[id]->configuration.domoticz.temperatureHumidity.idx > 0) {
      sprintf(value, "%-.1f;%-.1f;%-d", _DHTSensor[id]->currentTemperature,
              _DHTSensor[id]->currentHumidity,
              _DHTSensor[id]->convertHumidyStatusDomoticz(
                  _DHTSensor[id]->currentHumidity));
      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.temperatureHumidity.idx,
          value);
      _ret = true;
    }

    /* Comfort */
    if (_DHTSensor[id]->configuration.domoticz.comfort.idx > 0) {
      ComfortState comfortStatus;
      char _charText[100]; // Max size of Comfort,Perception from lang.pack +
                           // %20 instead of space
      String _stringText;
      _DHTSensor[id]->comfort(comfortStatus, _DHTSensor[id]->currentTemperature,
                              _DHTSensor[id]->currentHumidity,
                              _DHTSensor[id]->configuration.temperature.unit ==
                                  AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_charText, (char *)pgm_read_dword(&(Comfort[comfortStatus])));

      // @TODO is there a better one?
      _stringText = _charText;
      _stringText.replace(" ", "%20");
      _stringText.toCharArray(_charText, sizeof(_charText));

      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.comfort.idx, _charText,
          _DHTSensor[id]->convertComfortDomoticz(comfortStatus));
      _ret = true;
    }

    /* Perception */
    if (_DHTSensor[id]->configuration.domoticz.perception.idx > 0) {
      char _charText[100]; // Max size of Comfort,Perception from lang.pack +
                           // %20 instead of space
      String _stringText;
      byte _perceptionId = _DHTSensor[id]->perception(
          _DHTSensor[id]->currentTemperature, _DHTSensor[id]->currentHumidity,
          _DHTSensor[id]->configuration.temperature.unit ==
              AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      strcpy_P(_charText,
               (char *)pgm_read_dword(&(dewPointPerception[_perceptionId])));

      // @TODO is there a better one?
      _stringText = _charText;
      _stringText.replace(" ", "%20");
      _stringText.toCharArray(_charText, sizeof(_charText));

      sendCustomSensorCommand(
          _DHTSensor[id]->configuration.domoticz.perception.idx, _charText,
          _DHTSensor[id]->convertPerceptionDomoticz(_perceptionId));
      _ret = true;
    }
  }

  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
void AFEAPIHTTPDomoticz::addClass(AFESensorBinary *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishBinarySensorState(uint8_t id) {
  return enabled
             ? sendSwitchCommand(_BinarySensor[id]->configuration.domoticz.idx,
                                 _BinarySensor[id]->get() == 0)
             : false;
}
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
void AFEAPIHTTPDomoticz::addClass(AFEMiFareCard *Sensor) {
  AFEAPI::addClass(Sensor);
}

boolean AFEAPIHTTPDomoticz::publishMiFareCardState(uint8_t id, uint8_t tagId,
                                                   uint8_t state,
                                                   const char *user) {
  boolean publishStatus = false;
  if (enabled && _MiFareCard[id]->configuration.domoticz[tagId].idx) {
    publishStatus = sendCustomSensorCommand(
        _MiFareCard[id]->configuration.domoticz[tagId].idx, user, state);
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

/* RGB LED are not supported in HTTP Domoticz API 
#ifdef AFE_CONFIG_HARDWARE_CLED
void AFEAPIHTTPDomoticz::addClass(AFECLED *CLed) { AFEAPI::addClass(CLed); }

boolean AFEAPIHTTPDomoticz::publishCLEDState(uint8_t id) {
  return enabled ? sendSwitchCommand(_CLED->configuration[id].domoticz.idx,
                                     _CLED->currentState[id].state == AFE_ON)
                 : false;
}

boolean AFEAPIHTTPDomoticz::publishCLEDEffectState(uint8_t id) {
  if (enabled) {
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