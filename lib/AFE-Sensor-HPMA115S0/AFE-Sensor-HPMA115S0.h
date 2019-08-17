/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

  

#ifndef _AFE_Sensor_HPMA115S0_h
#define _AFE_Sensor_HPMA115S0_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-UART.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

#define HPMA115S0_RETRY 5

class AFESensorHPMA115S0 {

private:
  const uint8_t commandRead[4] = {0x68, 0x01, 0x04, 0x93};
  const uint8_t commandAutoOFF[4] = {0x68, 0x01, 0x20, 0x77};
  const uint8_t commandTurnON[4] = {0x68, 0x01, 0x01, 0x96};
  const uint8_t commandTurnOFF[4] = {0x68, 0x01, 0x02, 0x95};

  /* HPMA115S00 commands not used
  const uint8_t commandAutoON[4] = {0x68, 0x01, 0x40, 0x57};

  */

  HPMA115S0 configuration;
  HPMA115S0_DATA current;
  HPMA115S0_DATA buffer;

  boolean ready = false;
  boolean _initialized = false;

  AFEUART UART;
  unsigned long startTime = 0;

  boolean _measuremntsON = false;
  boolean _autoReading = true;

  /* Method reads the data from the sensor */
  boolean read(boolean expectingACK = false);

public:
  /* Constructor */
  AFESensorHPMA115S0();

  /* Turns On sensor */
  void begin();

  /* returns PM2.5 and PM10 */
  HPMA115S0_DATA get();

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  boolean sendCommand(const uint8_t *command,
                      uint8_t howManyTimesRetry = HPMA115S0_RETRY);

  /* Return relay IDX in Domoticz */
  void getDomoticzIDX(HPMA115S0_DOMOTICZ *idx);
};

#endif
