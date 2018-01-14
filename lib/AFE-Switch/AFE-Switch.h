/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Switch_h
#define _AFE_Switch_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-LED.h>
//#include <Streaming.h>

class AFESwitch {

private:
  SWITCH SwitchConfiguration;
  boolean _initialized = false;
  boolean state;
  boolean previousState; // Actually this stores current switch state

  unsigned long startTime = 0;

  boolean pressed = false; // It's set to true once button pressed physically
  boolean _pressed = false;

  boolean pressed4fiveSeconds =
      false; // It's set to true when switch is pressed for 5s
  boolean _pressed4fiveSeconds = false; // used to control LED
  boolean pressed4tenSeconds =
      false; // It's set to true when switch is pressed for 10s
  AFELED Led;

public:
  /* Constructors */
  AFESwitch();
  AFESwitch(uint8_t id);

  /* Init switch */
  void begin(uint8_t id);

  /* Method: returns TRUE if state of the switch is pressed. It does not mean it
   * has to be pressed physically (applicable for BiStable switch types */
  boolean getState();

  /* Method returns true if switch has been pressed. Sensitiveness it taken into
   * account. It does not mean switch is pressed physically. Once True capture
   * getState() method should be called to get the state of the Switch */
  boolean isPressed();

  /* Method returns true after switch is pressed for 5sec. */
  boolean is5s();

  /* Method return trye after switch is pressed for 10sec */
  boolean is10s();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();

  /* Methods returns type of switch functionality. See Data-Structure for
   * SWITCH_.... constants */
  uint8_t getFunctionality();
};

#endif
