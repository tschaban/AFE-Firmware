/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Regulator_h
#define _AFE_Regulator_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR

#include <AFE-Data-Access.h>

class AFERegulator {

private:
  boolean ready = false;

  /* Method enables / disables regulator */
  void enable(boolean state);

public:
  REGULATOR configuration;
  /* Keeps what should be the state of controlled device: false = off, true =
 * on */
  boolean deviceState = false;

  /* Constructors */
  AFERegulator();

  /* Method initialize regulator */
  void begin(AFEDataAccess *,uint8_t id);

  /* Method returns true if regulator is enabled */
  boolean enabled();

  /* Method rises event if thermostat themperatures have been exceeded */
  boolean listener(float value);

  /* Method turns on thermostat */
  void on();

  /* Method turns off thermostat */
  void off();

  /* Method turns thermostat to opposite to current state */
  void toggle();
};

#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#endif // _AFE_Regulator_h