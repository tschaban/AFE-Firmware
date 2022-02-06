/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-ASYNC-MQTT.h"

boolean AFEAsyncMQTTClient::eventConnected = false;
boolean AFEAsyncMQTTClient::isConnected = false;

MQTT_MESSAGE
AFEAsyncMQTTClient::messagesBuffer[AFE_CONFIG_MQTT_MESSAGES_BUFFER];
uint8_t AFEAsyncMQTTClient::numberOfMessagesInBuffer = 0;

AFEAsyncMQTTClient::AFEAsyncMQTTClient(){};

#ifdef AFE_CONFIG_HARDWARE_LED
boolean AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device,
                                  AFELED *Led) {
  _Led = Led;
  return begin(Data, Device);
}
#endif

boolean AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device) {
  boolean _isConfigured = true;
  Data->getConfiguration(&configuration);

  _Broker.onConnect(AFEAsyncMQTTClient::onMqttConnect);
  _Broker.onDisconnect(AFEAsyncMQTTClient::onMqttDisconnect);
  _Broker.onMessage(AFEAsyncMQTTClient::onMqttMessage);

#ifdef DEBUG
  _Broker.onSubscribe(AFEAsyncMQTTClient::onMqttSubscribe);
  _Broker.onUnsubscribe(AFEAsyncMQTTClient::onMqttUnsubscribe);
  _Broker.onPublish(AFEAsyncMQTTClient::onMqttPublish);
#endif

  sprintf(_DeviceName, "%s-%s", Device->configuration.name, Device->deviceId);

  _Broker.setClientId(_DeviceName);
  _Broker.setMaxTopicLength(AFE_CONFIG_MQTT_TOPIC_CMD_LENGTH);
  if (strlen(configuration.user) > 0 && strlen(configuration.password) > 0) {
    _Broker.setCredentials(configuration.user, configuration.password);
  }

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (configuration.lwt.idx > 0) {
    sprintf(_lwtMessage, "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":"
                         "0,\"svalue\":\"%s\",\"Battery\":0,\"RSSI\":0}",
            configuration.lwt.idx, L_DISCONNECTED);

    _Broker.setWill(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, 1,
                    configuration.retainLWT, _lwtMessage);
  }
#else
  if (strlen(configuration.lwt.topic) > 0) {
    _Broker.setWill(configuration.lwt.topic, configuration.qos,
                    configuration.retainLWT, "disconnected");
  }
#endif

  if (strlen(configuration.ip) > 0) {
    IPAddress ip;
    if (ip.fromString(configuration.ip)) {
      _Broker.setServer(configuration.ip, configuration.port);
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: Problem with MQTT IP address: ") << configuration.ip;
    }
#endif
  } else if (strlen(configuration.host) > 0) {
    _Broker.setServer(configuration.host, configuration.port);
  } 
  else {
    _isConfigured = false;
  }

#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT Configuration") << endl
         << F("INFO: Host: ") << configuration.host << endl
         << F("INFO: IP: ") << configuration.ip << endl
         << F("INFO: Port: ") << configuration.port << endl
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
         << F("INFO: LWT IDX: ") << configuration.lwt.idx;
#else
         << F("INFO: LWT Topic: ") << configuration.lwt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif
return _isConfigured;
}

void AFEAsyncMQTTClient::subscribe(const char *topic) {
  if (strlen(topic) > 0) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
    _Broker.subscribe(topic, configuration.qos);
#ifdef DEBUG
    Serial << endl << F(" - ") << topic;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
  }
}

boolean AFEAsyncMQTTClient::listener() {
  boolean _ret = false;
  if (_Broker.connected()) {
    if (messageProcessed != AFEAsyncMQTTClient::numberOfMessagesInBuffer) {
#ifdef DEBUG
      Serial << endl
             << F("INFO: MQTT: Processing message: ") << messageProcessed;
#endif
      message = AFEAsyncMQTTClient::messagesBuffer[messageProcessed];
      messageProcessed++;
      if (messageProcessed == AFE_CONFIG_MQTT_MESSAGES_BUFFER) {
        messageProcessed = 0;
      }
      _ret = true;
    }
  } else {
    _Broker.connect();
  }

  return _ret;
}

boolean AFEAsyncMQTTClient::connected() {
  boolean returnValue = AFEAsyncMQTTClient::eventConnected;
  if (returnValue) {
    publishConnected();
    AFEAsyncMQTTClient::eventConnected = false;
  }
  return returnValue;
}

boolean AFEAsyncMQTTClient::publish(const char *topic, const char *message) {
  boolean _ret = _Broker.connected();
  if (_ret) {
    uint16_t _publishedId = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG
    Serial << endl << F("----------- Publish MQTT -----------");
    Serial << endl << F("Topic: ") << topic;
    Serial << endl << F("Message: ") << message;
    Serial << endl
           << F("Retain: ") << (configuration.retainAll ? F("YES") : F("NO"));
#endif
    if (strlen(topic) > 0) {
      _publishedId = _Broker.publish(topic, configuration.qos,
                                     configuration.retainAll, message);
    }
#ifdef DEBUG
    else {
      Serial << endl << F("WARN: No MQTT topic.");
    }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
#ifdef DEBUG
    Serial << endl << F("Message sent. Id: ") << _publishedId;
    Serial << endl << F("------------------------------------");
#endif
  }

  return _ret;
}

void AFEAsyncMQTTClient::publishConnected() {
#ifdef DEBUG
  Serial << endl << F("INFO: Sending message: device is connected ...");
#endif

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  if (configuration.lwt.idx > 0) {
    char lwtMessage[100]; // checked with AJ
    sprintf(
        lwtMessage,
        "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":1,\"svalue\":\"%s\","
        "\"Battery\":100,\"RSSI\":%d}",
        configuration.lwt.idx, L_NETWORK_CONNECTED, getRSSI());

    boolean _retainAll = configuration.retainAll;
    configuration.retainAll = configuration.retainLWT;
    publish(AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, lwtMessage);
    configuration.retainAll = _retainAll;
  }
#else
  if (strlen(configuration.lwt.topic) > 0) {
    boolean _retainAll = configuration.retainAll;
    configuration.retainAll = configuration.retainLWT;
    publish(configuration.lwt.topic, "connected");
    configuration.retainAll = _retainAll;
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
  AFEAsyncMQTTClient::eventConnected = true;
  AFEAsyncMQTTClient::isConnected = true;
}

void AFEAsyncMQTTClient::onMqttDisconnect(
    AsyncMqttClientDisconnectReason reason) {

  if (AFEAsyncMQTTClient::isConnected) {
#ifdef DEBUG
    Serial << endl << F("ERROR: MQTT: Problem connecting to MQTT Broker : ");

    switch ((uint8_t)reason) {
    case 0:
      Serial << "TCP Disconnected";
      break;
    case 1:
      Serial << "Unacceptable protocol version";
      break;
    case 2:
      Serial << "Device identifier rejected";
      break;
    case 3:
      Serial << "MQTT Server unavailable";
      break;
    case 4:
      Serial << "Malformed credentials";
      break;
    case 5:
      Serial << "Not authorized";
      break;
    case 6:
      Serial << "ESP8266 Not enought memory";
      break;
    case 7:
      Serial << "LS Bad fingerprint";
      break;
    }
#endif
    AFEAsyncMQTTClient::isConnected = false;
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

#ifdef DEBUG
  Serial << endl
         << F(" : Idx in buffer ")
         << AFEAsyncMQTTClient::numberOfMessagesInBuffer;
#endif

  AFEAsyncMQTTClient::numberOfMessagesInBuffer++;
  if (AFEAsyncMQTTClient::numberOfMessagesInBuffer ==
      AFE_CONFIG_MQTT_MESSAGES_BUFFER) {
    AFEAsyncMQTTClient::numberOfMessagesInBuffer = 0;
  }
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