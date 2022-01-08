/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_ASYNC_MQTT_h
#define _AFE_ASYNC_MQTT_h

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-NETWORK-Structure.h>
#include <AsyncMqttClient.h>
#include <WiFiClient.h>

#ifdef AFE_ESP32 /* ESP82xx */
#include <ESP32Ping.h>
#else
#ifndef INT_MAX // Required for ESP8266 Lib
#define INT_MAX 0
#endif
#include <ESP8266Ping.h>
#endif


#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef DEBUG
#include <Streaming.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

class AFEAsyncMQTTClient {

private:
  AsyncMqttClient _Broker;
  AFEDataAccess *_Data;
  AFEDevice *_Device;
  NETWORK _NetworkConfiguration;
  PingClass Ping;

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *_Led;
  unsigned long _ledStartTime = 0;
#endif

  unsigned long _sleepStartTime = 0;
  boolean _sleepMode = false;
  boolean _isConfigured =
      true; // if it's falsed it does not connect to MQTT Broker

  uint8_t _connections = 0;
  unsigned long _delayStartTime = 0;
  boolean _eventConnectionEstablished = false;

  /* Sets parameters related to reconnection to MQTT Broker if there was
   * problems to connect to it */
  void setReconnectionParams(
      uint8_t no_connection_attempts,
      uint8_t duration_between_connection_attempts,
      uint8_t duration_between_next_connection_attempts_series);

  /* Connecting to MQTT Broker */
  void connect();

  /* Method turns off MQTT API */
  void disconnect();

  /* Check if host is available */
  boolean pingHost(void);

public:
  MQTT_MESSAGE message;
  WiFiClient esp;
  MQTT configuration;

  /* Constructor: it sets all necessary parameters */
  AFEAsyncMQTTClient();

/* Initialization of the class */
#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#endif
  void begin(AFEDataAccess *, AFEDevice *);

  /* Methods establishes connection from MQTT Broker, subscribed and set relay
 * default values */
  boolean listener();

  /* Method returns true if device connected to MQTT Broker */
  boolean eventConnected();

  /* Methods publishes a message to MQTT Broker */
  boolean publish(const char *topic, const char *message);

  /* Methods subsribes to topic in the MQTT Broker */
  void subscribe(const char *);
};

#endif