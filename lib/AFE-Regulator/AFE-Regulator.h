/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Regulator_h
#define _AFE_Regulator_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

#include <AFE-Data-Access.h>
#include <AFE-Relay.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFERegulator {

private:
  boolean ready = false;
  AFEDataAccess *_Data;
  AFERelay *_Relay;
  uint8_t _id;

#ifdef DEBUG
  AFEDebugger *_Debugger;
#endif

  /* Method enables / disables regulator */
  void enable(void);

public:
  REGULATOR *configuration = new REGULATOR;
  /* Keeps what should be the state of controlled device: false = off, true =
 * on */
  byte deviceState;
  boolean initialized = false;

/* Constructors */
#ifdef DEBUG
  AFERegulator(AFEDataAccess *, AFEDebugger *);
#else
  AFERegulator(AFEDataAccess *);
#endif

  /* Method initialize regulator */
  void begin(uint8_t id, AFERelay *Relay);

  /* Method rises event if thermostat themperatures have been exceeded */
  boolean listener(float value);

  /* Method turns on thermostat */
  void on(void);

  /* Method turns off thermostat */
  void off(void);

  /* Method turns thermostat to opposite to current state */
  void toggle(void);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);
};

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#endif // _AFE_Regulator_h