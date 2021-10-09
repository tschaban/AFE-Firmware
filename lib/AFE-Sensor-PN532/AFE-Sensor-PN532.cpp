/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-PN532.h"

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

AFESensorPN532::AFESensorPN532(){};

#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
void AFESensorPN532::begin(uint8_t _id, AFEDataAccess *_Data,
                           AFEDevice *_Device, TwoWire *WirePort0,
                           TwoWire *WirePort1) {
  _WirePort0 = WirePort0;
  _WirePort1 = WirePort1;
  begin(_id, _Data, _Device);
}
#else
void AFESensorPN532::begin(uint8_t _id, AFEDataAccess *_Data,
                           AFEDevice *_Device, TwoWire *WirePort0) {
  _WirePort0 = WirePort0;
  begin(_id, _Data, _Device);
}
#endif // AFE_ESP32
#endif // AFE_CONFIG_HARDWARE_I2C

void AFESensorPN532::begin(uint8_t _id, AFEDataAccess *_Data,
                           AFEDevice *_Device) {

#ifdef DEBUG
  Serial << endl << "INFO: PN532: Initializing PN532";
#endif

  Data = _Data;
  Data->getConfiguration(_id, &configuration);

#ifdef DEBUG
  Serial << endl << "INFO: PN532: Configuration";
  Serial << endl
         << " - Interface: "
         << (configuration.interface == AFE_HARDWARE_PN532_INTERFACE_IIC
                 ? "I2C"
                 : "UART");
  if (configuration.interface == AFE_HARDWARE_PN532_INTERFACE_UART) {
    Serial << endl << " - UART RX: " << configuration.rx;
    Serial << endl << " - UART TX: " << configuration.tx;
  }
  Serial << endl
         << " - Processing time: " << configuration.requestProcessingTime
         << "msec";
  Serial << endl
         << " - Listener time: " << configuration.listenerTimeout << "msec";

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
#else
  Serial << endl << " - MQTT Topic: " << configuration.mqtt.topic;
#endif
#endif // DEBUG

  if (configuration.interface == AFE_HARDWARE_PN532_INTERFACE_UART) {
    PN532UARTInterface.setSerial(configuration.rx, configuration.tx);
    NFCReader.setInterface(PN532UARTInterface);
    _initialized = true;
  }
#ifdef AFE_CONFIG_HARDWARE_I2C
  else if (configuration.interface == AFE_HARDWARE_PN532_INTERFACE_IIC) {
#ifdef AFE_ESP32
    _WirePort0 = configuration.wirePortId == 0 ? _WirePort0 : _WirePort1;
#endif

    if (
#ifdef AFE_ESP32
        configuration.wirePortId != AFE_HARDWARE_ITEM_NOT_EXIST &&
#endif
        configuration.i2cAddress != 0) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: PN532: Address: 0x") << _HEX(configuration.i2cAddress);
#endif

      PN532I2CInterface.begin(configuration.i2cAddress, _WirePort0);
      NFCReader.setInterface(PN532I2CInterface);
      _initialized = true;
    }
  }
#endif // AFE_CONFIG_HARDWARE_I2C

  if (_initialized) {

#ifdef DEBUG
    Serial << endl << "INFO: PN532: Looking for PN532...";
#endif

    NFCReader.begin();

    uint32_t versiondata = NFCReader.getFirmwareVersion();
    if (!versiondata) {
#ifdef DEBUG
      Serial << endl << "INFO: PN532: Didn't find PN53x board";
#endif
      _initialized = false;
      return;
    }

    NFCReader.setPassiveActivationRetries(10);

// Got ok data, print it out!
#ifdef DEBUG
    Serial << endl << "Found chip PN5" << _HEX((versiondata >> 24) & 0xFF);
    Serial << endl
           << "Firmware ver. " << _DEC((versiondata >> 16) & 0xFF) << "."
           << _DEC((versiondata >> 8) & 0xFF) << endl;
#endif

    // configure board to read RFID tags
    NFCReader.SAMConfig();

#ifdef AFE_CONFIG_HARDWARE_LED
    if (configuration.ledID != AFE_HARDWARE_ITEM_NOT_EXIST &&
        _Device->configuration.noOfLEDs >= configuration.ledID) {
      Led.begin(Data, configuration.ledID);
    }
#endif
  }
  
}

boolean AFESensorPN532::authenticatedBlock(uint32_t blockId) {
#ifdef DEBUG
  Serial << endl
         << "INFO: PN532: Autenticating : " << blockId << endl
         << " - with keyId: " << AFE_HARDWARE_PN532_SENSOR_KEY_B
         << ", status: ";
#endif
  boolean authenticated = NFCReader.mifareclassic_AuthenticateBlock(
      cardUID, cardUIDLength, blockId, AFE_HARDWARE_PN532_SENSOR_KEY_B,
      (uint8_t *)KEY_DEFAULT_KEYAB);

  if (!authenticated) {

#ifdef DEBUG
    Serial << "access denied" << endl
           << " - with keyId: " << AFE_HARDWARE_PN532_SENSOR_KEY_A
           << ", status: ";
#endif
    authenticated = NFCReader.mifareclassic_AuthenticateBlock(
        cardUID, cardUIDLength, blockId, AFE_HARDWARE_PN532_SENSOR_KEY_A,
        (uint8_t *)KEY_DEFAULT_KEYAB);
  }

#ifdef DEBUG
  Serial << (authenticated ? "access granted" : "access denied");
#endif

  return authenticated;
}

boolean AFESensorPN532::foundCard() {
  boolean _ret = false;
  uint8_t found = NFCReader.readPassiveTargetID(
      PN532_MIFARE_ISO14443A, cardUID, &cardUIDLength, configuration.timeout);
  yield();
  if (found) {
#ifdef DEBUG
    Serial << endl
           << "INFO: PN532: Found an ISO14443A card:  UID Length: "
           << _DEC(cardUIDLength) << " bytes,  UID Value: ";
    NFCReader.PrintHex(cardUID, cardUIDLength);
    for (uint8_t i = 0; i < cardUIDLength; i++) {
      Serial << _HEX(cardUID[i]) << " ";
    }
#endif
    if (cardUIDLength == 4) {
      _ret = true;
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "WARN: PN532: Card doesn't seem to be a Mifare Classic card!";
    }
#endif
  }
#ifdef DEBUG
  Serial << ".";
#endif
  return _ret;
}

uint8_t AFESensorPN532::listener() {
  if (_initialized) {
    if (_requestTime == 0) {
      if (_listenerTime == 0) {
        _listenerTime = millis();
      }
      if (millis() - _listenerTime > configuration.listenerTimeout) {
        Led.on();
        _request = foundCard();
        Led.off();
        _listenerTime = 0;
      }
      if (_request) {
        _requestTime = millis();
        Led.on();
        return AFE_HARDWARE_PN532_LISTENER_EVENT_FOUND;
      }
    } else {
      if (millis() - _requestTime > configuration.requestProcessingTime) {
        _requestTime = 0;
        return AFE_HARDWARE_PN532_LISTENER_EVENT_PROCESSING_FINISHED;
      } else {
        return AFE_HARDWARE_PN532_LISTENER_EVENT_PROCESSING;
      }
    }
  }
  return AFE_HARDWARE_PN532_LISTENER_EVENT_WAITING;
}

void AFESensorPN532::getJSON(char *json) {
  sprintf(json, "{\"tag1\":\"%s\",\"tag2\":\"%s\",\"tag3\":\"%s\",\"tag4\":\"%"
                "s\",\"tag5\":\"%s\",\"tag6\":\"%s\"}",
          tag.block[0].value, tag.block[1].value, tag.block[2].value,
          tag.block[3].value, tag.block[4].value, tag.block[5].value);
}

void AFESensorPN532::formattingNFC() {
  const char *url = "smartnydom.pl";
  uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;
  Led.on();
  if (foundCard()) {
    if (authenticatedBlock(0)) {
      if (NFCReader.mifareclassic_FormatNDEF()) {
#ifdef DEBUG
        Serial
            << endl
            << "INFO: PN532: Card has been formatted for NDEF data using MAD1";
#endif
        if (authenticatedBlock(4)) {
          if (strlen(url) <= 38) {
            if (NFCReader.mifareclassic_WriteNDEFURI(1, ndefprefix, url)) {
#ifdef DEBUG
              Serial << endl
                     << "INFO: PN532: NDEF URI Record written to sector 1";
            } else {
              Serial << endl << "ERROR: PN532: NDEF Record creation failed! :(";
#endif
            }
          }
#ifdef DEBUG
          else {
            Serial << endl << "ERROR: PN532: Tag text is too long";
          }
#endif
        }
      }
#ifdef DEBUG
      else {
        Serial << endl << "ERROR: PN532: Unable to format the card for NDEF";
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << "WARN: PN532: Card couldn't be formatted";
    }
#endif
  }
  Led.off();
}

void AFESensorPN532::formattingClassic() {
  uint8_t blockBuffer[AFE_HARDWARE_PN532_BLOCK_SIZE]; // Buffer to store block
                                                      // contents
  uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
  uint8_t idx = 0;
  Led.on();
  if (foundCard()) {
#ifdef DEBUG
    Serial << endl << "INFO: PN532: Reformatting card for Mifare Classic... ";
#endif
    // Now run through the card sector by sector
    for (idx = 0; idx < AFE_HARDWARE_PN532_NUMBER_OF_SECTORS; idx++) {

#ifdef DEBUG
      Serial << endl << endl << "INFO: PN532: Formatting sector: " << idx;
#endif

      if (authenticatedBlock(BLOCK_NUMBER_OF_SECTOR_TRAILER(idx))) {

        // Step 2: Write to the other blocks
        if (idx == AFE_HARDWARE_PN532_NUMBER_OF_SECTORS) {
          memset(blockBuffer, 0, sizeof(blockBuffer));
          if (!(NFCReader.mifareclassic_WriteDataBlock(
                  (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
#ifdef DEBUG
            Serial << endl
                   << "ERROR: PN532: Unable to write to sector: " << idx
                   << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3;
#endif
            return;
          }
        } // 16 sector

        if ((idx == 0) || (idx == AFE_HARDWARE_PN532_NUMBER_OF_SECTORS)) {
          memset(blockBuffer, 0, sizeof(blockBuffer));
          if (!(NFCReader.mifareclassic_WriteDataBlock(
                  (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
#ifdef DEBUG
            Serial << endl
                   << "ERROR: PN532: Unable to write to sector: " << idx
                   << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2;
#endif
            return;
          }
        }    /* End: sector 0 and 16 */
        else /* Sectors 1 .. 16 */
        {
          memset(blockBuffer, 0, sizeof(blockBuffer));
          if (!(NFCReader.mifareclassic_WriteDataBlock(
                  (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
#ifdef DEBUG
            Serial << endl
                   << "ERROR: PN532: Unable to write to sector: " << idx
                   << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3;
#endif
            return;
          }
          if (!(NFCReader.mifareclassic_WriteDataBlock(
                  (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
#ifdef DEBUG
            Serial << endl
                   << "ERROR: PN532: Unable to write to sector: " << idx
                   << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2;
#endif
            return;
          }
        }

        /* all sectors */
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer))) {
#ifdef DEBUG
          Serial << endl
                 << "ERROR: PN532: Unable to write to sector: " << idx
                 << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1;
#endif
          return;
        }

        // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
        memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
        memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
        blockBuffer[9] = 0x69;
        memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

        // Step 4: Write the trailer block
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer))) {
#ifdef DEBUG
          Serial << endl
                 << "ERROR: PN532: Unable to write trailer block of sector: "
                 << idx << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx));
#endif
          return;
        }
#ifdef DEBUG
        Serial << endl << "INFO: PN532: Sector: " << idx << " formatted";
#endif
      }
    }
  }
  Led.off();
}

#ifdef DEBUG
void AFESensorPN532::readNFC() {
  uint8_t currentblock;       // Counter to keep track of which block we're on
  bool authenticated = false; // Flag to indicate if the sector is authenticated
  uint8_t data[AFE_HARDWARE_PN532_BLOCK_SIZE]; // Array to store block data
                                               // during reads
  Led.on();

  if (foundCard()) {
    for (currentblock = 0; currentblock < AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS;
         currentblock++) {
      if (NFCReader.mifareclassic_IsFirstBlock(currentblock))
        authenticated = false;
      if (!authenticated) {
#ifdef DEBUG
        Serial << endl
               << "------------------------Sector "
               << _DEC(currentblock /
                       AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR)
               << "-------------------------";
#endif
        authenticated = authenticatedBlock(currentblock);
#ifdef DEBUG
        Serial << endl;
#endif
      }

      if (authenticated) {
        if (NFCReader.mifareclassic_ReadDataBlock(currentblock, data)) {
#ifdef DEBUG
          Serial << "Block " << _DEC(currentblock)
                 << (currentblock < 10 ? "  " : " ");
          NFCReader.PrintHexChar(data, AFE_HARDWARE_PN532_BLOCK_SIZE);
#endif
        }
#ifdef DEBUG
        else {
          Serial << "ERROR: PN532: Block " << currentblock
                 << " unable to read this block" << endl;
        }
#endif
      }
    }
  }

  Led.off();
}
#endif

boolean AFESensorPN532::readBlock(uint8_t blockId, char *data,
                                  boolean lookForCard) {
  boolean _ret = false;
  uint8_t _data[AFE_HARDWARE_PN532_BLOCK_SIZE + 1];
  data[0] = AFE_EMPTY_STRING;

  if (lookForCard) {
    if (!foundCard()) {
      return _ret;
    }
  }

  if (authenticatedBlock(blockId)) {

#ifdef DEBUG
    Serial << endl << "INFO: PN532: Reading block: " << blockId << endl;
#endif
    if (NFCReader.mifareclassic_ReadDataBlock(blockId, _data)) {

      for (uint8_t i = 0; i < AFE_HARDWARE_PN532_BLOCK_SIZE; i++) {
        data[i] = _data[i];
      }
      data[strlen(data) + 1] = AFE_EMPTY_STRING;
      _ret = true;
#ifdef DEBUG
      NFCReader.PrintHexChar(_data, AFE_HARDWARE_PN532_BLOCK_SIZE);
    } else {
      Serial << endl << "WARN: PN532: Something went wrong with card reading";
#endif
    }
  }
  return _ret;
}

void AFESensorPN532::writeBlock(uint8_t blockId, const char *data) {
  Led.on();
  if (strlen(data) > AFE_HARDWARE_PN532_BLOCK_SIZE) {
#ifdef DEBUG
    Serial << endl << "ERROR: PN532: Too long string to save in the block";
#endif
    return;
  }

  uint8_t _data[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (uint8_t i = 0; i < strlen(data); i++) {
    _data[i] = data[i];
  }

  if (foundCard()) {
    if (authenticatedBlock(blockId)) {
#ifdef DEBUG
      Serial << endl
             << "INFO: PN532: Writing to primary block: " << blockId
             << ", text: " << data;
#endif

      if (!NFCReader.mifareclassic_WriteDataBlock(blockId, _data)) {
#ifdef DEBUG
        Serial << endl
               << "ERROR: PN532: Problem with saving data primary block";
#endif
      }
      delay(10);
    }

    Led.off();

    if (authenticatedBlock(blockId +
                           AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR)) {
#ifdef DEBUG
      Serial << endl
             << "INFO: PN532: Writing to backup block: "
             << blockId + AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR
             << ", text: " << data;
#endif

      if (!NFCReader.mifareclassic_WriteDataBlock(
              blockId + AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR,
              _data)) {
#ifdef DEBUG
        Serial << endl
               << "ERROR: PN532: Problem with saving data to backup block";
#endif
      }
      delay(10);
    }
  }
}

boolean AFESensorPN532::readTag() {
  boolean _ret = false;
  boolean _success = false;
  if (foundCard()) {

#ifdef DEBUG
    Serial << "INFO: PN532: Reading Tag" << endl;
#endif

    for (uint8_t i = 0;
         i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
      _success = readBlock(AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i,
                           tag.block[i].value, false);
      /* Reading backup block if there was a problem with reading the
         primary */
      if (!_success) {
        _success = readBlock(AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i +
                                 AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR,
                             tag.block[i].value, false);
      }
#ifdef DEBUG
      if (_success) {
        Serial << " :[" << AFE_HARDWARE_PN532_FIRST_TAG_FIRST_BLOCK + i
               << "] : " << tag.block[i].value;
      }
#endif
    }

    if (_success) {

      for (uint8_t i = 0;
           i < AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR; i++) {
        _success = readBlock(AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i,
                             tag.block[i + 3].value, false);
        /* Reading backup block if there was a problem with reading the
         * primary
         */
        if (!_success) {
          _success =
              readBlock(AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i +
                            AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR,
                        tag.block[i + 3].value, false);
        }

        if (_success) {
#ifdef DEBUG
          Serial << " :[" << AFE_HARDWARE_PN532_FIRST_TAG_SECOND_BLOCK + i
                 << "] : " << tag.block[i + 3].value;
#endif
          if (i ==
              AFE_HARDWARE_PN532_NUMBER_OF_WRITABLE_BLOCKS_PER_SECTOR - 1) {
            _ret = true;
          }
        }
      }
    }
  }
  return _ret;
}

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
