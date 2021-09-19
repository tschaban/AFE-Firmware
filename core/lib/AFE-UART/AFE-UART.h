/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_UART_h
#define _AFE_UART_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_UART


#include <AFE-Data-Access.h>
#include <AFE-SoftwareSerial.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEUART {

private:
  AFEDataAccess Data;

public:
  AFESoftwareSerial SerialBus;

  /* Constructor */
  AFEUART();
  void begin();

  /* Method sends data to Serial Port */
  void send(const uint8_t *cmd);

  /* Clean Buffer */
  void clean();
};

#endif // AFE_CONFIG_HARDWARE_UART
#endif
