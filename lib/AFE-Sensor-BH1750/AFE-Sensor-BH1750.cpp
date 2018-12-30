/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-BH1750.h"

AFESensorBH1750::AFESensorBH1750(){};

void AFESensorBH1750::begin() {
  AFEDataAccess Data;
  configuration = Data.getBH1750SensorConfiguration();

#ifdef DEBUG
  Serial << endl << endl << "----- BH1750: Initializing -----";
#endif
  if (configuration.i2cAddress != 0) {
#ifdef DEBUG
    Serial << endl << "Address: 0x" << _HEX(configuration.i2cAddress);
#endif
    _initialized = bh1750.begin(BH1750LightSensor::ONE_TIME_HIGH_RES_MODE_2,
                                configuration.i2cAddress);
  }
#ifdef DEBUG
  else {
    Serial << endl << "Error: Address not set";
#endif
  }

#ifdef DEBUG
  if (_initialized) {
    Serial << endl << "Mode: " << configuration.mode;
    Serial << endl << "Interval: " << configuration.interval;
    Serial << endl << "IDX: " << configuration.idx;
  }
  Serial << endl
         << "Device: " << (_initialized ? "Found" : "Not found: check wiring");
  Serial << endl << "---------------------------------";
#endif
}

float AFESensorBH1750::get() { return currentLightLevel; }

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

      currentLightLevel = bh1750.readLightLevel();
      if (currentLightLevel >= 0) {
        ready = true;
      }

#if defined(DEBUG)
      Serial << endl << "Lux: " << currentLightLevel << "lx";
      Serial << endl << "---------------------------";
#endif

      startTime = millis();
    }
  }
}

unsigned long AFESensorBH1750::getDomoticzIDX() { return configuration.idx; }
