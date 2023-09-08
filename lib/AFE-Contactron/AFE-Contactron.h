/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Contactron_h
#define _AFE_Contactron_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON

#include <AFE-CONTACTRON-Structure.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif
#include <arduino.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEContactron {

public:
  CONTACTRON *configuration = new CONTACTRON;

  // ID of the GATE the contactron is assigned to. 255 None.
  uint8_t gateId = AFE_HARDWARE_ITEM_NOT_EXIST;

  /* Constructors */
  AFEContactron();

  /* Initialize. Must be run per each Gate object */
  void begin(uint8_t id, AFEDevice *, AFEDataAccess *);

  /* Method returns contactorn state */
  boolean get();

  /* Method returns true if cotactron state ahs changed */
  boolean changed();

  /* Method has to be added to the loop in order to listen for switch changes */
  void listener();

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

private:
  AFEDevice *Device;
  AFEDataAccess *Data;

  boolean _initialized = false;
  boolean state;            // It stores actual contactron state
  boolean _changed = false; // True if contractor changed state

  unsigned long startTime = 0;
#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *ContactronLed = new AFELED();
#endif
};

#endif // AFE_CONFIG_HARDWARE_CONTACTRON
#endif // _AFE_Contactron_h