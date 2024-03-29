/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMP180_h
#define _AFE_Sensor_BMP180_h

#ifndef AFE_ESP32

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BMEX80

#include <Arduino.h>

#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBMP180 {

private:
  Adafruit_BMP085 *bme = new Adafruit_BMP085();
  BMEX80_DATA *data = new BMEX80_DATA;
public:
  BMEX80 *configuration = new BMEX80;
  

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMP180();

  boolean begin(BMEX80 *, TwoWire *, BMEX80_DATA *);

  boolean read();

};

#endif // AFE_CONFIG_HARDWARE_BMEX80
#endif // AFE_ESP32
#endif // _AFE_Sensor_BMP180_h
