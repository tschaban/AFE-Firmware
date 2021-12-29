/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_AS3935_h
#define _AFE_Sensor_AS3935_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_AS3935

#include <arduino.h>

#include <AFE-Data-Access.h>
#include <AFE-I2C-Scanner.h>

#include <AS3935.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorAS3935 {

private:
  AS3935Sensor AS3935LightingSensor;

  boolean ready = false;
  

public:
  AS3935 configuration;

  uint8_t test = 0;

  /* Stories information about the distance to the storm */
  uint8_t distance;
  uint8_t eventType;

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 5];
#endif

  /* Constructor */
  AFESensorAS3935();

  /* Turns On sensor, true if OK, False if failure */
  boolean begin(uint8_t id);

  /* To call after strike has been detected. It reads information from the
   * sensor about the event. It's called by the interaption handler attached to
   * the GPIO  */
  void interruptionReported();

  /* Returnes true if strike has been detected */
  boolean strikeDetected();

  /* Returns sensor data in a form of the JSON */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_HARDWARE_AS3935
#endif // _AFE_Sensor_AS3935_h
