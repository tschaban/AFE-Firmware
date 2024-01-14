/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMEX80.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#ifdef DEBUG
AFESensorBMEX80::AFESensorBMEX80(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort,
                                 AFEDebugger *_Debugger) {
  Data = _Data;
  WirePort = _WirePort;
  Debugger = _Debugger;
};
#else
AFESensorBMEX80::AFESensorBMEX80(AFEDataAccess *_Data,
                                 AFEWireContainer *_WirePort) {
  Data = _Data;
  WirePort = _WirePort;
};

#endif

void AFESensorBMEX80::begin(uint8_t id) {

  Data->getConfiguration(id, configuration);

#if defined(DEBUG)
  Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
  Debugger->printBulletPoint(F("BMEX80: Initializing"));
#endif

#ifdef AFE_ESP32
  if (configuration->wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif

    if (configuration->type == AFE_BME680_SENSOR) {
#ifdef AFE_ESP32
      _initialized = s6->begin(
          configuration->wirePortId == 0 ? WirePort->Port0 : WirePort->Port1);
#else
    _initialized = s6->begin(WirePort->Port0);
#endif

    } else if (configuration->type == AFE_BME280_SENSOR ||
               configuration->type == AFE_BMP280_SENSOR) {
#ifdef AFE_ESP32
      _initialized = s2->begin(
          configuration->wirePortId == 0 ? WirePort->Port0 : WirePort->Port1);
#else
    _initialized = s2->begin(WirePort->Port0);
#endif

#ifndef AFE_ESP32
    } else if (configuration->type == AFE_BMP180_SENSOR) {
      _initialized = s1->begin(WirePort->Port0);
#endif // AFE_ESP32
    } else {
      _initialized = false;
    }

#ifdef AFE_ESP32
  }
#endif

#ifdef DEBUG
  Debugger->printBulletPoint(F("Sensor: "));
  Debugger->printValue(_initialized ? F("Found")
                                    : F("NOT found: check wiring"));
  Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
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
    boolean readStatus = false;

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }
    if (time - startTime >= configuration->interval * 1000) {

#if defined(DEBUG)
      Debugger->printHeader(2, 1, 30, AFE_DEBUG_HEADER_TYPE_DASH);
      Debugger->printBulletPoint(F("BMEX80: Reading"));
#endif
      if (configuration->type == AFE_BME680_SENSOR) {
        /**
         * @brief Reading Bosch BME680 Sensor
         *
         */
        readStatus = s6->read();
      } else if (configuration->type != AFE_BME680_SENSOR) {
/**
 * @brief Reading Bosch BME280/BMP280 or BMP180 (for ESP82xx only) Sensor
 *
 */

#ifdef AFE_ESP32
        readStatus = s2->read();
#else  // ESP8266
        readStatus =
            configuration->type == AFE_BME280_SENSOR || AFE_BMP280_SENSOR
                ? s2->read()
                : s1->read();
#endif // ESP32/ESP8266
      }
      if (readStatus) {
        /*&
                if (configuration->type == AFE_BME680_SENSOR) {
                  data = s6->data;
                } else {
        #ifdef AFE_ESP32
                  data = s2->data;
        #else  // ESP8266
                  data = configuration->type == AFE_BME280_SENSOR ||
        AFE_BMP280_SENSOR
                             ? s2->data
                             : s1->data;
        #endif // ESP32/ESP8266
                }
                */
        applyCorrections();
        ready = true;

#ifdef DEBUG
        Debugger->printBulletPoint(F("Temperature: "));
        Debugger->printValue(data->temperature.value);
        Debugger->printBulletPoint(F("Pressure (abs): "));
        Debugger->printValue(data->pressure.value);
        Debugger->printBulletPoint(F("Pressure (rel): "));
        Debugger->printValue(data->relativePressure.value);

        if (configuration->type != AFE_BMP180_SENSOR &&
            configuration->type != AFE_BMP280_SENSOR) {
          Debugger->printBulletPoint(F("Humidity: "));
          Debugger->printValue(data->humidity.value);
          Debugger->printBulletPoint(F("Dew Point: "));
          Debugger->printValue(data->dewPoint.value);
        }
        if (configuration->type == AFE_BME680_SENSOR) {

          Debugger->printBulletPoint(F("Gas Resistance: "));
          Debugger->printValue(data->gasResistance.value);

          Debugger->printBulletPoint(F("IAQ: "));
          Debugger->printValue(data->iaq.value);
          Debugger->printBulletPoint(F("IAQ Accuracy: "));
          Debugger->printValue(data->iaq.accuracy);

          Debugger->printBulletPoint(F("Static IAQ: "));
          Debugger->printValue(data->staticIaq.value);
          Debugger->printBulletPoint(F("Static IAQ Accuracy: "));
          Debugger->printValue(data->staticIaq.accuracy);

          Debugger->printBulletPoint(F("BVOC: "));
          Debugger->printValue(data->breathVocEquivalent.value);
          Debugger->printBulletPoint(F("BVOC Accuracy: "));
          Debugger->printValue(data->breathVocEquivalent.accuracy);

          Debugger->printBulletPoint(F("CO2: "));
          Debugger->printValue(data->co2Equivalent.value);
          Debugger->printBulletPoint(F("CO2 Accuracy: "));
          Debugger->printValue(data->co2Equivalent.accuracy);
        }
      } else {
        Debugger->printBulletPoint(F("Warning: No data"));

#endif // DEBUG
      }
      startTime = 0;
#ifdef DEBUG
      Debugger->printHeader(1, 2, 30, AFE_DEBUG_HEADER_TYPE_DASH);
#endif
    }
  }
}

void AFESensorBMEX80::getJSON(char *json) {

  StaticJsonBuffer<AFE_CONFIG_API_JSON_BMEX80_DATA_REAL_LENGTH> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  JsonObject &temperature = root.createNestedObject("temperature");
  JsonObject &pressure = root.createNestedObject("pressure");
  JsonObject &relativePressure = root.createNestedObject("relativePressure");

  temperature["value"] = data->temperature.value;
  temperature["unit"] =
      configuration->temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                      : "F";
  temperature["correction"] = configuration->temperature.correction;

  pressure["value"] = data->pressure.value;
  pressure["unit"] = configuration->pressure.unit == AFE_PRESSURE_UNIT_HPA
                         ? AFE_UNIT_PRESSURE
                         : "?";
  pressure["correction"] = configuration->pressure.correction;

  relativePressure["value"] = data->relativePressure.value;
  relativePressure["unit"] = pressure["unit"];

  /**
   * @brief Not applicable for BMP280/BMP180 Sensor
   *
   */
  if (configuration->type != AFE_BMP180_SENSOR &&
      configuration->type != AFE_BMP280_SENSOR) {
    char _perception[90]; // Max size of dewPointPerception from lang.pack
    strcpy_P(_perception,
             (char *)pgm_read_dword(&(dewPointPerception[perception(
                 data->temperature.value, data->humidity.value,
                 configuration->temperature.unit ==
                     AFE_TEMPERATURE_UNIT_FAHRENHEIT)])));

    char _comfort[30]; // Max size of dewPointPerception from lang.pack
    ComfortState comfortStatus;
    float _comfortRatio = comfort(
        comfortStatus, data->temperature.value, data->humidity.value,
        configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));

    JsonObject &jsonDewPoint = root.createNestedObject("dewPoint");
    JsonObject &jsonHumidity = root.createNestedObject("humidity");
    JsonObject &jsonAbsoluteHumidity =
        root.createNestedObject("absoluteHumidity");
    JsonObject &jsonHeatIndex = root.createNestedObject("heatIndex");
    JsonObject &jsonPerception = root.createNestedObject("perception");
    JsonObject &jsonComfort = root.createNestedObject("comfort");

    jsonHeatIndex["value"] = data->heatIndex.value;
    jsonHeatIndex["unit"] = temperature["unit"];

    jsonDewPoint["value"] = data->dewPoint.value;
    jsonDewPoint["unit"] = temperature["unit"];

    jsonHumidity["value"] = data->humidity.value;
    jsonHumidity["unit"] = "%H";
    jsonHumidity["correction"] = configuration->humidity.correction;
    jsonHumidity["rating"] = data->humidity.rating;

    jsonPerception["value"] = perception(
        data->temperature.value, data->humidity.value,
        configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    jsonPerception["description"] = _perception;

    jsonComfort["value"] = (uint8_t)comfortStatus;
    jsonComfort["ratio"] = _comfortRatio;
    jsonComfort["unit"] = "%";
    jsonComfort["description"] = _comfort;

    jsonAbsoluteHumidity["value"] = absoluteHumidity(
        data->temperature.value, data->humidity.value,
        configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    jsonAbsoluteHumidity["unit"] = "%H";
  }

  /* Only for BME680 Sensor */
  if (configuration->type == AFE_BME680_SENSOR) {
    JsonObject &iaq = root.createNestedObject("iaq");
    JsonObject &staticIaq = root.createNestedObject("staticIaq");
    JsonObject &co2Equivalent = root.createNestedObject("co2Equivalent");
    JsonObject &breathVocEquivalent =
        root.createNestedObject("breathVocEquivalent");
    JsonObject &gasResistance = root.createNestedObject("gasResistance");

    breathVocEquivalent["value"] = data->breathVocEquivalent.value;
    breathVocEquivalent["unit"] = "?";
    breathVocEquivalent["accuracy"] = data->breathVocEquivalent.accuracy;

    gasResistance["value"] = data->gasResistance.value;
    gasResistance["unit"] = "kOm";

    iaq["value"] = data->iaq.value;
    iaq["rating"] = data->iaq.rating;
    iaq["accuracy"] = data->iaq.accuracy;

    staticIaq["value"] = data->staticIaq.value;
    staticIaq["rating"] = data->staticIaq.rating;
    staticIaq["accuracy"] = data->staticIaq.accuracy;

    co2Equivalent["value"] = data->co2Equivalent.value;
    co2Equivalent["unit"] = AFE_UNIT_CO2;
    co2Equivalent["rating"] = data->co2Equivalent.rating;
    co2Equivalent["accuracy"] = data->co2Equivalent.accuracy;

    //{"temperature":{"value":20.76112,"unit":"C","correction":0},"pressure":{"value":1004.08,"unit":"hPa","correction":0},"relativePressure":{"value":1012.219,"unit":"hPa"},"dewPoint":{"value":12.63424,"unit":"C"},"humidity":{"value":59.61296,"unit":"%H","correction":0,"rating":1},"absoluteHumidity":{"value":10.76806,"unit":"%H"},"heatIndex":{"value":20.44935,"unit":"C"},"perception":{"value":1,"description":"Bardzo
    // komfortowo"},"comfort":{"value":2,"ratio":99.31985,"unit":"%","description":"Za
    // zimno"},"iaq":{"value":25,"rating":1,"accuracy":0},"staticIaq":{},"co2Equivalent":{},"breathVocEquivalent":{"value":0.5,"unit":"?","accuracy":0},"gasResistance":{"value":42.212,"unit":"kOm"}}
  }
  /**
   * @brief There is a conversion to the real JSON string size. Workaround as
   * ASyncMQTTCrashes with larger strings crashes @TODO T6 investigate the
   * problem furhter
   * It looks it actually ArduinJSON crases. Same case for HTTP API
   *
   */
  root.printTo(json, AFE_CONFIG_API_JSON_BMEX80_DATA_REAL_LENGTH);
}

void AFESensorBMEX80::applyCorrections() {
#ifdef DEBUG
  Debugger->printBulletPoint(F("Applying correction to values"));
#endif

  if (configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT) {
    data->temperature.value = celsiusToFerenheit(data->temperature.value);
  }

  if (configuration->temperature.correction != 0) {
    data->temperature.value += configuration->temperature.correction;
  }

  if (configuration->type != AFE_BMP180_SENSOR &&
      configuration->type != AFE_BMP280_SENSOR) {

    if (configuration->humidity.correction != 0) {
      data->humidity.value += configuration->humidity.correction;
    }

    data->heatIndex.value =
        configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
            ? heatIndex((double)data->temperature.value,
                        (double)data->humidity.value, true)
            : heatIndex((double)data->temperature.value,
                        (double)data->humidity.value, false);

    data->dewPoint.value =
        configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
            ? celsiusToFerenheit(
                  dewPoint(ferenheitToCelsius(data->temperature.value),
                           data->humidity.value))
            : dewPoint(data->temperature.value, data->humidity.value);

#ifdef AFE_CONFIG_HUMIDITY
    data->humidity.rating = humidityRating(data->humidity.value);
#endif
  }

  if (configuration->pressure.correction != 0) {
    data->pressure.value += configuration->pressure.correction;
  }
  data->relativePressure.value = relativePressure(
      data->pressure.value, configuration->altitude, data->temperature.value);

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  if (configuration->type == AFE_BME680_SENSOR) {
    data->iaq.rating = iaqRating(data->iaq.value);
    data->staticIaq.rating = iaqRating(data->staticIaq.value);
    data->co2Equivalent.rating = co2Rating(data->co2Equivalent.value);
  }
#endif
}

#endif // AFE_CONFIG_HARDWARE_BMEX80
