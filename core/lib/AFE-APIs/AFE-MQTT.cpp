/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-MQTT.h"

AFEMQTT::AFEMQTT(){};

#ifdef AFE_CONFIG_HARDWARE_LED
void AFEMQTT::begin(AFEDataAccess *Data, char *DeviceName, AFELED *Led) {
  _Led = Led;
  begin(Data, DeviceName);
}
#endif

void AFEMQTT::begin(AFEDataAccess *Data, char *DeviceName) {
  _Data = Data;
  _DeviceName = DeviceName;
  _Data->getConfiguration(&configuration);
  _Data->getConfiguration(&_NetworkConfiguration);

  esp.setTimeout(configuration.timeout);
  _Broker.setClient(esp);
  _Broker.setBufferSize(AFE_CONFIG_MQTT_DEFAULT_BUFFER_SIZE);

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
  } else {
    _isConfigured = false;
  }

  Data = {};

#ifdef DEBUG
  Serial << endl
         << F("INFO: MQTT Configuration") << endl
         << F("INFO: Host: ") << configuration.host << endl
         << F("INFO: IP: ") << configuration.ip << endl
         << F("INFO: Port: ") << configuration.port << endl
         << F("INFO: User: ") << configuration.user << endl
         << F("INFO: Password: ") << configuration.password << endl
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
         << F("INFO: LWT IDX: ") << configuration.lwt.idx;
#else
         << F("INFO: LWT Topic: ") << configuration.lwt.topic;
#endif // AFE_CONFIG_API_DOMOTICZ_ENABLED
#endif
}


void AFEMQTT::subscribe(const char *topic) {
  if (strlen(topic) > 0) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
    _Broker.subscribe(topic);
#ifdef DEBUG
    Serial << endl << F(" - ") << topic;
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->off();
#endif
  }
}


void AFEMQTT::disconnect() {
  if (_Broker.connected()) {
    _Broker.disconnect();
  }
}

boolean AFEMQTT::listener() {
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
}

void AFEMQTT::connect() {

  if (_isConfigured) {
    if (_sleepMode) {
      if (millis() - _sleepStartTime >=
          _NetworkConfiguration.waitTimeSeries * 1000) {
        _sleepMode = false;
      }
    } else {
#ifdef AFE_CONFIG_HARDWARE_LED
      if (_ledStartTime == 0) {
        _ledStartTime = millis();
      }
#endif
      if (_delayStartTime == 0) {
        _delayStartTime = millis();
        /* Connecing to MQTT Broker depending on LWT topics being set or no */

        boolean _connected;

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        char lwtMessage[100];

        if (configuration.lwt.idx > 0) {
          sprintf(lwtMessage, "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":"
                              "0,\"svalue\":\"%s\",\"Battery\":0,\"RSSI\":0}",
                  configuration.lwt.idx, L_DISCONNECTED);

          _connected = _Broker.connect(
              _DeviceName, configuration.user, configuration.password,
              AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, 2, false, lwtMessage);
        } else {
          _connected = _Broker.connect(_DeviceName, configuration.user,
                                      configuration.password);
        }
#else
        _connected = strlen(configuration.lwt.topic) > 0
                         ? _Broker.connect(_DeviceName, configuration.user,
                                          configuration.password,
                                          configuration.lwt.topic, 2, false,
                                          "disconnected")
                         : _Broker.connect(_DeviceName, configuration.user,
                                          configuration.password);
#endif

        if (_connected) {
          _eventConnectionEstablished = true;
          _delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
          _ledStartTime = 0;
          _Led->off();
#endif
          _connections = 0;

#ifdef DEBUG
          Serial << endl << F("INFO: Connected to MQTT Broker");
#endif
          return;
        }
      }
#ifdef AFE_CONFIG_HARDWARE_LED
      if (millis() > _ledStartTime + 500) {
        _Led->toggle();
        _ledStartTime = 0;
      }
#endif
      if (millis() >
          _delayStartTime + (_NetworkConfiguration.waitTimeConnections * 1000)) {

        _connections++;
        yield();
#ifdef DEBUG
        Serial << endl
               << F("INFO: MQTT Connection attempt: ") << _connections + 1
               << F(" from ") << _NetworkConfiguration.noConnectionAttempts
               << F(", connection status: ") << _Broker.state()
               << F(", connection time: ") << millis() - _delayStartTime
               << F("ms");

#endif
        _delayStartTime = 0;
      }

      if (_connections >= _NetworkConfiguration.noConnectionAttempts) {
        _sleepMode = true;
        _sleepStartTime = millis();

        _delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
        _ledStartTime = 0;
        _Led->off();
#endif
        _connections = 0;

#ifdef DEBUG
        Serial << endl
               << F("WARN: MQTT: Not able to connect to MQTT.Going to sleep "
                    "mode for ")
               << _NetworkConfiguration.waitTimeSeries << F("sec.");
#endif
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  _NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  _NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  _NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

boolean AFEMQTT::eventConnected() {
  boolean returnValue = _eventConnectionEstablished;
  _eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEMQTT::publish(const char *topic, const char *message) {

  boolean pubslishingStatus = false;

  if (_Broker.state() == MQTT_CONNECTED) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG
    Serial << endl << F("----------- Publish MQTT -----------");
    Serial << endl << F("Topic: ") << topic;
    Serial << endl << F("Message: ") << message;
#endif
    if (strlen(topic) > 0) {
      pubslishingStatus = _Broker.publish(topic, message);
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
           << F("Status: ")
           << (pubslishingStatus ? F("published") : F("NOT pubslished"));
    Serial << endl << F("------------------------------------");
#endif
  }

  return pubslishingStatus;
}