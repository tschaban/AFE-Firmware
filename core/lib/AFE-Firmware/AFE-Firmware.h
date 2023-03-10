/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Firmware_h
#define _AFE_Firmware_h

#include <AFE-Data-Access.h>
#include <AFE-Device.h>

#include <AFE-API-JSONRPC.h>
#include <AFE-WiFi.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmware {

private:
  unsigned long miliseconds = 0;
  unsigned long minutes = 0;
  /**
   * @brief Checking if the device is launched for a first time. If so it loads
   * default configuration
   *
   */
  void firstBooting(void);

public:
  struct GLOBAL_API_OBJECTS {
    AFEWiFi *Network = new AFEWiFi();
    AFEJSONRPC *REST = new AFEJSONRPC();
    AFEDataAccess *Flash = new AFEDataAccess();
  };

  struct GLOBAL_HARDWARE_OBJECTS {
#ifdef AFE_CONFIG_HARDWARE_LED
    AFELED *SystemLed = new AFELED();
#endif
  };

  struct GLOBAL_CONFIGURATION_OBJECTS {
    PRO_VERSION *Pro = new PRO_VERSION;
    FIRMWARE *Version = new FIRMWARE;
  };

  struct TIMER_OBJECT {
    unsigned long miliseconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    uint8_t months;
  };

  TIMER_OBJECT *timer = new TIMER_OBJECT;

  GLOBAL_API_OBJECTS *API = new GLOBAL_API_OBJECTS;
  GLOBAL_HARDWARE_OBJECTS *Hardware = new GLOBAL_HARDWARE_OBJECTS;
  GLOBAL_CONFIGURATION_OBJECTS *Configuration =
      new GLOBAL_CONFIGURATION_OBJECTS;

  AFEDevice *Device = new AFEDevice();

  /* Constructor */
  AFEFirmware();

  void begin();

  void initializeNetwork(void);
  void checkFirmwareVersion(void);
  void validateProVersion(void);

#ifdef AFE_CONFIG_HARDWARE_LED
  void initializeSystemLED(void);
#endif

};

#endif
