/*
  Switch mono and bi stable component
  AFE Firmware for smart home devices build on ESP8266
  More info: https://github.com/tschaban/AFE-Firmware
  LICENCE: http://opensource.org/licenses/MIT
*/

#ifndef _AFE_Switch_h
#define _AFE_Switch_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-structures.h>
#include <Streaming.h>

/* Types of switch */

#define SWITCH_TYPE_MONO 0 // Mono stable switch
#define SWITCH_TYPE_BI 1   // Bistable switch

class AFESwitch {

private:

  SWITCH SwitchConfiguration;
  boolean state;
  boolean previousState; // Actually this stores current switch state

  unsigned long startTime = 0;

  boolean pressed = false; // It's set to true once button pressed physically
  boolean _pressed = false;

  boolean pressed4fiveSeconds =  false; // It's set to true when switch is pressed for 5s
  boolean pressed4tenSeconds = false; // It's set to true when switch is pressed for 10s

public:

  /* Constructors */
  AFESwitch();
  AFESwitch(SWITCH configuration);

  /* Init switch */
  void begin(SWITCH configuration);

  /* Method: returns TRUE if state of the switch is pressed. It does not mean it has to be pressed physically (applicable for BiStable switch types */
  boolean getState();

  /* Method toggles switch state */
  void toggleState();

  /* Method returns true if switch has been pressed. Sensitiveness it taken into account.
   * It does not mean switch is pressed physically. Once True capture getState() method
   * should be called to get the state of the Switch */
  boolean isPressed();

  /* Method returns true after switch is pressed for 5sec. */
  boolean is5s();

  /* Method return trye after switch is pressed for 10sec */
  boolean is10s();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();
};

#endif