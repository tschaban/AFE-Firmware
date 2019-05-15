/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Analog-Input.h"

AFEAnalogInput::AFEAnalogInput(){};

void AFEAnalogInput::begin() {
  AFEDataAccess Data;
  configuration = Data.getADCInputConfiguration();
  _initialized = true;

#ifdef DEBUG
  Serial << endl << endl << "------------ AC VCC Input ------------";
  Serial << endl
         << "- Initialized" << endl
         << "- GPIO: " << configuration.gpio
         << ", Interval: " << configuration.interval
         << ", No of Samples: " << configuration.numberOfSamples;
  Serial << endl << "-------------------------------------";

#endif
}

ADCINPUT_DATA AFEAnalogInput::get() {
  ADCINPUT_DATA data;
  data.raw = analogData;
  data.percent = (float)analogData * 100 / 1024;
  data.voltage = (double)analogData / 1024;
  return data;
}

boolean AFEAnalogInput::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFEAnalogInput::listener() {
  if (_initialized) {
    unsigned long time = millis();

    if (startTime == 0) {
      startTime = time;
    }

    if (time - startTime >= configuration.interval * 1000) {

      if (counterOfSamplings < configuration.numberOfSamples) {
        temporaryAnalogData += analogRead(configuration.gpio);
        counterOfSamplings++;
      } else {

        analogData =
            (uint16_t)temporaryAnalogData / configuration.numberOfSamples;

#ifdef DEBUG
        Serial << endl
               << " - Number of samples: " << counterOfSamplings << endl
               << " - Analog value = " << analogData << endl
               << " - Sampling time = "
               << millis() - startTime - configuration.interval * 1000
               << "msec.";
#endif

        counterOfSamplings = 0;
        temporaryAnalogData = 0;
        ready = true;
        startTime = 0;

#ifdef DEBUG
        Serial << endl << "-------------------------------------------";
#endif
      }
    }
  }
}

void AFEAnalogInput::getDomoticzIDX(ADCINPUT_DOMOTICZ *idx) {
  *idx = configuration.idx;
}
