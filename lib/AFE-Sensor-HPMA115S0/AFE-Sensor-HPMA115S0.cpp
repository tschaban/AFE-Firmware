/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-HPMA115S0.h"
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0

AFESensorHPMA115S0::AFESensorHPMA115S0(){};

void AFESensorHPMA115S0::begin(uint8_t id) {
  AFEDataAccess Data;
  Data.getConfiguration(id, configuration);

  /* Opening Serial port */
  UART->begin();

  /* Clean transmit buffer */
  UART->SerialBus.flush();
  _initialized = true;

  data->pm10 = data->pm25 = buffer->pm10 = buffer->pm25 = 0;

#if defined(DEBUG)
  Serial << endl << endl << F("----- HPMA115S0: Initializing -----");
  Serial << endl << F("Cleaning serial. Size=") << UART->SerialBus.available();
#endif
  UART->clean();

#if defined(DEBUG)
  Serial << endl << F("Turning Autosending: OFF");
#endif

  /* Turn off autosending */
  sendCommand(commandAutoOFF);

#if defined(DEBUG)
  Serial << endl << F("Cleaning serial. Size=") << UART->SerialBus.available();
#endif

  /* Clean buffer */
  UART->clean();

  /* Turning on / off the device */
  if (configuration->timeToMeasure == 0) {
    sendCommand(commandTurnON) ? _measuremntsON = true : _measuremntsON = false;
  } else {
    sendCommand(commandTurnOFF) ? _measuremntsON = true
                                : _measuremntsON = false;
  }

#if defined(DEBUG)
  Serial << endl << F("Device is: ") << (_measuremntsON ? F("ON") : F("OFF"));
  Serial << endl << F("-----------------------------------");
#endif
}

boolean AFESensorHPMA115S0::read(boolean expectingACK) {
  uint8_t index = 0;
  uint8_t responseBuffer[32];
  uint8_t responseSize = 32;
  unsigned long start = millis(); /* Used for counting time to timeout */

#ifdef DEBUG
  Serial << endl
         << F("UART: Size of a data in the buffer =")
         << UART->SerialBus.available();
#endif

  // UART->SerialBus.print("UART: Size of a data in the buffer =");
  // UART->SerialBus.println(UART->SerialBus.available());

  /* Wait for a data from UART-> Max 1 sec */
  while (UART->SerialBus.available() == 0 && millis() - start < 1000) {
  }

#ifdef DEBUG
  if (UART->SerialBus.available() > 0) {
    Serial << endl << F("UART Raw data: ");
  }
#endif

  if (UART->SerialBus.available() > 0) {

    while (UART->SerialBus.available() > 0 && index < responseSize) {
      responseBuffer[index] = UART->SerialBus.read();

#ifdef DEBUG
      Serial << F(" ");
      Serial.print(responseBuffer[index], HEX);
#endif

      /* The first one should a Head if not the skip */
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
        responseSize = 32;
      }

      /* Setting response size: normal read */
      if (responseBuffer[0] == 0x40 && responseBuffer[2] == 0x4 &&
          responseSize != 8) { // Reading
        responseSize = 8;
      }

      if (((responseBuffer[0] == 0x96 && responseBuffer[1] == 0x96) ||
           (responseBuffer[0] == 0xA5 && responseBuffer[1] == 0xA5)) &&
          responseSize != 2) {
        responseSize = 2;
      }

      if (responseBuffer[0] == 0x96 && index == 2 &&
          responseBuffer[1] != 0x96) {
        if (responseBuffer[1] == 0x40 || responseBuffer[1] == 0x42 ||
            responseBuffer[1] == 0xA5) {
          responseBuffer[0] = responseBuffer[1];
          index = 1;
        } else {
          index = 0;
        }
        responseSize = 32;
      }
    }

    if (index > 0) {

#ifdef DEBUG
      Serial << endl << F("UART data to process:");
      for (index = 0; index < responseSize; index++) {
        Serial << F(" ");
        Serial.print(responseBuffer[index], HEX);
      }
      Serial << endl << F("Message: ");
#endif

      /* Check if ACK response */
      if (responseBuffer[0] == 0xA5 && responseBuffer[1] == 0xA5) {
#ifdef DEBUG
        Serial << F(" Success (ACK)");
#endif
        return true;
      }

      /* Check if NACK response */
      if (responseBuffer[0] == 0x96 && responseBuffer[1] == 0x96) {
#ifdef DEBUG
        Serial << F(" Failure (NACK)");
#endif
        return false;
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
          buffer->pm25 = responseBuffer[3] * 256 + responseBuffer[4];
          buffer->pm10 = responseBuffer[5] * 256 + responseBuffer[6];
#ifdef DEBUG
          Serial << F(" Success (PM2.5=") << buffer->pm25 << F("ug/m3, ")
                 << F("PM10=") << buffer->pm10 << F("ug/m3)");
#endif
          return expectingACK ? false : true;
        } else {

#ifdef DEBUG
          Serial << F(" Failure (Checksum mismatch)");
#endif
          return false;
        }
      }

      if (responseBuffer[3] == 0x1C) { // Autoreading
        int checkSumToCompare = 0;
        uint8_t counter;
        // DATA
        for (counter = 0; counter <= responseBuffer[3]; counter++) {
          // Serial.println(buf[c]);
          checkSumToCompare += responseBuffer[counter];
        }
        int checksum = responseBuffer[30] * 256 + responseBuffer[31];

        if (checkSumToCompare == checksum) {
          // calculate PM values
          buffer->pm25 = responseBuffer[6] * 256 + responseBuffer[7];
          buffer->pm10 = responseBuffer[8] * 256 + responseBuffer[9];
#ifdef DEBUG
          Serial << F(" Success (PM2.5=") << buffer->pm25 << F("ug/m3, ")
                 << F("PM10=") << buffer->pm10 << F("ug/m3)");
#endif
          return expectingACK ? false : true;
        } else {
#ifdef DEBUG
          Serial << F(" Failure (Checksum mismatch)");
#endif
        }
      }
    }

#ifdef DEBUG
    Serial << F(" Failure (Wrong or no response)");
#endif
    return false;
  }
#ifdef DEBUG
  else {
    Serial << F(" Failure (TimeOut)");
  }
#endif

  return false;
}

boolean AFESensorHPMA115S0::sendCommand(const uint8_t *command,
                                        uint8_t howManyTimesRetry) {

  uint8_t counter = 0;
  UART->send(command);
  boolean _ret = read(true);

#ifdef DEBUG
  Serial << endl
         << (_ret ? F("UART: No need to retry")
                  : F("UART: Command not accepted"));
#endif

  if (!_ret) {
    if (howManyTimesRetry > 0) {
      while (!_ret && counter < howManyTimesRetry) {
        // UART->send(commandAutoOFF);
        UART->send(command);
        _ret = read(true);
        counter++;
      }
    }
  }
  return _ret;
}

boolean AFESensorHPMA115S0::listener() {
  boolean ready = false;
  if (_initialized) {
    if ((millis() - startTime >= configuration->interval * 1000) &&
        _measuremntsON) {
#if defined(DEBUG)
      Serial << endl << endl << F("----- HPMA115S0: Reading -----");
      Serial << endl << F("Time: ") << (millis() - startTime) / 1000 << F("s");
#endif
      startTime = millis();

//      UART->send(commandTurnON);
//      read() ? _measuremntsON = true : _measuremntsON = false;

#if defined(DEBUG)
      Serial << endl
             << F("Device is: ") << (_measuremntsON ? F("ON") : F("OFF"));
#endif

      UART->send(commandRead);
      if (read()) {
        data->pm25 = buffer->pm25;
        data->pm10 = buffer->pm10;
        if (data->pm25 != 0 || data->pm10 != 0) {
          data->whoPM25Norm = configuration->whoPM25Norm > 0
                                 ? 100 / configuration->whoPM25Norm * data->pm25
                                 : 0;
          data->whoPM10Norm = configuration->whoPM10Norm > 0
                                 ? 100 / configuration->whoPM10Norm * data->pm10
                                 : 0;

          ready = true;
        }
      }

      /* Put a sensor to a sleep mode */
      if (configuration->timeToMeasure > 0) {
        UART->send(commandTurnOFF);
        read(true) ? _measuremntsON = false : _measuremntsON = true;
      }
#if defined(DEBUG)
      Serial << endl
             << F("Device is: ") << (_measuremntsON ? F("ON") : F("OFF"));
      Serial << endl << F("------------------------------");
#endif
    }
    /* Wake up sensor */
    else if (millis() - startTime >=
                 ((configuration->interval - configuration->timeToMeasure) *
                  1000) &&
             !_measuremntsON) {

#ifdef DEBUG
      Serial << endl << endl << F("----- HPMA115S0: Turning ON -----");
#endif
      if (!_measuremntsON) {
        UART->send(commandTurnON);
        read(true) ? _measuremntsON = true : _measuremntsON = false;
      }
#if defined(DEBUG)
      Serial << endl
             << F("Device is: ") << (_measuremntsON ? F("ON") : F("OFF"));
      Serial << endl << F("---------------------------------");
#endif
    }
  }
  return ready;
}

void AFESensorHPMA115S0::getJSON(char *json) {
  sprintf(
      json, "{\"PM25\":{\"value\":%.2f,\"unit\":\"%s\"},\"PM10\":{\"value\":"
            "%.2f,\"unit\":\"%s\"},\"WHO\":{\"PM25\":{\"value\":%.2f,\"unit\":"
            "\"%%\"},\"PM10\":{"
            "\"value\":%.2f,\"unit\":\"%%\"}}}",
      data->pm25, AFE_UNIT_PARTICLE, data->pm10, AFE_UNIT_PARTICLE,
      data->whoPM25Norm, data->whoPM10Norm);
}

#endif // AFE_CONFIG_HARDWARE_HPMA115S0