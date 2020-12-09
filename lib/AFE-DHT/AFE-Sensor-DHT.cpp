/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#include "AFE-Sensor-DHT.h"

#ifdef AFE_CONFIG_HARDWARE_DHT

AFESensorDHT::AFESensorDHT() {}

void AFESensorDHT::begin(AFEDataAccess *_Data, uint8_t id) {
  Data = _Data;

  Data->getConfiguration(id, &configuration);

#ifdef DEBUG
  Serial << endl << "INFO: DHT: Initializing...";
#endif

  dht.begin(configuration.gpio, configuration.type);

#ifdef DEBUG
  Serial << "DONE";
#endif

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Defining get and state MQTT Topics */
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
  }

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = AFE_EMPTY_STRING;
  }

#ifdef DEBUG
  Serial << endl
         << "INFO: DHT: MQTT Topic [state]: " << mqttStateTopic
         << ", [cmd]:" << mqttCommandTopic;
#endif

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  _initialized = true;
}

float AFESensorDHT::getTemperature() { return currentTemperature; }

float AFESensorDHT::getHumidity() { return currentHumidity; }

float AFESensorDHT::getDewPoint() { return currentDewPoint; }

float AFESensorDHT::getHeatIndex() { return currentHeatIndex; }

boolean AFESensorDHT::listener() {

  boolean _ready = false;

  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

      boolean readResult = dht.acquireAndWait(3000);

      if (readResult == DHTLIB_OK) {
        float _temperature =
            (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                 ? dht.getCelsius()
                 : dht.getFahrenheit()) +
            configuration.temperature.correction;

        float _humidity = dht.getHumidity() + configuration.humidity.correction;
        float _dewPoint = dht.getDewPoint();
        float _heatIndex =
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                ? dht.getHeatIndexCelsius()
                : dht.getHeatIndexFahrenheit();

        _ready = true;
        if (configuration.sendOnlyChanges) {
          if (_temperature == currentTemperature &&
              _humidity == currentHumidity && _dewPoint == currentDewPoint &&
              _heatIndex == currentHeatIndex) {
            _ready = false;
          }
        }

        if (_ready) {
          currentTemperature = _temperature;
          currentHumidity = _humidity;
          currentDewPoint = _dewPoint;
          currentHeatIndex = _heatIndex;
        }

#ifdef DEBUG
        Serial << endl
               << F("INFO: DHT: Time:   ") << (time - startTime) / 1000
               << F("sec, ") << F(", Temp: ") << currentTemperature
               << F(", Humi: ") << currentHumidity << F(", DewPoint: ")
               << currentDewPoint << F(", HeatIndex: ") << currentHeatIndex;
      } else {
        Serial << endl
               << F("ERROR: DHT: Problem with reading data from the sensor. "
                    "ErrorID: ")
               << readResult;
#endif
      }
      startTime = 0;
    }
  }

  return _ready;
}

void AFESensorDHT::getJSON(char *json) {
  sprintf(json, "{\"temperature\":{\"value\":%.3f,\"unit\":\"%s\"}}",
          currentTemperature,
          configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                         : "F");
}

#endif // AFE_CONFIG_HARDWARE_DHT
