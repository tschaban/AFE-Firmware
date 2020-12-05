/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP-Domoticz.h"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

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

boolean AFEAPIHTTPDomoticz::sendSwitchCommand(unsigned int idx,
                                              const char *value) {
  boolean _return = false;
  if (enabled) {
    String call = getApiCall("switchlight", idx);
    call += "&switchcmd=";
    call += value;
    _return = callURL(call);
  }
  return _return;
}

boolean AFEAPIHTTPDomoticz::sendCustomSensorCommand(unsigned int idx,
                                                    const char *value,
                                                    uint8_t nvalue) {
  boolean _return = false;
  if (enabled) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=";
    call += nvalue;
    call += "&svalue=";
    call += value;
    _return = callURL(call);
  }
  return _return;
}

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHTTPDomoticz::addClass(AFERelay *Relay) { AFEAPI::addClass(Relay); }
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIHTTPDomoticz::publishRelayState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Relay[id]->configuration.domoticz.idx > 0) {
    publishStatus =
        sendSwitchCommand(_Relay[id]->configuration.domoticz.idx,
                          _Relay[id]->get() == AFE_RELAY_ON ? "On" : "Off");
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHTTPDomoticz::addClass(AFESwitch *Switch) {
  AFEAPI::addClass(Switch);
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIHTTPDomoticz::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Switch[id]->configuration.domoticz.idx) {
    publishStatus =
        sendSwitchCommand(_Switch[id]->configuration.domoticz.idx,
                          _Switch[id]->getPhisicalState() ? "Off" : "On");
  }
  return publishStatus;
}
#endif // AFE_CONFIG_HARDWARE_SWITCH

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIHTTPDomoticz::addClass(AFEAnalogInput *Analog) {
  AFEAPI::addClass(Analog);
}
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
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
#endif // AFE_CONFIG_HARDWARE_ADC_VCC

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
                _BMx80Sensor[id]->getDomoticzHumidityState(
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
                _BMx80Sensor[id]->getDomoticzHumidityState(
                    _BMx80Sensor[id]->data.humidity.value),
                _BMx80Sensor[id]->data.pressure.value);
        sendCustomSensorCommand(
            _BMx80Sensor[id]
                ->configuration.domoticz.temperatureHumidityPressure.idx,
            value);
      }

      if (_BMx80Sensor[id]->configuration.domoticz.humidity.idx > 0) {
        sprintf(value, "%d", _BMx80Sensor[id]->getDomoticzHumidityState(
                                 _BMx80Sensor[id]->data.humidity.value));
        sendCustomSensorCommand(
            _BMx80Sensor[id]->configuration.domoticz.humidity.idx, value,
            (uint8_t)_BMx80Sensor[id]->data.humidity.value);
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
    char value[20]; // @TODO CHeck the max size
    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx > 0) {
      sprintf(value, "%-.d", _HPMA115S0Sensor[id]->data.pm25);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.pm25.idx, value);
    }
    if (_HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx > 0) {
      sprintf(value, "%-.d", _HPMA115S0Sensor[id]->data.pm10);
      sendCustomSensorCommand(
          _HPMA115S0Sensor[id]->configuration.domoticz.pm10.idx, value);
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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
void AFEAPIHTTPDomoticz::addClass(AFESensorAnemometer *Sensor) {
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
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
void AFEAPIHTTPDomoticz::addClass(AFESensorRainmeter *Sensor) {
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
#endif // AFE_CONFIG_HARDWARE_RAINMETER_SENSOR

#ifdef AFE_CONFIG_HARDWARE_GATE
void AFEAPIHTTPDomoticz::addClass(AFEGate *Item) { AFEAPI::addClass(Item); }

boolean AFEAPIHTTPDomoticz::publishGateState(uint8_t id) {
  boolean _ret = false;
  if (enabled && _Gate[id]->configuration.domoticz.idx > 0) {
    _ret =
        sendSwitchCommand(_Gate[id]->configuration.domoticz.idx,
                          _Gate[id]->get() == AFE_GATE_CLOSED ? "Off" : "On");
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
void AFEAPIHTTPDomoticz::addClass(AFEContactron *Item) {
  AFEAPI::addClass(Item);
}

boolean AFEAPIHTTPDomoticz::publishContactronState(uint8_t id) {
  boolean _ret = false;
  if (enabled && _Contactron[id]->configuration.domoticz.idx > 0) {
    _ret = sendSwitchCommand(
        _Contactron[id]->configuration.domoticz.idx,
        _Contactron[id]->get() == AFE_CONTACTRON_OPEN ? "On" : "Off");
  }
  return _ret;
}
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_DS18B20
void AFEAPIHTTPDomoticz::addClass(AFESensorDS18B20 *Sensor) {
  AFEAPI::addClass(Sensor);
}
boolean AFEAPIHTTPDomoticz::publishDS18B20SensorData(uint8_t id) {
  boolean _ret = false;
  if (enabled && _DS18B20Sensor[id]->configuration.domoticz.idx > 0) {
    char value[9]; //Max size: -999.999
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
  boolean publishStatus = false;
  if (enabled && _Regulator[id]->configuration.domoticz.idx > 0) {
    publishStatus =
        sendSwitchCommand(_Regulator[id]->configuration.domoticz.idx,
                          _Regulator[id]->configuration.enabled ? "On" : "Off");
  }
  return publishStatus;
}
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
void AFEAPIHTTPDomoticz::addClass(AFEThermalProtector *Protector) {
  AFEAPI::addClass(Protector);
}
boolean AFEAPIHTTPDomoticz::publishThermalProtectorState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _ThermalProtector[id]->configuration.domoticz.idx > 0) {
    publishStatus = sendSwitchCommand(
        _ThermalProtector[id]->configuration.domoticz.idx,
        _ThermalProtector[id]->configuration.enabled ? "On" : "Off");
  }
  return publishStatus;
}
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED