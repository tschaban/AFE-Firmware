/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Sensor-HPMA115S0.h"

AFESensorHPMA115S0::AFESensorHPMA115S0(){};

void AFESensorHPMA115S0::begin() {
  AFEDataAccess Data;
  configuration = Data.getHPMA115S0SensorConfiguration();

  /* Opening Serial port */
  UART.begin();
  _initialized = true;

  /* Turning off automatic data sending from the sensor */

  UART.send(commandAutoOFF);
  read();
  /* Turning on / off measurments */
  if (configuration.timeToMeasure == 0) {
    UART.send(commandMeasuringOFF);
    read() ? _measuremntsON = false : _measuremntsON = true;
  } else {
    UART.send(commandMeasuringON);
    read() ? _measuremntsON = true : _measuremntsON = false;
  }
}

boolean AFESensorHPMA115S0::read() {
  uint8_t index = 0;
  uint8_t responseBuffer[32];
  uint8_t responseSize = 32;
  unsigned long start = millis(); /* Used for counting time to timeout */

  while (UART.SerialBus.available() > 0 && index < responseSize) {

    responseBuffer[index] = UART.SerialBus.read();

    /* If not a HEAD then skip it */
    if (index == 0 &&
        !(responseBuffer[0] == 0x40 || responseBuffer[0] == 0x42 ||
          responseBuffer[0] == 0xA5 || responseBuffer[0] == 0x96)) {
      continue;
    } else {
      index++;
    }

    /* Setting response size: autosend */
    if (responseBuffer[0] == 0x42 && responseBuffer[1] == 0x4d &&
        responseSize != 32) {
#ifdef DEBUG
      Serial << " Response size: 32B";
#endif
      responseSize = 32;
    }

    /* Setting response size: normal read */
    if (responseBuffer[0] == 0x40 && responseBuffer[2] == 0x4 &&
        responseSize != 8) { // Reading
#ifdef DEBUG
      Serial << " Response size: 8B";
#endif
      responseSize = 8;
    }

    /* Check if ACK response */
    if (responseBuffer[0] == 0xA5 && responseBuffer[1] == 0xA5) {
#ifdef DEBUG
      Serial << " Success (ACK)";
#endif
      return true;
    }

    /* Check if NACK response */
    if (responseBuffer[0] == 0x96 && responseBuffer[1] == 0x96) {
#ifdef DEBUG
      Serial << " Failure (NACK)";
#endif
      return false;
    }

    /* If not data after 1sek return timeout */
    if (millis() - start > 1000) {
#ifdef DEBUG
      Serial << " Failure (TimeOut)";
#endif
      return false;
    }
  }

  /* Processing data */
  if (responseBuffer[2] == 0x04) {
    int checkSumToCompare =
        responseBuffer[0] + responseBuffer[1] + responseBuffer[2];
    uint8_t counter;

    for (counter = 3; counter < (2 + responseBuffer[1]); counter++) {
      checkSumToCompare += responseBuffer[counter];
    }
    checkSumToCompare = (65536 - checkSumToCompare) % 256;

    if (checkSumToCompare == responseBuffer[counter]) {
      _bufferPM25 = responseBuffer[3] * 256 + responseBuffer[4];
      _bufferPM10 = responseBuffer[5] * 256 + responseBuffer[6];
#ifdef DEBUG
      Serial << " Success (PM2.5=" << _bufferPM25 << "ug/m3, "
             << "PM10=" << _bufferPM10 << "ug/m3)";
#endif
      return true;
    } else {

#ifdef DEBUG
      Serial << " Failure (Checksum mismatch)";
#endif
      return false;
    }
  }

#ifdef DEBUG
  Serial << " Failure (Wrong or no response)";
#endif

  return false;
}

void AFESensorHPMA115S0::get(uint16_t *pm25, uint16_t *pm10) {
  *pm25 = currentPM25;
  *pm10 = currentPM10;
}

boolean AFESensorHPMA115S0::isReady() {
  if (ready) {
    ready = false;
    return true;
  } else {
    return false;
  }
}

void AFESensorHPMA115S0::listener() {
  if (_initialized) {
    if ((millis() - startTime >= configuration.interval * 1000) &&
        _measuremntsON) {

      startTime = millis();

      UART.send(commandRead);
      if (read()) {
        if (!configuration.sendOnlyChanges ||
            (configuration.sendOnlyChanges &&
             (currentPM25 != _bufferPM25 || currentPM10 != _bufferPM10))) {
          currentPM25 = _bufferPM25;
          currentPM10 = _bufferPM10;
          if (currentPM25 != 0 && currentPM10 != 0) {
            ready = true;
          }
        }
      }

      /* Put a sensor to a sleep mode */
      if (configuration.timeToMeasure > 0) {
        UART.send(commandMeasuringOFF);
        read() ? _measuremntsON = false : _measuremntsON = true;
      }
    }
    /* Wake up sensor */
    else if (millis() - startTime >=
                 ((configuration.interval - configuration.timeToMeasure) *
                  1000) &&
             !_measuremntsON) {

      if (!_measuremntsON) {
        UART.send(commandMeasuringON);
        read() ? _measuremntsON = true : _measuremntsON = false;
      }
    }
  }
}

unsigned long AFESensorHPMA115S0::getDomoticzIDX(byte type) {
  if (type == HPMA115S0_TYPE_PM25) {
    return configuration.idxPM25;
  } else {
    return configuration.idxPM10;
  }
}
