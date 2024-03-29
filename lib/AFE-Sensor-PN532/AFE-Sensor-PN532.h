/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_PN532_h
#define _AFE_Sensor_PN532_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

#include <AFE-Data-Access.h>

#include <PN532.h>
#include <PN532_I2C.h>
#ifdef AFE_ESP32
#include <PN532_HSU.h>
#else
#include <PN532_SWHSU.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

#define NR_SHORTSECTOR (32)         // Number of short sectors on Mifare 1K/4K
#define NR_LONGSECTOR (8)           // Number of long sectors on Mifare 4K
#define NR_BLOCK_OF_SHORTSECTOR (4) // Number of blocks in a short sector
#define NR_BLOCK_OF_LONGSECTOR (16) // Number of blocks in a long sector

// Determine the sector trailer block based on sector number
#define BLOCK_NUMBER_OF_SECTOR_TRAILER(sector)                                 \
  (((sector) < NR_SHORTSECTOR)                                                 \
       ? ((sector)*NR_BLOCK_OF_SHORTSECTOR + NR_BLOCK_OF_SHORTSECTOR - 1)      \
       : (NR_SHORTSECTOR * NR_BLOCK_OF_SHORTSECTOR +                           \
          (sector - NR_SHORTSECTOR) * NR_BLOCK_OF_LONGSECTOR +                 \
          NR_BLOCK_OF_LONGSECTOR - 1))

// Determine the sector's first block based on the sector number
#define BLOCK_NUMBER_OF_SECTOR_1ST_BLOCK(sector)                               \
  (((sector) < NR_SHORTSECTOR)                                                 \
       ? ((sector)*NR_BLOCK_OF_SHORTSECTOR)                                    \
       : (NR_SHORTSECTOR * NR_BLOCK_OF_SHORTSECTOR +                           \
          (sector - NR_SHORTSECTOR) * NR_BLOCK_OF_LONGSECTOR))

// The default Mifare Classic key
static const uint8_t KEY_DEFAULT_KEYAB[6] = {0xFF, 0xFF, 0xFF,
                                             0xFF, 0xFF, 0xFF};

class AFESensorPN532 {

private:
  boolean _initialized = false;
  boolean _request = false;
  uint32_t _requestTime = 0;
  uint32_t _listenerTime = 0;

  AFEDataAccess *Data;
  TwoWire *_WirePort0;
#ifdef AFE_ESP32
  TwoWire *_WirePort1;
#endif

#ifdef AFE_ESP32
  PN532_HSU PN532UARTInterface;
#else
  PN532_SWHSU PN532UARTInterface;
#endif
  PN532_I2C PN532I2CInterface;
  PN532 NFCReader;

  uint8_t cardUID[7] = {0, 0, 0, 0,
                        0, 0, 0}; // Buffer to store the returned UID

  uint8_t cardUIDLength; // Length of the UID (4 or 7 bytes depending on
                         // ISO14443A card type)

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif

  boolean foundCard();
  boolean authenticatedBlock(uint32_t blockId);

#ifdef AFE_CONFIG_HARDWARE_I2C
  void begin(uint8_t _id, AFEDataAccess *, AFEDevice *);
#endif

public:
  PN532_SENSOR configuration;
  PN532_TAGS tag;

  /* Constructor */
  AFESensorPN532();

/* Init switch */
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
  void begin(uint8_t _id, AFEDataAccess *, AFEDevice *, TwoWire *, TwoWire *);
#else
  void begin(uint8_t _id, AFEDataAccess *, AFEDevice *, TwoWire *);
#endif
#else
  void begin(uint8_t _id, AFEDataAccess *, AFEDevice *);
#endif // AFE_CONFIG_HARDWARE_I2C

  /* Listens for state changes, taking into account bouncing */
  uint8_t listener(void);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

  /* Methods format the card to NFC or Classic */
  void formattingNFC();
  void formattingClassic();

  /* Read block from the card.
  if lookForCard = false: assumes card is already near the reader */
  boolean readBlock(uint8_t blockId, char *data, boolean lookForCard = true);

  /* Write data to the block */
  void writeBlock(uint8_t blockId, const char *data);

  /* Read AFE TAG, uses backup TAG in case of problems */
  boolean readTag();

#ifdef DEBUG
  /* Reads all blocks. Only available in DEBUG mode. It's logged to the Serial
   * port */
  void readNFC();
#endif
};

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
#endif // _AFE_Sensor_PN532_h
