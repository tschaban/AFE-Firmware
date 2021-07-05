/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/
The class is used to handle On/Off or Open/Closed states
*/

#ifndef _AFE_MiFare_Card_h
#define _AFE_MiFare_Card_h

#include <AFE-Configuration.h>
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR

#include <AFE-Data-Access.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEMiFareCard {

private:
  boolean _statusChanged = false;
  unsigned long _timer = 0;

public:
  MIFARE_CARD configuration;
  uint8_t state = AFE_HARDWARE_MIFARE_CARD_ACTION_OFF;
  /* Constructor */
  AFEMiFareCard();

  /* Init switch */
  void begin(uint8_t id, AFEDataAccess *);

  /* Used to set On/Off or Open/Closed state. When card read and with access
   * rights */
  void set();

  /* Required to process MiFare Card states */ 
  boolean listener();
};

#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR
#endif // _AFE_MiFare_Card_h
