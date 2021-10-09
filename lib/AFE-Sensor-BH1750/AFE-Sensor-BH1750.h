/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BH1750_h
#define _AFE_Sensor_BH1750_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BH1750

#include <arduino.h>

#include <AFE-Data-Access.h>
#include <AFE-I2C-Scanner.h>
#include <BH1750.h>
#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBH1750 {

private:
  BH1750LightSensor bh1750;

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  TwoWire *_WirePort0;
#ifdef AFE_ESP32
  TwoWire *_WirePort1;
  void begin(uint8_t id, TwoWire *WirePort0);
#endif

public:
  BH1750 configuration;
  float data = -1; // stories lux value

  /* Constructor */
  AFESensorBH1750();

/* Turns On sensor */
#ifdef AFE_ESP32
  void begin(uint8_t _id, TwoWire *WirePort0, TwoWire *WirePort1);
#else
  void begin(uint8_t _id, TwoWire *WirePort0);
#endif

  /* Is true when data has been read from the sensor */
  boolean isReady();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_BH1750
#endif // _AFE_Sensor_BH1750_h
