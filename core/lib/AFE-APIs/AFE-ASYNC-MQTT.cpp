/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#include "AFE-ASYNC-MQTT.h"

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
 // uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  //Serial.println(packetIdSub);
  //mqttClient.publish("test/lol", 0, true, "test 1");
  Serial.println("Publishing at QoS 0");
  //uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
  Serial.print("Publishing at QoS 1, packetId: ");
  //Serial.println(packetIdPub1);
  //uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
  Serial.print("Publishing at QoS 2, packetId: ");
  //Serial.println(packetIdPub2);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload,
                   AsyncMqttClientMessageProperties properties, size_t len,
                   size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

AFEAsyncMQTTClient::AFEAsyncMQTTClient(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device,
                               AFELED *Led) {
  _Led = Led;
  begin(Data, Device);
}
#endif

void AFEAsyncMQTTClient::begin(AFEDataAccess *Data, AFEDevice *Device) {
  _Data = Data;
  _Device = Device;
  _Data->getConfiguration(&configuration);
  _Data->getConfiguration(&_NetworkConfiguration);
  esp.setTimeout(configuration.timeout);

  _Broker.onPublish(onMqttPublish);
  _Broker.onConnect(onMqttConnect);
  _Broker.onDisconnect(onMqttDisconnect);
  _Broker.onSubscribe(onMqttSubscribe);
  _Broker.onUnsubscribe(onMqttUnsubscribe);
  _Broker.onMessage(onMqttMessage);

  /*
  _Broker.setClient(esp);
  _Broker.setBufferSize(AFE_CONFIG_MQTT_DEFAULT_BUFFER_SIZE);
*/
  if (strlen(configuration.ip) > 0) {
    IPAddress ip;
    if (ip.fromString(configuration.ip)) {
      //    _Broker.setServer(configuration.ip, configuration.port);
    }
#ifdef DEBUG
    else {
      Serial << endl
             << F("ERROR: Problem with MQTT IP address: ") << configuration.ip;
    }
#endif
  } else if (strlen(configuration.host) > 0) {
    _Broker.setServer(configuration.host, configuration.port);
  } else {
    _isConfigured = false;
  }

  Data = {};

  if (configuration.pingHostBeforeConnection) {
    // Trigger pinging host
    _sleepMode = true;
    _sleepStartTime = millis() + _NetworkConfiguration.waitTimeSeries * 1000;
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
}

void AFEAsyncMQTTClient::subscribe(const char *topic) {
  if (strlen(topic) > 0) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
    _Broker.subscribe(topic, 2);
#ifdef DEBUG
    Serial << endl << F(" - ") << topic;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
  }
}

void AFEAsyncMQTTClient::disconnect() {
  // if (_Broker.connected()) {
  //// _Broker.disconnect();
  // }
}

boolean AFEAsyncMQTTClient::listener() {
  /*
  boolean _ret = false;
  if (_Broker.connected()) {
    _ret = _Broker.listen();
    if (_ret) {
      message.topic = _Broker.topic;
      message.content = _Broker.payload;
      message.length = _Broker.length;
    }
  } else {
    connect();
  }
  return _ret;
  */
}

void AFEAsyncMQTTClient::connect() {}

void AFEAsyncMQTTClient::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  _NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  _NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  _NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

boolean AFEAsyncMQTTClient::pingHost(void) {
  boolean _hostReachable = false;
  if (strlen(configuration.ip) > 0) {
    IPAddress addr;
    addr.fromString(configuration.ip);
#ifdef DEBUG
    Serial << endl << F("INFO: MQTT: PING: Sent to ") << configuration.ip;
#endif
    _hostReachable = Ping.ping(addr, AFE_CONFIG_MQTT_DEFAULT_HOST_PINGS_NUMBER);

  } else if (strlen(configuration.host) > 0) {
#ifdef DEBUG
    Serial << endl << F("INFO: MQTT: PING: Sent to ") << configuration.host;
#endif

    _hostReachable = Ping.ping(configuration.host,
                               AFE_CONFIG_MQTT_DEFAULT_HOST_PINGS_NUMBER);
  }

#ifdef DEBUG
  Serial << F(" ... ") << (_hostReachable ? F("") : F("NOT "))
         << F("reachable");
#endif

  return _hostReachable;
}

boolean AFEAsyncMQTTClient::eventConnected() {
  boolean returnValue = _eventConnectionEstablished;
  _eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEAsyncMQTTClient::publish(const char *topic, const char *message) {

  boolean _status = false;
  /*
    if (_Broker.state() == MQTT_CONNECTED) {
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
      //  _status = _Broker.publish(topic, message, configuration.retainAll);
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
      Serial << endl
             << F("Status: ") << (_status ? F("published") : F("NOT
  pubslished"));
      Serial << endl << F("------------------------------------");
  #endif
    }
  */
  return _status;
}