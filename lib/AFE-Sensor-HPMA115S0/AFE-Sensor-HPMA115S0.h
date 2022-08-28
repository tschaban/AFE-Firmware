/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_HPMA115S0_h
#define _AFE_Sensor_HPMA115S0_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0

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

  HPMA115S0_DATA *buffer = new HPMA115S0_DATA;
  boolean _initialized = false;

  AFEUART *UART = new AFEUART();
  unsigned long startTime = 0;

  boolean _measuremntsON = false;
  boolean _autoReading = true;

  /* Method reads the data from the sensor */
  boolean read(boolean expectingACK = false);

public:
  HPMA115S0 *configuration = new HPMA115S0;
  HPMA115S0_DATA *data = new HPMA115S0_DATA;

  /* Constructor */
  AFESensorHPMA115S0();

  /* Turns On sensor */
  void begin(uint8_t id);

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  boolean listener(void);
 
  /* Method sends command to the sensor */
  boolean sendCommand(const uint8_t *command,
                      uint8_t howManyTimesRetry = HPMA115S0_RETRY);
  
  /* It returns the data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_HPMA115S0
#endif
