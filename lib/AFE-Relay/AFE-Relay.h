/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Relay_h
#define _AFE_Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
#include <AFE-LED.h>
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
#include <AFE-Thermal-Protection.h>
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
#include <AFE-Thermostat.h>
#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
#include <AFE-Humidistat.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFERelay {

private:
  uint8_t _id;
  AFEDataAccess Data; // @TODO nie jest konsekwentnie jak np. w switch
  RELAY RelayConfiguration;

  char mqttCommandTopic[sizeof(RelayConfiguration.mqtt.topic) + 4];
  char mqttStateTopic[sizeof(RelayConfiguration.mqtt.topic) + 6];

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  AFELED Led;
#endif

  unsigned long turnOffCounter = 0;
#ifdef CONFIG_RELAY_AUTOONOFF_LISTENER
  boolean timerUnitInSeconds = true;
#endif

  /* Method set relay state after power restore or connection to MQTT is
   * established */
#ifdef CONFIG_FUNCTIONALITY_RELAY
  void setRelayAfterRestore(uint8_t option);
#endif

public:
#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
  AFEThermostat Thermostat;
#endif

#ifdef CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
  AFEThermalProtection ThermalProtection;
#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
  AFEHumidistat Humidistat;
#endif

  /* Constructors */
  AFERelay();
  AFERelay(uint8_t id);

  /* Method: initiates relay */
  void begin(uint8_t id);

  /* Method returns MQTT topic for this relay */
  const char *getMQTTCommandTopic();
  const char *getMQTTStateTopic();

#ifdef CONFIG_FUNCTIONALITY_RELAY
  /* Method sets relay state after device is turned on / power is restored / or
   * after device has been crash */
  void setRelayAfterRestoringPower();

  /* Method sets relay state after device is connected / reconnected to MQTT
   * Broker. It returns TRUE if relay state has been set, false it relay state
   * should be manged through MQTT Broker*/
  boolean setRelayAfterRestoringMQTTConnection();
#endif

  /* Method: Returns RELAY_OFF if relay is OFF, RELAY_ON if relay is ON */
  byte get();

  /* Turns on relay */
  void on(boolean invert = false);

  /* Turns off relay */
  void off(boolean invert = false);

  /* Toggles relay state from ON to OFF or from OFF to ON */
  void toggle();

#ifdef CONFIG_RELAY_AUTOONOFF_LISTENER
  /* Methods automatically turns off/on relay */
  boolean autoTurnOff(boolean invert = false);
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY
  /* Methods returns relay name */
  const char *getName();
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
  /* It sets timer to auto-switch of the relay */
  void setTimer(float timer);
#endif

#ifdef CONFIG_HARDWARE_PIR
  /* It removed timer for auto-switch of the relay */
  void clearTimer();
#endif

#if CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  /* It returns ID of the LED that shoud indicated Relay status */
  uint8_t getControlledLedID();
#endif

#ifdef CONFIG_FUNCTIONALITY_GATE
  /* It sets unit of relay to auto turn off timer. Possible options: true -
   * secods, false - miliseconds */
  void setTimerUnitToSeconds(boolean value);
#endif

#ifdef CONFIG_FUNCTIONALITY_RELAY
  /* Return relay IDX in Domoticz */
  unsigned long getDomoticzIDX();
#endif
};

#endif
