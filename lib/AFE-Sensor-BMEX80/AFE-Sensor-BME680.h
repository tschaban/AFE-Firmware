/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <arduino.h>
#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <EEPROM.h>
#include <bsec.h>

//#include <bsec_serialized_configurations_iaq.h>

#include "config/generic_33v_3s_4d/bsec_serialized_configurations_iaq.h"

#define STATE_SAVE_PERIOD                                                      \
  UINT32_C(360 * 60 * 1000) // 360 minutes - 4 times a day

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBME680 {

private:
  Bsec Bme;

  void loadState(void);
  void updateState(void);

  uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
  uint16_t stateUpdateCounter = 0;

  boolean dataInBuffer = false;
  BMEX80_DATA data;

#ifdef DEBUG
  void checkBmeStatus();
#endif

public:
  BMEX80 *configuration;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBME680();

  boolean begin(BMEX80 *, I2CPORT *);
  boolean read();

  void get(BMEX80_DATA &_data);
};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // _AFE_Sensor_BME680_h
