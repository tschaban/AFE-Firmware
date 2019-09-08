/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMEX80.h"

AFESensorBMEX80::AFESensorBMEX80(){};

void AFESensorBMEX80::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getBMEX80SensorConfiguration(id);

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }

#if defined(DEBUG)
  Serial << endl << endl << "-------- BMEX80: Initializing --------";
#endif

  switch (configuration.type) {
  case AFE_BME680_SENSOR:
    _initialized = s6.begin(&configuration);
    break;
  case AFE_BME280_SENSOR:
    _initialized = s2.begin(&configuration);
    break;
  case AFE_BMP180_SENSOR:
    _initialized = s1.begin(&configuration);
  default:
    _initialized = false;
    break;
  }

#ifdef DEBUG
  Serial << endl
         << "Device: " << (_initialized ? "Found" : "Not found: check wiring");
  Serial << endl << "--------------------------------------" << endl;
#endif
}

BMEX80_DATA AFESensorBMEX80::get() {
  ready = false;
  return sensorData;
}

boolean AFESensorBMEX80::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBMEX80::listener() {
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
          configuration.type == AFE_BME680_SENSOR
              ? s6.read()
              : configuration.type == AFE_BME280_SENSOR ? s2.read() : s1.read();

      if (readStatus) {
        sensorData = configuration.type == AFE_BME680_SENSOR
                         ? s6.data
                         : configuration.type == AFE_BME280_SENSOR ? s2.data : s1.data;

        ready = true;

#if defined(DEBUG)
        Serial << endl
               << "Temperature = " << sensorData.temperature << endl
               << "Pressure = " << sensorData.pressure;
        if (configuration.type != AFE_BMP180_SENSOR) {
          Serial << endl << "Humidity = " << sensorData.humidity;
        }
        if (configuration.type == AFE_BME680_SENSOR) {
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
