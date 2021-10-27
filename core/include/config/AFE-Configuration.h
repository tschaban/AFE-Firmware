/**
 * @file AFE-Configuration.h
 * @author Tschaban-A (github@adrian.czabanowski.com)
 * @brief files contains firmware configuration parameters
 * @version 3.1.0
 * @date 2021-10-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

/* Serial port speed */
//#define AFE_CONFIG_SERIAL_SPEED 9600
#define AFE_CONFIG_SERIAL_SPEED 115200

/* Turn On/Off debbuging of Ping class */
#ifdef DEBUG
#define ENABLE_DEBUG_PING
#endif

/* Urls */
#define AFE_URL_DOCUMENTATION "https://afe.smartnydom.pl"
#define AFE_URL_HELP "https://www.smartnydom.pl/forum/afe-firmware/"

/* Upgrade types */
#define AFE_UPGRADE_NONE 0
#define AFE_UPGRADE_VERSION 1
#define AFE_UPGRADE_VERSION_TYPE 2

/* Device operating modes */
#define AFE_MODE_NORMAL 0
#define AFE_MODE_CONFIGURATION 1
#define AFE_MODE_ACCESS_POINT 2
#define AFE_MODE_NETWORK_NOT_SET 4
#define AFE_MODE_FIRST_TIME_LAUNCH 5

/* Host to check WAN Access */
#define AFE_WAN_ACCSSS_HOST "8.8.8.8"
#define AFE_WAN_ACCSSS_PINGS 1
#define AFE_WAN_ACCSSS_TIMEOUT 3000 // performs check if access to the network longer than this time in ms

/* APIs */
#define AFE_API_STANDARD 0
#define AFE_API_DOMOTICZ 1
#define AFE_API_HOME_ASSISTANT 2

/* Frequency of AFE Key validation in minutes */
#define AFE_KEY_FREQUENCY_VALIDATION 60 // 1440

/* Types of URL requests */
#define AFE_SERVER_CMD_SAVE 1
#define AFE_SERVER_CMD_NONE 0


/* Set 4MB Flash for all ESP32 firmwares */
#ifdef AFE_ESP32
#ifndef ESP_4MB
#define ESP_4MB
#endif
#endif

/* Not existing hardware item. Used as a default value */
#define AFE_HARDWARE_ITEM_NOT_EXIST 255
#define AFE_NONE AFE_HARDWARE_ITEM_NOT_EXIST

/* Empty string */
#define AFE_EMPTY_STRING '\0'

/* Default time to auto-logout from config panel: in minutes */
#define AFE_AUTOLOGOFF_DEFAULT_TIME 10

/* Number of GPIOs available in ESP8266, used in the conjunction with
 * AFE-Site-components.h table */
#ifndef AFE_ESP32 /* ESP82xx */
#define AFE_NUMBER_OF_GPIOS 13
#define AFE_NUMBER_OF_ADC_GPIOS 1
#else /* ESP32 */
#ifdef ESP32_30_PINS
#define AFE_NUMBER_OF_GPIOS 25
#else // ESP32_38_PINS
#define AFE_NUMBER_OF_GPIOS 34
#endif // ESP32_30_PINS
#define AFE_NUMBER_OF_ADC_GPIOS 4
#endif

/* The size of AFE Firmware file, used by OTA */ 
#define AFE_FIRMARE_FILE_NAME_LENGTH 100

/* ***************** DEVICES *********************/
#include <devices/AFE-Devices.h>

/* ***************** FILE SYSTEM *********************/
#include <AFE-Files.h>

/* ***************** AFE VERSIONS *********************/
#if defined(T0_CONFIG)
#include <firmwares/AFE-T0.h>
#elif defined(T1_CONFIG)
#include <firmwares/AFE-T1.h>
#elif defined(T2_CONFIG)
#include <firmwares/AFE-T2.h>
#elif defined(T3_CONFIG)
#include <firmwares/AFE-T3.h>
#elif defined(T4_CONFIG)
#include <firmwares/AFE-T4.h>
#elif defined(T5_CONFIG)
#include <firmwares/AFE-T5.h>
#elif defined(T6_CONFIG)
#include <firmwares/AFE-T6.h>
#elif defined(T7_CONFIG)
#include <firmwares/AFE-T7.h>
#endif 

/* ***************** WEB FORMS *********************/
#include <AFE-Webforms.h>


/* ***************** NETWORK *********************/

#define AFE_CONFIG_NETWORK_DEFAULT_CONNECTION_ATTEMPTS 30
#define AFE_CONFIG_NETWORK_DEFAULT_WAIT_TIME 1
#define AFE_CONFIG_NETWORK_DEFAULT_WAIT_SERIES 20
#define AFE_CONFIG_NETWORK_DEFAULT_SWITCH_NETWORK_AFTER 2
#define AFE_CONFIG_NETWORK_DEFAULT_NONE_SSID "<nOOn>"

/* ***************** HARDWARE: Config and defaults *********************/

/* Configs related to a relay functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
#ifndef AFE_CONFIG_HARDWARE_RELAY
#define AFE_CONFIG_HARDWARE_RELAY
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_RELAY

/* Config releated to battery meter functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#ifndef AFE_CONFIG_HARDWARE_ADC_VCC
#define AFE_CONFIG_HARDWARE_ADC_VCC
#endif
#endif // AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

/* Configs related to analog input functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_ADC
#ifndef AFE_CONFIG_HARDWARE_ADC_VCC
#define AFE_CONFIG_HARDWARE_ADC_VCC
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


/* ***************** HARDWARE: Config and defaults *********************/

/* UART Defaults */
#ifdef AFE_CONFIG_HARDWARE_UART
#include <bus/AFE-UART.h>
#endif 

/* I2C Defaults */
#ifdef AFE_CONFIG_HARDWARE_I2C
#include <bus/AFE-IIC.h>
#endif

/* MCP23017 GPIO extension board */
#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <hardwares/AFE-MCP23017.h>
#endif

/* LED */
#ifdef AFE_CONFIG_HARDWARE_LED
#include <hardwares/AFE-LED.h>
#endif

/* RGB LED: WS281x */
#ifdef AFE_CONFIG_HARDWARE_CLED
#include <hardwares/AFE-RGB-LED.h>
#endif

/* SWITCH */
#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <hardwares/AFE-Switch.h>
#endif 

/* RELAY */
#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <hardwares/AFE-Relay.h>
#endif

/* ADC Input and Battery measure functionality */
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <hardwares/AFE-ADC.h>
#endif 

/* PIR Sesnor */
#ifdef AFE_CONFIG_HARDWARE_PIR
#ifndef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#endif
#endif // AFE_CONFIG_HARDWARE_PIR

/* Gate */
#ifdef AFE_CONFIG_HARDWARE_GATE
#include <hardwares/AFE-Gate.h>
#endif

/* Contactron */
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <hardwares/AFE-Contactron.h>
#endif


/* BINARY SENSOR (PIR, etc) */
#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <sensors/AFE-Binary.h>
#endif 

/* PN532 Sensor */
#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <sensors/AFE-PN532.h>
#endif

/* DS18B20 Sensor */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <sensors/AFE-DS18B20.h>
#endif

/* DHxx sesnors like DHT21,DHT22 */
#ifdef AFE_CONFIG_HARDWARE_DHT
#include <sensors/AFE-DHT.h>
#endif

/* HPMA115S0 Sensor */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <sensors/AFE-HPMA115S0.h>
#endif

/* BMx80 Defaults */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <sensors/AFE-Bosch-BMx80.h>
#endif

/* BH1750 Defualts */
#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <sensors/AFE-Bosch-BMx80.h>
#endif

/* TSL2561 Defualts */
#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <sensors/AFE-TSL2561-Sensor.h>
#endif

/* AS3935 Defaults */
#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <sensors/AFE-AS3935.h>
#endif

/* Anemometer Sensor Defaults */
#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <sensors/AFE-Anememeter.h>
#endif

/* Rainmeter defaults */
#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <sensors/AFE-Rainmeter.h>
#endif

/* Regulator defaults */
#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <functionalities/AFE-Regulator.h>
#endif

/* Relay thermal protection */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <functionalities/AFE-Thermal-Protection.h>
#endif 

/* Common configuration for sensors */
#include <AFE-Sensors-Common.h>


/* ***************** APIs: Config and defaults *********************/


/* Configs releated to Domoticz APIs */
#if defined(AFE_CONFIG_API_DOMOTICZ_ENABLED)
#include <api/AFE-Domoticz.h>
#elif defined(AFE_CONFIG_API_HOME_ASSISTANT_ENABLED)
#include <api/AFE-Home-Assistant.h>
#else
#include <api/<AFE-Standard.h>
#endif


/* Defaults for each MQTT Version (standard and domoticz) */
#include <api/AFE-MQTT.h>

/* JSONRPC API */
#include <api/AFE-JSONRPC.h>

#endif // _AFE_Configuration_h
