/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_TSL2561_h
#define _AFE_Sensor_TSL2561_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_TSL2561

#include <Arduino.h>

#include <AFE-Data-Access.h>
#include <AFE-I2C-Scanner.h>
#include <Adafruit_TSL2561_U.h>
#include <Wire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorTSL2561
{

private:
  Adafruit_TSL2561_Unified *tls2561 = new Adafruit_TSL2561_Unified();

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;

  TwoWire *WirePort0;

#if defined(DEBUG) && defined(AFE_ESP32)
  void begin(uint8_t _id, TwoWire *_WirePort0, TwoWire *_WirePort1);
#endif

#if defined(AFE_ESP32) || (!defined(AFE_ESP) && defined(DEBUG))
  TwoWire *WirePort1;
  void begin(uint8_t id, TwoWire *_WirePort0);
#endif

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

public:
  TSL2561 *configuration = new TSL2561;
  /* Sensor's data */
  uint32_t illuminance = 0;
  uint16_t broadband, ir;

  /* Constructor */
  AFESensorTSL2561();

/* Turns On sensor */
#ifdef DEBUG
#ifdef AFE_ESP32
  void begin(uint8_t _id, TwoWire *_WirePort0, TwoWire *_WirePort1, AFEDebugger *_Debugger);
#else
  void begin(uint8_t _id, TwoWire *_WirePort0, AFEDebugger *_Debugger);
#endif
#else
#ifdef AFE_ESP32
  void begin(uint8_t _id, TwoWire *_WirePort0, TwoWire *_WirePort1);
#else
  void begin(uint8_t _id, TwoWire *_WirePort0);
#endif
#endif
  /* Method has to be added to the loop in order to listen for sensor value
   * changes, Is true when data has been read from the sensor */
  boolean listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_TSL2561
#endif // _AFE_Sensor_TSL2561_h
