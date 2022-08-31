/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */
#include "AFE-Sensor-DHT.h"

#ifdef AFE_CONFIG_HARDWARE_DHT

AFESensorDHT::AFESensorDHT() {}

void AFESensorDHT::begin(AFEDataAccess *_Data, uint8_t id) {
  Data = _Data;

  Data->getConfiguration(id, configuration);

#ifdef DEBUG
  Serial << endl
         << F("INFO: DHT: Initializing...GPIO: ") << configuration->gpio
         << F(", Type: DHT") << configuration->type;
#endif

  dht->setup(configuration->gpio,
            configuration->type == AFE_CONFIG_HARDWARE_DHT_TYPE_AUTO
                ? DHTesp::AUTO_DETECT
                : configuration->type == AFE_CONFIG_HARDWARE_DHT_TYPE_DHT11
                      ? DHTesp::DHT11
                      : configuration->type == AFE_CONFIG_HARDWARE_DHT_TYPE_DHT22
                            ? DHTesp::DHT22
                            : configuration->type ==
                                      AFE_CONFIG_HARDWARE_DHT_TYPE_AM2302
                                  ? DHTesp::AM2302
                                  : configuration->type ==
                                            AFE_CONFIG_HARDWARE_DHT_TYPE_RHT03
                                        ? DHTesp::RHT03
                                        : DHTesp::AUTO_DETECT);
#ifdef DEBUG
  Serial << F("DONE");
#endif

  _initialized = true;
}

boolean AFESensorDHT::listener() {

  boolean _ready = false;

  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration->interval * 1000) {

#ifdef DEBUG
      Serial << endl << F("INFO: DHT: Reading data from the sensor...");
#endif

      float _humidity = dht->getHumidity();
      float _temperature = dht->getTemperature();

      if (!isnan(_humidity) && !isnan(_temperature)) {

        if (configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT) {
          _temperature = dht->toFahrenheit(_temperature);
        }

        // applying corrections
        _temperature += configuration->temperature.correction;
        _humidity += configuration->humidity.correction;

        _ready = true;

        if (configuration->sendOnlyChanges) {
          if (_temperature == currentTemperature &&
              _humidity == currentHumidity) {
            _ready = false;
          }
        }

        if (_ready) {
          currentTemperature = _temperature;
          currentHumidity = _humidity;
        }

#ifdef DEBUG
        Serial << endl
               << F("INFO: DHT: Time:   ") << (time - startTime) / 1000
               << F("sec, ") << F(", Temperature: ") << currentTemperature
               << F(", Humidity: ") << currentHumidity;

#endif
      }
#ifdef DEBUG
      else {
        Serial << endl
               << F("INFO: DHT: Time:   ") << (time - startTime) / 1000
               << F("sec, ")
               << F("Returned NaN: check if the sensor is connected");
      }
#endif

      startTime = 0;
    }
  }

  return _ready;
}

void AFESensorDHT::getJSON(char *json) {

  char _perception[90]; // Max size of dewPointPerception from lang.pack
  strcpy_P(_perception, (char *)pgm_read_dword(&(dewPointPerception[perception(
                            currentTemperature, currentHumidity,
                            configuration->temperature.unit ==
                                AFE_TEMPERATURE_UNIT_FAHRENHEIT)])));


  char _comfort[30]; // Max size of dewPointPerception from lang.pack
  ComfortState comfortStatus;
  float _comfortRatio = comfort(
      comfortStatus, currentTemperature, currentHumidity,
      configuration->temperature.unit == AFE_TEMPERATURE_UNIT_FAHRENHEIT);
  strcpy_P(_comfort, (char *)pgm_read_dword(&(Comfort[comfortStatus])));

  sprintf(json, "{\"temperature\":{\"value\":%.1f,\"unit\":\"%s\"},"
                "\"humidity\":{\"value\":%.1f,\"unit\":\"%%H\"},"
                "\"absoluteHumidity\":{\"value\":%.2f,\"unit\":\"%%H\"},"
                "\"heatIndex\":{\"value\":%.2f,\"unit\":\"%s\"},\"dewPoint\":{"
                "\"value\":%.2f,\"unit\":\"%s\"},\"perception\":{\"value\":%d,"
                "\"description\":\"%s\"},\"comfort\":{\"value\":%d,\"ratio\":%."
                "2f,\"unit\":"
                "\"%%\",\"description\":\"%s\"}}",
          currentTemperature,
          configuration->temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                         : "F",
          currentHumidity,
          absoluteHumidity(currentTemperature, currentHumidity,
                           configuration->temperature.unit ==
                               AFE_TEMPERATURE_UNIT_FAHRENHEIT),
          heatIndex(currentTemperature, currentHumidity,
                    configuration->temperature.unit ==
                        AFE_TEMPERATURE_UNIT_FAHRENHEIT),
          configuration->temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                         : "F",
          dewPoint(currentTemperature, currentHumidity,
                   configuration->temperature.unit ==
                       AFE_TEMPERATURE_UNIT_FAHRENHEIT),
          configuration->temperature.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C"
                                                                         : "F",
          perception(currentTemperature, currentHumidity,
                     configuration->temperature.unit ==
                         AFE_TEMPERATURE_UNIT_FAHRENHEIT),
          _perception, comfortStatus, _comfortRatio, _comfort);

}

#endif // AFE_CONFIG_HARDWARE_DHT
