/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-Sensor-DHT.h"

AFESensorDHT::AFESensorDHT(){};

void AFESensorDHT::begin() {
  AFEDataAccess Data;
  configuration = Data.getDHTConfiguration();
  _initialized = true;
}

float AFESensorDHT::getTemperature() {
  DHT dht(configuration.gpio, configuration.type == 1
                                  ? DHT11
                                  : configuration.type == 2 ? DHT21 : DHT22);
  dht.begin();
  return dht.readTemperature(configuration.temperature.unit == 0 ? false
                                                                 : true) +
         configuration.temperature.correction;
}

float AFESensorDHT::getHumidity() {
  DHT dht(configuration.gpio, configuration.type == 1
                                  ? DHT11
                                  : configuration.type == 2 ? DHT21 : DHT22);
  dht.begin();
  return dht.readHumidity() + configuration.humidity.correction;
}

float AFESensorDHT::getLatestTemperature() {
  temperatureInBuffer = false;
  return currentTemperature;
}

float AFESensorDHT::getLatestHumidity() {
  humidityInBuffer = false;
  return currentHumidity;
}

float AFESensorDHT::getHeatIndex() {
  DHT dht(configuration.gpio, configuration.type == 1
                                  ? DHT11
                                  : configuration.type == 2 ? DHT21 : DHT22);
  dht.begin();
  return dht.computeHeatIndex(currentTemperature, currentHumidity,
                              configuration.temperature.unit == 0 ? false
                                                                  : true);
}

boolean AFESensorDHT::temperatureSensorReady() {
  if (temperatureInBuffer) {
    temperatureInBuffer = false;
    return true;
  } else {
    return false;
  }
}

boolean AFESensorDHT::humiditySensorReady() {
  if (humidityInBuffer) {
    humidityInBuffer = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorDHT::listener() {
  if (_initialized) {
    unsigned long temperatureTime = millis();
    unsigned long humidityTime = temperatureTime;

    if (temperatureCounterStartTime == 0) {

      if (configuration.temperature.interval ==
              configuration.humidity.interval &&
          humidityCounterStartTime == 0) {
        temperatureCounterStartTime = temperatureTime - 4000;
      } else {
        temperatureCounterStartTime = temperatureTime;
      }
    }

    if (humidityCounterStartTime == 0) {
      humidityCounterStartTime = humidityTime;
    }

    if (temperatureTime - temperatureCounterStartTime >=
        configuration.temperature.interval * 1000) {
      float newTemperature = getTemperature();
      if (!configuration.sendOnlyChanges ||
          newTemperature != currentTemperature) {
        currentTemperature = newTemperature;
        temperatureInBuffer = true;
      }
      temperatureCounterStartTime = 0;
    }

    if (humidityTime - humidityCounterStartTime >=
        configuration.humidity.interval * 1000) {
      float newHumidity = getHumidity();
      if (!configuration.sendOnlyChanges || newHumidity != currentHumidity) {
        currentHumidity = newHumidity;
        humidityInBuffer = true;
      }
      humidityCounterStartTime = 0;
    }
  }
}

unsigned long AFESensorDHT::getDomoticzIDX(uint8_t type) {
  unsigned long idx;
  if (type == IDX_TYPE_TEMPERATURE) {
    idx = configuration.temperatureIdx;
  } else if (type == IDX_TYPE_HUMIDITY) {
    idx = configuration.humidityIdx;
  } else {
    idx = configuration.temperatureAndHumidityIdx;
  }
  return idx;
}

boolean AFESensorDHT::publishHeatIndex() {
  return configuration.publishHeatIndex;
}
