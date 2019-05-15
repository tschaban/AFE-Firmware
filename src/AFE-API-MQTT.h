/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_MQTT_h
#define _AFE_MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-NETWORK-Structure.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEMQTT {

private:
  WiFiClient esp;
  PubSubClient Broker;
  char mqttTopicForSubscription[34];
  char deviceName[32];
  NETWORK NetworkConfiguration;
  unsigned long sleepStartTime = 0;
  boolean sleepMode = false;
  boolean isConfigured =
      true; // if it's falsed it does not connect to MQTT Broker

  uint8_t connections = 0;
  unsigned long delayStartTime = 0;
  boolean eventConnectionEstablished = false;
#ifdef CONFIG_HARDWARE_LED
  unsigned long ledStartTime = 0;
#endif

  /* Method pushes to the MQTT Broker MQTT Message */
  void publishToMQTTBroker(const char *topic, const char *message);
  /* Connecting to MQTT Broker */
  void connect();

public:
  /* Constructor: it sets all necessary parameters */
  AFEMQTT();

  /* Sets parameters related to reconnection to MQTT Broker if there was
   * problems to connect to it */
  void setReconnectionParams(
      uint8_t no_connection_attempts,
      uint8_t duration_between_connection_attempts,
      uint8_t duration_between_next_connection_attempts_series);

  void begin();

  /* Publishing MQTT Message. It calls private method publishToMQTTBroker */
  void publish(const char *type, const char *message);
  void publish(const char *type, float value, uint8_t width = 2,
               uint8_t precision = 2);

  /* Publishing MQTT Message to a specific MQTT Topic. It calls private method
   * publish MQTT Broker */
  void publish(const char *topic, const char *type, const char *message);

  /* Methods establishes connection from MQTT Broker, subscribed and set relay
   * default values */
  void listener();

  /* Method turns off MQTT API */
  void disconnect();

  /* Method returns true if device connected to MQTT Broker */
  boolean eventConnected();
};

#endif
