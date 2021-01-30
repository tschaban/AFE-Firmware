/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BH1750_h
#define _AFE_Sensor_BH1750_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_BH1750

#include <arduino.h>

#include <AFE-Data-Access.h>
#include <BH1750.h>
#include <AFE-I2C-Scanner.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBH1750 {

private:
  BH1750LightSensor bh1750;

  boolean ready = false;
  boolean _initialized = false;

  unsigned long startTime = 0;


public:
  BH1750 configuration;
  float data = -1; // stories lux value

  /* Constructor */
  AFESensorBH1750();

  /* Turns On sensor */
  void begin(uint8_t id);

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
