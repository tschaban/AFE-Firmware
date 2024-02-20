/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Firmware_h
#define _AFE_Firmware_h

#include <AFE-API-JSONRPC.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-WiFi.h>



#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFEFirmware //: public AFECoreHardware
{

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
#ifdef DEBUG
  AFEDebugger *Debugger = new AFEDebugger();
#endif

  struct GLOBAL_API_OBJECTS {
    AFEWiFi *Network = new AFEWiFi();
    AFEJSONRPC *REST = new AFEJSONRPC();
    AFEDataAccess *Flash = new AFEDataAccess();
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
    time_t currentTime;
  };

  TIMER_OBJECT *timer = new TIMER_OBJECT;

  GLOBAL_API_OBJECTS *API = new GLOBAL_API_OBJECTS;
  GLOBAL_CONFIGURATION_OBJECTS *Configuration =
      new GLOBAL_CONFIGURATION_OBJECTS;

  AFEDevice *Device = new AFEDevice();

  /* Constructor */
  AFEFirmware();

  void begin();
  /**
   * @brief Initialization of the LITLLEFS or SPIFFS file system
   *
   * @return true success, false failure
   */
  boolean initializeFS(void);

  void initializeNetwork(void);
  void checkFirmwareVersion(void);
  void validateProVersion(void);
  void synchronizeTime(void);
 /* Currently not used. If needed uncomment it */
  void getCurrentTime(char *timestamp);

};

#endif
