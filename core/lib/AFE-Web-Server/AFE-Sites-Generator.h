/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#include <arduino.h>

#include <AFE-API-JSONRPC.h>
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

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Sensor-PN532.h>
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
  AFEJSONRPC *RestAPI;

  String _HtmlResponse;

  char deviceID[17];

  void generateHeader(String &page, uint16_t redirect);

  /* Method addes configuration block to the site */
  void openSection(String &page, const char *title,
                   const __FlashStringHelper *description);
  void openSection(String &page, const __FlashStringHelper *title,
                   const __FlashStringHelper *description);

  void closeSection(String &page);

  void openMessageSection(String &page, const char *title,
                          const __FlashStringHelper *description);

  void openMessageSection(String &page, const __FlashStringHelper *title,
                          const __FlashStringHelper *description);

  void closeMessageSection(String &page);

  /* Item: HTML <input type=""> */
  void addInputFormItem(String &item, const char *type, const char *name,
                        const char *label, const char *value,
                        const char *size = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *min = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *max = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *step = AFE_FORM_ITEM_SKIP_PROPERTY,
                        const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                        boolean readonly = false);

  /* Item: HTML input checkbox or radio */
  void _addSelectionFormItem(String &item, boolean type, const char *name,
                             const char *label, const char *value,
                             boolean checked,
                             const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                             boolean disabled = false);

  /* Item: HTML <input type="checkbox" /> */
  void addCheckboxFormItem(String &item, const char *name, const char *label,
                           const char *value, boolean checked,
                           const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                           boolean disabled = false);

  /* Item: HTML <input type="radio" /> */
  void addRadioButtonFormItem(String &item, const char *name, const char *label,
                              const char *value, boolean checked,
                              const char *hint = AFE_FORM_ITEM_SKIP_PROPERTY,
                              boolean disabled = false);

  /* Item: HTML <select><option></option></select> */
  void addSelectFormItemOpen(String &item, const __FlashStringHelper *name,
                             const __FlashStringHelper *label);
  void addSelectOptionFormItem(String &item, const char *label,
                               const char *value, boolean selected);
  void addSelectFormItemClose(String &item);

  /* Item: Menu */
  void addMenuItem(String &item, const __FlashStringHelper *title,
                   uint8_t siteId);

  void addMenuItemExternal(String &item, const __FlashStringHelper *title,
                           const __FlashStringHelper *url);
  void addMenuHeaderItem(String &item, const __FlashStringHelper *title);
  void addMenuSubItem(String &item, const char *title, uint8_t numberOfItems,
                      uint8_t siteId);

  /* Item: HTML <select> populated with GPIOs */
  void addListOfGPIOs(String &item, const __FlashStringHelper *field,
                      uint8_t selected, const char *title = "GPIO");

  /* Item: HTML <select> populated with <option> for number of items selection
   */
  void addListOfHardwareItem(String &item, uint8_t noOfItems,
                             uint8_t noOffConnected,
                             const __FlashStringHelper *field,
                             const __FlashStringHelper *label,
                             boolean disabled = false);

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  void addListOfMCP23017GPIOs(String &item, const char *field, uint8_t selected,
                              const char *title = "MCP23017 GPIO");
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void addRegulatorControllerItem(String &item, REGULATOR *configuration);
#endif

/* Item: list of gate states */
#ifdef AFE_CONFIG_HARDWARE_GATE
  void addGateStatesListItem(String &item, const __FlashStringHelper *name,
                             byte state);
#endif

/* Item: list of contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void addGateContactronsListItem(String &item, const __FlashStringHelper *name,
                                  uint8_t contactronId);
#endif

/* Item: list discovered IIC devices */
#ifdef AFE_CONFIG_HARDWARE_I2C
  void addDeviceI2CAddressSelectionItem(String &page, uint8_t address);
#endif

/* Item: list of LEDs */
#ifdef AFE_CONFIG_HARDWARE_LED
  void addLEDSelectionItem(String &page, uint8_t id);
#endif

public:
  /* Constructor*/
  AFESitesGenerator();

  void begin(AFEDataAccess *, AFEDevice *, AFEFirmwarePro *, AFEJSONRPC *);

  /* Method generates site header with menu. When redirect param is diff than 0
    then it will redirect page to main page after redirect param time (in sec)
   */
  void generateEmptyMenu(String &page, uint16_t redirect = 0);
  void generateMenu(String &page, uint16_t redirect = 0);

  /* Method generates site footer */
  void generateFooter(String &page, boolean extended = false);

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  /* These methods generates firmware upgrade sections */
  void siteUpgrade(String &page);
  void sitePostUpgrade(String &page, boolean status);
  void siteWANUpgrade(String &page, const __FlashStringHelper *title);
#endif

  /* Method generate restore to defaults section. Command = 0 is pre-reset site,
   * 1 is a post reset site */
  void siteReset(String &page);
  void sitePostReset(String &page);

  /* Method addes info that device is being reset */
  void siteExit(String &page, uint8_t command);

  /* Method generates section shown when device is in norma mode */
  void siteIndex(String &page, boolean authorized);

  /* All following methods generates configuration sections */
  void siteDevice(String &page);
  void siteNetwork(String &page);
  void siteConnecting(String &page);
  void siteMQTTBroker(String &page);
#ifdef AFE_CONFIG_API_DOMOTICZ_ENABLED
  void siteDomoticzServer(String &page);
#endif
  void sitePassword(String &page);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void siteRelay(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void siteSwitch(String &page, uint8_t id);
#endif

  void siteProKey(String &page);

#ifdef AFE_CONFIG_HARDWARE_LED
  void siteLED(String &page, uint8_t id);
  void siteSystemLED(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void siteDS18B20Sensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  void siteDHTSensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void siteRegulator(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void siteThermalProtector(String &page, uint8_t id);
#endif

#if defined(T3_CONFIG)
  String sitePIR(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void siteGate(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void siteContactron(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void siteHPMA115S0Sensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void siteBMEX80Sensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void siteBH1750Sensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void siteAS3935Sensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void siteAnemometerSensor(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  void siteADCInput(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
  void siteUARTBUS(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_I2C
  void siteI2CBUS(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void siteRainmeterSensor(String &page);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void siteBinarySensor(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  void sitePN532Sensor(String &page, uint8_t id);
  void sitePN532SensorAdmin(String &page, uint8_t id);
  void siteMiFareCard(String &page, uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  void siteCLED(String &page, uint8_t id);
#endif
};

#endif
