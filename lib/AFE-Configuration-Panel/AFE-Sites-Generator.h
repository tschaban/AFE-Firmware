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

#include <AFE-CSS.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-EEPROM.h>
#include <AFE-Firmware.h>
#include <ESP8266WiFi.h>

#ifdef CONFIG_HARDWARE_UART
#include <AFE-I2C-Scanner.h>
#endif

#ifdef CONFIG_HARDWARE_BMX80
#include <AFE-Sensor-BMx80.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

#if defined(AFE_LANGUAGE) && AFE_LANGUAGE == pl_PL
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

class AFESitesGenerator {

private:
  AFEEEPROM Eeprom;
  AFEDataAccess Data;
  AFEDevice *Device;
  FIRMWARE firmware;
  AFEFirmware Firmware;
  char deviceID[17];

  const String generateHeader(uint8_t redirect);

  /* Method generates GPIO selecton list */
  const String generateConfigParameter_GPIO(const char *field, uint8_t selected,
                                            const String title = "GPIO");
#ifdef CONFIG_FUNCTIONALITY_REGULATOR
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

#ifdef CONFIG_FUNCTIONALITY_THERMOSTAT
  String addThermostateMenuItem();
#endif

#ifdef CONFIG_FUNCTIONALITY_HUMIDISTAT
  String addHumidistatMenuItem();
#endif

public:
  /* Constructor*/
  AFESitesGenerator();

  void begin(AFEDevice *);

  /* Method generates site header with menu. When redirect param is diff than 0
    then it will redirect page to main page after redirect param time (in sec)
   */
  const String generateOneColumnLayout(uint8_t redirect = 0);
  const String generateTwoColumnsLayout(uint8_t redirect = 0);

  /* Method generates site footer */
  const String generateFooter(boolean extended = false);

  /* All following methods generates configuration sections */
  String addDeviceConfiguration();
  String addNetworkConfiguration();
  String addFirstLaunchConfiguration();
  String addConnectingSite();
  String addMQTTBrokerConfiguration();
  String addDomoticzServerConfiguration();
  String addPasswordConfigurationSite();
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);

#ifdef CONFIG_HARDWARE_LED
  String addLEDConfiguration(uint8_t id);
  String addSystemLEDConfiguration();
#endif

#ifdef CONFIG_HARDWARE_DS18B20
  String addDS18B20Configuration();
#endif

#ifdef CONFIG_HARDWARE_DHXX
  String addDHTConfiguration();
#endif

#ifdef CONFIG_FUNCTIONALITY_REGULATOR
  String addRegulatorConfiguration(uint8_t type);
#endif

#if defined(T3_CONFIG)
  String addPIRConfiguration(uint8_t id);
#endif

#if defined(T5_CONFIG)
  String addGateConfiguration();
  String addContactronConfiguration(uint8_t id);
#endif

#ifdef CONFIG_HARDWARE_HPMA115S0
  String addHPMA115S0Configuration();
#endif

#ifdef CONFIG_HARDWARE_BMX80
  String addBMx80Configuration();
#endif

#ifdef CONFIG_HARDWARE_BH1750
  String addBH1750Configuration();
#endif

#ifdef CONFIG_HARDWARE_ADC_VCC
  String addAnalogInputConfiguration();
#endif

#ifdef CONFIG_HARDWARE_UART
  String addSerialPortConfiguration();
#endif

#ifdef CONFIG_HARDWARE_UART
  String addDeviceI2CAddressSelection(uint8_t address);
#endif

  /* These methods generates firmware upgrade sections */
  String addUpgradeSection();
  String addPostUpgradeSection(boolean status);

  /* Method generate restore to defaults section. Command = 0 is pre-reset site,
   * 1 is a post reset site */
  String addResetSection();
  String addPostResetSection();

  /* Method addes info that device is being reset */
  String addExitSection(uint8_t command);

  /* Method generates section shown when device is in norma mode */
  String addIndexSection(boolean authorized);

  String addProVersionSite();
};

#endif
