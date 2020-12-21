/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Switch_h
#define _AFE_Switch_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_SWITCH

#include <AFE-Data-Access.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Broker.h>
#endif

class AFESwitch {

private:
  boolean _initialized = false;
  boolean state;
  boolean previousState; // Actually this stores current switch state

  unsigned long startTime = 0;

  boolean pressed = false; // It's set to true once button pressed physically
  boolean _pressed = false;

  boolean phisicallyPressed = false;
  boolean phisicallyState = false;

  /* Forst one is set to true after X secodns. Second one controls LED */
  boolean pressed4fiveSeconds = false;
  boolean _pressed4fiveSeconds = false;

  boolean pressed4tenSeconds = false;
  boolean _pressed4tenSeconds = false;

  boolean pressed4thirteenSeconds = false;
  boolean _pressed4thirteenSeconds = false;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *Led;
  void begin(uint8_t id, AFEDataAccess *);
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  AFEMCP23017Broker *_MCP23017Broker;
  boolean _MCP23017ReferenceAdded = false;
  boolean _expanderUsed = false;
  uint8_t _MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

public:
  SWITCH configuration;
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  char mqttCommandTopic[sizeof(configuration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(configuration.mqtt.topic) + 6];
#endif

  /* Constructors */
  AFESwitch();

/* Init switch */
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(uint8_t id, AFEDataAccess *, AFELED *);
#else
  void begin(uint8_t id, AFEDataAccess *);
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  void addMCP23017Reference(AFEMCP23017Broker *);
#endif


  /* Method: returns TRUE if state of the switch is pressed. It does not mean it
   * has to be pressed physically (applicable for BiStable switch types */
  boolean getState();
  boolean getPhisicalState();

  /* Method returns true if switch has been pressed. Sensitiveness it taken into
   * account. It does not mean switch is pressed physically. Once True capture
   * getState() method should be called to get the state of the Switch */
  boolean isPressed(boolean phisically = false);

  /* Method returns true after switch is pressed for 5sec. */
  boolean is5s();

  /* Method return true after switch is pressed for 10sec */
  boolean is10s();

  /* Method return true after switch is pressed for 30sec */
  boolean is30s();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();
};

#endif // AFE_CONFIG_HARDWARE_SWITCH
#endif // _AFE_Switch_h
