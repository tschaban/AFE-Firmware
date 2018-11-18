/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

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

#define HPMA115S0_TYPE_PM25 0
#define HPMA115S0_TYPE_PM10 1

class AFESensorHPMA115S0 {

private:
  const uint8_t commandRead[4] = {0x68, 0x01, 0x04, 0x93};
  const uint8_t commandAutoOFF[4] = {0x68, 0x01, 0x20, 0x77};

  /* HPMA115S00 commands not used
  const uint8_t commandAutoON[4] = {0x68, 0x01, 0x40, 0x57};
  const uint8_t commandMeasuringON[4] = {0x68, 0x01, 0x01, 0x96};
  const uint8_t commandMeasuringOFF[4] = {0x68, 0x01, 0x02, 0x95};
  */

  HPMA115S0 configuration;
  uint16_t currentPM25, _bufferPM25 = 0;
  uint16_t currentPM10, _bufferPM10 = 0;

  boolean ready = false;
  boolean _initialized = false;

  AFEUART UART;
  unsigned long startTime = 0;

  /* Method reads the data from the sensor */
  boolean read();

public:
  /* Constructor */
  AFESensorHPMA115S0();

  /* Turns On sensor */
  void begin();

  /* returns PM2.5 and PM10 */
  void get(uint16_t *pm25, uint16_t *pm10);

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Return relay IDX in Domoticz */
  unsigned long getDomoticzIDX(byte type);
};

#endif
