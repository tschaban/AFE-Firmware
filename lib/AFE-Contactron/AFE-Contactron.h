/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Contactron_h
#define _AFE_Contactron_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-CONTACTRON-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-LED.h>
//#include <Streaming.h>

class AFEContactron {

public:
  CONTACTRON configuration;

  /* Constructors */
  AFEContactron();

  void begin(uint8_t id, AFEDevice *, AFEDataAccess *);

  /* Method returns contactorn state */
  byte get();

  /* Method returns GateID to which contracton is assigned to */
  uint8_t getGateId();

  /* Method returns MQTT topic for this contactron */
  const char *getMQTTCommandTopic();
  const char *getMQTTStateTopic();

  /* Method returns true if cotactron state ahs changed */
  boolean changed();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();

  /* Return IDX in Domoticz */
  unsigned long getDomoticzIDX();

protected:
private:
  boolean _initialized = false;
  boolean state;            // It stores actual contactron state
  byte _state;              // This contains contactron state to return
  boolean _changed = false; // True if contractor changed state

  unsigned long startTime = 0;

  AFELED ContactronLed;

  uint8_t gateId =
      255; // ID of the GATE the contracton is assigned to. 255 None.

  void convert();

  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];

  AFEDevice *Device;
  AFEDataAccess *Data;
};

#endif
