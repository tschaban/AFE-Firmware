/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#include "AFE-Sensor-DHT.h"

#ifdef AFE_CONFIG_HARDWARE_DHT

AFESensorDHT::AFESensorDHT() {}

void AFESensorDHT::begin(AFEDataAccess *_Data, uint8_t id) {
  Data = _Data;

  Data->getConfiguration(id, &configuration);

#ifdef DEBUG
  Serial << endl
         << "INFO: DHT: Initializing...GPIO: " << configuration.gpio
         << ", Type: DHT" << configuration.type;
#endif

  // dht.begin(configuration.gpio, configuration.type);

  dht.setup(configuration.gpio, DHTesp::DHT22);

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

#ifdef DEBUG
      Serial << endl << "INFO: DHT: Reading data from the sensor...";
#endif

      float _humidity = dht.getHumidity();
      float _temperature = dht.getTemperature();
      if (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT) {
        _temperature = dht.toFahrenheit(_temperature);
      }

      // applying corrections
      _temperature += configuration.temperature.correction;
      _humidity += configuration.humidity.correction;

      byte _ready = true;
      if (configuration.sendOnlyChanges) {
        if (_temperature == currentTemperature &&
            _humidity == currentHumidity) {
          _ready = false;
        }
      }

      if (_ready) {
        currentTemperature = _temperature;
        currentHumidity = _humidity;
        currentAbsoluteHumidity = dht.computeAbsoluteHumidity(
            currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        currentDewPoint = dht.computeDewPoint(
            currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        currentDewPoint = dht.computeDewPoint(
            currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        currentHeatIndex = dht.computeHeatIndex(
            currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        currentPerception = dht.computePerception(
            currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
        ComfortState comfortState;
        currentComfortRatio = dht.getComfortRatio(
            comfortState, currentTemperature, currentHumidity,
            configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: DHT: Time:   ") << (time - startTime) / 1000
             << F("sec, ") << F(", Temperature: ") << currentTemperature
             << F(", Humidity: ") << currentHumidity
             << F(", Absolute Humidity: ") << currentAbsoluteHumidity
             << F(", DewPoint: ") << currentDewPoint << F(", HeatIndex: ")
             << currentHeatIndex << F(", Perception: ") << currentPerception
             << F(", Comfort Ratio: ") << currentComfortRatio;
#endif
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
