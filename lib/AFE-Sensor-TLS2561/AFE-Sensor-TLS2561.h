/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_TLS2561_h
#define _AFE_Sensor_TLS2561_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_TLS2561

#include <arduino.h>

#include <AFE-Data-Access.h>
#include <AFE-I2C-Scanner.h>
#include <Adafruit_TSL2561_U.h>
#include <Wire.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorTLS2561 {

private:
  Adafruit_TSL2561_Unified tls2561;

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  TwoWire *_WirePort0;
#ifdef AFE_ESP32
  TwoWire *_WirePort1;
  void begin(uint8_t id, TwoWire *WirePort0);
#endif

public:
  TLS2561 configuration;
  /* Sensor's data */
  uint32_t illuminance = 0;
  uint16_t broadband, ir;
  
  /* Constructor */
  AFESensorTLS2561();

/* Turns On sensor */
#ifdef AFE_ESP32
  void begin(uint8_t _id, TwoWire *WirePort0, TwoWire *WirePort1);
#else
  void begin(uint8_t _id, TwoWire *WirePort0);
#endif

  /* Method has to be added to the loop in order to listen for sensor value
   * changes, Is true when data has been read from the sensor */
  boolean listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_TLS2561
#endif // _AFE_Sensor_TLS2561_h
