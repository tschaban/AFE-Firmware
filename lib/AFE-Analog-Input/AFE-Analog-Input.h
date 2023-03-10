/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_ADC_VCC_h
#define _AFE_Sensor_ADC_VCC_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#include <AFE-Firmware.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAnalogInput {

private:
  boolean ready = false;
  unsigned long startTime = 0;
  boolean _initialized = false;
  uint8_t counterOfSamplings = 0;
  uint16_t temporaryAnalogData = 0;

public:
  ADCINPUT *configuration = new ADCINPUT;
  ADCINPUT_DATA *data = new ADCINPUT_DATA;

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  float batteryPercentage = 0;
#endif

  /* Constructor */
  AFEAnalogInput();

/* Initialized analog input using configuration parameters */
#ifdef AFE_ESP32
  void begin(uint8_t id, AFEFirmware *_Firmware);
#else  // ESP8266
  void begin(AFEFirmware *_Firmware);
#endif // AFE_ESP32

  /* Returns values from Analog Input */
  ADCINPUT_DATA get();

  /* Returns true if there is a new value available from analog input */
  boolean isReady();

  /* Main method that takes care for analog reads based on config parameters */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
  /* Returns the batter data in JSON format */
  void getBatteryMeterJSON(char *json);
#endif
};

#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT
#endif // _AFE_Sensor_ADC_VCC_h
