/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMx80.h"

AFESensorBMx80::AFESensorBMx80(){};

void AFESensorBMx80::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getBMx80SensorConfiguration(id);

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }

#if defined(DEBUG)
  Serial << endl << endl << "-------- BMx80: Initializing --------";
#endif

  _initialized = configuration.type == TYPE_BME680_SENSOR
                     ? s6.begin(&configuration)
                     : sensorType == TYPE_BME280_SENSOR
                           ? s2.begin(&configuration)
                           : s1.begin(&configuration);

#ifdef DEBUG
  Serial << endl
         << "Device: " << (_initialized ? "Found" : "Not found: check wiring");
  Serial << endl << "--------------------------------------" << endl;
#endif
}

BMx80_DATA AFESensorBMx80::get() {
  ready = false;
  return sensorData;
}

boolean AFESensorBMx80::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBMx80::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

#if defined(DEBUG)
      Serial << endl
             << endl
             << "--------"
             << " Reading sensor data "
             << "--------";
#endif

      boolean readStatus =
          sensorType == TYPE_BME680_SENSOR
              ? s6.read()
              : sensorType == TYPE_BME280_SENSOR ? s2.read() : s1.read();

      if (readStatus) {
        sensorData = sensorType == TYPE_BME680_SENSOR
                         ? s6.data
                         : sensorType == TYPE_BME280_SENSOR ? s2.data : s1.data;

        ready = true;

#if defined(DEBUG)
        Serial << endl
               << "Temperature = " << sensorData.temperature << endl
               << "Pressure = " << sensorData.pressure;
        if (sensorType != TYPE_BMP180_SENSOR) {
          Serial << endl << "Humidity = " << sensorData.humidity;
        }
        if (sensorType == TYPE_BME680_SENSOR) {
          Serial << endl << "Gas level = " << sensorData.gasResistance;
        }

      } else {
        Serial << endl << "No data found";
#endif
      }
      startTime = 0;
#if defined(DEBUG)
      Serial << endl << "------------------------------------" << endl;
#endif
    }
  }
}
