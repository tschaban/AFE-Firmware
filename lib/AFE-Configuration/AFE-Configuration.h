/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Configuration_h
#define _AFE_Configuration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Serial port speed */
#define AFE_CONFIG_SERIAL_SPEED 115200

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

/* APIs */
#define API_HTTP 0
#define API_MQTT 1
#define API_DOMOTICZ 2

#define AFE_MAX_MQTT_TOPIC_LENGTH 70

/* URLs to AFE WebService */
#define AFE_URL_VALIDATE_KEY "http://api.smartnydom.pl/key/validate/"
#define AFE_URL_ADD_KEY "http://api.smartnydom.pl/key/add/"
#define AFE_KEY_FREQUENCY_VALIDATION 1440

/* Types of URL requests */
#define AFE_SERVER_CMD_SAVE 1
#define AFE_SERVER_CMD_NONE 0

/* Switches, types */
#define AFE_SWITCH_TYPE_MONO 0 // Mono stable switch
#define AFE_SWITCH_TYPE_BI 1   // Bistable switch

/* Switches functionalities */
#define AFE_SWITCH_FUNCTIONALITY_NONE 0
#define AFE_SWITCH_FUNCTIONALITY_MULTI 1
#define AFE_SWITCH_FUNCTIONALITY_RELAY 2

/* Switche bouncing */
#define AFE_SWITCH_BOUNCING 50

/* Not existing hardware item. Used as a default value */
#define AFE_HARDWARE_ITEM_NOT_EXIST 255

/* Types of the devices */

#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_DEVICE_TYPE_NAME "Sonoff Basic V1"
#define AFE_DEVICE_TYPE_ID 1
#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_DEVICE_TYPE_NAME "Sonoff 4CH"
#define AFE_DEVICE_TYPE_ID 2
#elif defined(AFE_DEVICE_SONOFF_DUAL_R2)
#define AFE_DEVICE_TYPE_NAME "Sonoff Dual R2"
#define AFE_DEVICE_TYPE_ID 3
#elif defined(AFE_DEVICE_SONOFF_TH10)
#define AFE_DEVICE_TYPE_NAME "Sonoff TH10"
#define AFE_DEVICE_TYPE_ID 4
#elif defined(AFE_DEVICE_SONOFF_TH16)
#define AFE_DEVICE_TYPE_NAME "Sonoff TH16"
#define AFE_DEVICE_TYPE_ID 5
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 1G"
#define AFE_DEVICE_TYPE_ID 6
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 2G"
#define AFE_DEVICE_TYPE_ID 7
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_DEVICE_TYPE_NAME "Sonoff Touch 3G"
#define AFE_DEVICE_TYPE_ID 8
#elif defined(AFE_DEVICE_SHELLY_1)
#define AFE_DEVICE_TYPE_NAME "Shelly-1"
#define AFE_DEVICE_TYPE_ID 20
#elif defined(AFE_DEVICE_iECSv20)
#define AFE_DEVICE_TYPE_NAME "iECS Gate Controller v2.0"
#define AFE_DEVICE_TYPE_ID 21
#else
#define AFE_DEVICE_TYPE_NAME "ESP Generic"
#define AFE_DEVICE_TYPE_ID 0
#endif

/* Basic Switch */
#if defined(T0_CONFIG)
#define AFE_FIRMWARE_VERSION "2.0.2"
#define AFE_FIRMWARE_TYPE 0

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#ifndef AFE_DEVICE_SHELLY_1
#define AFE_CONFIG_FUNCTIONALITY_ADC
#endif
/* Enables API On/Off via APIs */
//#define AFE_CONFIG_FUNCTIONALITY_API_CONTROL

/* Max number of hardware items, per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 5
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 5

/* Max number of hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 2
#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SHELLY_1)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 0
#define AFE_CONFIG_HARDWARE_SWITCH_GPIO_DIGIT_INPUT // Sets switch DigitialPin
                                                    // to INPUT
/* Generic version */
#else
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 5
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 5
#endif

/* Basic Switch + DS18B20 */
#elif defined(T1_CONFIG)
#define AFE_FIRMWARE_VERSION "1.4.0"
#define AFE_FIRMWARE_TYPE 1
#define AFE_CONFIG_HARDWARE_DS18B20
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
#define AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
/* Basic Switch + DTHxx */
#elif defined(T2_CONFIG)
#define AFE_FIRMWARE_VERSION "1.4.0"
#define AFE_FIRMWARE_TYPE 2
#define AFE_CONFIG_HARDWARE_DHXX
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
#define AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
#define AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTION
/* 4xRelay + PIR */
#elif defined(T3_CONFIG)
#define AFE_FIRMWARE_TYPE 3
#define AFE_FIRMWARE_VERSION "1.3.0"
#define AFE_CONFIG_HARDWARE_PIR
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
/* 4xRelay */
#elif defined(T4_CONFIG)
#define AFE_FIRMWARE_VERSION "1.3.1"
#define AFE_FIRMWARE_TYPE 4
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Gate */
#elif defined(T5_CONFIG)
#define AFE_FIRMWARE_VERSION "2.0.0"
#define AFE_FIRMWARE_TYPE 5

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_RELAY_CONTROL_AUTOONOFF_TIME
#define AFE_CONFIG_FUNCTIONALITY_GATE
#define AFE_CONFIG_FUNCTIONALITY_ADC

/* Max number of hardware items per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 3
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_CONTACTRONS 4
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_GATES 2

/* Max number of hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_iECSv20)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_GATES 2
#else
/* Generic version */
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_GATES 2
#endif

/* Default values for hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_iECSv20)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CONTACTRONS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES 1
#else
/* Generic version */
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_CONTACTRONS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_GATES 0
#endif

/* Wheater Station */
#elif defined(T6_CONFIG)
#define AFE_FIRMWARE_VERSION "2.0.0.B3"
#define AFE_FIRMWARE_TYPE 6

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_ADC

#define AFE_CONFIG_HARDWARE_BMEX80
#define AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_HARDWARE_BH1750

/* Max number of hardware items, per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750 1

/* Max number of hardware items per specyfic hardware device */
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750 1

/* Default values for hardware items per specyfic hardware device */
/* Generic version */
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750 0

#endif

/* Configs related to a relay functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY
#ifndef AFE_CONFIG_HARDWARE_RELAY
#define AFE_CONFIG_HARDWARE_RELAY
#endif
#endif

/* Configs related to thermostat functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_THERMOSTAT
#ifndef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#define AFE_CONFIG_FUNCTIONALITY_REGULATOR
#endif
#endif

/* Configs related to humidistat functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_HUMIDISTAT
#ifndef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#define AFE_CONFIG_FUNCTIONALITY_REGULATOR
#endif
#endif

/* Configs related to analog input functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_ADC
#ifndef AFE_CONFIG_HARDWARE_ADC_VCC
#define AFE_CONFIG_HARDWARE_ADC_VCC
#endif
#endif

/* Configs related to automatic relay switch off functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
/* Below three enables Relay.autoTurnOff */
#ifndef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
#define AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
#endif
#endif

/* Configs related to a gate functionality */
#ifdef AFE_CONFIG_FUNCTIONALITY_GATE
#ifndef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
#define AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
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
#endif

/* Configs related to specyfic hardware */

/* Relay */
#ifdef AFE_CONFIG_HARDWARE_RELAY
#define AFE_RELAY_ON 1
#define AFE_RELAY_OFF 0
#endif

/* DS18B20 Sensor */
#ifdef AFE_CONFIG_HARDWARE_DS18B20
#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#endif

/* DHxx sesnors like DHT21,DHT22 */
#ifdef AFE_CONFIG_HARDWARE_DHXX
#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#ifndef AFE_CONFIG_HUMIDITY
#define AFE_CONFIG_HUMIDITY
#endif
#endif

/* BMEX80 Sensors */
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#ifndef AFE_CONFIG_HUMIDITY
#define AFE_CONFIG_HUMIDITY
#endif
#ifndef AFE_CONFIG_PRESSURE
#define AFE_CONFIG_PRESSURE
#endif
#ifndef AFE_CONFIG_HARDWARE_I2C
#define AFE_CONFIG_HARDWARE_I2C
#endif
#ifndef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#ifndef AFE_CONFIG_HARDWARE_I2C
#define AFE_CONFIG_HARDWARE_I2C
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#define HPMA115S0_TYPE_PM25 0
#define HPMA115S0_TYPE_PM10 1
#ifndef AFE_CONFIG_HARDWARE_UART
#define AFE_CONFIG_HARDWARE_UART
#endif
#ifndef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#ifndef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif

#define AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_GPIO 17
#define AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_INTERVAL 60
#define AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_NUMBER_OF_SAMPLES 1
#define AFE_CONFIG_HARDWARE_ADC_VCC_DEFAULT_MAX_VCC 1

#endif

#ifdef AFE_CONFIG_HARDWARE_PIR
#ifndef AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
#define AFE_CONFIG_RELAY_AUTOONOFF_LISTENER
#endif
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
/* Gate's states */
#define AFE_GATE_OPEN 0           // Open
#define AFE_GATE_CLOSED 1         // Closed
#define AFE_GATE_PARTIALLY_OPEN 2 // Being opened
#define AFE_GATE_UNKNOWN 9        // Unknown state

/* Gate MQTT messages */
#define AFE_MQTT_GATE_OPEN "open"
#define AFE_MQTT_GATE_CLOSED "closed"
#define AFE_MQTT_GATE_PARTIALLY_OPEN "partiallyOpen"
#define AFE_MQTT_GATE_UNKNOWN "unknown"
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
/* Contactron types */
#define AFE_CONTACTRON_NO 0 // Normally open
#define AFE_CONTACTRON_NC 1 // Normally closed
/* Contactron states */
#define AFE_CONTACTRON_OPEN 0   // Open
#define AFE_CONTACTRON_CLOSED 1 // Closed
/* Contactron defaults */
#define AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_BOUNCING 200
#define AFE_CONFIG_HARDWARE_CONTACTRON_DEFAULT_OUTPUT_TYPE AFE_CONTACTRON_NO
/* Contactron MQTT messages */
#define AFE_MQTT_CONTACTRON_OPEN "open"
#define AFE_MQTT_CONTACTRON_CLOSED "closed"
#endif


#ifdef AFE_CONFIG_TEMPERATURE
#define AFE_TEMPERATURE_UNIT_CELSIUS 1
#define AFE_TEMPERATURE_UNIT_FAHRENHEIT  2
#endif

#ifdef AFE_CONFIG_HUMIDITY
typedef enum
{
    AFE_HUMIDITY_EXCELLENT = 1,
    AFE_HUMIDITY_GOOD = 2,
    AFE_HUMIDITY_FAIR = 3,
    AFE_HUMIDITY_POOR = 4,
    AFE_HUMIDITY_INADEQUATE = 5
} afe_humidity_ratings_t;
#endif


/* UART Defaults */
#ifdef AFE_CONFIG_PRESSURE
#define AFE_CONFIG_DEFAULT_SEA_LEVEL_PRESSURE 1013.25
#endif

#ifdef AFE_CONFIG_HARDWARE_UART
#define AFE_CONFIG_HARDWARE_UART_DEFAULT_RXD 3
#define AFE_CONFIG_HARDWARE_UART_DEFAULT_TXD 1
#endif

/* HPMA115S0 Defaults */
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_INTERVAL 300
#define AFE_CONFIG_HARDWARE_HPMA115S_DEFAULT_TIME_TO_MEASURE 0
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#define AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL 60
#define AFE_BMX_UNKNOWN_SENSOR 255
#define AFE_BMP180_SENSOR 1
#define AFE_BME280_SENSOR 2
#define AFE_BME680_SENSOR 6

typedef enum
{
    AFE_IAQ_GOOD = 1,
    AFE_IAQ_AVARAGE = 2,
    AFE_IAQ_LITTLE_BAD = 3,
    AFE_IAQ_BAD = 4,
    AFE_IAQ_WORSE = 5,
    AFE_IAQ_VERY_BAD = 5,
    AFE_IAQ_UNKNOWN = 255
} afe_iaq_ratings_t;


typedef enum
{
    AFE_CO2_EXCELLENT = 1,
    AFE_CO2_GOOD = 2,
    AFE_CO2_FAIR = 3,
    AFE_CO2_POOR = 4,
    AFE_CO2_INADEQUATE = 5
} afe_co2_ratings_t;

#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#define AFE_CONFIG_HARDWARE_BH1750_DEFAULT_INTERVAL 60
#define AFE_CONFIG_HARDWARE_BH1750_DEFAULT_MODE 0
#endif

/* Config sites IDs */
#define AFE_CONFIG_SITE_INDEX 0
#define AFE_CONFIG_SITE_DEVICE 1
#define AFE_CONFIG_SITE_NETWORK 2
#define AFE_CONFIG_SITE_MQTT 3
#define AFE_CONFIG_SITE_MQTT_TOPICS 4
#define AFE_CONFIG_SITE_DOMOTICZ 5
#define AFE_CONFIG_SITE_LED 6
#define AFE_CONFIG_SITE_RELAY 7
#define AFE_CONFIG_SITE_SWITCH 8
#define AFE_CONFIG_SITE_PASSWORD 9
#define AFE_CONFIG_SITE_PRO_VERSION 10
#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#define AFE_CONFIG_SITE_ANALOG_INPUT 11
#endif
#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#define AFE_CONFIG_SITE_CONTACTRON 12
#endif
#ifdef AFE_CONFIG_HARDWARE_GATE
#define AFE_CONFIG_SITE_GATE 13
#endif
#define AFE_CONFIG_SITE_EXIT 14
#define AFE_CONFIG_SITE_RESET 15
#define AFE_CONFIG_SITE_POST_RESET 16
#define AFE_CONFIG_SITE_FIRST_TIME 17
#define AFE_CONFIG_SITE_FIRST_TIME_CONNECTING 18
#define AFE_CONFIG_SITE_UPGRADE 19
#define AFE_CONFIG_SITE_POST_UPGRADE 20
#ifdef AFE_CONFIG_HARDWARE_UART
#define AFE_CONFIG_SITE_UART 21
#endif
#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_SITE_HPMA115S0 22
#endif
#ifdef AFE_CONFIG_HARDWARE_BMEX80
#define AFE_CONFIG_SITE_BMEX80 23
#endif
#ifdef AFE_CONFIG_HARDWARE_BH1750
#define AFE_CONFIG_SITE_BH1750 24
#endif

#endif
