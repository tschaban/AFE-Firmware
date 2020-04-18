/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Gate_h
#define _AFE_Gate_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Configuration.h>
#include <AFE-Contactron.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-GATE-Structure.h>
#include <AFE-Relay.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEGate {

public:
  /* Via this class there is access to contactrons */
  AFEContactron Contactron[2];
  GATE configuration;

#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  /* Constructors */
  AFEGate();

  /* Initializing gate */
  void begin(uint8_t id, AFEDevice *, AFEDataAccess *);

  /* Triggering gate state changed and saving it's new value if there is not
   * contactrons */
  void toggle();

  /* Returns gate state*/
  uint8_t get();

  /* Returns number of contactrons for a gate */
  uint8_t getNoOfContactrons();

  /* Returns contactron ID for selected contactron index (1,2) */
  uint8_t getContactronId(uint8_t index);

  /* Returns true if gate state has changed */
  boolean event();

  /* It triggers event - used eg when contactron state is changed */
  void triggerEvent();

    /* Returns the sensor data in JSON format */
  void getJSON(char *json);

private:
  AFEDevice *Device;
  AFEDataAccess *Data;

  uint8_t gateId;                  // ID of the gate
  uint8_t numberOfContractons = 0; // Number of Contractons assigned to a gate
  boolean _event = false;
  AFERelay GateRelay;

  /* Returns gate state based on contactron state */
  uint8_t getGateStateBasedOnContractons();
};

#endif
