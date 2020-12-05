/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_DHT

#include "AFE-Sensor-DHT.h"

AFESensorDHT::AFESensorDHT() {
  AFEDataAccess Data;
  configuration = Data.getSensorConfiguration();
  _initialized = true;
};

void AFESensorDHT::dht_wrapper() { dht.isrCallback(); }

float AFESensorDHT::getTemperature() { return currentTemperature; }

float AFESensorDHT::getHumidity() { return currentHumidity; }

float AFESensorDHT::getDewPoint() { return currentDewPoint; }

float AFESensorDHT::getHeatIndex() { return currentHeatIndex; }

boolean AFESensorDHT::isReady() {
  if (dataInMemory) {
    dataInMemory = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorDHT::listener() {

  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

      readResult = dht.acquireAndWait(1000);
      if (readResult == 0) {
        if (configuration.temperature.unit == UNIT_CELCIUS) {
          if (currentTemperature != dht.getCelsius()) {
            currentTemperature =
                dht.getCelsius() + configuration.temperature.correction;
            dataInMemory = true;
          }
        } else {
          if (currentTemperature != dht.getFahrenheit()) {
            currentTemperature =
                dht.getFahrenheit() + configuration.temperature.correction;
            dataInMemory = true;
          }
        }

        if (currentHumidity != dht.getHumidity()) {
          currentHumidity =
              dht.getHumidity() + configuration.humidity.correction;
          dataInMemory = true;
        }

        if (!dataInMemory && !configuration.sendOnlyChanges) {
          dataInMemory = true;
        }

        currentDewPoint = dht.getDewPoint();
        currentHeatIndex = dht.getHeatIndex(configuration.temperature.unit);

#ifdef DEBUG
        Serial << endl
               << F("DH: Time =  ") << (time - startTime) / 1000 << F("sec, "
               << F(", readResult=") << readResult << F(", T=") << currentTemperature
               << F(", H=") << currentHumidity << F(", D=") << currentDewPoint
               << F(", HI=") << currentHeatIndex;
      } else {
        Serial << endl << F("DH: ReadResult = ") << readResult;
#endif
      }
      startTime = 0;
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

boolean AFESensorDHT::publishDewPoint() {
  return configuration.publishDewPoint;
}

#endif
