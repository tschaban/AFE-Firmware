/**
 * @file AFE-Configuration.h
 * @author Tschaban-A (github@adrian.czabanowski.com)
 * @brief files contain firmware configuration parameters
 * @version 3.3.0
 * @date 2022-01-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

/* Serial port speed */
#define AFE_CONFIG_SERIAL_SPEED 115200

/* Configs releated to Debugger Enabled */
#ifdef DEBUG

/* Turn On/Off debbuging of Ping class */
#define ENABLE_DEBUG_PING

#endif // DEBUG

/* Urls */
#define AFE_URL_DOCUMENTATION "https://afe.smartnydom.pl"
#define AFE_URL_HELP "https://www.smartnydom.pl/forum/afe-firmware/"
#define AFE_API_URL "http://api.smartnydom.pl"

/**
 * @brief types of firmware upgrades
 * 
 */
#define AFE_UPGRADE_NONE 0
#define AFE_UPGRADE_VERSION 1
#define AFE_UPGRADE_VERSION_TYPE 2

/**
 * @brief firmware operating modes
 * 
 */
#define AFE_MODE_NORMAL 0
#define AFE_MODE_CONFIGURATION 1
#define AFE_MODE_ACCESS_POINT 2
#define AFE_MODE_NETWORK_NOT_SET 4
#define AFE_MODE_FIRST_TIME_LAUNCH 5

/**
 * @brief mDNS states
 * 
 */

#define AFE_CONFIG_NETWORK_MDNS_ACTIVE 1
#define AFE_CONFIG_NETWORK_MDNS_INACTIVE 0


/**
 * @brief include configuration related to network configuration
 * 
 */
#define AFE_CONFIG_NETWORK_DEFAULT_CONNECTION_ATTEMPTS 20
#define AFE_CONFIG_NETWORK_DEFAULT_WAIT_TIME 1
#define AFE_CONFIG_NETWORK_DEFAULT_WAIT_SERIES 20
#define AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER 2
#define AFE_CONFIG_NETWORK_DEFAULT_NONE_SSID "<nOOn>"
#define AFE_CONFIG_NETWORK_SSID_LABEL_SIZE sizeof(configuration.primary.ssid) + 30 // _(Sygnał: Bardzo dobry)
#define AFE_CONFIG_NETWORK_DEFAULT_RADIO_MODE AFE_NONE
#define AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER AFE_NONE
#define AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MIN 0
#define AFE_CONFIG_NETWORK_DEFAULT_OUTPUT_POWER_MAX 20.5
#define AFE_CONFIG_NETWORK_DEFAULT_DNS1 "8.8.8.8"
#define AFE_CONFIG_NETWORK_DEFAULT_DNS2 "8.8.4.4"
#define AFE_CONFIG_NETWORK_DEFAULT_MDNS AFE_CONFIG_NETWORK_MDNS_ACTIVE 


/**
 * @brief params related checking if a device has an access to the Internet
 * 
 */

#define AFE_WAN_ACCSSS_HOST AFE_CONFIG_NETWORK_DEFAULT_DNS1
#define AFE_WAN_ACCSSS_PINGS 1
#define AFE_WAN_ACCSSS_TIMEOUT 3000 // performs check if access to the network longer than this time in ms

/**
 * @brief types for APIs
 * 
 */
#define AFE_FIRMWARE_API_STANDARD 0
#define AFE_FIRMWARE_API_DOMOTICZ 1
#define AFE_FIRMWARE_API_HOME_ASSISTANT 2

/**
 * @brief standard default values for not used devices and empty string
 * 
 */
#define AFE_HARDWARE_ITEM_NOT_EXIST 255
#define AFE_NONE AFE_HARDWARE_ITEM_NOT_EXIST
#define AFE_EMPTY_STRING '\0'

/**
 * @brief Timers
 * 
 */

#define AFE_TIMER_ONE_MINUTE 60000 // in milseconds
#define AFE_TIMER_ONE_HOUR 59 // minutes
#define AFE_TIMER_ONE_DAY 23 // hours
#define AFE_TIMER_ONE_MONTH 29 // days

/**
 * @brief different binary default values
 * 
 */
#define AFE_OFF 0
#define AFE_ON 1
#define AFE_OPEN AFE_OFF
#define AFE_CLOSED AFE_ON
#define AFE_OFF_LABEL "off"
#define AFE_ON_LABEL "on"
#define AFE_OPEN_LABEL "open"
#define AFE_CLOSED_LABEL "closed"


/**
 * @brief Default time to auto-logout from config panel. In minutes
 * 
 */
#define AFE_AUTOLOGOFF_DEFAULT_TIME 10


/**
 * @brief include default values depending on ESP microcontroller type
 * 
 */
#ifdef AFE_ESP32 
#include <devices/AFE-ESP32.h>
#else /* ESP8266 */
#include <devices/AFE-ESP8266.h>
#endif

/**
 * @brief max firmware file size. used by OTA
 * 
 */
#define AFE_FIRMARE_FILE_NAME_LENGTH 100


/**
 * @brief include parameters related to mainly sensor's unites
 * 
 */
#include <AFE-Units.h>

/**
 * @brief include parameters related to hardwares types
 * 
 */
#include <devices/AFE-Devices.h>

/**
 * @brief include configuration files parameters
 * 
 */
#include <AFE-Files.h>

/**
 * @brief include configuration files per each AFE version
 * 
 */
#if defined(T0_CONFIG)
#include <firmwares/AFE-T0.h>
#include <builds/AFE-T0-Build.h>
#elif defined(T1_CONFIG)
#include <firmwares/AFE-T1.h>
#include <builds/AFE-T1-Build.h>
#elif defined(T2_CONFIG)
#include <firmwares/AFE-T2.h>
#include <builds/AFE-T2-Build.h>
#elif defined(T3_CONFIG)
#include <firmwares/AFE-T3.h>
#include <builds/AFE-T3-Build.h>
#elif defined(T4_CONFIG)
#include <firmwares/AFE-T4.h>
#include <builds/AFE-T4-Build.h>
#elif defined(T5_CONFIG)
#include <firmwares/AFE-T5.h>
#include <builds/AFE-T5-Build.h>
#elif defined(T6_CONFIG)
#include <firmwares/AFE-T6.h>
#include <builds/AFE-T6-Build.h>
#elif defined(T7_CONFIG)
#include <firmwares/AFE-T7.h>
#include <builds/AFE-T7-Build.h>
#endif 

/**
 * @brief include parameters related to configuration panel
 * and web server
 * 
 */
#include <AFE-Webforms.h>



/* ***************** HARDWARE: Config and defaults *********************/

/* Configs related to a relay functionality
@TODO refactoring needed: remove this type of config parameter */
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
#ifndef AFE_CONFIG_HARDWARE_RELAY
#define AFE_CONFIG_HARDWARE_RELAY
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_RELAY

/* Config releated to battery meter functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifndef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#define AFE_CONFIG_HARDWARE_ANALOG_INPUT
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

/* Configs related to analog input functionality
@TODO refactoring needed: remove this type of config parameter */
#ifdef AFE_CONFIG_FUNCTIONALITY_ADC
#ifndef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#define AFE_CONFIG_HARDWARE_ANALOG_INPUT
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_ADC

/* Configs related to a gate functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_GATE
#ifndef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#endif
#ifndef AFE_CONFIG_HARDWARE_RELAY
#define AFE_CONFIG_HARDWARE_RELAY
#endif
#ifndef AFE_CONFIG_HARDWARE_GATE
#define AFE_CONFIG_HARDWARE_GATE
#endif
#ifndef AFE_CONFIG_HARDWARE_CONTACTRON
#define AFE_CONFIG_HARDWARE_CONTACTRON
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_GATE

/* PIR Sesnor */
#ifdef AFE_CONFIG_HARDWARE_PIR
#ifndef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#endif
#endif // AFE_CONFIG_HARDWARE_PIR


/* ***************** HARDWARE: Config and defaults *********************/

/**
 * @brief include LED configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_LED
#include <hardwares/AFE-LED.h>
#endif

/**
 * @brief include RGB LED WS281x configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_CLED
#include <hardwares/AFE-RGB-LED.h>
#endif

/**
 * @brief include Switch configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <hardwares/AFE-Switch.h>
#endif 

/**
 * @brief include Relay configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <hardwares/AFE-Relay.h>
#endif

/**
 * @brief include Analog input and battery measurment configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#include <hardwares/AFE-ADC.h>
#endif 

/**
 * @brief include Gate configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_GATE
#include <hardwares/AFE-Gate.h>
#endif

/**
 * @brief include Contactron configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <hardwares/AFE-Contactron.h>
#endif


/**
 * @brief include Binary sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <sensors/AFE-Binary.h>
#endif 

/**
 * @brief include PN532 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <sensors/AFE-PN532.h>
#endif

/**
 * @brief include DS18B20 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <sensors/AFE-DS18B20.h>
#endif

/**
 * @brief include DHTxx sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_DHT
#include <sensors/AFE-DHT.h>
#endif

/**
 * @brief include HPMA115S0 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <sensors/AFE-HPMA115S0.h>
#endif

/**
 * @brief include Bosch BMx80 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <sensors/AFE-Bosch-BMx80.h>
#endif

/**
 * @brief include BH1759 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <sensors/AFE-BH1750.h>
#endif

/**
 * @brief include TSL2561 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <sensors/AFE-TSL2561.h>
#endif

/**
 * @brief include AS3935 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <sensors/AFE-AS3935.h>
#endif

/**
 * @brief include Anememeter sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <sensors/AFE-Anememeter.h>
#endif

/**
 * @brief include Rainmeter sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <sensors/AFE-Rainmeter.h>
#endif

/**
 * @brief include FS3000 sensor configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_FS3000
#include <sensors/AFE-FS3000.h>
#endif

/**
 * @brief common configuration parameters for sensors
 * 
 */
#include <sensors/AFE-Common.h>

/**
 * @brief include regulators configuration parameters
 * 
 */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <functionalities/AFE-Regulator.h>
#endif

/**
 * @brief include thermal protection configuration parameters
 * 
 */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <functionalities/AFE-Thermal-Protection.h>
#endif 

/**
 * @brief include UART bus configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_UART
#include <bus/AFE-UART.h>
#endif 

/**
 * @brief include I2C bus configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_I2C
#include <bus/AFE-IIC.h>
#endif

/**
 * @brief include MCP23017 configuration parameters
 * 
 */
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <hardwares/AFE-MCP23017.h>
#endif



/**
 * @brief include configuration parameters for APIs
 * 
 */
#include <api/AFE-HTTP.h>
#include <api/AFE-MQTT.h>
#if defined(AFE_CONFIG_API_DOMOTICZ_ENABLED)
#include <api/AFE-Domoticz.h>
#elif defined(AFE_CONFIG_API_HOME_ASSISTANT_ENABLED)
#include <api/AFE-Home-Assistant.h>
#else
#include <api/AFE-Standard.h>
#endif


/**
 * @brief include JSONRPC API used to exchange data with afe infrastructure
 * 
 */
#include <api/AFE-JSONRPC.h>

/**
 * @brief include Firmware check/update config
 * 
 */
#include <functionalities/AFE-Firmware-Update.h>

#endif // _AFE_Configuration_h
