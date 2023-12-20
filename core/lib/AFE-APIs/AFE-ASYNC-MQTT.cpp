/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-ASYNC-MQTT.h"

boolean AFEAsyncMQTTClient::eventConnected = false;
boolean AFEAsyncMQTTClient::eventDisconnected = false;
boolean AFEAsyncMQTTClient::isConnected = false;

MQTT_MESSAGE
AFEAsyncMQTTClient::messagesBuffer[AFE_CONFIG_MQTT_MESSAGES_BUFFER];
uint8_t AFEAsyncMQTTClient::numberOfMessagesInBuffer = 0;

AFEAsyncMQTTClient::AFEAsyncMQTTClient(){};

#ifdef DEBUG
void AFEAsyncMQTTClient::addReference(AFEDebugger *_Debugger) {
  Debugger = _Debugger;
}
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
boolean AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device,
                                  AFELED *Led) {
  _Led = Led;
  return begin(Data, Device);
}
#endif

boolean AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device) {
  boolean _isConfigured = true;
  Data->getConfiguration(configuration);

  _Broker->onConnect(AFEAsyncMQTTClient::onMqttConnect);
  _Broker->onDisconnect(AFEAsyncMQTTClient::onMqttDisconnect);
  _Broker->onMessage(AFEAsyncMQTTClient::onMqttMessage);

#ifdef DEBUG
  _Broker->onSubscribe(AFEAsyncMQTTClient::onMqttSubscribe);
  _Broker->onUnsubscribe(AFEAsyncMQTTClient::onMqttUnsubscribe);
  _Broker->onPublish(AFEAsyncMQTTClient::onMqttPublish);
#endif
  char _deviceId[AFE_CONFIG_DEVICE_ID_SIZE];
  Data->getDeviceID(_deviceId);
  sprintf(_DeviceName, "%s-%s", Device->configuration.name, _deviceId);

  _Broker->setClientId(_DeviceName);
  _Broker->setMaxTopicLength(AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH);

 // _Broker->setKeepAlive(60);


  if (strlen(configuration->user) > 0 && strlen(configuration->password) > 0) {
    _Broker->setCredentials(configuration->user, configuration->password);
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (configuration->lwt.idx > 0) {
    char _lwtMessage[AFE_CONFIG_API_MQTT_LWT_MESSAGE_LENGTH];
    sprintf(_lwtMessage, "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":"
                         "0,\"svalue\":\"%s\",\"Battery\":0,\"RSSI\":0}",
            configuration->lwt.idx, L_DISCONNECTED);

    _Broker->setWill(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, 1,
                     configuration->retainLWT, _lwtMessage);
  }
#else
  if (strlen(configuration->lwt.topic) > 0) {
    _Broker->setWill(configuration->lwt.topic, configuration->qos,
                     configuration->retainLWT, "disconnected");
  }
#endif

  if (strlen(configuration->ip) > 0) {
    IPAddress ip;
    if (ip.fromString(configuration->ip)) {
      _Broker->setServer(configuration->ip, configuration->port);
    }
#ifdef DEBUG
    else {
      Debugger->printError(F("Problem with MQTT IP address:"), F("MQTT"));
      Debugger->printValue(configuration->ip);
    }
#endif
  } else if (strlen(configuration->host) > 0) {
    _Broker->setServer(configuration->host, configuration->port);
  } else {
    _isConfigured = false;
  }

#ifdef DEBUG
  Debugger->printInformation(F("Configuration"), F("MQTT"));
  Debugger->printBulletPoint(F("Device ID: "));
  Debugger->printValue(_DeviceName);
  Debugger->printBulletPoint(F("Host: "));
  Debugger->printValue(configuration->host);
  Debugger->printBulletPoint(F("IP: "));
  Debugger->printValue(configuration->ip);
  Debugger->printBulletPoint(F("Port: "));
  Serial << configuration->port;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  Debugger->printBulletPoint(F("LWT IDX: "));
  Serial << configuration->lwt.idx;
#else
  Debugger->printBulletPoint(F("LWT Topic: "));
  Serial << configuration->lwt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif
  return _isConfigured;
}

void AFEAsyncMQTTClient::subscribe(const char *topic) {
  if (strlen(topic) > 0) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
    _Broker->subscribe(topic, configuration->qos);
#ifdef DEBUG
    Debugger->printBulletPoint(F(""));
    Debugger->printValue(topic);
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
  }
}

boolean AFEAsyncMQTTClient::listener() {
  boolean _ret = false;
  if (_Broker->connected()) {
    if (messageProcessed != AFEAsyncMQTTClient::numberOfMessagesInBuffer) {
#ifdef DEBUG
      Debugger->printInformation(F("Processing message"), F("MQTT"));
      Debugger->printValue(messageProcessed);
#endif
      message = AFEAsyncMQTTClient::messagesBuffer[messageProcessed];
      messageProcessed++;
      if (messageProcessed == AFE_CONFIG_MQTT_MESSAGES_BUFFER) {
        messageProcessed = 0;
      }
      _ret = true;
    }
  } else {
    if (_reconnectionTimeout == 0) {
#ifdef DEBUG
      Debugger->printInformation(F("Connecting to MQTT Broker"), F("MQTT"));
#endif
      _Broker->connect();
      _reconnectionTimeout = millis();
    } else if (millis() - _reconnectionTimeout >
               AFE_CONFIG_MQTT_DEFAULT_RECONNECT_TIMEOUT) {
#ifdef DEBUG
      Debugger->printInformation(F("Connecting to MQTT Broker"), F("MQTT"));
#endif
      _Broker->connect();
      _reconnectionTimeout = millis();
    }
  }

  return _ret;
}

boolean AFEAsyncMQTTClient::connectedEvent() {
  boolean returnValue = AFEAsyncMQTTClient::eventConnected;
  if (returnValue) {
    publishConnected();
    AFEAsyncMQTTClient::eventConnected = false;
    _reconnectionTimeout = 0;
  }
  return returnValue;
}

boolean AFEAsyncMQTTClient::disconnectedEvent() {
  boolean returnValue = AFEAsyncMQTTClient::eventDisconnected;
  if (returnValue) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG
    Debugger->printWarning(
        F("Forcing disconnection from MQTT Broker on object level"), F("MQTT"));
#endif
    AFEAsyncMQTTClient::eventDisconnected = false;
  //  _Broker->disconnect(true);
  }

#ifdef AFE_CONFIG_HARDWARE_LED
  _Led->off();
#endif
  return returnValue;
}

boolean AFEAsyncMQTTClient::publish(const char *topic, const char *message) {
  boolean _ret = _Broker->connected();
  if (_ret) {
    uint16_t _publishedId = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG

    Debugger->printHeader(1, 1, 40, AFE_DEBUG_HEADER_TYPE_DASH);
    Debugger->printValue(F("Publish MQTT"));
    Debugger->printBulletPoint(F("Topic: "));
    Debugger->printValue(topic);
    Debugger->printBulletPoint(F("Message: "));
    Debugger->printValue(message);
    Debugger->printBulletPoint(F("Retain: "));
    Debugger->printValue((configuration->retainAll ? F("YES") : F("NO")));
    Debugger->printBulletPoint(F("Message size: "));
    Serial << sizeof(message);
    Debugger->getFreeMemorySize();

#endif
    if (strlen(topic) > 0) {
      _publishedId = _Broker->publish(topic, configuration->qos,
                                      configuration->retainAll, message);
    }
#ifdef DEBUG
    else {
      Debugger->printBulletPoint(F("Sttus: Error: No MQTT topic"));
    }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
#ifdef DEBUG
    Debugger->printBulletPoint(F("Status: Sent: Id: "));
    Serial << _publishedId;
    Debugger->printHeader(1, 0, 40, AFE_DEBUG_HEADER_TYPE_DASH);

#endif
  }

  return _ret;
}

void AFEAsyncMQTTClient::publishConnected() {
#ifdef DEBUG
  Debugger->printInformation(F("Sending message: device is connected"),
                             F("MQTT"));
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (configuration->lwt.idx > 0) {
    char lwtMessage[100]; // checked with AJ
    sprintf(lwtMessage, (const char *)F("{\"command\":\"udevice\",\"idx\":%d,"
                                        "\"nvalue\":1,\"svalue\":\"%s\","
                                        "\"Battery\":100,\"RSSI\":%d}"),
            configuration->lwt.idx, L_NETWORK_CONNECTED, getRSSI());

    boolean _retainAll = configuration->retainAll;
    configuration->retainAll = configuration->retainLWT;
    publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, lwtMessage);
    configuration->retainAll = _retainAll;
  }
#else
  if (strlen(configuration->lwt.topic) > 0) {
    boolean _retainAll = configuration->retainAll;
    configuration->retainAll = configuration->retainLWT;
    publish(configuration->lwt.topic, "connected");
    configuration->retainAll = _retainAll;
  }
#endif
}

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
uint8_t AFEAsyncMQTTClient::getRSSI() {
  uint8_t _ret;
  long current = WiFi.RSSI();
  if (current > -50) {
    _ret = 10;
  } else if (current < -98) {
    _ret = 0;
  } else {
    _ret = ((current + 97) / 5) + 1;
  }

  return _ret;
}
#endif

void AFEAsyncMQTTClient::onMqttConnect(bool sessionPresent) {
#ifdef DEBUG
  Serial << endl << F("INFO: MQTT: Connected to MQTT Broker");
#endif
  AFEAsyncMQTTClient::isConnected = true;
  AFEAsyncMQTTClient::eventConnected = true;
  AFEAsyncMQTTClient::eventDisconnected = false;
}

void AFEAsyncMQTTClient::onMqttDisconnect(
    AsyncMqttClientDisconnectReason reason) {

  if (AFEAsyncMQTTClient::isConnected) {
#ifdef DEBUG
    Serial << endl << F("WARN: MQTT: Disconnected from MQTT Broker : ");

    switch ((uint8_t)reason) {
    case 0:
      Serial << F("TCP Disconnected");
      break;
    case 1:
      Serial << F("Unacceptable protocol version");
      break;
    case 2:
      Serial << F("Device identifier rejected");
      break;
    case 3:
      Serial << F("MQTT Server unavailable");
      break;
    case 4:
      Serial << F("Malformed credentials");
      break;
    case 5:
      Serial << F("Not authorized");
      break;
    case 6:
      Serial << F("ESP8266 Not enought memory");
      break;
    case 7:
      Serial << F("LS Bad fingerprint");
      break;
    }
#endif
    AFEAsyncMQTTClient::isConnected = false;
    AFEAsyncMQTTClient::eventConnected = false;
    AFEAsyncMQTTClient::eventDisconnected = true;
  }
}

void AFEAsyncMQTTClient::onMqttMessage(
    char *topic, char *payload, AsyncMqttClientMessageProperties properties,
    size_t len, size_t index, size_t total) {

#ifdef DEBUG

  Serial << endl << F("INFO: MQTT: Got message:");
  Serial << endl
         << F(" : Topic   ") << topic << F(" | length: ") << strlen(topic);
  // Serial << endl << F(" : Message ") << payload;
  Serial << endl << F(" : QOS           ") << properties.qos;
  Serial << endl << F(" : Retain        ") << properties.retain;
  Serial << endl << F(" : Dup           ") << properties.dup;
  Serial << endl << F(" : Index         ") << index;
  Serial << endl << F(" : Length        ") << len;
  Serial << endl << F(" : Total         ") << total;
#endif

  if (strlen(topic) > AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH) {
#ifdef DEBUG
    Serial << endl
           << F("WARN: MQTT: Topic legnth: ") << strlen(topic)
           << F("too long. Max size: ") << AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH;
#endif
    return;
  }

  if (len > AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH) {
#ifdef DEBUG
    Serial << endl
           << F("WARN: MQTT: Message legnth: ") << strlen(topic)
           << F("too long. Max size: ") << AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH;
#endif
    return;
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ

  /**
   * @brief Adding standard MQTT message to the buffer
   *
   */

  char _content[AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH];

  for (uint16_t i = 0; i < len; i++) {
    _content[i] = payload[i];
  }
  _content[len] = AFE_EMPTY_STRING;

  StaticJsonBuffer<AFE_CONFIG_MQTT_CMD_MESSAGE_LENGTH> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(_content);

  if (root.success()) {
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.domoticz.idx = root["idx"] | AFE_DOMOTICZ_DEFAULT_IDX;

    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.nvalue = root["nvalue"] | AFE_NONE;

    if (strlen(root["svalue1"] | "") < AFE_CONFIG_MQTT_CMD_SVALUE_LENGTH) {
      sprintf(AFEAsyncMQTTClient::messagesBuffer
                  [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
                      .command.svalue,
              root["svalue1"] | "");

#ifdef DEBUG
    } else {
      Serial << endl
             << F("WARN: MQTT: Incoming SVALUE is: ") << strlen(root["svalue1"])
             << F(" and it's too long. Max size: ")
             << AFE_CONFIG_MQTT_CMD_SVALUE_LENGTH
             << F(". Request not processed");
#endif
    }

#ifdef AFE_CONFIG_HARDWARE_CLED
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.led.brightness = root["Level"];
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.led.color.blue = root["Color"]["b"];
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.led.color.red = root["Color"]["r"];
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .command.led.color.green = root["Color"]["g"];
#endif

#ifdef DEBUG
    Serial << endl
           << F("INFO: Domoticz command: ") << F(" : IDX: ") << endl
           << AFEAsyncMQTTClient::messagesBuffer
                  [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
                      .command.domoticz.idx
           << F(" : NValue: ") << endl
           << AFEAsyncMQTTClient::messagesBuffer
                  [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
                      .command.nvalue
           << F(" : SValue: ")
           << AFEAsyncMQTTClient::messagesBuffer
                  [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
                      .command.svalue;
#endif

    AFEAsyncMQTTClient::numberOfMessagesInBuffer++;
    if (AFEAsyncMQTTClient::numberOfMessagesInBuffer ==
        AFE_CONFIG_MQTT_MESSAGES_BUFFER) {
      AFEAsyncMQTTClient::numberOfMessagesInBuffer = 0;
    }
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: MQTT: Problem with JSON pharsing");
  }
#endif

#else
  sprintf(AFEAsyncMQTTClient::messagesBuffer
              [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
                  .topic,
          topic);

  for (uint16_t i = 0; i < len; i++) {
    AFEAsyncMQTTClient::messagesBuffer
        [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
            .content[i] = payload[i];
  }

  AFEAsyncMQTTClient::messagesBuffer
      [AFEAsyncMQTTClient::numberOfMessagesInBuffer]
          .content[len] = AFE_EMPTY_STRING;

  AFEAsyncMQTTClient::numberOfMessagesInBuffer++;
  if (AFEAsyncMQTTClient::numberOfMessagesInBuffer ==
      AFE_CONFIG_MQTT_MESSAGES_BUFFER) {
    AFEAsyncMQTTClient::numberOfMessagesInBuffer = 0;
  }

#endif
}

#ifdef DEBUG
void AFEAsyncMQTTClient::onMqttPublish(uint16_t packetId) {
  Serial << endl
         << F("INFO: MQTT: Broker acknowledged message Id: ") << packetId;
}

void AFEAsyncMQTTClient::onMqttUnsubscribe(uint16_t packetId) {
  Serial << endl
         << F("INFO: MQTT: Broker acknowledged unsubscribe, Id: ") << packetId;
}

void AFEAsyncMQTTClient::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial << endl
         << F("INFO: MQTT: Broker acknowledged subscribe. Id: ") << packetId
         << F(", QOS: ") << qos;
}
#endif // DEBUG