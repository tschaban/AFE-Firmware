#include "AFE-Debugger.h"

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
  Serial << number;
  printValue(number, F(""), newLineAfter);
}

void AFEDebugger::printValue(unsigned long number,
                             const __FlashStringHelper *text,
                             uint8_t newLineAfter) {
  Serial << number;
  addAdditionalText(text, newLineAfter);
}

void AFEDebugger::printValue(unsigned long number, uint8_t newLineAfter) {
  Serial << number;
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

void AFEDebugger::printValue(float number,
                             const __FlashStringHelper *text,
                             uint8_t newLineAfter) {
  Serial << number;
  addAdditionalText(text, newLineAfter);
}

void AFEDebugger::printValue(float number, uint8_t newLineAfter) {
  Serial << number;
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

