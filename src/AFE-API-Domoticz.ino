/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-API-Domoticz.h"

AFEDomoticz::AFEDomoticz() {}

void AFEDomoticz::begin() {
  configuration = Data.getDomoticzConfiguration();
  char _user[45] = {0}; // base64 conversion takes ceil(n/3)*4 size of mem
  char _pass[45] = {0};
  char authorization[20 + sizeof(_user) + sizeof(_pass) + 1] = {0};

  if (configuration.user && configuration.password) {
    rbase64.encode(configuration.user);
    sprintf(_user, rbase64.result());
    rbase64.encode(configuration.password);
    sprintf(_pass, rbase64.result());
    sprintf(authorization, "&username=%s&password=%s", _user, _pass);
  }

  sprintf(serverURL, "%s%s:%d/json.htm?type=command%s",
          configuration.protocol == 0 ? "http://" : "https://",
          configuration.host, configuration.port, authorization);

  initialized = true;
}

void AFEDomoticz::disconnect() { initialized = false; }

void AFEDomoticz::sendSwitchCommand(unsigned int idx, const char *value) {
  if (initialized) {
    String call = getApiCall("switchlight", idx);
    call += "&switchcmd=";
    call += value;
    callURL(call);
  }
}

const String AFEDomoticz::getApiCall(const char *param, unsigned int idx) {
  char url[sizeof(serverURL) + 18 + strlen(param)];
  sprintf(url, "%s&param=%s&idx=%u", serverURL, param, idx);
  return url;
}

void AFEDomoticz::callURL(const String url) {

#ifdef DEBUG
  Serial << endl << endl << "--------------- Domoticz ---------------";
  Serial << endl << url;
  Serial << endl << "----------------------------------------" << endl;
#endif

  http.begin(url);
  http.GET();

  //#ifdef DEBUG
  //  String payload = http.getString();
  //  Serial << endl << payload;
  //#endif

  http.end();
}

#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)
void AFEDomoticz::sendTemperatureCommand(unsigned int idx, float value) {
  if (initialized) {
    char _temperatureChar[7];
    dtostrf(value, 4, 2, _temperatureChar);
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += _temperatureChar;
    callURL(call);
  }
}
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
void AFEDomoticz::sendHumidityCommand(unsigned int idx, float value) {
  if (initialized) {
    char _humidityChar[7];
    dtostrf(value, 5, 2, _humidityChar);
    String call = getApiCall("udevice", idx);
    call += "&nvalue=";
    call += _humidityChar;
    call += "&svalue=";
    call += getHumidityState(value);
    callURL(call);
  }
}

void AFEDomoticz::sendTemperatureAndHumidityCommand(unsigned int idx,
                                                    float temperatureValue,
                                                    float humidityValue) {
  if (initialized) {
    char _floatToChar[7];
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    dtostrf(temperatureValue, 4, 2, _floatToChar);
    call += _floatToChar;
    call += ";";
    dtostrf(humidityValue, 5, 2, _floatToChar);
    call += _floatToChar;
    call += ";";
    call += getHumidityState(humidityValue);
    callURL(call);
  }
}
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG) || defined(T6_CONFIG)
uint8_t AFEDomoticz::getHumidityState(float value) {
  if (value < 40) {
    return HUMIDITY_DRY;
  } else if (value >= 40 && value <= 60) {
    return HUMIDITY_COMFORTABLE;
  } else if (value > 60) {
    return HUMIDITY_WET;
  } else {
    return HUMIDITY_NORMAL;
  }
}
#endif

#if defined(T6_CONFIG)
void AFEDomoticz::sendTemperatureAndHumidityAndPressureCommand(
    unsigned int idx, float temperatureValue, float humidityValue,
    float pressureValue) {
  if (initialized) {
    char _floatToChar[8];
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    dtostrf(temperatureValue, 4, 2, _floatToChar);
    call += _floatToChar;
    call += ";";
    dtostrf(humidityValue, 5, 2, _floatToChar);
    call += _floatToChar;
    call += ";";
    call += getHumidityState(humidityValue);
    call += ";";
    dtostrf(pressureValue, 6, 2, _floatToChar);
    call += _floatToChar;
    call += ";0"; // Hardcoded 0 means no forecast data
    callURL(call);
  }
}
#endif

#if defined(T3_CONFIG)
void AFEDomoticz::sendPirCommand(unsigned int idx, const char *value) {
  sendSwitchCommand(idx, value);
}
#endif

#if defined(T5_CONFIG) /* @TODO it could be removed and replaced by switch */
void AFEDomoticz::sendGateCommand(unsigned int idx, const char *value) {
  sendSwitchCommand(idx, value);
}

void AFEDomoticz::sendContactronCommand(unsigned int idx, const char *value) {
  sendSwitchCommand(idx, value);
}
#endif

#if defined(T6_CONFIG)
void AFEDomoticz::sendCustomSensorCommand(unsigned int idx, uint16_t value) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += value;
    callURL(call);
  }
}
#endif
