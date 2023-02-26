/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Firmware_h
#define _AFE_Firmware_h

#include <AFE-Data-Access.h>
#include <AFE-API-JSONRPC.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmware {

private:
  AFEDataAccess *Data;
  AFEJSONRPC *RestAPI;
  unsigned long miliseconds = 0;
  unsigned long minutes = 0;

public:
  PRO_VERSION *pro = new PRO_VERSION;
  FIRMWARE *version = new FIRMWARE;

  /* Constructor */
  AFEFirmware();

  void begin(AFEDataAccess *, AFEJSONRPC *);

  /* Checking if the key is still valid */
  void validate(void);
  void listener(void);
  void getLatestFirmwareVersion(void);
};

#endif
