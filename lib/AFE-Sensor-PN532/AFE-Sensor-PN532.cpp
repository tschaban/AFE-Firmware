/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-Sensor-PN532.h"

#ifdef AFE_CONFIG_HARDWARE_PN532

AFESensorPN532::AFESensorPN532(){};

void AFESensorPN532::begin(uint8_t id, AFEDataAccess *_Data) {

  PN532UARTInterface.setSerial(13, 12);
  NFCReader.setInterface(PN532UARTInterface);

  Serial << endl << "Looking for PN532...";

  NFCReader.begin();

  uint32_t versiondata = NFCReader.getFirmwareVersion();
  if (!versiondata) {
    Serial << endl << "Didn't find PN53x board";
    return;
  }

  // Got ok data, print it out!
  Serial << endl << "Found chip PN5" << _HEX((versiondata >> 24) & 0xFF);
  Serial << endl
         << "Firmware ver. " << _DEC((versiondata >> 16) & 0xFF) << "."
         << _DEC((versiondata >> 8) & 0xFF);

  Serial << endl;
  // configure board to read RFID tags
  NFCReader.SAMConfig();

  _initialized = true;
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
           << " - with keyId: " << AFE_HARDWARE_PN532_SENSOR_KEY_A;
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
  uint8_t found = NFCReader.readPassiveTargetID(PN532_MIFARE_ISO14443A, cardUID,
                                                &cardUIDLength);

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

  return _ret;
}

boolean AFESensorPN532::listener() {
  if (!_tagInCache) {
    _tagInCache = readSector(2, tag);
  }
  return _tagInCache;
}

void AFESensorPN532::getJSON(char *json) {
  sprintf(json,
          "{\"tag1\":\"%s\",\"tag2\":\"%s\",\"tag3\":\"%s\",\"tag4\":\"%s\"}",
          tag.block[0].value, tag.block[1].value, tag.block[2].value,
          tag.block[3].value);
          _tagInCache = false;
}

void AFESensorPN532::formattingNFC() {
  const char *url = "smartnydom.pl";
  uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

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
}

void AFESensorPN532::formattingClassic() {
  uint8_t blockBuffer[AFE_HARDWARE_PN532_BLOCK_SIZE]; // Buffer to store block
                                                      // contents
  uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
  uint8_t idx = 0;

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
          Serial << endl
                 << "ERROR: PN532: Unable to write to sector: " << idx
                 << ", block: " << (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1;
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
}

void AFESensorPN532::readNFC() {
  uint8_t currentblock;       // Counter to keep track of which block we're on
  bool authenticated = false; // Flag to indicate if the sector is authenticated
  uint8_t data[AFE_HARDWARE_PN532_BLOCK_SIZE]; // Array to store block data
                                               // during reads

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
        Serial << endl;
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
}

void AFESensorPN532::readBlock(uint8_t blockId, String &data) {
  uint8_t _data[AFE_HARDWARE_PN532_BLOCK_SIZE];
  char _text[AFE_HARDWARE_PN532_BLOCK_SIZE];

  if (foundCard()) {
    if (authenticatedBlock(blockId)) {

#ifdef DEBUG
      Serial << "INFO: PN532: Reading block: " << blockId << endl;
#endif
      if (NFCReader.mifareclassic_ReadDataBlock(blockId, _data)) {

        for (uint8_t i = 0; i < AFE_HARDWARE_PN532_BLOCK_SIZE; i++) {
          _text[i] = _data[i];
        }

        data = _text;

#ifdef DEBUG
        NFCReader.PrintHexChar(_data, AFE_HARDWARE_PN532_BLOCK_SIZE);
      } else {
        Serial << "WARN: PN532: Something went wrong with card reading";
#endif
      }
    }
  }
}

void AFESensorPN532::writeBlock(uint8_t blockId, const char *data) {
  if (foundCard()) {
    if (authenticatedBlock(blockId)) {

#ifdef DEBUG
      Serial << endl
             << "INFO: PN532: Writing to block: " << blockId
             << ", text: " << data;
#endif

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

      if (!NFCReader.mifareclassic_WriteDataBlock(blockId, _data)) {
#ifdef DEBUG
        Serial << endl << "ERROR: PN532: Problem with saving data";
#endif
      }
    }
  }
}

boolean AFESensorPN532::readSector(uint8_t sectorId, PN532_SECTOR &data) {
  boolean _ret = false;

  if (foundCard()) {
    uint8_t _data[AFE_HARDWARE_PN532_BLOCK_SIZE];

#ifdef DEBUG
    Serial << "INFO: PN532: Reading sector: " << sectorId << endl;
#endif

    for (uint8_t i = 0; i < AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR;
         i++) {
      if (authenticatedBlock(
              sectorId * AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR + i)) {

        if (NFCReader.mifareclassic_ReadDataBlock(
                sectorId * AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR + i,
                _data)) {

          for (uint8_t idx = 0; idx < AFE_HARDWARE_PN532_BLOCK_SIZE; idx++) {
            data.block[i].value[idx] = _data[idx];
          }

          Serial << " :["
                 << sectorId * AFE_HARDWARE_PN532_NUMBER_OF_BLOCKS_PER_SECTOR +
                        i
                 << "] : " << data.block[i].value;

#ifdef DEBUG
          NFCReader.PrintHexChar(_data, AFE_HARDWARE_PN532_BLOCK_SIZE);
        } else {
          Serial << "WARN: PN532: Something went wrong with card reading";
#endif
        }
      }
    }
    _ret = true;
  }
  return _ret;
}

void AFESensorPN532::get(PN532_SECTOR &data) {
  data = tag;
  _tagInCache = false;
}

#endif // AFE_CONFIG_HARDWARE_PN532
