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
}

boolean AFESensorHPMA115S0::read() {
  uint8_t index = 0;
  uint8_t responseBuffer[8];
  unsigned long start = millis(); /* Used for counting time to timeout */

  while (UART.SerialBus.available() > 0 && index < sizeof(responseBuffer)) {

    responseBuffer[index] = UART.SerialBus.read();

    /* If not a HEAD then skip it */
    if (index == 0 &&
        !(responseBuffer[0] == 0x40 || responseBuffer[0] == 0x42 ||
          responseBuffer[0] == 0xA5 || responseBuffer[0] == 0x96)) {
      continue;
    } else {
      index++;
    }

    /* Check if ACK */
    if (responseBuffer[0] == 0xA5 && responseBuffer[1] == 0xA5) {
      return true;
    }

    /* Check if NACK */
    if (responseBuffer[0] == 0x96 && responseBuffer[1] == 0x96) { // Neg. ACK
      return false;
    }

    /* If not data after 1sek return timeout */
    if (millis() - start > 1000) {
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
      return true;
    } else {

#ifdef DEBUG
      Serial << endl << "Checksum mismatch";
#endif
    }
  }

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
    if (millis() - startTime >= configuration.interval * 1000 ||
        startTime == 0) {
      startTime = millis();

      UART.send(commandRead);
      if (read()) {
        if (!configuration.sendOnlyChanges ||
            (configuration.sendOnlyChanges &&
             (currentPM25 != _bufferPM25 || currentPM10 != _bufferPM10))) {
          currentPM25 = _bufferPM25;
          currentPM10 = _bufferPM10;
          ready = true;
        }
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
