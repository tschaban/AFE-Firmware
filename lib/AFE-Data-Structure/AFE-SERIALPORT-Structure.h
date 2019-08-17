/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_UART_Structure_h
#define _AFE_UART_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct SERIALPORT {
  uint8_t RXD;
  uint8_t TXD;
};

#endif
