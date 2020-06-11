/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-DS18B20.h"

AFESensorDS18B20::AFESensorDS18B20(){};

void AFESensorDS18B20::begin(uint8_t id) {
  AFEDataAccess Data;
  Data.getConfiguration(id,&configuration);
  WireBUS.begin(configuration.gpio);
  Sensor.setOneWire(&WireBUS);
  _initialized = false;
}

float AFESensorDS18B20::getTemperature() {
  float temperature = -127;
  if (_initialized) {
    // OneWire wireProtocol(configuration.gpio);
    // DallasTemperature sensor(&wireProtocol);
    Sensor.begin();

    do {
      Sensor.requestTemperatures();
      temperature = configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                        ? Sensor.getTempCByIndex(0)
                        : Sensor.getTempFByIndex(0);
    } while (temperature == 85.0 || temperature == (-127.0));
  }
  return temperature + configuration.correction;
}

float AFESensorDS18B20::getLatestTemperature() {
  ready = false;
  return currentTemperature;
}

boolean AFESensorDS18B20::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorDS18B20::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {
      float newTemperature = getTemperature();
      if (!configuration.sendOnlyChanges ||
          newTemperature != currentTemperature) {
        currentTemperature = newTemperature;
        ready = true;
        startTime = 0;
      }
    }
  }
}

void AFESensorDS18B20::scan(uint8_t gpio) {
#ifdef DEBUG
  Serial << endl << "INFO: Looking for DS18B20 sensors on GPIO: " << gpio;
#endif

  WireBUS.begin(gpio);
  Serial << endl << "INFO: Wire Bus initialized";
  Sensor.setOneWire(&WireBUS);
  Serial << endl << "INFO: Bus set for DS18B20";
  // Sensor.setPullupPin(gpio);
  Sensor.begin();
  Serial << endl << "INFO: DS18B20 started";
  numberOfDevicesOnBus = Sensor.getDS18Count();

#ifdef DEBUG
  Serial << endl
         << "INFO: Found: " << numberOfDevicesOnBus << " DS18B20 sensors";

  if (numberOfDevicesOnBus > 0) {
    for (uint8_t i = 0; i < numberOfDevicesOnBus; i++) {
      Sensor.getAddress(addressesOfScannedItems[i], i);
      Serial << endl << " : ";
      for (uint8_t adr = 0; adr < 8; adr++) {
        Serial << "0x";
        if (addressesOfScannedItems[i][adr] < 0x10)
          Serial << "0";
        Serial << _HEX(addressesOfScannedItems[i][adr]);
        if (i < 7)
          Serial << ", ";
      }

      //      Serial << endl
      //           << " - ID: " << i << " 0x" <<
      //           _HEX(addressesOfScannedItems[i]);
    }
  }
#endif
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
unsigned long AFESensorDS18B20::getDomoticzIDX() {
  return configuration.domoticz.idx;
}
#endif