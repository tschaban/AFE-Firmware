/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Contactron_h
#define _AFE_Contactron_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

#include <AFE-CONTACTRON-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-LED.h>
#include <arduino.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEContactron {

public:
  CONTACTRON configuration;

  // ID of the GATE the contactron is assigned to. 255 None.
  uint8_t gateId = AFE_HARDWARE_ITEM_NOT_EXIST;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  /* Constructors */
  AFEContactron();

  /* Initialize. Must be run per each Gate object */
  void begin(uint8_t id, AFEDevice *, AFEDataAccess *);

  /* Method returns contactorn state */
  boolean get();

  /* Method returns true if cotactron state ahs changed */
  boolean changed();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

private:
  AFEDevice *Device;
  AFEDataAccess *Data;

  boolean _initialized = false;
  boolean state;            // It stores actual contactron state
  boolean _changed = false; // True if contractor changed state

  unsigned long startTime = 0;

  AFELED ContactronLed;
};

#endif // AFE_CONFIG_HARDWARE_CONTACTRON
#endif // _AFE_Contactron_h
