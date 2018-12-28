/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BMEx80.h"

AFESensorBMEx80::AFESensorBMEx80(){};

void AFESensorBMEx80::begin(uint8_t type) {
  sensorType = type;

#if defined(DEBUG)
  Serial << endl << endl << "-------- BMEx80: Initializing --------";
#endif

  _initialized =
      (sensorType == TYPE_BME680_SENSOR
           ? s6.begin()
           : (sensorType == TYPE_BME280_SENSOR ? s2.begin() : false));

#if defined(DEBUG)
  Serial << endl << "--------------------------------------" << endl;
#endif
}

BMEx80_DATA AFESensorBMEx80::get() {
  ready = false;
  return sensorData;
}

boolean AFESensorBMEx80::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBMEx80::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= (sensorType == TYPE_BME680_SENSOR
                                 ? s6.configuration.interval
                                 : s2.configuration.interval) *
                                1000) {

#if defined(DEBUG)
      Serial << endl
             << endl
             << "--------"
             << " Reading sensor data "
             << "--------";
#endif

      boolean readStatus =
          (sensorType == TYPE_BME680_SENSOR ? s6.read() : s2.read());

      if (readStatus) {
        sensorData = sensorType == TYPE_BME680_SENSOR ? s6.data : s2.data;

#if defined(DEBUG)
        Serial << endl
               << "Temperature = " << sensorData.temperature << endl
               << "Humidity = " << sensorData.humidity << endl
               << "Pressure = " << sensorData.pressure;
        if (sensorType == TYPE_BME680_SENSOR) {
          Serial << endl << "Gas level = " << sensorData.gasResistance;
        }
      } else {
        Serial << "No data found";
#endif

        ready = true;
      }
      startTime = 0;
#if defined(DEBUG)
      Serial << endl << "------------------------------------" << endl;
#endif
    }
  }
}

void AFESensorBMEx80::getDomoticzIDX(BMEx80_DOMOTICZ *idx) {
  *idx = sensorType == TYPE_BME680_SENSOR ? s6.configuration.idx
                                          : s2.configuration.idx;
}
