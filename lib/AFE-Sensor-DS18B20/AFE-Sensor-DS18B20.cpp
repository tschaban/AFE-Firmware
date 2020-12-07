/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-DS18B20.h"

#ifdef AFE_CONFIG_HARDWARE_DS18B20

AFESensorDS18B20::AFESensorDS18B20(){};

void AFESensorDS18B20::begin(AFEDataAccess *_Data, uint8_t id) {
  _Data->getConfiguration(id, &configuration);
  WireBUS.begin(configuration.gpio);
  Sensor.setOneWire(&WireBUS);
  Sensor.begin();

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  /* Defining get and state MQTT Topics */
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = AFE_EMPTY_STRING;
  }

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttStateTopic, "%s/state", configuration.mqtt.topic);
  } else {
    mqttStateTopic[0] = AFE_EMPTY_STRING;
  }

#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

  if (Sensor.isConnected(configuration.address)) {
    _initialized = true;
#ifdef DEBUG
    char addressTxt[17];
    addressToChar(configuration.address, addressTxt);
    Serial << endl << "INFO: Sensor DS18B20[" << addressTxt << "] initialized";
#endif
  } else {
    _initialized = false;
#ifdef DEBUG
    char addressTxt[17];
    addressToChar(configuration.address, addressTxt);
    Serial << endl
           << "WARN: Sensor DS18B20[" << addressTxt
           << "] not found. Not initialized";
#endif
  }
}

float AFESensorDS18B20::getCurrentTemperature() {
  float temperature = DEVICE_DISCONNECTED_C;
  if (_initialized) {
#ifdef DEBUG
    Serial << endl
           << "INFO: Reading temperature from DS18B20[" << configuration.name
           << "] ";
#endif

    if (readTimeOut == 0) {
      readTimeOut = millis();
    }

    if (Sensor.isConnected(configuration.address)) {

      Sensor.requestTemperaturesByAddress(configuration.address);

      do {
        temperature = configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS
                          ? Sensor.getTempC(configuration.address)
                          : Sensor.getTempF(configuration.address);
        if (millis() - readTimeOut > AFE_CONFIG_HARDWARE_DS18B20_READ_TIMEOUT) {
          break;
        }
      } while (temperature == 85.0 || temperature == (-127.0));
      temperature = temperature + configuration.correction;
    }
  }
#ifdef DEBUG
  else {
    Serial << ": NOT CONNECTED: ";
  }
  Serial << ": " << temperature
         << ", read duration: " << (millis() - readTimeOut) << "msec.";
#endif
  readTimeOut = 0;
  return temperature;
}

float AFESensorDS18B20::getTemperature() { return currentTemperature; }

boolean AFESensorDS18B20::listener() {
  boolean ready = false;
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {
      float newTemperature = getCurrentTemperature();

      if (newTemperature != DEVICE_DISCONNECTED_C) {
        if ((configuration.sendOnlyChanges &&
             newTemperature != currentTemperature) ||
            !configuration.sendOnlyChanges) {
          currentTemperature = newTemperature;
          ready = true;
        }
      }
#ifdef DEBUG
      else {
        Serial
            << endl
            << "WARN: DS18B20: Sensor returned -127: means it's disconnected";
      }
#endif

      startTime = 0;
    }
  }

  return ready;
}

uint8_t AFESensorDS18B20::scan(uint8_t gpio, DS18B20Addresses &addresses) {
  uint8_t _found = 0;
  uint8_t numberOfDevicesOnBus = 0;
#ifdef DEBUG
  Serial << endl << "INFO: Scanning for DS18B20 sensors on GPIO: " << gpio;
  Serial << endl << " - Wire Bus initialized";
#endif
  Sensor.setOneWire(&WireBUS);
  WireBUS.begin(gpio);
  Sensor.begin();
  numberOfDevicesOnBus = Sensor.getDS18Count();
#ifdef DEBUG
  Serial << endl
         << " - Number of detected DS18B20 sensors: " << numberOfDevicesOnBus;
#endif

  /* @TODO This is a workaround as getDS18Count doesn't retun number of sensor
   */
  if (numberOfDevicesOnBus == 0) {
    numberOfDevicesOnBus = AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20;
#ifdef DEBUG
    Serial << endl
           << " - WARN: No sensors found. Hard scanning for "
           << numberOfDevicesOnBus << " addresses";
#endif
  }

  if (numberOfDevicesOnBus > 0) {
    DeviceAddress _address;

    for (uint8_t i = 0; i < numberOfDevicesOnBus; i++) {
      Sensor.getAddress(_address, i);

      if (_address[0] != 0) {
        if (i == 0 || (i > 0 && (memcmp(addresses[i - 1], _address,
                                        sizeof(addresses[i - 1])) != 0))) {

          memcpy(addresses[i], _address, sizeof(_address[0]) * 8);
          _found++;
        } else {
          break;
        }
      } else {
        break;
      }
#ifdef DEBUG
      Serial << endl
             << " - Found sensor: " << addresses[i][0] << ":" << addresses[i][1]
             << ":" << addresses[i][2] << ":" << addresses[i][3] << ":"
             << addresses[i][4] << ":" << addresses[i][5] << ":"
             << addresses[i][6] << ":" << addresses[i][7];

      Sensor.requestTemperatures();
      Serial << endl << " - Temperature : " << Sensor.getTempC(addresses[i]);

#endif
    }
  }
  return _found;
}

void AFESensorDS18B20::addressToChar(DeviceAddress &address,
                                     char *addressString) {
  sprintf(addressString, "%02X%02X%02X%02X%02X%02X%02X%02X", address[0],
          address[1], address[2], address[3], address[4], address[5],
          address[6], address[7]);
}

void AFESensorDS18B20::addressToInt(char *addressString,
                                    DeviceAddress &address) {

  int x;
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_DS18B20_ADDRESS_LENGTH; i++) {
    x = 0;
    for (uint8_t index = 0; index < 2; index++) {
      char c = *addressString;
      if (c >= '0' && c <= '9') {
        x *= 16;
        x += c - '0';
      } else if (c >= 'A' && c <= 'F') {
        x *= 16;
        x += (c - 'A') + 10;
      }
      addressString++;
    }
    address[i] = x;
  }
}

void AFESensorDS18B20::addressNULL(DeviceAddress &address) {
  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_DS18B20_ADDRESS_LENGTH; i++) {
    address[i] = 0;
  }
}

void AFESensorDS18B20::getJSON(char *json) {
  sprintf(json, "{\"temperature\":{\"value\":%.3f,\"unit\":\"%s\"}}",
          currentTemperature,
          configuration.unit == AFE_TEMPERATURE_UNIT_CELSIUS ? "C" : "F");
}

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
unsigned long AFESensorDS18B20::getDomoticzIDX() {
  return configuration.domoticz.idx;
}
#endif

#endif // AFE_CONFIG_HARDWARE_DS18B20
