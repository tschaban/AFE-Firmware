/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BME680_h
#define _AFE_Sensor_BME680_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <EEPROM.h>
#include <arduino.h>
#include <bsec.h>
#include <Wire.h>

//#include <bsec_serialized_configurations_iaq.h>
//#include "config/generic_33v_3s_4d/bsec_serialized_configurations_iaq.h"

#define STATE_SAVE_PERIOD                                                      \
  UINT32_C(360 * 60 * 1000) // 360 minutes - 4 times a day

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBME680 {

private:
  Bsec Bme;

  const uint8_t bsec_config_iaq[454] = {
      0,   8,   4,   1,   61,  0,   0,   0,   0,   0,   0,   0,   174, 1,   0,
      0,   48,  0,   1,   0,   0,   192, 168, 71,  64,  49,  119, 76,  0,   0,
      225, 68,  137, 65,  0,   191, 205, 204, 204, 190, 0,   0,   64,  191, 225,
      122, 148, 190, 0,   0,   0,   0,   216, 85,  0,   100, 0,   0,   0,   0,
      0,   0,   0,   0,   28,  0,   2,   0,   0,   244, 1,   225, 0,   25,  0,
      0,   128, 64,  0,   0,   32,  65,  144, 1,   0,   0,   112, 65,  0,   0,
      0,   63,  16,  0,   3,   0,   10,  215, 163, 60,  10,  215, 35,  59,  10,
      215, 35,  59,  9,   0,   5,   0,   0,   0,   0,   0,   1,   88,  0,   9,
      0,   229, 208, 34,  62,  0,   0,   0,   0,   0,   0,   0,   0,   218, 27,
      156, 62,  225, 11,  67,  64,  0,   0,   160, 64,  0,   0,   0,   0,   0,
      0,   0,   0,   94,  75,  72,  189, 93,  254, 159, 64,  66,  62,  160, 191,
      0,   0,   0,   0,   0,   0,   0,   0,   33,  31,  180, 190, 138, 176, 97,
      64,  65,  241, 99,  190, 0,   0,   0,   0,   0,   0,   0,   0,   167, 121,
      71,  61,  165, 189, 41,  192, 184, 30,  189, 64,  12,  0,   10,  0,   0,
      0,   0,   0,   0,   0,   0,   0,   229, 0,   254, 0,   2,   1,   5,   48,
      117, 100, 0,   44,  1,   112, 23,  151, 7,   132, 3,   197, 0,   92,  4,
      144, 1,   64,  1,   64,  1,   144, 1,   48,  117, 48,  117, 48,  117, 48,
      117, 100, 0,   100, 0,   100, 0,   48,  117, 48,  117, 48,  117, 100, 0,
      100, 0,   48,  117, 48,  117, 100, 0,   100, 0,   100, 0,   100, 0,   48,
      117, 48,  117, 48,  117, 100, 0,   100, 0,   100, 0,   48,  117, 48,  117,
      100, 0,   100, 0,   44,  1,   44,  1,   44,  1,   44,  1,   44,  1,   44,
      1,   44,  1,   44,  1,   44,  1,   44,  1,   44,  1,   44,  1,   44,  1,
      44,  1,   8,   7,   8,   7,   8,   7,   8,   7,   8,   7,   8,   7,   8,
      7,   8,   7,   8,   7,   8,   7,   8,   7,   8,   7,   8,   7,   8,   7,
      112, 23,  112, 23,  112, 23,  112, 23,  112, 23,  112, 23,  112, 23,  112,
      23,  112, 23,  112, 23,  112, 23,  112, 23,  112, 23,  112, 23,  255, 255,
      255, 255, 255, 255, 255, 255, 220, 5,   220, 5,   220, 5,   255, 255, 255,
      255, 255, 255, 220, 5,   220, 5,   255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 44,  1,   0,   0,   0,   0,
      237, 52,  0,   0};

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

  boolean begin(BMEX80 *, TwoWire *WirePort);
  boolean read();

  void get(BMEX80_DATA &_data);
};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // _AFE_Sensor_BME680_h
