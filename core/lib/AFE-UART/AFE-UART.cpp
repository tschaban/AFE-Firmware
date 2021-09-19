/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-UART.h"
#ifdef AFE_CONFIG_HARDWARE_UART

AFEUART::AFEUART() {}

void AFEUART::begin() {
  AFEDataAccess Data;
  SERIALPORT configuration;
  Data.getConfiguration(&configuration);
  SerialBus.init(configuration.RXD, configuration.TXD, false, 64,9600);
  SerialBus.println();

#ifdef DEBUG
  Serial << endl
         << F("UART: Initialized (RX=") << configuration.RXD
         << F(",TX=") << configuration.TXD << F(")");
#endif
}

void AFEUART::send(const uint8_t *cmd) {
  for (uint8_t i = 0; i < sizeof(cmd); i++) {
    SerialBus.write(cmd[i]);
  }
  delay(100);
#ifdef DEBUG
  Serial << endl
         << F("UART: Push: ")
         << (cmd[3] == 0X93
                 ? F("READ DATA")
                 : cmd[3] == 0x77
                       ? F("AUTO OFF")
                       : cmd[3] == 0x95
                             ? F("DEVICE OFF")
                             : cmd[3] == 0x96 ? F("DEVICE ON") : F("ERROR"));
#endif
}

void AFEUART::clean() {
#ifdef DEBUG
  Serial << endl << F("UART: Cleaning ");
#endif
  while (SerialBus.available()) {
    SerialBus.read();
#if defined(DEBUG)
    Serial << F(".");
#endif
  }
}
#endif //AFE_CONFIG_HARDWARE_UART