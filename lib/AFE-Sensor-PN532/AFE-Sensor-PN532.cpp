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
  boolean authenticated = NFCReader.mifareclassic_AuthenticateBlock(
      cardUID, cardUIDLength, blockId, AFE_HARDWARE_PN532_SENSOR_DEFAULT_KEY,
      (uint8_t *)KEY_DEFAULT_KEYAB);

#ifdef DEBUG
  Serial << endl
         << "INFO: PN532: Block: " << blockId << " => "
         << (authenticated ? "" : "NOT ") << "authenticated";
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

boolean AFESensorPN532::listener() { return false; }

void AFESensorPN532::getJSON(char *json) {}

void AFESensorPN532::formattingNFC() {

  const char *url = "smartnydom.pl";
  uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

  uint8_t success; // Flag to check if there was an error with the PN532

  success =

#ifdef DEBUG
      Serial << endl
             << "INFO: PN532: Formatting card";
#endif

  if (foundCard()) {
    if (authenticatedBlock(0)) {

      if (NFCReader.mifareclassic_FormatNDEF()) {
#ifdef DEBUG
        Serial
            << endl
            << "INFO: PN532: Card has been formatted for NDEF data using MAD1";
#endif


// code after formatting

      }
#ifdef DEBUG
      else {
        Serial << endl
               << "ERROR: PN532: Unable to format the card for NDEF"
                  "formatting!";
      }
#endif

    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: PN532: Unable to authenticate block 0 to enable card"
                "formatting!";
    }
#endif
  }



  // Try to authenticate block 4 (first block of sector 1) using our key
  success = NFCReader.mifareclassic_AuthenticateBlock(
      cardUID, cardUIDLength, 4, 1, (uint8_t *)KEY_DEFAULT_KEYAB);

  // Make sure the authentification process didn't fail
  if (!success) {
    Serial << endl << "Authentication failed.";
    return;
  }

  // Try to write a URL
  Serial << endl << "Writing URI to sector 1 as an NDEF Message";

  // Authenticated seems to have worked
  // Try to write an NDEF record to sector 1
  // Use 0x01 for the URI Identifier Code to prepend "http://www."
  // to the url (and save some space).  For information on URI ID Codes
  // see http://www.ladyada.net/wiki/private/articlestaging/NFCReader/ndef
  if (strlen(url) > 38) {
    // The length is also checked in the WriteNDEFURI function, but lets
    // warn users here just in case they change the value and it's bigger
    // than it should be
    Serial << endl
           << "URI is too long ... must be less than 38 characters long";
    return;
  }

  // URI is within size limits ... write it to the card and report
  // success/failure
  success = NFCReader.mifareclassic_WriteNDEFURI(1, ndefprefix, url);
  if (success) {
    Serial << endl << "NDEF URI Record written to sector 1";
  } else {
    Serial << endl << "NDEF Record creation failed! :(";
  }

  success = NFCReader.mifareclassic_AuthenticateBlock(
      cardUID, cardUIDLength, 8, 1, (uint8_t *)KEY_DEFAULT_KEYAB);

  success = NFCReader.mifareclassic_WriteNDEFURI(2, NDEF_URIPREFIX_TEL,
                                                 "48505197305");
  if (success) {
    Serial << endl << "NDEF writing phone";
  } else {
    Serial << endl << "NDEF Record writing phone creation failed! :(";
  }

  success = NFCReader.mifareclassic_AuthenticateBlock(
      cardUID, cardUIDLength, 20, 1, (uint8_t *)KEY_DEFAULT_KEYAB);

  // Make sure the authentification process didn't fail
  if (!success) {
    Serial << endl << "Authentication failed.";
    return;
  }

  uint8_t adata[16];
  adata[0] = 'A';
  adata[1] = 'd';
  adata[2] = 'r';
  adata[3] = 'i';
  adata[4] = 'a';
  adata[5] = 'n';
  for (uint8_t i = 6; i < 16; i++) {
    adata[i] = 0;
  }

  success = NFCReader.mifareclassic_WriteDataBlock(20, adata);
  if (success) {
    Serial << endl << "NDEF Writing 20 to block";
  } else {
    Serial << endl << "NDEF Record creation failed! :(";
  }
}
// Wait a bit before trying again
Serial << endl << "\n\nDone!";

// -----
}

void AFESensorPN532::formattingClassic() {

  uint8_t success; // Flag to check if there was an error with the PN532

  bool authenticated = false; // Flag to indicate if the sector is
  uint8_t blockBuffer[16];    // Buffer to store block contents
  uint8_t blankAccessBits[3] = {0xff, 0x07, 0x80};
  uint8_t idx = 0;
  uint8_t numOfSector =
      16; // Assume Mifare Classic 1K for now (16 4-block sectors)

  while (Serial.available())
    Serial.read();

  // Wait for an ISO14443A type card (Mifare, etc.).  When one is found
  // 'cardUID' will be populated with the UID, and cardUIDLength will indicate
  // if the cardUID is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = NFCReader.readPassiveTargetID(PN532_MIFARE_ISO14443A, cardUID,
                                          &cardUIDLength);

  if (success) {
    // We seem to have a tag ...
    // Display some basic information about it
    Serial.println("Found an ISO14443A card/tag");
    Serial.print("  UID Length: ");
    Serial.print(cardUIDLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    NFCReader.PrintHex(cardUID, cardUIDLength);
    Serial.println("");

    // Make sure this is a Mifare Classic card
    if (cardUIDLength != 4) {
      Serial.println(
          "Ooops ... this doesn't seem to be a Mifare Classic card!");
      return;
    }

    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    Serial.println("");
    Serial.println(
        "Reformatting card for Mifare Classic (please don't touch it!) ... ");

    // Now run through the card sector by sector
    for (idx = 0; idx < numOfSector; idx++) {
      // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF
      // 0xFF
      // 0xFF 0xFF
      success = NFCReader.mifareclassic_AuthenticateBlock(
          cardUID, cardUIDLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1,
          (uint8_t *)KEY_DEFAULT_KEYAB);
      if (!success) {
        Serial.print("Authentication failed for sector ");
        Serial.println(numOfSector);
        return;
      }

      // Step 2: Write to the other blocks
      if (idx == 16) {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
          Serial.print("Unable to write to sector ");
          Serial.println(numOfSector);
          return;
        }
      }
      if ((idx == 0) || (idx == 16)) {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
          Serial.print("Unable to write to sector ");
          Serial.println(numOfSector);
          return;
        }
      } else {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer))) {
          Serial.print("Unable to write to sector ");
          Serial.println(numOfSector);
          return;
        }
        if (!(NFCReader.mifareclassic_WriteDataBlock(
                (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer))) {
          Serial.print("Unable to write to sector ");
          Serial.println(numOfSector);
          return;
        }
      }
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(NFCReader.mifareclassic_WriteDataBlock(
              (BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer))) {
        Serial.print("Unable to write to sector ");
        Serial.println(numOfSector);
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
        Serial.print("Unable to write trailer block of sector ");
        Serial.println(numOfSector);
        return;
      }
    }
  }

  // Wait a bit before trying again
  Serial.println("\n\nDone!");
}

void AFESensorPN532::readNFC() {

  uint8_t success; // Flag to check if there was an error with the PN532

  uint8_t currentblock;       // Counter to keep track of which block we're on
  bool authenticated = false; // Flag to indicate if the sector is authenticated
  uint8_t data[16];           // Array to store block data during reads

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'cardUID' will be populated with the UID, and cardUIDLength will indicate
  // if the cardUID is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = NFCReader.readPassiveTargetID(PN532_MIFARE_ISO14443A, cardUID,
                                          &cardUIDLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");
    Serial.print(cardUIDLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    for (uint8_t i = 0; i < cardUIDLength; i++) {
      Serial.print(cardUID[i], HEX);
      Serial.print(' ');
    }
    Serial.println("");

    if (cardUIDLength == 4) {
      // We probably have a Mifare Classic card ...
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++) {
        // Check if this is a new block so that we can reauthenticate
        if (NFCReader.mifareclassic_IsFirstBlock(currentblock))
          authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated) {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");
          Serial.print(currentblock / 4, DEC);
          Serial.println("-------------------------");
          if (currentblock == 0) {
            // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic
            // (non-NDEF!)
            // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using
            // key a,
            // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF
            // 0xFF)
            success = NFCReader.mifareclassic_AuthenticateBlock(
                cardUID, cardUIDLength, currentblock, 1,
                (uint8_t *)KEY_DEFAULT_KEYAB);
          } else {
            // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic
            // (non-NDEF!)
            // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using
            // key a,
            // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF
            // 0xFF)
            success = NFCReader.mifareclassic_AuthenticateBlock(
                cardUID, cardUIDLength, currentblock, 1,
                (uint8_t *)KEY_DEFAULT_KEYAB);
          }
          if (success) {
            authenticated = true;
          } else {
            Serial.println("Authentication error");
          }
        }
        // If we're still not authenticated just skip the block
        if (!authenticated) {
          Serial.print("Block ");
          Serial.print(currentblock, DEC);
          Serial.println(" unable to authenticate");
        } else {
          // Authenticated ... we should be able to read the block now
          // Dump the data into the 'data' array
          success = NFCReader.mifareclassic_ReadDataBlock(currentblock, data);
          if (success) {
            // Read successful
            Serial.print("Block ");
            Serial.print(currentblock, DEC);
            if (currentblock < 10) {
              Serial.print("  ");
            } else {
              Serial.print(" ");
            }
            // Dump the raw data
            NFCReader.PrintHexChar(data, 16);
          } else {
            // Oops ... something happened
            Serial.print("Block ");
            Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
          }
        }
      }
    } else {
      Serial.println(
          "Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_PN532
