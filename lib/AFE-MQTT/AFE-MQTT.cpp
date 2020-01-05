/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-MQTT.h"

AFEMQTT::AFEMQTT(){};

void AFEMQTT::begin(AFEDataAccess *Data, char *deviceName) {
  _Data = Data;
  _deviceName = deviceName;
  configuration = _Data->getMQTTConfiguration();
  NetworkConfiguration = _Data->getNetworkConfiguration();

#ifdef AFE_CONFIG_HARDWARE_LED
  uint8_t systeLedID = _Data->getSystemLedID();
  if (systeLedID != AFE_HARDWARE_ITEM_NOT_EXIST) {
    Led.begin(systeLedID);
  }
#endif

  Broker.setClient(esp);
  if (strlen(configuration.host) > 0) {
    Broker.setServer(configuration.host, configuration.port);
  } else if (strlen(configuration.ip) > 0) {
    IPAddress ip;
    if (ip.fromString(configuration.ip)) {
      Broker.setServer(configuration.ip, configuration.port);
    }
#ifdef DEBUG
    else {
      Serial << endl
             << "ERROR: Problem with MQTT IP address: " << configuration.ip;
    }
#endif
  } else {
    isConfigured = false;
  }

  Data = {};

#ifdef DEBUG
  Serial << endl
         << endl
         << "---------------------- MQTT ----------------------" << endl
         << "Host: " << configuration.host << endl
         << "IP: " << configuration.ip << endl
         << "Port: " << configuration.port << endl
         << "User: " << configuration.user << endl
         << "Password: " << configuration.password << endl
#ifdef AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
         << "LWT Topic: " << configuration.lwt.topic << endl
#endif
         << "--------------------------------------------------";
#endif
}

#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS
void AFEMQTT::subscribe(const char *topic) {
#ifdef DEBUG
  Serial << endl << "MQTT: Subscribing to : " << topic;
#endif
  if (strlen(topic) > 0) {
    Broker.subscribe(topic);
  }
}
#endif

void AFEMQTT::disconnect() {
  if (Broker.connected()) {
    Broker.disconnect();
  }
}

boolean AFEMQTT::listener() {
  boolean _ret = false;
  if (Broker.connected()) {
    _ret = Broker.loop();
#ifdef AFE_CONFIG_API_PROCESS_MQTT_REQUESTS    
    message.topic = Broker.topic;
    message.content = Broker.payload;
    message.length = Broker.length;
/*
#ifdef DEBUGA
  Serial << endl << endl << "--------- Got MQTT request ---------";
  Serial << endl << "Topic: " << message.topic;
  Serial << endl << "Message: ";
  for (uint8_t _i = 0; _i < message.length; _i++) {
    Serial << char(message.content[_i]);
  }
   Serial << endl << "-----------------------------------";
#endif
*/
#endif
  } else {
    connect();
  }
  return _ret;
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          NetworkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {
#ifdef AFE_CONFIG_HARDWARE_LED
      if (ledStartTime == 0) {
        ledStartTime = millis();
      }
#endif
      if (delayStartTime == 0) {
        delayStartTime = millis();

/* Connecing to MQTT Broker depending on LWT topics being set or no */

#ifdef AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
        boolean _connected =
            strlen(configuration.lwt.topic) > 0
                ? Broker.connect(
                      _deviceName, configuration.user, configuration.password,
                      configuration.lwt.topic, 2, false, "disconnected")
                : Broker.connect(deviceName, configuration.user,
                                 _deviceName.password);
#else
        boolean _connected = Broker.connect(_deviceName, configuration.user,
                                            configuration.password);
#endif

        if (_connected) {
          eventConnectionEstablished = true;
          delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
          ledStartTime = 0;
          Led.off();
#endif
          connections = 0;

          return;
        }
      }
#ifdef AFE_CONFIG_HARDWARE_LED
      if (millis() > ledStartTime + 500) {
        Led.toggle();
        ledStartTime = 0;
      }
#endif
      if (millis() >

          delayStartTime + (NetworkConfiguration.waitTimeConnections * 1000)) {
        connections++;
#ifdef DEBUG
        Serial << endl
               << "MQTT Connection attempt: " << connections + 1 << " from "
               << NetworkConfiguration.noConnectionAttempts
               << ", connection status: " << Broker.state()
               << ", connection time: " << millis() - delayStartTime << "ms";

#endif
        delayStartTime = 0;
      }

      if (connections >= NetworkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();

        delayStartTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
        ledStartTime = 0;
        Led.off();
#endif
        connections = 0;

#ifdef DEBUG
        Serial << endl
               << "MQTT: WARN: Not able to connect to MQTT.Going to sleep mode "
                  "for "
               << NetworkConfiguration.waitTimeSeries << "sec.";
#endif
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

boolean AFEMQTT::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}

boolean AFEMQTT::publish(const char *topic, const char *message) {

  boolean pubslishingStatus = false;

  if (Broker.state() == MQTT_CONNECTED) {
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.on();
#endif
#ifdef DEBUG
    Serial << endl << endl << "----------- Publish MQTT -----------";
    Serial << endl << "Topic: " << topic;
    Serial << endl << "Message: " << message;
#endif
    if (strlen(topic) > 0) {
      pubslishingStatus = Broker.publish(topic, message);
    }
#ifdef DEBUG
    else {
      Serial << endl << "Status: WARNING, missing MQTT Topic";
    }
#endif
#ifdef AFE_CONFIG_HARDWARE_LED
    Led.off();
#endif
#ifdef DEBUG
    Serial << endl
           << "Status: "
           << (pubslishingStatus ? "published" : "NOT pubslished");
    Serial << endl << "------------------------------------";
#endif
  }

  return pubslishingStatus;
}


#ifdef AFE_CONFIG_FUNCTIONALITY_MQTT_LWT
const char *AFEMQTT::getLWTTopic() { return configuration.lwt.topic; }
#endif
