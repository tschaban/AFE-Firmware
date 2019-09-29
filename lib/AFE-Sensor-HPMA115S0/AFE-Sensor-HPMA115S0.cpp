/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-HPMA115S0.h"

AFESensorHPMA115S0::AFESensorHPMA115S0(){};

void AFESensorHPMA115S0::begin(uint8_t id) {
  AFEDataAccess Data;
  configuration = Data.getHPMA115S0SensorConfiguration(id);

  if (strlen(configuration.mqtt.topic) > 0) {
    sprintf(mqttCommandTopic, "%s/cmd", configuration.mqtt.topic);
  } else {
    mqttCommandTopic[0] = '\0';
  }

  /* Opening Serial port */
  UART.begin();
  //  UART.SerialBus.flush();
  _initialized = true;

  data.pm10 = data.pm25 = buffer.pm10 = buffer.pm25 = 0;

#if defined(DEBUG)
  Serial << endl << endl << "----- HPMA115S0: Initializing -----";
  Serial << endl << "Cleaning serial. Size=" << UART.SerialBus.available();
#endif
  UART.clean();

#if defined(DEBUG)
  Serial << endl << "Turning Autosending: OFF";
#endif

  /* Turn off autosending */
  sendCommand(commandAutoOFF);

#if defined(DEBUG)
  Serial << endl << "Cleaning serial. Size=" << UART.SerialBus.available();
#endif

  /* Clean buffer */
  UART.clean();

  /* Turning on / off the device */
  if (configuration.timeToMeasure == 0) {
    sendCommand(commandTurnON) ? _measuremntsON = true : _measuremntsON = false;
  } else {
    sendCommand(commandTurnOFF) ? _measuremntsON = true
                                : _measuremntsON = false;
  }

#if defined(DEBUG)
  Serial << endl << "Device is: " << (_measuremntsON ? "ON" : "OFF");
  Serial << endl << "-----------------------------------";
#endif
}

boolean AFESensorHPMA115S0::read(boolean expectingACK) {
  uint8_t index = 0;
  uint8_t responseBuffer[32];
  uint8_t responseSize = 32;
  unsigned long start = millis(); /* Used for counting time to timeout */

#ifdef DEBUG
  Serial << endl
         << "UART: Size of a data in the buffer ="
         << UART.SerialBus.available();
#endif
  
  UART.SerialBus.print("UART: Size of a data in the buffer =");
  UART.SerialBus.println(UART.SerialBus.available());


  /* Wait for a data from UART. Max 1 sec */
  while (UART.SerialBus.available() == 0 && millis() - start < 1000) {
  }

#ifdef DEBUG
  if (UART.SerialBus.available() > 0) {
    Serial << endl << "UART Raw data: ";
  }
#endif

  if (UART.SerialBus.available() > 0) {

    while (UART.SerialBus.available() > 0 && index < responseSize) {
      responseBuffer[index] = UART.SerialBus.read();

#ifdef DEBUG
      Serial << " ";
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
      Serial << endl << "UART data to process:";
      for (index = 0; index < responseSize; index++) {
        Serial << " ";
        Serial.print(responseBuffer[index], HEX);
      }
      Serial << endl << "Message: ";
#endif

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
          buffer.pm25 = responseBuffer[3] * 256 + responseBuffer[4];
          buffer.pm10 = responseBuffer[5] * 256 + responseBuffer[6];
#ifdef DEBUG
          Serial << " Success (PM2.5=" << buffer.pm25 << "ug/m3, "
                 << "PM10=" << buffer.pm10 << "ug/m3)";
#endif
          return expectingACK ? false : true;
        } else {

#ifdef DEBUG
          Serial << " Failure (Checksum mismatch)";
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
          buffer.pm25 = responseBuffer[6] * 256 + responseBuffer[7];
          buffer.pm10 = responseBuffer[8] * 256 + responseBuffer[9];
#ifdef DEBUG
          Serial << " Success (PM2.5=" << buffer.pm25 << "ug/m3, "
                 << "PM10=" << buffer.pm10 << "ug/m3)";
#endif
          return expectingACK ? false : true;
        } else {
#ifdef DEBUG
          Serial << " Failure (Checksum mismatch)";
#endif
        }
      }
    }

#ifdef DEBUG
    Serial << " Failure (Wrong or no response)";
#endif
    return false;
  }
#ifdef DEBUG
  else {
    Serial << " Failure (TimeOut)";
  }
#endif

  return false;
}

boolean AFESensorHPMA115S0::sendCommand(const uint8_t *command,
                                        uint8_t howManyTimesRetry) {

  uint8_t counter = 0;
  UART.send(command);
  boolean _ret = read(true);

#ifdef DEBUG
  Serial << endl
         << (_ret ? "UART: No need to retry" : "UART: Command not accepted");
#endif

  if (!_ret) {
    if (howManyTimesRetry > 0) {
      while (!_ret && counter < howManyTimesRetry) {
        UART.send(commandAutoOFF);
        _ret = read(true);
        counter++;
      }
    }
  }
  return _ret;
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
#if defined(DEBUG)
      Serial << endl << endl << "----- HPMA115S0: Reading -----";
      Serial << endl << "Time: " << (millis() - startTime) / 1000 << "s";
#endif
      startTime = millis();

//      UART.send(commandTurnON);
//      read() ? _measuremntsON = true : _measuremntsON = false;

#if defined(DEBUG)
      Serial << endl << "Device is: " << (_measuremntsON ? "ON" : "OFF");
#endif

      UART.send(commandRead);
      if (read()) {
        data.pm25 = buffer.pm25;
        data.pm10 = buffer.pm10;
        if (data.pm25 != 0 || data.pm10 != 0) {
          ready = true;
        }
      }

      /* Put a sensor to a sleep mode */
      if (configuration.timeToMeasure > 0) {
        UART.send(commandTurnOFF);
        read(true) ? _measuremntsON = false : _measuremntsON = true;
      }
#if defined(DEBUG)
      Serial << endl << "Device is: " << (_measuremntsON ? "ON" : "OFF");
      Serial << endl << "------------------------------";
#endif
    }
    /* Wake up sensor */
    else if (millis() - startTime >=
                 ((configuration.interval - configuration.timeToMeasure) *
                  1000) &&
             !_measuremntsON) {

#if defined(DEBUG)
      Serial << endl << endl << "----- HPMA115S0: Turning ON -----";
#endif
      if (!_measuremntsON) {
        UART.send(commandTurnON);
        read(true) ? _measuremntsON = true : _measuremntsON = false;
      }
#if defined(DEBUG)
      Serial << endl << "Device is: " << (_measuremntsON ? "ON" : "OFF");
      Serial << endl << "---------------------------------";
#endif
    }
  }
}

void AFESensorHPMA115S0::getJSON(char *json) {

  sprintf(json, "{\"PM25\":{\"value\":%d,\"unit\":\"µg/m3\"},\"PM10\":{\"value\":"
                "%d,\"unit\":\"µg/m3\"}}",
          data.pm25, data.pm10);
}