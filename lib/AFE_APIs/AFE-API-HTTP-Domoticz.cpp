/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-API-HTTP-Domoticz.h"

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED

AFEAPIHTTPDomoticz::AFEAPIHTTPDomoticz(){};

void AFEAPIHTTPDomoticz::begin(AFEDataAccess *Data, AFEDevice *Device) {
  _Data = Data;
  _Device = Device;

  configuration = _Data->getDomoticzConfiguration();

  char _user[45] = {0}; // base64 conversion takes ceil(n/3)*4 size of mem
  char _pass[45] = {0};
  char authorization[20 + sizeof(_user) + sizeof(_pass) + 1] = {0};

  if (configuration.user[0] != '\0' && configuration.password[0] != '\0') {
    rbase64.encode(configuration.user);
    sprintf(_user, rbase64.result());
    rbase64.encode(configuration.password);
    sprintf(_pass, rbase64.result());
    sprintf(authorization, "&username=%s&password=%s", _user, _pass);
  }

  sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port, authorization);

  enabled = true;
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
// Led.on();
#endif

#ifdef DEBUG
  Serial << endl << endl << "--------------- Domoticz ---------------";
  Serial << endl << url;
  Serial << endl << "----------------------------------------" << endl;
#endif
  http.begin(client, url);
  _return = http.GET() == 200 ? true : false;
  http.end();
  delay(10);
#ifdef AFE_CONFIG_HARDWARE_LED
//  Led.off();
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
                                                 const char *value) {
  boolean _return = false;
  if (enabled) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += value;
    _return = callURL(call);
  }
  return _return;
}

#ifdef AFE_CONFIG_API_PROCESS_REQUESTS
boolean AFEAPIHTTPDomoticz::idxForProcessing(uint32_t idx) {
  boolean _ret = true;
  if (idx == bypassProcessing.idx) {
    bypassProcessing.idx = 0;
    _ret = false;
  }
  return _ret;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
void AFEAPIHTTPDomoticz::addClass(AFERelay *Relay) {
#ifdef DEBUG
  Serial << endl << endl << "INFO: adding Relay's IDXs to Cache";
#endif

  // Cache idx
  uint8_t index = 0;
  for (uint8_t i = 0; i < _Device->configuration.noOfRelays; i++) {
    _Relay[i] = Relay + i;
#ifdef DEBUG
    Serial << endl << " - Relay[" << i << "]: " << _Relay[i]->getName() << " ";
#endif
    if (_Relay[i]->configuration.domoticz.idx > 0) {
      idxCache[index].domoticz.idx = _Relay[i]->configuration.domoticz.idx;
      idxCache[index].id = i;
      idxCache[index].type = AFE_DOMOTICZ_DEVICE_RELAY;
#ifdef DEBUG
      Serial << "added IDX: " << idxCache[index].domoticz.idx;
#endif
      index++;
    }
#ifdef DEBUG
    else {
      Serial << endl << "IDX not set";
    }
#endif
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
boolean AFEAPIHTTPDomoticz::publishRelayState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Relay[id]->configuration.domoticz.idx > 0 &&
      bypassProcessing.idx != _Relay[id]->configuration.domoticz.idx) {
    if (_Relay[id]->configuration.domoticz.idx > 0) {
      publishStatus =
          sendSwitchCommand(_Relay[id]->configuration.domoticz.idx,
                            _Relay[id]->get() == AFE_RELAY_ON ? "On" : "Off");
      bypassProcessing.idx = _Relay[id]->configuration.domoticz.idx;
    }
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
void AFEAPIHTTPDomoticz::addClass(AFESwitch *Switch) {
  for (uint8_t i = 0; i < _Device->configuration.noOfSwitches; i++) {
    _Switch[i] = Switch + i;
  }
}
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
boolean AFEAPIHTTPDomoticz::publishSwitchState(uint8_t id) {
  boolean publishStatus = false;
  if (enabled && _Switch[id]->configuration.domoticz.idx) {
    publishStatus = sendSwitchCommand(_Switch[id]->configuration.domoticz.idx,
                      _Switch[id]->getPhisicalState() ? "Off" : "On");
  }
  return publishStatus;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIHTTPDomoticz::addClass(AFEAnalogInput *Analog) { _Analog = Analog; }
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
void AFEAPIHTTPDomoticz::publishADCValues() {
  if (enabled) {
    char value[20];
    if (_Analog->configuration.domoticz.percent > 0) {
      sprintf(value, "%-.2f", _Analog->data.percent);
      sendCustomSensorCommand(_Analog->configuration.domoticz.percent, value);
    }
    if (_Analog->configuration.domoticz.voltage > 0) {
      sprintf(value, "%-.4f", _Analog->data.voltage);
      sendCustomSensorCommand(_Analog->configuration.domoticz.voltage, value);
    }
    if (_Analog->configuration.domoticz.voltageCalculated > 0) {
      sprintf(value, "%-.4f", _Analog->data.voltageCalculated);
      sendCustomSensorCommand(_Analog->configuration.domoticz.voltageCalculated,
                              value);
    }
    if (_Analog->configuration.domoticz.raw > 0) {
      sprintf(value, "%-d", _Analog->data.raw);
      sendCustomSensorCommand(_Analog->configuration.domoticz.raw, value);
    }
  }
}
#endif

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED