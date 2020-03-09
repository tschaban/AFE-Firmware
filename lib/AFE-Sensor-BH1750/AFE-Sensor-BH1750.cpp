/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-BH1750.h"

AFESensorBH1750::AFESensorBH1750(){};

void AFESensorBH1750::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getBH1750SensorConfiguration(id);
  I2CPORT I2C = Data.getI2CPortConfiguration();

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }
#endif

#ifdef DEBUG
  Serial << endl << endl << "----- BH1750: Initializing -----";
#endif
  if (configuration.i2cAddress != 0) {

#ifdef DEBUG
    Serial << endl << "Checking if the sensor is connected";
#endif
    AFEI2CScanner I2CScanner;
    I2CScanner.begin();
    if (I2CScanner.scan(configuration.i2cAddress)) {

#ifdef DEBUG
      Serial << endl << "Setting I2C: SDA:" << I2C.SDA << ", SCL:" << I2C.SCL;
#endif

      bh1750.setI2C(I2C.SDA, I2C.SCL);
#ifdef DEBUG
      Serial << endl << "Sensor address: 0x" << _HEX(configuration.i2cAddress);
#endif
      _initialized = bh1750.begin(BH1750LightSensor::ONE_TIME_HIGH_RES_MODE_2,
                                  configuration.i2cAddress);

    }
#ifdef DEBUG
    else {
      Serial << endl
             << "Error: Device not found under I2C Address: 0x"
             << _HEX(configuration.i2cAddress);
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << "Error: Address not set";
  }
#endif

#ifdef DEBUG
  if (_initialized) {
     Serial << endl << "Name: " << configuration.name;
    Serial << endl << "Mode: " << configuration.mode;
    Serial << endl << "Interval: " << configuration.interval;
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
    Serial << endl << "IDX: " << configuration.domoticz.idx;
#endif
  }
  Serial << endl
         << "Device: " << (_initialized ? "Found" : "Not found: check wiring");
  Serial << endl << "---------------------------------";
#endif
}

boolean AFESensorBH1750::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorBH1750::listener() {

  if (_initialized) {
    if (millis() - startTime >= configuration.interval * 1000) {

#if defined(DEBUG)
      Serial << endl << endl << "----- BH1750: Reading -----";
      Serial << endl << "Time: " << (millis() - startTime) / 1000 << "s";
#endif

      data = bh1750.readLightLevel();
      if (data >= 0) {
        ready = true;
      }

#if defined(DEBUG)
      Serial << endl << "Lux: " << data << "lx";
      Serial << endl << "---------------------------";
#endif

      startTime = millis();
    }
  }
}

void AFESensorBH1750::getJSON(char *json) {
  sprintf(json, "{\"illuminance\":{\"value\":%.2f,\"unit\":\"lux\"}}", data);
}
