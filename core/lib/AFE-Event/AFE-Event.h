#ifndef _AFE_FIRMWARE_EVENT_h
#define _AFE_FIRMWARE_EVENT_h

#include <AFE-Configuration.h>
#include <AFE-Firmware.h>

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
#include <AFE-API-HTTP-Domoticz.h>
#include <AFE-API-MQTT-Domoticz.h>
#else
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
#include <AFE-API-HomeAssistant-Integration.h>
#endif
#include <AFE-API-MQTT-Standard.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEEvent {

private:
  AFEFirmware *Firmware;

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  AFEAPIMQTTDomoticz *MqttAPI;
  AFEAPIHTTPDomoticz *HttpDomoticzAPI;
#else
  AFEAPIMQTTStandard *MqttAPI;
#endif

  void conenctedToNetwork(void);
  void disconnectedFromNetwork(void);
  void connectedToMQTTBroker(void);
  void disconnectedFromMQTTBroker(void);
  void publishFirmwareVersion(void);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  void publishMQTTHADiscoveryConfiguration(void);
#endif

public:
  AFEEvent();

public:
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void begin(AFEFirmware *, AFEAPIMQTTDomoticz *, AFEAPIHTTPDomoticz *);
#else
  void begin(AFEFirmware *, AFEAPIMQTTStandard *);
#endif

  void begin(AFEFirmware *);

  void listener(void);
};

#endif //_AFE_FIRMWARE_EVENT_h