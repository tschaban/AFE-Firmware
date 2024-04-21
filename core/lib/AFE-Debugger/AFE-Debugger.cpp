#include "AFE-Debugger.h"

#ifdef DEBUG

AFEDebugger::AFEDebugger(){};

void AFEDebugger::print(const char *text,
                        const __FlashStringHelper *messageCategory,
                        uint8_t type, uint8_t newLineBefore,
                        uint8_t newLineAfter, uint8_t intent) {
  addMessageHeader(type, messageCategory, newLineBefore, intent);
  Serial << text;
  addNewLines(newLineAfter);
}

void AFEDebugger::print(const __FlashStringHelper *text,
                        const __FlashStringHelper *messageCategory,
                        uint8_t type, uint8_t newLineBefore,
                        uint8_t newLineAfter, uint8_t intent) {
  addMessageHeader(type, messageCategory, newLineBefore, intent);
  Serial << text;
  addNewLines(newLineAfter);
}

void AFEDebugger::printValue(uint8_t number, const __FlashStringHelper *text,
                             uint8_t newLineAfter) {
  Serial << number;
  addAdditionalText(text, newLineAfter);
}

void AFEDebugger::printValue(uint8_t number, uint8_t newLineAfter) {
  printValue(number, F(""), newLineAfter);
}

void AFEDebugger::printValue(unsigned long number,
                             const __FlashStringHelper *text,
                             uint8_t newLineAfter) {
  Serial << number;
  addAdditionalText(text, newLineAfter);
}

void AFEDebugger::printValue(unsigned long number, uint8_t newLineAfter) {
  printValue(number, F(""), newLineAfter);
}

void AFEDebugger::printValue(const char *text, uint8_t newLineBefore,
                             uint8_t newLineAfter) {
  print(text, F(""), AFE_DEBUG_TYPE_LINE, newLineBefore, newLineAfter, 0);
}

void AFEDebugger::printValue(const __FlashStringHelper *text,
                             uint8_t newLineBefore, uint8_t newLineAfter) {
  print(text, F(""), AFE_DEBUG_TYPE_LINE, newLineBefore, newLineAfter, 0);
}

void AFEDebugger::printValue(float number, const __FlashStringHelper *text,
                             uint8_t newLineAfter) {
  Serial << number;
  addAdditionalText(text, newLineAfter);
}

void AFEDebugger::printValue(float number, uint8_t newLineAfter) {
  printValue(number, F(""), newLineAfter);
}

void AFEDebugger::addNewLines(uint8_t noOfLines) {
  for (uint8_t i = 0; i < noOfLines; i++) {
    Serial << endl;
  }
}

void AFEDebugger::addMessageHeader(uint8_t type,
                                   const __FlashStringHelper *messageCategory,
                                   uint8_t newLineBefore, uint8_t noOfIntents) {
  addNewLines(newLineBefore);

  for (uint8_t i = 0; i < noOfIntents; i++) {
    Serial << F(" ");
  }

  Serial << (type == AFE_DEBUG_TYPE_INFORMATION
                 ? F("INFO")
                 : (type == AFE_DEBUG_TYPE_WARNING
                        ? F("WARN")
                        : (type == AFE_DEBUG_TYPE_ERROR
                               ? F("ERROR")
                               : (type == AFE_DEBUG_TYPE_BULLET_POINT
                                      ? F("* ")
                                      : F("")))));

  if (type == AFE_DEBUG_TYPE_ERROR || type == AFE_DEBUG_TYPE_INFORMATION ||
      type == AFE_DEBUG_TYPE_WARNING) {
    Serial << F(": ") << messageCategory << F(": ");
  }
}

void AFEDebugger::addAdditionalText(const __FlashStringHelper *text,
                                    uint8_t newLineAfter) {
  if (text) {
    Serial << F(" ") << text;
  }

  addNewLines(newLineAfter);
}

void AFEDebugger::printInformation(const char *text,
                                   const __FlashStringHelper *messageCategory,
                                   uint8_t newLineBefore,
                                   uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_INFORMATION, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printInformation(const __FlashStringHelper *text,
                                   const __FlashStringHelper *messageCategory,
                                   uint8_t newLineBefore,
                                   uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_INFORMATION, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printWarning(const char *text,
                               const __FlashStringHelper *messageCategory,
                               uint8_t newLineBefore, uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_WARNING, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printWarning(const __FlashStringHelper *text,
                               const __FlashStringHelper *messageCategory,
                               uint8_t newLineBefore, uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_WARNING, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printError(const char *text,
                             const __FlashStringHelper *messageCategory,
                             uint8_t newLineBefore, uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_ERROR, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printError(const __FlashStringHelper *text,
                             const __FlashStringHelper *messageCategory,
                             uint8_t newLineBefore, uint8_t newLineAfter) {
  print(text, messageCategory, AFE_DEBUG_TYPE_ERROR, newLineBefore,
        newLineAfter, 0);
}

void AFEDebugger::printBulletPoint(const char *text, uint8_t newLineAfter) {
  print(text, F(""), AFE_DEBUG_TYPE_BULLET_POINT, 1, newLineAfter, 2);
}

void AFEDebugger::printBulletPoint(const __FlashStringHelper *text,
                                   uint8_t newLineAfter) {
  print(text, F(""), AFE_DEBUG_TYPE_BULLET_POINT, 1, newLineAfter, 2);
}
void AFEDebugger::printHeader(uint8_t newLineBefore, uint8_t newLineAfter,
                              uint8_t length, uint8_t type) {
  addNewLines(newLineBefore);
  for (uint8_t i = 0; i < length; i++) {
    Serial << (type == AFE_DEBUG_HEADER_TYPE_DASH
                   ? F("-")
                   : (type == AFE_DEBUG_HEADER_TYPE_SPACE ? F(" ") : F("#")));
  }
  addNewLines(newLineAfter);
}

void AFEDebugger::getFreeMemorySize() {
  printInformation(F("Free: "), F("RAM"));
#ifdef AFE_ESP32
  Serial << (esp_get_free_heap_size() / 1024) << F("kB");
#else
  Serial << (system_get_free_heap_size() / 1024) << F("kB");
#endif
}

void AFEDebugger::getFileSystemDubugInformation() {
  printInformation(F("Summary: "), F("FS"));
  printBulletPoint(F("Used: "));

#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
  SPIFFS.info(fileSystem);
  Serial << fileSystem.usedBytes / 1024 << F("/")
         << fileSystem.totalBytes / 1024 << F("kB");
#else
  Serial << LITTLEFS.usedBytes() / 1024 << F("/")
         << LITTLEFS.totalBytes() / 1024 << F("kB");
#endif
}

void AFEDebugger::getESPHardwareInformation() {
  printInformation(F("Hardware information"), F("ESP"));

#ifndef AFE_ESP32
  printBulletPoint(F("ID: "));
  Serial << ESP.getFlashChipId();
#endif

#ifdef AFE_ESP32
  printBulletPoint(F("Model: "));
  Serial << ESP.getChipModel();

  printBulletPoint(F("CPU cores: "));
  Serial << ESP.getChipCores();

#endif

#ifdef AFE_ESP32
  printBulletPoint(F("CPU: "));
  Serial << (ESP.getCpuFreqMHz() / 1000000) << F(" MHz");
#endif

  printBulletPoint(F("Flash size: "));
  if (ESP.getFlashChipSize() >= 1048576) {
    Serial << (ESP.getFlashChipSize() / 1048576) << F(" Mbits");
  } else {
    Serial << (ESP.getFlashChipSize() / 1024) << F(" Kbits");
  }

#ifndef AFE_ESP32
  printBulletPoint(F("Real Flash size: "));
  if (ESP.getFlashChipRealSize() >= 1048576) {
    Serial << (ESP.getFlashChipRealSize() / 1048576) << F(" Mbits");
  } else {
    Serial << (ESP.getFlashChipRealSize() / 1024) << F(" Kbits");
  }
#endif

  printBulletPoint(F("Flash Speed: "));
  Serial << (ESP.getFlashChipSpeed() / 1000000) << F(" MHz");

  printBulletPoint(F("Flash Chip Mode: "));
  Serial << ESP.getFlashChipMode();

#ifdef AFE_ESP32
  printBulletPoint(F("Firmware size: "));
  Serial << ESP.getSketchSize();

  printBulletPoint(F("Firmware free space size: "));
  Serial << ESP.getFreeSketchSpace();
#endif
}

void AFEDebugger::getFirmwareFlashInformation() {

#ifdef AFE_ESP32
  uint32_t maxSketchSpace = UPDATE_SIZE_UNKNOWN;
#else  // ESP8266
  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#endif // AFE_ESP32

  printBulletPoint(F("Current: "));
  Serial << (ESP.getSketchSize() / 1024) << F("Kb");
  printBulletPoint(F("Available space: "));
  Serial << (ESP.getFreeSketchSpace() / 1024) << F("Kb");
  printBulletPoint(F("Max available space: "));
  Serial << (maxSketchSpace / 1024) << F("Kb");

#ifdef AFE_ESP32
  printBulletPoint(F("Max size: "));
  Serial << (UPDATE_SIZE_UNKNOWN / 1024 / 1024) << F("KB");
#endif // ESP32
}

void AFEDebugger::getFirmwareAllDebugInformation() {
  getFreeMemorySize();
  getFileSystemDubugInformation();
}

void AFEDebugger::printProcessingRequest(
    const __FlashStringHelper *deviceItemName, uint8_t deviceId,
    const __FlashStringHelper *type) {
  printInformation(F("Processing: "), type);
  Serial << deviceItemName;
  if (deviceId != AFE_NONE) {
    Serial << F(", Id: ") << deviceId;
  }
}

#endif
