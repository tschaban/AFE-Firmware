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

  http.begin(client, url);
  http.GET();

  //#ifdef DEBUG
  //  String payload = http.getString();
  //  Serial << endl << payload;
  //#endif

  http.end();
}

#ifdef CONFIG_TEMPERATURE
void AFEDomoticz::sendTemperatureCommand(unsigned int idx, float value) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += value;
    callURL(call);
  }
}
#endif

#ifdef CONFIG_HARDWARE_BH1750
void AFEDomoticz::sendSValueCommand(unsigned int idx, float value) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&svalue=";
    call += value;
    callURL(call);
  }
}
#endif

#ifdef CONFIG_HUMIDITY
void AFEDomoticz::sendHumidityCommand(unsigned int idx, float value) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=";
    call += value;
    call += "&svalue=";
    call += getHumidityState(value);
    callURL(call);
  }
}
#endif

#if defined(CONFIG_TEMPERATURE) && defined(CONFIG_HUMIDITY)
void AFEDomoticz::sendTemperatureAndHumidityCommand(unsigned int idx,
                                                    float temperatureValue,
                                                    float humidityValue) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += temperatureValue;
    call += ";";
    call += humidityValue;
    call += ";";
    call += getHumidityState(humidityValue);
    callURL(call);
  }
}
#endif

#ifdef CONFIG_HUMIDITY
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

#if (defined(CONFIG_TEMPERATURE) && defined(CONFIG_HUMIDITY))
#if defined(CONFIG_PRESSURE)
void AFEDomoticz::sendTemperatureAndHumidityAndPressureCommand(
    unsigned int idx, float temperatureValue, float humidityValue,
    float pressureValue) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += temperatureValue;
    call += ";";
    call += humidityValue;
    call += ";";
    call += getHumidityState(humidityValue);
    call += ";";
    call += pressureValue;
    call += ";0"; // Hardcoded 0 means no forecast data
    callURL(call);
  }
}
#endif
#endif

#ifdef CONFIG_PRESSURE
void AFEDomoticz::sendPressureCommand(unsigned int idx, float pressureValue) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += pressureValue;
    call += ";0";
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

#ifdef CONFIG_DOMOTICZ_CUSTOME_SENSOR
void AFEDomoticz::sendCustomSensorCommand(unsigned int idx, double value,
                                          uint8_t precision) {
  if (initialized) {
    String call = getApiCall("udevice", idx);
    call += "&nvalue=0&svalue=";
    call += String(value, precision);
    callURL(call);
  }
}
void AFEDomoticz::sendCustomSensorCommand(unsigned int idx, float value,
                                          uint8_t precision) {
  sendCustomSensorCommand(idx, (double)value, precision);
}

void AFEDomoticz::sendCustomSensorCommand(unsigned int idx, uint16_t value) {
  sendCustomSensorCommand(idx, (double)value, 0);
}
#endif
