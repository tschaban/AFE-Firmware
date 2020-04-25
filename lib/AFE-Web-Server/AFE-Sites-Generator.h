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

  void generateHeader(String &page, uint16_t redirect);

  /* Method generates GPIO selecton list */
  void generateConfigParameter_GPIO(String &item, const char *field,
                                    uint8_t selected,
                                    const char *title = "GPIO");

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  /* These three methods generates checkboxes for Switch, Relay and LED */
  const String generateTwoValueController(REGULATOR configuration,
                                          uint8_t type);
#endif

  /* Method addes configuration block to the site */
  void addConfigurationBlock(String &page, const char *, const char *);

  /* It uses generateHardwareItemsList() */
  void generateHardwareList(String &item, uint8_t noOfItems,
                            uint8_t noOffConnected, const char *field,
                            const char *label, uint8_t index, uint8_t noneValue,
                            boolean disabled = false);

  void generateHardwareItemsList(String &item, uint8_t noOfItems,
                                 uint8_t noOffConnected, const char *field,
                                 const char *label, boolean disabled = false);

  void addItem(String &item, const char *type, const char *name,
               const char *label, const char *value, const char *size = "?",
               const char *min = "?", const char *max = "?",
               const char *step = "?", const char *hint = "?",
               boolean readonly = false);

#if defined(T5_CONFIG)
  void generateGateStatesList(String &item, uint8_t id, byte state);
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
  void generateOneColumnLayout(String &page, uint16_t redirect = 0);
  void generateTwoColumnsLayout(String &page, uint16_t redirect = 0);

  /* Method generates site footer */
  void generateFooter(String &page, boolean extended = false);

  /* All following methods generates configuration sections */
  void addDeviceConfiguration(String &page);
  void addNetworkConfiguration(String &page);
  void addConnectingSite(String &page);
  void addMQTTBrokerConfiguration(String &page);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void addDomoticzServerConfiguration(String &page);
#endif
  void addPasswordConfigurationSite(String &page);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void addRelayConfiguration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void addSwitchConfiguration(String &page, uint8_t id);
#endif

  void addProVersionSite(String &page);

#ifdef AFE_CONFIG_HARDWARE_LED
  void addLEDConfiguration(String &page, uint8_t id);
  void addSystemLEDConfiguration(String &page);
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
  void addGateConfiguration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void addContactronConfiguration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void addHPMA115S0Configuration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void addBMEX80Configuration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void addBH1750Configuration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void addAS3935Configuration(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_WIND_SENSOR
  void addWindSensorConfiguration(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void addAnalogInputConfiguration(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  void addSerialPortConfiguration(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  void addI2CPortConfiguration(String &page);
  // String addDeviceI2CAddressSelection(uint8_t address);
  void addDeviceI2CAddressSelection(String &page, uint8_t address);
#endif

  /* These methods generates firmware upgrade sections */
  void addUpgradeSection(String &page);
  void addPostUpgradeSection(String &page, boolean status);

  /* Method generate restore to defaults section. Command = 0 is pre-reset site,
   * 1 is a post reset site */
  void addResetSection(String &page);
  void addPostResetSection(String &page);

  /* Method addes info that device is being reset */
  void addExitSection(String &page, uint8_t command);

  /* Method generates section shown when device is in norma mode */
  void addIndexSection(String &page, boolean authorized);
};

#endif
