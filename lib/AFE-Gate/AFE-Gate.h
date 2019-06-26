/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Gate_h
#define _AFE_Gate_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Contactron.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-GATE-Structure.h>
#include <AFE-Relay.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEGate {

private:
  AFEDevice Device;
  AFEDataAccess Data;
  GATE gateConfiguration;
  uint8_t numberOfContractors = 0;
  boolean _event = false;
  AFERelay Relay[CONFIG_HARDWARE_NUMBER_OF_RELAYS];

  /* Returns gate state based on contactron state */
  uint8_t getGateStateBasedOnContractons();

  char mqttCommandTopic[sizeof(gateConfiguration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(gateConfiguration.mqtt.topic) + 6];

public:
  /* Via this class there is access to contactrons */
  AFEContactron Contactron[sizeof(Device.configuration.isContactron)];

  /* Constructors */
  AFEGate();

  /* Initializing gate */
  void begin();

  /* Triggering gate state changed and saving it's new value if there is not
   * contactrons */
  void toggle();

  /* Returns gate state*/
  uint8_t get();

  /* Returns true if gate state has changed */
  boolean event();

  /* It should be added to main loop to listen for gate state changes and
   * request to be processed by the class */
  void listener();

  /* Get MQTT Topics */
  const char *getMQTTCommandTopic();
  const char *getMQTTStateTopic();

  /* Return IDX in Domoticz */
  unsigned long getDomoticzIDX();
};

#endif
