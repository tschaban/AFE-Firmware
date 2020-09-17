/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Regulator_h
#define _AFE_Regulator_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

#include <AFE-Data-Access.h>

class AFERegulator {

private:
  boolean ready = false;
  AFEDataAccess *_Data;
  uint8_t _id;

  /* Method enables / disables regulator */
  void enable(void);

public:
  REGULATOR configuration;
  /* Keeps what should be the state of controlled device: false = off, true =
 * on */
  boolean deviceState = false;

  /* Constructors */
  AFERegulator();

  /* Method initialize regulator */
  void begin(AFEDataAccess *,uint8_t id);

  /* Method rises event if thermostat themperatures have been exceeded */
  boolean listener(float value);

  /* Method turns on thermostat */
  void on(void);

  /* Method turns off thermostat */
  void off(void);

  /* Method turns thermostat to opposite to current state */
  void toggle(void);
};

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#endif // _AFE_Regulator_h