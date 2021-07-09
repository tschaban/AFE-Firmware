/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMP180_h
#define _AFE_Sensor_BMP180_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <arduino.h>

#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBMP180 {

private:
  Adafruit_BMP085 bme;

public:
  BMEX80 *configuration;
  BMEX80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMP180();

  boolean begin(BMEX80 *, TwoWire *);

  boolean read();

};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // _AFE_Sensor_BMP180_h
