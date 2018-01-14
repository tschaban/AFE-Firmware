/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Data-Structures.h>
#include <AFE-Device.h>
#include <AFE-EEPROM.h>
#include <Streaming.h>

class AFESitesGenerator {

private:
  AFEEEPROM Eeprom;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;

  /* Method generates GPIO selecton list */
  const String generateConfigParameter_GPIO(const char *field,
                                            uint8_t selected);

  /* Method addes configuration block to the site */
  String addConfigurationBlock(const String title, const String description,
                               const String body);

  const String generateHardwareItemsList(uint8_t noOfItems,
                                         uint8_t noOffConnected,
                                         const char *field, const char *label);

public:
  /* Constructor*/
  AFESitesGenerator();

  /* Method generates site header with menu. When redirect param is diff than 0
   * then it will redirect page to main page after redirect param time (in sec)
   */
  const String generateHeader(uint8_t redirect = 0);

  /* Method generates site footer */
  const char *generateFooter();

  /* All following methods generates configuration sections */
  String addLanguageConfiguration();
  String addDeviceConfiguration();
  String addNetworkConfiguration();
  String addMQTTBrokerConfiguration();
  String addLEDConfiguration(uint8_t id);
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);
  String addPIRConfiguration(uint8_t id);

  /* These methods generates firmware upgrade sections */
  String addUpgradeSection();
  String addPostUpgradeSection(boolean status);

  /* Method generate restore to defaults section. Command = 0 is pre-reset site,
   * 1 is a post reset site */
  String addResetSection(uint8_t command);

  /* Method addes info that device is being reset */
  String addExitSection();

  /* Method generates section shown when device is in norma mode */
  String addHelpSection();
};

#endif
