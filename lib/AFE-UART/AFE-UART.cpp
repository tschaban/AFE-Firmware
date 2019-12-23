/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#include "AFE-UART.h"

AFEUART::AFEUART() {}

void AFEUART::begin() {
  AFEDataAccess Data;
  SERIALPORT configuration = Data.getSerialPortConfiguration();
  SerialBus.init(configuration.RXD, configuration.TXD, false, 64,9600);
  SerialBus.println();

#ifdef DEBUG
  Serial << endl
         << "UART: Initialized (RX=" << configuration.RXD
         << ",TX=" << configuration.TXD << ")";
#endif
}

void AFEUART::send(const uint8_t *cmd) {
  for (uint8_t i = 0; i < sizeof(cmd); i++) {
    SerialBus.write(cmd[i]);
  }
  delay(100);
#ifdef DEBUG
  Serial << endl
         << "UART: Push: "
         << (cmd[3] == 0X93
                 ? "READ DATA"
                 : cmd[3] == 0x77
                       ? "AUTO OFF"
                       : cmd[3] == 0x95
                             ? "DEVICE OFF"
                             : cmd[3] == 0x96 ? "DEVICE ON" : "ERROR");
#endif
}

void AFEUART::clean() {
#ifdef DEBUG
  Serial << endl << "UART: Cleaning ";
#endif
  while (SerialBus.available()) {
    SerialBus.read();
#if defined(DEBUG)
    Serial << ".";
#endif
  }
}
