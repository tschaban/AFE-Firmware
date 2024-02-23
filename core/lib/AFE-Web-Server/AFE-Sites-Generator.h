#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#include <AFE-Firmware.h>
#include <AFE-Site-components.h>
#include <Arduino.h>

#ifdef AFE_ESP32
#include <WiFi.h>
#else /* ESP8266 */
#include <ESP8266WiFi.h>
#endif // ESP32/ESP8266

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-Wire-Container.h>
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Sensor-PN532.h>
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#if AFE_LANGUAGE == 0
#include <pl_PL.h>
#else
#include <en_EN.h>
#endif

class AFESitesGenerator {

private:
  AFEFirmware *Firmware;
  String _HtmlResponse;

#ifdef AFE_CONFIG_HARDWARE_I2C
  AFEWireContainer *WirePort;
  void begin(AFEFirmware *);
#endif // AFE_CONFIG_HARDWARE_I2C

  void generateHeader(String &page, uint16_t redirect);

  /**
   * @brief Method addes configuration block to the site
   *
   * @param  page             return string with section HTML code
   * @param  title            Title of the section
   * @param  description      Sub-title of the section
   */
  void openSection(String &page, const char *title,
                   const __FlashStringHelper *description);

  /**
   * @brief Method addes configuration block to the site
   *
   * @param  page             return string with section HTML code
   * @param  title            Title of the section
   * @param  description      Sub-title of the section
   */
  void openSection(String &page, const __FlashStringHelper *title,
                   const __FlashStringHelper *description);

  /**
   * @brief Closes section
   *
   * @param  page             return string with section HTML code
   */
  void closeSection(String &page);

  void openMessageSection(String &page, const char *title,
                          const __FlashStringHelper *description);

  void openMessageSection(String &page, const __FlashStringHelper *title,
                          const __FlashStringHelper *description);

  void closeMessageSection(String &page);

  /**
 * @brief Generates HTML <input> tag
 *
 * @param  item             return string with generated tag
 * @param  type             type of the input
 * @param  name             name if the input itme
 * @param  label            desc
 * @param  value            value of the input
 * @param  size             size of the input
 * @param  min              minimal value if a number
 * @param  max              maximum value if a number
 * @param  step             step if a number value
 * @param  hint             hint text
 * @param  readonly         if true then input is readonly
 */
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

  /* Item: HTML <select> populated with GPIOs
  */
  void addListOfGPIOs(String &item, const __FlashStringHelper *field,
                      uint8_t selected, const char *title = "GPIO",
                      boolean generatedADCGpios = false);

  /* Item: HTML <select> populated with <option> for number of items selection
   */
  void addListOfHardwareItem(String &item, uint8_t noOfItems,
                             uint8_t noOffConnected,
                             const __FlashStringHelper *field,
                             const __FlashStringHelper *label,
                             boolean disabled = false);

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  void addListOfMCP23XXXGPIOs(String &item, const char *field,
                              uint8_t selected);

  void addMCP23XXXSelection(String &item, const char *field, uint8_t selected);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void addRegulatorControllerItem(String &item, REGULATOR *configuration);
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

/* Item: list of gate states */
#ifdef AFE_CONFIG_HARDWARE_GATE
  void addGateStatesListItem(String &item, const __FlashStringHelper *name,
                             byte state);
#endif // AFE_CONFIG_HARDWARE_GATE

/* Item: list of contactrons */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void addGateContactronsListItem(String &item, const __FlashStringHelper *name,
                                  uint8_t contactronId);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

/* Item: list discovered IIC devices */
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
  void addDeviceI2CAddressSelectionItem(String &item, uint8_t wirePortId,
                                        uint8_t address);
#else  // ESP8266
  void addDeviceI2CAddressSelectionItem(String &item, uint8_t address);
#endif // ESP32/ESP8266
#endif // AFE_CONFIG_HARDWARE_I2C

/* Item: list of LEDs */
#ifdef AFE_CONFIG_HARDWARE_LED
  void addLEDSelectionItem(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_LED

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void addAPIsSection(String &page, const __FlashStringHelper *header,
                      const __FlashStringHelper *info, const char *label,
                      uint32_t *idx);
#else
  void addAPIsSection(String &page, const __FlashStringHelper *header,
                      const __FlashStringHelper *info, const char *label,
                      const char *topic);
#endif

  /**
   * @brief Adds warning session to the configuration form is a
   * configuration
   * file doesn't exist
   *
   * @param  page             return string
   */
  void addFileNotFound(String &page);

  /**
   * @brief Adds information item in the form
   *
   * @param  item             return string
   * @param  information      information text
   */
  void addInformationItem(String &item, const __FlashStringHelper *information);

#ifdef AFE_CONFIG_HARDWARE_CLED
  /**
   * @brief Adds url item (<a> html tag) to the list group. openMessageSection
   * and closeMessageSection are required
   *
   * @param  item             return string
   * @param  option           site ID -> url option "o"
   * @param  id               object id -> url option "i"
   * @param  label            label of <a>label</a>
   */

  void addUrlItem(String &item, uint8_t option, uint8_t id, const char *label);
  void addCLEDMenuSection(String &section, uint8_t effectId, uint8_t id);
  void addCLEDColorItem(String &item, CLED_RGB *color, const char *labelPrefix,
                        const __FlashStringHelper *label);
#endif // AFE_CONFIG_HARDWARE_CLED

public:
  /**
   * @brief Construct a new AFESitesGenerator object
   *
   */
  AFESitesGenerator();

#ifdef AFE_CONFIG_HARDWARE_I2C
  void begin(AFEFirmware *, AFEWireContainer *);
#else
  void begin(AFEFirmware *);
#endif // AFE_CONFIG_HARDWARE_I2C

  /**
   * @brief Method generates site header with menu. When redirect param is diff
   than 0
      then it will redirect page to main page after redirect param time (in sec)
   *
   * @param  page             desc
   * @param  redirect         desc
   */
  void generateMenuHeader(String &page, uint16_t redirect = 0);
  void generateMenu(String &page, uint16_t redirect = 0);
  void generateEmptyMenu(String &page, uint16_t redirect = 0);

  /**
   * @brief Generates the site footer part. It also replace the site template
   * {{parameters}} to thier values
   *
   * @param  page             return string
   * @param  extended         if true addess additinal information to the site.
   *
   */
  void generateFooter(String &page, boolean extended = false);

  /**
 * @brief replace all {{attributes}}
 *
 * @param  page             site with replaced attributes
 */
  void setAttributes(String *page);

#ifndef AFE_CONFIG_OTA_NOT_UPGRADABLE
  /**
   * @brief These methods generates firmware upgrade sections
   *
   * @param  page             desc
   */
  void siteUpgrade(String &page);
  void sitePostUpgrade(String &page, boolean status);
  void siteWANUpgrade(String &page, const __FlashStringHelper *title);
#endif // AFE_CONFIG_OTA_NOT_UPGRADABLE

  /**
   * @brief Method generate restore to defaults section
   *
   * @param  page             return string with the site HTML
   */
  void siteReset(String &page);

  void sitePostReset(String &page);

  /**
   * @brief Method addes info that device is being reset
   *
   * @param  page             return string with the site HTML
   * @param  configMode       config mode: over WiFi or HotSpot
   */
  void siteExit(String &page, uint8_t configMode);

  /* Method generates section shown when device is in norma mode */
  void siteIndex(String &page, boolean authorized);

  /**
   * @brief Site: Firmware version
   *
   * @param  page
   */

  /**
   * @brief Site: Firmware version
   *
   * @param  page             return string with the site
   * @param  details          add additional information to the site
   */
  void siteFirmware(String &page, boolean details = false);

  /**
   * @brief Site: Logs
   *
   * @param  page             return string with the site
   */
  void siteLogs(String &page);

  /**
   * @brief All following methods generates configuration sections
   *
   * @param  page             desc
   */
  void siteDevice(String &page);
  void siteNetwork(String &page);
  void siteConnecting(String &page);
  void siteMQTTBroker(String &page);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  void siteDomoticzServer(String &page);
#elif AFE_FIRMWARE_API == AFE_FIRMWARE_API_HOME_ASSISTANT
  void siteHomeAssistantDiscoveryConfiguration(String &page);
#endif // Domoticz or Home Assistant API

  void sitePassword(String &page);

#ifdef AFE_CONFIG_HARDWARE_RELAY
  void siteRelay(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void siteSwitch(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_SWITCH

  void siteProKey(String &page);

#ifdef AFE_CONFIG_HARDWARE_LED
  void siteLED(String &page, uint8_t id);
  void siteSystemLED(String &page);
#endif // AFE_CONFIG_HARDWARE_LED

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void siteDS18B20Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DS18B20

#ifdef AFE_CONFIG_HARDWARE_DHT
  void siteDHTSensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_DHT

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void siteRegulator(String &page, uint8_t id);
#endif // AFE_CONFIG_FUNCTIONALITY_REGULATOR

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  void siteThermalProtector(String &page, uint8_t id);
#endif // AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR

#if defined(T3_CONFIG)
  String sitePIR(uint8_t id);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void siteGate(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_GATE

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void siteContactron(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CONTACTRON

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void siteHPMA115S0Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_HPMA115S0

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void siteBMEX80Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BMEX80

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void siteBH1750Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BH1750

#ifdef AFE_CONFIG_HARDWARE_AS3935
  void siteAS3935Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_AS3935

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  void siteAnemometerSensor(String &page);
#endif // AFE_CONFIG_HARDWARE_ANEMOMETER

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  void siteADCInput(String &page, uint8_t id);
#else  // ESP8266
  void siteADCInput(String &page);
#endif // ESP32/ESP88266
#endif // AFE_CONFIG_HARDWARE_ANALOG_INPUT

#ifdef AFE_CONFIG_HARDWARE_UART
  void siteUARTBUS(String &page);
#endif // AFE_CONFIG_HARDWARE_UART

#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef AFE_ESP32
  void siteI2CBUS(String &page, uint8_t id);
#else  // ESP8266
  void siteI2CBUS(String &page);
#endif // ESP32
#endif // AFE_CONFIG_HARDWARE_I2C

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  void siteRainmeterSensor(String &page);
#endif // AFE_CONFIG_HARDWARE_RAINMETER

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void siteBinarySensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_BINARY_SENSOR

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
  void sitePN532Sensor(String &page, uint8_t id);
  void sitePN532SensorAdmin(String &page, uint8_t id);
  void siteMiFareCard(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

#ifdef AFE_CONFIG_HARDWARE_CLED
  void siteCLED(String &page, uint8_t id);
  void siteCLEDEffectBlinking(String &page, uint8_t id);
  void siteCLEDEffectWave(String &page, uint8_t id);
  void siteCLEDEffectFadeInOut(String &page, uint8_t id);
  void siteCLEDPN532SensoreEffect(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  void siteTSL2561Sensor(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  void siteMCP23XXX(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_MCP23XXX

#ifdef AFE_CONFIG_HARDWARE_FS3000
  void siteFS3000(String &page, uint8_t id);
#endif // AFE_CONFIG_HARDWARE_FS3000
};
#endif // _AFE_Sites_Generator_h
