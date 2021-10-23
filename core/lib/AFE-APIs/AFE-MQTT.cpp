/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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

        if (configuration.pingHostBeforeConnection) {
          /* Performing Network PING before starting MQTT connection */

          if (pingHost()) {
            _sleepMode = false;
          } else {
            _sleepMode = true;
            _sleepStartTime = millis();
#ifdef DEBUG
            Serial << endl
                   << F("WARN: MQTT: Looks MQTT host: ") << configuration.ip
                   << F(" is not reachable. Waiting next ")
                   << _NetworkConfiguration.waitTimeSeries
                   << F(" sec. to try again");
#endif
          }
        } else { // No PING Mode
          _sleepMode = false;
        }
      }
    } else {

/* Starting establishing connection to MQTT Broker */

#ifdef AFE_CONFIG_HARDWARE_LED
      if (_ledStartTime == 0) {
        _ledStartTime = millis();
      }
#endif
      if (_delayStartTime == 0) {
        _delayStartTime = millis();
        /* Connecing to MQTT Broker depending on LWT topics being set or no */

        boolean _connected = false;
        if (configuration.pingHostBeforeConnection) {
          if (!pingHost()) {
            _sleepMode = true;
            _sleepStartTime =
                millis() + _NetworkConfiguration.waitTimeSeries * 1000;
            return;
          }
        }

#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
        char lwtMessage
            [100]; // {"command":"udevice","idx":999999,"nvalue":"0,"svalue":"disconnected","Battery":100,"RSSI":1000}

        if (configuration.lwt.idx > 0) {
          sprintf(lwtMessage, "{\"command\":\"udevice\",\"idx\":%d,\"nvalue\":"
                              "0,\"svalue\":\"%s\",\"Battery\":0,\"RSSI\":0}",
                  configuration.lwt.idx, L_DISCONNECTED);

          _connected = _Broker.connect(_DeviceName, configuration.user,
                                       configuration.password,
                                       AFE_CONFIG_API_DOMOTICZ_TOPIC_IN, 1,
                                       configuration.retainLWT, lwtMessage);
        } else {
          _connected = _Broker.connect(_DeviceName, configuration.user,
                                       configuration.password);
        }
#else
        _connected = strlen(configuration.lwt.topic) > 0
                         ? _Broker.connect(
                               _DeviceName, configuration.user,
                               configuration.password, configuration.lwt.topic,
                               1, configuration.retainLWT, "disconnected")
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
      if (millis() > _delayStartTime +
                         (_NetworkConfiguration.waitTimeConnections * 1000)) {

        _connections++;
        //yield(); // @TODO removed with T7
#ifdef DEBUG
        Serial << endl
               << F("INFO: MQTT Connection attempt: ") << _connections
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

boolean AFEMQTT::pingHost(void) {
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

boolean AFEMQTT::eventConnected() {
  boolean returnValue = _eventConnectionEstablished;
  _eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEMQTT::publish(const char *topic, const char *message) {

  boolean _status = false;

  if (_Broker.state() == MQTT_CONNECTED) {
#ifdef AFE_CONFIG_HARDWARE_LED
    _Led->on();
#endif
#ifdef DEBUG
    Serial << endl << F("----------- Publish MQTT -----------");
    Serial << endl << F("Topic: ") << topic;
    Serial << endl << F("Message: ") << message;
    Serial << endl << F("Retain: ") << (configuration.retainAll ? "YES" : "NO");
#endif
    if (strlen(topic) > 0) {
      _status =
          _Broker.publish(topic, message, configuration.retainAll);
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
           << (_status ? F("published") : F("NOT pubslished"));
    Serial << endl << F("------------------------------------");
#endif
  }

  return _status;
}