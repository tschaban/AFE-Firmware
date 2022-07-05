/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_RELAY

#include <AFE-Data-Access.h>
#include <AFE-MQTT-Structure.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Broker.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFERelay {

private:
  uint8_t _id;
  AFEDataAccess *_Data;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED Led;
#endif

  unsigned long turnOffCounter = 0;
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  boolean timerUnitInSeconds = true;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  AFEMCP23017Broker *_MCP23017Broker;
  boolean _expanderUsed = false;
  uint8_t _MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  /* Method set relay state after power restore or connection to MQTT is
   * established */

  void setRelayAfterRestore(uint8_t option);

public:
#ifdef AFE_CONFIG_HARDWARE_GATE
  uint8_t gateId = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif
  RELAY configuration;

  /**
   * @brief Construct a new AFERelay object
   *
   */
  AFERelay();

  /**
   * @brief Method: initiates relay
   *
   * @param  id               desc
   */
  void begin(AFEDataAccess *, uint8_t id);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  void addMCP23017Reference(AFEMCP23017Broker *);
#endif

  /**
   * @brief Method sets relay state after device is turned on / power is
   * restored / or
     * after device has been crash
   *
   */
  void setRelayAfterRestoringPower();

#if AFE_FIRMWARE_API != AFE_FIRMWARE_API_DOMOTICZ
  /**
   * @brief Method sets relay state after device is connected / reconnected to
   * MQTT
   * Broker. It returns TRUE if relay state has been set, false it relay state
   * should be manged through MQTT Broker
   *
   * @return boolean
   */
  boolean setRelayAfterRestoringMQTTConnection();
#endif

  /**
   * @brief Method: Returns AFE_RELAY_OFF if relay is OFF, AFE_RELAY_ON if relay
   * is ON
   *
   * @return byte
   */
  byte get();

  /**
   * @brief Turns on relay
   *
   */
  void on();

  /**
   * @brief Turns off relay
   *
   */
  void off();

  /**
   * @brief Toggles relay state from ON to OFF or from OFF to ON
   *
   */
  void toggle();

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
  /**
   * @brief Methods automatically turns off/on relay
   *
   * @return boolean
   */
  boolean autoTurnOff();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
  /**
   * @brief It sets timer to auto-switch of the relay
   *
   * @param  timer            desc
   */
  void setTimer(float timer);
#endif

#ifdef AFE_CONFIG_HARDWARE_PIR
  /**
   * @brief It removed timer for auto-switch of the relay
   *
   */
  void clearTimer();
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  /**
   * @brief It sets unit of relay to auto turn off timer. Possible options: true
   * -
   * secods, false - miliseconds
   *
   * @param  value            desc
   */
  void setTimerUnitToSeconds(boolean value);
#endif
};

#endif // AFE_CONFIG_HARDWARE_RELAY
#endif // _AFE_Relay_h
