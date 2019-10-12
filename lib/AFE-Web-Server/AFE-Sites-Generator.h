/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

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
#include <AFE-Firmware-Pro.h>
#include <ESP8266WiFi.h>

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-I2C-Scanner.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

class AFESitesGenerator {

private:
  AFEDataAccess Data;
  AFEDevice *Device;
  FIRMWARE firmware;
  AFEFirmwarePro *Firmware;
  char deviceID[17];

  const String generateHeader(uint8_t redirect);

  /* Method generates GPIO selecton list */
  const String generateConfigParameter_GPIO(const char *field, uint8_t selected,
                                            const String title = "GPIO");
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  /* These three methods generates checkboxes for Switch, Relay and LED */
  const String generateTwoValueController(REGULATOR configuration,
                                          uint8_t type);
#endif

  /* Method addes configuration block to the site */
  String addConfigurationBlock(const String title, const String description,
                               const String body);

  const String generateHardwareList(uint8_t noOfItems, uint8_t noOffConnected,
                                    const char *field, const char *label,
                                    uint8_t index, uint8_t noneValue);

  /* It uses generateHardwareItemsList() */
  const String generateHardwareItemsList(uint8_t noOfItems,
                                         uint8_t noOffConnected,
                                         const char *field, const char *label);

  const String addItem(const char *type, const char *name, const char *label,
                       const char *value, const char *size = "?",
                       const char *min = "?", const char *max = "?",
                       const char *step = "?", const char *hint = "?",
                       boolean readonly = false);

#if defined(T5_CONFIG)
  const String generateGateStatesList(uint8_t id, byte state);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
  String addThermostateMenuItem();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
  String addHumidistatMenuItem();
#endif

public:
  /* Constructor*/
  AFESitesGenerator();

  void begin(AFEDevice *, AFEFirmwarePro *);

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
  String addConnectingSite();
  String addMQTTBrokerConfiguration();
  String addDomoticzServerConfiguration();
  String addPasswordConfigurationSite();
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);
  String addProVersionSite();

#if AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS > 0
  String addLEDConfiguration(uint8_t id);
  String addSystemLEDConfiguration();
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  String addDS18B20Configuration();
#endif

#ifdef AFE_CONFIG_HARDWARE_DHXX
  String addDHTConfiguration();
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  String addRegulatorConfiguration(uint8_t type);
#endif

#if defined(T3_CONFIG)
  String addPIRConfiguration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  String addGateConfiguration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  String addContactronConfiguration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  String addHPMA115S0Configuration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  String addBMEX80Configuration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  String addBH1750Configuration(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  String addAS3935Configuration();
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  String addAnalogInputConfiguration();
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  String addSerialPortConfiguration();
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  String addI2CPortConfiguration();
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
};

#endif
