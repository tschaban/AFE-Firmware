/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */
#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_DHT

//#include <arduino.h>
#include <AFE-Data-Access.h>
#include <PietteTech_DHT.h>


#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorDHT {

private:
  AFEDataAccess *Data;
  float currentTemperature;
  float currentHumidity;
  float currentDewPoint;
  float currentHeatIndex;

  boolean _initialized = false;

  unsigned long startTime = 0;

  PietteTech_DHT dht;

public:
  DHT configuration;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  /* Initializing method */
  void begin(AFEDataAccess *, uint8_t id);

  /* Method returns temperature */
  float getTemperature();

  /* Method returns humidity */
  float getHumidity();

  /* Method returns heat index */
  float getHeatIndex();

  /* Method returns Dew Point */
  float getDewPoint();

  /* Method should be added to the main loop to check temperature / humidity in
   * defined time frame */
  boolean listener();

    /* Returns the sensor data in JSON format */
  void getJSON(char *json);

};

#endif // AFE_CONFIG_HARDWARE_DHT
#endif // _AFE_Sensor_DHT_h
