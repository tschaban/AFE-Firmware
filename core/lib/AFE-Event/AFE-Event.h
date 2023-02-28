#ifndef _AFE_FIRMWARE_EVENT_h
#define _AFE_FIRMWARE_EVENT_h

#include <AFE-Configuration.h>
#include <AFE-Global-Object.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEEvent {

private:
  AFEGlobalObject *Object;

  struct EVENT_TIMER {
    unsigned long oneMinute;
    uint8_t oneHour;
    uint8_t oneDay;
  };

  EVENT_TIMER *timer = new EVENT_TIMER;

  void conenctedToNetwork(void);
  void disconnectedFromNetwork(void);
  void connectedToMQTTBroker(void);
  void disconnectedFromMQTTBroker(void);
  void checkingFirmwareVersion(void);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  void publishMQTTHADiscoveryConfiguration(void);
#endif

public:
  AFEEvent();
  void begin(AFEGlobalObject *);
  void listener(void);
};

#endif //_AFE_FIRMWARE_EVENT_h