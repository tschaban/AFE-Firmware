/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Thermal_Protection_h
#define _AFE_Thermal_Protection_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION

#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEThermalProtection {

private:

  AFEDataAccess *_Data;
  uint8_t _id;

  /* Method enables / disables thermal protector */
  void enable(void);

public:
  THERMAL_PROTECTION configuration;
  boolean turnOff = false;
  /* Constructors */
  AFEThermalProtection();

  /* Method initialize protection */
  void begin(AFEDataAccess *, uint8_t id);

  /* Method monitors current temeprature and allowed one */
  bool listener(float currentTemperature);

  /* Method turns on thermal protector */
  void on(void);

  /* Method turns off therthermal protector mostat */
  void off(void);

  /* Method turns thermal protector  to opposite to current state */
  void toggle(void);
};

#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#endif // _AFE_Thermal_Protection_h
