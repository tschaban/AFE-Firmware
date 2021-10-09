/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_PIR_h
#define _AFE_PIR_h


#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_PIR

#include <AFE-Data-Access.h>
#include <AFE-LED.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEPIR {

private:
  boolean _initialized = false;
  boolean _stateChanged = false;
  boolean state;

  AFELED Led;

  /* Returns PIR State */
  byte get();

public:
  PIR configuration;

  /* Constructors */
  AFEPIR();
  AFEPIR(uint8_t id);

  /* Init PIR */
  void begin(uint8_t id);

  /* Returns true if motion has been detected */
  boolean motionDetected();

  /* Returns PIR MQTT Topic */
  const char *getMQTTTopic();

  /* Returns true is PIR state changed */
  boolean stateChanged();

  /* Methods returns PIR name */
  const char *getName();

  /* Method should be added to main loop to listen for PIR events */
  void listener();

  /* Method returns Domoticz IDX */
  unsigned long getDomoticzIDX();
};

#endif // AFE_CONFIG_HARDWARE_PIR
#endif
