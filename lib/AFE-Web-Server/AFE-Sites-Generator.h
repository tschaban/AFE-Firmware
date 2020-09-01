/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Site-components.h>
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

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
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
  AFEDataAccess *Data;
  AFEDevice *Device;
  FIRMWARE Firmware;
  AFEFirmwarePro *FirmwarePro;
  char deviceID[17];

  void generateHeader(String &page, uint16_t redirect);

  /* Method generates GPIO selecton list */
  void addListOfGPIOs(String &item, const char *field, uint8_t selected,
                      const char *title = "GPIO");

  /* Method addes configuration block to the site */
  void openSection(String &page, const char *, const char *);
  void closeSection(String &page);

  void addInputFormItem(String &item, const char *type, const char *name,
                        const char *label, const char *value,
                        const char *size = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *min = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *max = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *step = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                        boolean readonly = false);

  /* Type: true = checkbox, false = radio */
  void addSelectionFormItem(String &item, boolean type, const char *name,
                            const char *label, const char *value,
                            boolean checked,
                            const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                            boolean disabled = false);

  void addCheckboxFormItem(String &item, const char *name, const char *label,
                           const char *value, boolean checked,
                           const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                           boolean disabled = false);

  void addRadioButtonFormItem(String &item, const char *name, const char *label,
                              const char *value, boolean checked,
                              const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                              boolean disabled = false);

  void addSelectFormItemOpen(String &item, const char *name, const char *label);
  void addSelectOptionFormItem(String &item, const char *label,
                               const char *value, boolean selected);
  void addSelectFormItemClose(String &item);

  void addMenuItem(String &item, const char *title, uint8_t siteId);
  void addMenuHeaderItem(String &item, const char *title);
  void addMenuSubItem(String &item, const char *title, uint8_t numberOfItems,
                      uint8_t siteId);

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

  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *);

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
  void addDS18B20Configuration(String &page, uint8_t id);
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

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER_SENSOR
  void addAnemometerSensorConfiguration(String &page);
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

#ifdef AFE_CONFIG_HARDWARE_RAINMETER_SENSOR
  void addRainmeterSensorConfiguration(String &page);
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
