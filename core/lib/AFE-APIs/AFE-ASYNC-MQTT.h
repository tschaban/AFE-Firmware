/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_ASYNC_MQTT_h
#define _AFE_ASYNC_MQTT_h

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-MQTT-Structure.h>
#include <AsyncMqttClient.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <WiFiClient.h>
#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

class AFEAsyncMQTTClient
{

private:
  char _DeviceName[33 + AFE_CONFIG_DEVICE_ID_SIZE + 1];

#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *_Led;
  unsigned long _ledStartTime = 0;
#endif

  unsigned long _reconnectionTimeout = 0;

#ifdef DEBUG
  AFEDebugger *Debugger;
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  /**
   * @brief Returns RSSI level in DOmoticz Range
   *
   * @return uint8_t
   */
  uint8_t getRSSI();

#endif

public:
  AsyncMqttClient *_Broker = new AsyncMqttClient();
  /**
   * @brief set to true after connecting/disconnecting to MQTT Broker
   *
   */
  static boolean eventConnected;
  static boolean eventDisconnected;

  /**
   * @brief is true when device is connected to MQTT broker
   *
   */
  static boolean isConnected;

  /**
   * @brief storeis latest received MQTT Message
   *
   */

  static MQTT_MESSAGE messagesBuffer[AFE_CONFIG_MQTT_MESSAGES_BUFFER];
  static uint8_t numberOfMessagesInBuffer;
  uint8_t messageProcessed = 0;
  MQTT_MESSAGE message;

  MQTT *configuration = new MQTT;

  /**
   * @brief Construct a new AFEAsyncMQTTClient object
   *
   */
  AFEAsyncMQTTClient();

#ifdef DEBUG
  void addReference(AFEDebugger *_Debugger);
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
  boolean begin(AFEDataAccess *, AFEDevice *, AFELED *);
#endif
  boolean begin(AFEDataAccess *, AFEDevice *);

  /**
   * @brief establishes connection to MQTT Broker and listens for messages
   *
   * @return boolean, true if message received
   */
  boolean listener();

  /**
   * @brief returns true if device connected to MQTT Broker
   *
   * @return boolean
   */
  boolean connectedEvent();

  /**
   * @brief returns true if device disconnected to MQTT Broker
   *
   * @return boolean
   */
  boolean disconnectedEvent();

  /**
   * @brief publishes a message to MQTT Broker
   *
   * @param  topic            topic
   * @param  message          message to publish
   * @return boolean          true is message published
   */
  boolean publish(const char *topic, const char *message);

  /**
   * @brief Methods subsribes to topic in the MQTT Broker
   *
   */
  void subscribe(const char *);

  /**
   * @brief publishes message that device is connected to mqtt broker
   *
   */
  void publishConnected();

  /**
   * @brief triggered if device established connection to MQTT Broker
   *
   * @param  sessionPresent   true if session is present
   */
  static void onMqttConnect(bool sessionPresent);

  /**
   * @brief triggered if device disconnected from MQTT Broker
   *
   * @param  reason           why disconnected
   */
  static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

  /**
   * @brief triggered after subsriving the topic
   *
   * @param  packetId         packahe Id
   * @param  qos              QOS
   */
  static void onMqttSubscribe(uint16_t packetId, uint8_t qos);

  /**
   * @brief triggered if topic is unsubsribed to MQTT Broker
   *
   * @param  packetId         package ID
   */
  static void onMqttUnsubscribe(uint16_t packetId);

  /**
   * @brief triggered if message received
   *
   * @param  topic            topic
   * @param  payload          message
   * @param  properties       message propoerties
   * @param  len              length of the message
   * @param  index            index of a message
   * @param  total            total size of the message
   */
  static void onMqttMessage(char *topic, char *payload,
                            AsyncMqttClientMessageProperties properties,
                            size_t len, size_t index, size_t total);
  /**
   * @brief triggered after publishing message
   *
   * @param  packetId         package ID
   */
  static void onMqttPublish(uint16_t packetId);
};

#endif