/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-EEPROM.h>
#include <ESP8266WiFi.h>
#if defined(T6_CONFIG)
#include <AFE-Sensor-BMx80.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESitesGenerator {

private:
  AFEEEPROM Eeprom;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;
  FIRMWARE firmware;

  /* Method generates GPIO selecton list */
  const String generateConfigParameter_GPIO(const char *field, uint8_t selected,
                                            const String title = "GPIO");
#if defined(T1_CONFIG) || defined(T2_CONFIG)
  /* These three methods generates checkboxes for Switch, Relay and LED */
  const String generateTwoValueController(REGULATOR configuration,
                                          uint8_t type);
#endif

  /* Method addes configuration block to the site */
  String addConfigurationBlock(const String title, const String description,
                               const String body);

  const String generateHardwareItemsList(uint8_t noOfItems,
                                         uint8_t noOffConnected,
                                         const char *field, const char *label);

#if defined(T5_CONFIG)
  const String generateGateStatesList(uint8_t id, byte state);
#endif

public:
  /* Constructor*/
  AFESitesGenerator();

  /* Method generates site header with menu. When redirect param is diff than 0
    then it will redirect page to main page after redirect param time (in sec)
   */
  const String generateHeader(uint8_t redirect = 0);

  /* Method generates site footer */
  const char *generateFooter();

  /* All following methods generates configuration sections */
  String addLanguageConfiguration();
  String addDeviceConfiguration();
  String addNetworkConfiguration();
  String addMQTTBrokerConfiguration();
  String addDomoticzServerConfiguration();
#ifndef T0_SHELLY_1_CONFIG
  String addLEDConfiguration(uint8_t id);
  String addSystemLEDConfiguration();
#endif
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);

#ifdef T1_CONFIG
  String addDS18B20Configuration();
#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
  String addDHTConfiguration();
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG)
  String addRegulatorConfiguration(uint8_t type);
#endif

#if defined(T3_CONFIG)
  String addPIRConfiguration(uint8_t id);
#endif

#if defined(T5_CONFIG)
  String addGateConfiguration();
  String addContactronConfiguration(uint8_t id);
#endif

#if defined(T6_CONFIG)
  String addHPMA115S0Configuration();
  String addBMx80Configuration();
  String addBH1750Configuration();
  String addSerialPortConfiguration();
#endif

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
