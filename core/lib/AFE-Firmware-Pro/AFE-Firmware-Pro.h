/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Firmware_Pro_h
#define _AFE_Firmware_Pro_h

#include <AFE-Data-Access.h>
#include <AFE-API-JSONRPC.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEFirmwarePro {

private:
  AFEDataAccess *Data;
  AFEJSONRPC *RestAPI;
  unsigned long miliseconds = 0;
  unsigned long minutes = 0;

public:
  PRO_VERSION Pro;

  /* Constructor */
  AFEFirmwarePro();

  void begin(AFEDataAccess *, AFEJSONRPC *);

  /* Checking if the key is still valid */
  void validate();
  void listener();
};

#endif
