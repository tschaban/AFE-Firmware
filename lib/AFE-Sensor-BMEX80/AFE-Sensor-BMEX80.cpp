/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BMEX80.h"
#ifdef AFE_CONFIG_HARDWARE_BMEX80

AFESensorBMEX80::AFESensorBMEX80(){};

#ifdef AFE_ESP32
void AFESensorBMEX80::begin(uint8_t id, TwoWire *WirePort0,
                            TwoWire *WirePort1) {
  AFEDataAccess Data;
  Data.getConfiguration(id, &configuration);
  begin(id, configuration.wirePortId == 0 ? WirePort0 : WirePort1);
}
#endif // AFE_ESP32

void AFESensorBMEX80::begin(uint8_t id, TwoWire *WirePort) {
#ifndef AFE_ESP32
  AFEDataAccess Data;
  Data.getConfiguration(id, &configuration);
#endif // AFE_ESP32

  _WirePort = WirePort;

#if defined(DEBUG)
  Serial << endl << endl << F("-------- BMEX80: Initializing --------");
#endif

#ifdef AFE_ESP32
  if (configuration.wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST) {
#endif

    switch (configuration.type) {
    case AFE_BME680_SENSOR:
      _initialized = s6.begin(&configuration, _WirePort);
      break;
    case AFE_BME280_SENSOR:
      _initialized = s2.begin(&configuration, _WirePort);
      break;
#ifndef AFE_ESP32
    case AFE_BMP180_SENSOR:
      _initialized = s1.begin(&configuration, _WirePort);
      break;
#endif // AFE_ESP32
    default:
      _initialized = false;
      break;
    }

#ifdef AFE_ESP32
  }
#endif

#ifdef DEBUG
  Serial << endl
         << F("INFO: Device: ")
         << (_initialized ? F("Found") : F("Not found: check wiring"));
  Serial << endl << F("--------------------------------------") << endl;
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

    if (configuration.type == AFE_BME680_SENSOR) {
      readStatus = s6.read();
    }

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

#if defined(DEBUG)
      Serial << endl
             << endl
             << F("--------") << F(" Reading sensor data ") << F("--------");
#endif

      if (configuration.type != AFE_BME680_SENSOR) {
#ifdef AFE_ESP32
        readStatus = s2.read();
#else  // ESP8266
        readStatus =
            configuration.type == AFE_BME280_SENSOR ? s2.read() : s1.read();
#endif // ESP32/ESP8266
      }

      if (readStatus) {

        if (configuration.type == AFE_BME680_SENSOR) {
          s6.get(data);
        } else {
#ifdef AFE_ESP32
          data = s2.data;
#else  // ESP8266
          data = configuration.type == AFE_BME280_SENSOR ? s2.data : s1.data;
#endif // ESP32/ESP8266
        }
        applyCorrections();
        ready = true;

#ifdef DEBUG
        Serial << endl
               << F(" - Temperature = ") << data.temperature.value << endl
               << F(" - Pressure = ") << data.pressure.value
               << F(", Relative = ") << data.relativePressure.value;
#ifndef AFE_ESP32
        if (configuration.type != AFE_BMP180_SENSOR) {
#endif // AFE_ESP32
          Serial << endl << F(" - Humidity = ") << data.humidity.value;
          Serial << endl << F(" - Dew Point = ") << data.dewPoint.value;
#ifndef AFE_ESP32
        }
#endif // AFE_ESP32
        if (configuration.type == AFE_BME680_SENSOR) {
          Serial << endl
                 << F(" - Gas Resistance = ") << data.gasResistance.value;
          Serial << endl << F(" - IAQ = ") << data.iaq.value;
          Serial << endl << F(" - IAQ Accuracy = ") << data.iaq.accuracy;
          Serial << endl << F(" - Static IAQ = ") << data.staticIaq.value;
          Serial << endl
                 << F(" - Static IAQ Accuracy = ") << data.staticIaq.accuracy;
          Serial << endl
                 << F(" - Breath VOC = ") << data.breathVocEquivalent.value;
          Serial << endl
                 << F(" - Breath VOC Accuracy = ")
                 << data.breathVocEquivalent.accuracy;
          Serial << endl << F(" - CO2 = ") << data.co2Equivalent.value;
          Serial << endl
                 << F(" - CO2 Accuracy = ") << data.co2Equivalent.accuracy;
        }

      } else {
        Serial << endl << F("WARN: No data found");
#endif // DEBUG
      }
      startTime = 0;
#ifdef DEBUG
      Serial << endl << F("------------------------------------") << endl;
#endif
    }
  }
}

void AFESensorBMEX80::getJSON(char *json) {

  //@TODO T5 T6  Estimate max size of JSON
  StaticJsonBuffer<AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH>
      jsonBuffer; // {"temperature":{"value":25.0273,"unit":"C","correction":25.0273},"pressure":{"value":993.6063,"unit":"hPa","correction":993.6063},"relativePressure":{"value":1003.809,"unit":"hPa"},"dewPoint":{"value":5.321408,"unit":"C"},"humidity":{"value":281.10078,"unit":"%H","correction":281.10078,"rating":3},"absoluteHumidity":{"value":6.480335,"unit":"%H"},"heatIndex":{"value":24.31934,"unit":"C"},"perception":{"value":0,"description":"W
  // porządku dla większości, ale wszyscy odczuwają wilgoć przy
  // górnej
  // krawędzi"},"comfort":{"value":4,"ratio":84.7268,"unit":"%","description":"Gorąco
  // i
  // wilgotno"},"iaq":{"value":1110,"rating":1,"accuracy":0},"staticIaq":{"value":1110,"rating":1,"accuracy":0},"co2Equivalent":{"value":1400,"unit":"ppm","rating":1,"accuracy":0},"breathVocEquivalent":{"value":0.342646,"unit":"?","accuracy":0},"gasResistance":{"value":2324.371,"unit":"kOm"}}
  JsonObject &root = jsonBuffer.createObject();

  JsonObject &temperature = root.createNestedObject("temperature");
  JsonObject &pressure = root.createNestedObject("pressure");
  JsonObject &relativePressure = root.createNestedObject("relativePressure");

  temperature["value"] = data.temperature.value;
  temperature["unit"] =
      configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                     : "F";
  temperature["correction"] = configuration.temperature.correction;

  pressure["value"] = data.pressure.value;
  pressure["unit"] =
      configuration.pressure.unit == AFE_PRESSURE_UNIT_HPA ? AFE_UNIT_PRESSURE : "?";
  pressure["correction"] = configuration.pressure.correction;

  relativePressure["value"] = data.relativePressure.value;
  relativePressure["unit"] = pressure["unit"];

#ifndef AFE_ESP32
  /* Not applicable for BMP180 Sensor */
  if (configuration.type != AFE_BMP180_SENSOR) {
#endif                    // AFE_ESP32
    char _perception[90]; // Max size of dewPointPerception from lang.pack
    strcpy_P(_perception,
             (char *)pgm_read_dword(&(dewPointPerception[perception(
                 data.temperature.value, data.humidity.value,
                 configuration.temperature.unit ==
                     AFE_TEMPERATURE_UNIT_FAHRENHEIT)])));

    char _comfort[30]; // Max size of dewPointPerception from lang.pack
    ComfortState comfortStatus;
    float _comfortRatio = comfort(
        comfortStatus, data.temperature.value, data.humidity.value,
        configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));

    JsonObject &jsonDewPoint = root.createNestedObject("dewPoint");
    JsonObject &jsonHumidity = root.createNestedObject("humidity");
    JsonObject &jsonAbsoluteHumidity =
        root.createNestedObject("absoluteHumidity");
    JsonObject &jsonHeatIndex = root.createNestedObject("heatIndex");
    JsonObject &jsonPerception = root.createNestedObject("perception");
    JsonObject &jsonComfort = root.createNestedObject("comfort");

    jsonHeatIndex["value"] = data.heatIndex.value;
    jsonHeatIndex["unit"] = temperature["unit"];

    jsonDewPoint["value"] = data.dewPoint.value;
    jsonDewPoint["unit"] = temperature["unit"];

    jsonHumidity["value"] = data.humidity.value;
    jsonHumidity["unit"] = "%H";
    jsonHumidity["correction"] = configuration.humidity.correction;
    jsonHumidity["rating"] = data.humidity.rating;

    jsonPerception["value"] = perception(
        data.temperature.value, data.humidity.value,
        configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    jsonPerception["description"] = _perception;

    jsonComfort["value"] = (uint8_t)comfortStatus;
    jsonComfort["ratio"] = _comfortRatio;
    jsonComfort["unit"] = "%";
    jsonComfort["description"] = _comfort;

    jsonAbsoluteHumidity["value"] = absoluteHumidity(
        data.temperature.value, data.humidity.value,
        configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
    jsonAbsoluteHumidity["unit"] = "%H";
#ifndef AFE_ESP32
  }
#endif // AFE_ESP32

  /* Only for BME680 Sensor */
  if (configuration.type == AFE_BME680_SENSOR) {
    JsonObject &iaq = root.createNestedObject("iaq");
    JsonObject &staticIaq = root.createNestedObject("staticIaq");
    JsonObject &co2Equivalent = root.createNestedObject("co2Equivalent");
    JsonObject &breathVocEquivalent =
        root.createNestedObject("breathVocEquivalent");
    JsonObject &gasResistance = root.createNestedObject("gasResistance");

    iaq["value"] = data.iaq.value;
    iaq["rating"] = data.iaq.rating;
    iaq["accuracy"] = data.iaq.accuracy;

    staticIaq["value"] = data.staticIaq.value;
    staticIaq["rating"] = data.staticIaq.rating;
    staticIaq["accuracy"] = data.staticIaq.accuracy;

    co2Equivalent["value"] = data.co2Equivalent.value;
    co2Equivalent["unit"] = AFE_UNIT_CO2;
    co2Equivalent["rating"] = data.co2Equivalent.rating;
    co2Equivalent["accuracy"] = data.co2Equivalent.accuracy;

    breathVocEquivalent["value"] = data.breathVocEquivalent.value;
    breathVocEquivalent["unit"] = "?";
    breathVocEquivalent["accuracy"] = data.breathVocEquivalent.accuracy;

    gasResistance["value"] = data.gasResistance.value;
    gasResistance["unit"] = "kOm";
  }

  //@TODO T5 T6 Estimate max size of JSON
  root.printTo(json, AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH);
}

void AFESensorBMEX80::applyCorrections() {
#ifdef DEBUG
  Serial << endl << F("INFO: Applying correction to values");
#endif

  if (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT) {
    data.temperature.value = celsiusToFerenheit(data.temperature.value);
  }

  if (configuration.temperature.correction != 0) {
    data.temperature.value += configuration.temperature.correction;
  }

#ifndef AFE_ESP32
  if (configuration.type != AFE_BMP180_SENSOR) {
#endif // AFE_ESP32
    if (configuration.humidity.correction != 0) {
      data.humidity.value += configuration.humidity.correction;
    }

    data.heatIndex.value =
        configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
            ? heatIndex((double)data.temperature.value,
                        (double)data.humidity.value, true)
            : heatIndex((double)data.temperature.value,
                        (double)data.humidity.value, false);

    data.dewPoint.value =
        configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT
            ? celsiusToFerenheit(
                  dewPoint(ferenheitToCelsius(data.temperature.value),
                           data.humidity.value))
            : dewPoint(data.temperature.value, data.humidity.value);

#ifdef AFE_CONFIG_HUMIDITY
    data.humidity.rating = humidityRating(data.humidity.value);
#endif
#ifndef AFE_ESP32
  }
#endif // AFE_ESP32  

  if (configuration.pressure.correction != 0) {
    data.pressure.value += configuration.pressure.correction;
  }
  data.relativePressure.value = relativePressure(
      data.pressure.value, configuration.altitude, data.temperature.value);

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  if (configuration.type == AFE_BME680_SENSOR) {
    data.iaq.rating = iaqRating(data.iaq.value);
    data.staticIaq.rating = iaqRating(data.staticIaq.value);
    data.co2Equivalent.rating = co2Rating(data.co2Equivalent.value);
  }
#endif
}

#endif // AFE_CONFIG_HARDWARE_BMEX80
