/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Debugger_h
#define _AFE_Debugger_h

#include <AFE-Configuration.h>
#ifdef DEBUG

#ifndef AFE_ESP32
extern "C" {
#include "user_interface.h"
}
#endif 

#include <Streaming.h>

#ifdef AFE_ESP32
#include <LITTLEFS.h>
#else /* ESP8266 */
#include <FS.h>
#if AFE_FILE_SYSTEM == AFE_FS_LITTLEFS
#include <LITTLEFS.h>
#endif
#endif // ESP32/ESP8266

/* Type for messages */
#define AFE_DEBUG_TYPE_LINE 0
#define AFE_DEBUG_TYPE_BULLET_POINT 1
#define AFE_DEBUG_TYPE_INFORMATION 7
#define AFE_DEBUG_TYPE_WARNING 8
#define AFE_DEBUG_TYPE_ERROR 9

/* Header char types */
#define AFE_DEBUG_HEADER_TYPE_HASH 0
#define AFE_DEBUG_HEADER_TYPE_DASH 1
#define AFE_DEBUG_HEADER_TYPE_SPACE 2

/* Header default length */
#define AFE_DEBUG_HEADER_DEFAULT_LENGTH 72

#ifndef UPDATE_SIZE_UNKNOWN
#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
#endif

class AFEDebugger {

private:
#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
  FSInfo fileSystem;
#endif

  void print(const char *text, const __FlashStringHelper *messageCategory,
             uint8_t type, uint8_t newLineBefore, uint8_t newLineAfter,
             uint8_t intent);
  void print(const __FlashStringHelper *text,
             const __FlashStringHelper *messageCategory, uint8_t type,
             uint8_t newLineBefore, uint8_t newLineAfter, uint8_t intent);
  void addMessageHeader(uint8_t type,
                        const __FlashStringHelper *messageCategory,
                        uint8_t newLineBefore, uint8_t noOfIntents);
  void addNewLines(uint8_t noOfLines);
  void addAdditionalText(const __FlashStringHelper *text, uint8_t newLineAfter);

public:
  AFEDebugger();

  void printInformation(const char *text,
                        const __FlashStringHelper *messageCategory,
                        uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);
  /**
   * @brief Generate debugge message of a type: INFO
   *
   * @param  text             Message
   * @param  messageCategory  Category, Location, Group
   * @param  newLineBefore    Default: 1
   * @param  newLineAfter     Defualt: 0
   */
  void printInformation(const __FlashStringHelper *text,
                        const __FlashStringHelper *messageCategory,
                        uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);

  void printWarning(const char *text,
                    const __FlashStringHelper *messageCategory,
                    uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);
  void printWarning(const __FlashStringHelper *text,
                    const __FlashStringHelper *messageCategory,
                    uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);

  void printError(const char *text, const __FlashStringHelper *messageCategory,
                  uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);
  void printError(const __FlashStringHelper *text,
                  const __FlashStringHelper *messageCategory,
                  uint8_t newLineBefore = 1, uint8_t newLineAfter = 0);

  void printBulletPoint(const char *text, uint8_t newLineAfter = 0);
  void printBulletPoint(const __FlashStringHelper *text,
                        uint8_t newLineAfter = 0);

  void printValue(const char *text, uint8_t newLineBefore = 0,
                  uint8_t newLineAfter = 0);
  void printValue(const __FlashStringHelper *text, uint8_t newLineBefore = 0,
                  uint8_t newLineAfter = 0);
  void printValue(uint8_t number, uint8_t newLineAfter = 0);
  void printValue(uint8_t number, const __FlashStringHelper *text,
                  uint8_t newLineAfter = 0);
  void printValue(unsigned long number, uint8_t newLineAfter = 0);
  void printValue(unsigned long number, const __FlashStringHelper *text,
                  uint8_t newLineAfter = 0);
  void printValue(float number, uint8_t newLineAfter = 0);
  void printValue(float number, const __FlashStringHelper *text,
                  uint8_t newLineAfter = 0);

  void printHeader(uint8_t newLineBefore = 1, uint8_t newLineAfter = 1,
                   uint8_t length = AFE_DEBUG_HEADER_DEFAULT_LENGTH,
                   uint8_t type = AFE_DEBUG_HEADER_TYPE_HASH);

  void getFreeMemorySize();
  void getFileSystemDubugInformation();
  void getESPHardwareInformation();
  void getFirmwareFlashInformation();

  void getFirmwareAllDebugInformation();

  void printProcessingRequest(const __FlashStringHelper *deviceItemName,
                              uint8_t deviceId,
                              const __FlashStringHelper *type);
};
#endif
#endif
