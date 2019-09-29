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
        sensorData =
            configuration.type == AFE_BME680_SENSOR
                ? s6.data
                : configuration.type == AFE_BME280_SENSOR ? s2.data : s1.data;

        applyCorrections();
        ready = true;

#ifdef DEBUG
        Serial << endl
               << "Temperature = " << sensorData.temperature.value << endl
               << "Pressure = " << sensorData.pressure.value
               << ", Relative = " << sensorData.relativePressure.value;
        if (configuration.type != AFE_BMP180_SENSOR) {
          Serial << endl << "Humidity = " << sensorData.humidity.value;
          Serial << endl << "Dew Point = " << sensorData.dewPoint.value;
        }
        if (configuration.type == AFE_BME680_SENSOR) {
          Serial << endl
                 << "Gas Resistance = " << sensorData.gasResistance.value;
          Serial << endl << "IAQ = " << sensorData.iaq.value;
          Serial << endl << "IAQ Accuracy = " << sensorData.iaq.accuracy;
          Serial << endl << "Static IAQ = " << sensorData.staticIaq.value;
          Serial << endl
                 << "Static IAQ Accuracy = " << sensorData.staticIaq.accuracy;
          Serial << endl
                 << "Breath VOC = " << sensorData.breathVocEquivalent.value;
          Serial << endl
                 << "Breath VOC Accuracy = "
                 << sensorData.breathVocEquivalent.accuracy;
          Serial << endl << "CO2 = " << sensorData.co2Equivalent.value;
          Serial << endl
                 << "CO2 Accuracy = " << sensorData.co2Equivalent.accuracy;
        }

      } else {
        Serial << endl << "No data found";
#endif
      }
      startTime = 0;
#ifdef DEBUG
      Serial << endl << "------------------------------------" << endl;
#endif
    }
  }
}

void AFESensorBMEX80::getJSON(char *json) {

  //@TODO Estimate max size of JSON
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  JsonObject &temperature = root.createNestedObject("temperature");
  JsonObject &pressure = root.createNestedObject("pressure");
  JsonObject &relativePressure = root.createNestedObject("relativePressure");

  temperature["value"] = sensorData.temperature.value;
  temperature["unit"] =
      configuration.temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                     : "F";
  temperature["correction"] = configuration.temperature.correction;

  pressure["value"] = sensorData.pressure.value;
  pressure["unit"] =
      configuration.pressure.unit == AFE_PRESSURE_UNIT_HPA ? "hPa" : "?";
  pressure["correction"] = configuration.pressure.correction;

  relativePressure["value"] = sensorData.relativePressure.value;
  relativePressure["unit"] = pressure["unit"];

  /* Not applicable for BMP180 Sensor */
  if (configuration.type != AFE_BMP180_SENSOR) {
    JsonObject &dewPoint = root.createNestedObject("dewPoint");
    JsonObject &humidity = root.createNestedObject("humidity");
    JsonObject &heatIndex = root.createNestedObject("heatIndex");

    dewPoint["value"] = sensorData.dewPoint.value;
    dewPoint["unit"] = temperature["unit"];
    humidity["value"] = sensorData.humidity.value;
    humidity["unit"] = "%";
    humidity["correction"] = configuration.humidity.correction;
    humidity["rating"] = sensorData.humidity.rating;

    heatIndex["value"] = sensorData.heatIndex.value;
    heatIndex["unit"] = temperature["unit"];
  }

  /* Only for BME680 Sensor */
  if (configuration.type == AFE_BME680_SENSOR) {
    JsonObject &iaq = root.createNestedObject("iaq");
    JsonObject &staticIaq = root.createNestedObject("staticIaq");
    JsonObject &co2Equivalent = root.createNestedObject("co2Equivalent");
    JsonObject &breathVocEquivalent =
        root.createNestedObject("breathVocEquivalent");
    JsonObject &gasResistance = root.createNestedObject("gasResistance");

    iaq["value"] = sensorData.iaq.value;
    iaq["rating"] = sensorData.iaq.rating;
    iaq["accuracy"] = sensorData.iaq.accuracy;

    staticIaq["value"] = sensorData.staticIaq.value;
    staticIaq["rating"] = sensorData.staticIaq.rating;
    staticIaq["accuracy"] = sensorData.staticIaq.accuracy;

    co2Equivalent["value"] = sensorData.co2Equivalent.value;
    co2Equivalent["unit"] = "ppm";
    co2Equivalent["rating"] = sensorData.co2Equivalent.rating;
    co2Equivalent["accuracy"] = sensorData.co2Equivalent.accuracy;

    breathVocEquivalent["value"] = sensorData.breathVocEquivalent.value;
    breathVocEquivalent["unit"] = "?";
    breathVocEquivalent["accuracy"] = sensorData.breathVocEquivalent.accuracy;

    gasResistance["value"] = sensorData.gasResistance.value;
    gasResistance["unit"] = "kOm";
  }
  //@TODO Estimate max size of JSON
  root.printTo(json, 1000);
}

void AFESensorBMEX80::applyCorrections() {
#ifdef DEBUG
  Serial << endl << "Applying correction to values";
#endif

  AFESensorsCommon calculation;

  if (configuration.temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT) {
    sensorData.temperature.value =
        calculation.celsiusToFerenheit(sensorData.temperature.value);
  }

  if (configuration.temperature.correction != 0) {
    sensorData.temperature.value += configuration.temperature.correction;
  }

  if (configuration.type != AFE_BMP180_SENSOR) {
    if (configuration.humidity.correction != 0) {
      sensorData.humidity.value += configuration.humidity.correction;
    }

    // @TODO in Ferenhiet
    sensorData.dewPoint.value = calculation.dewPoint(
        sensorData.temperature.value, sensorData.humidity.value);

    sensorData.heatIndex.value =
        calculation.heatIndex((double)sensorData.temperature.value,
                              (double)sensorData.humidity.value);

#ifdef AFE_CONFIG_HUMIDITY
    sensorData.humidity.rating =
        calculation.humidityRating(sensorData.humidity.value);
#endif
  }

  if (configuration.pressure.correction != 0) {
    sensorData.pressure.value += configuration.pressure.correction;
  }
  sensorData.relativePressure.value = calculation.relativePressure(
      sensorData.pressure.value, configuration.altitude,
      sensorData.temperature.value);

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  if (configuration.type == AFE_BME680_SENSOR) {
    sensorData.iaq.rating = calculation.iaqRating(sensorData.iaq.value);
    sensorData.staticIaq.rating =
        calculation.iaqRating(sensorData.staticIaq.value);
    sensorData.co2Equivalent.rating =
        calculation.co2Rating(sensorData.co2Equivalent.value);
  }
#endif
}