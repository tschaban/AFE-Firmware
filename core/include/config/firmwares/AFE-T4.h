#ifndef _AFE_Configuration_T_h
#define _AFE_Configuration_T_h

#define AFE_FIRMWARE_VERSION "3.8.0.B0"
#define AFE_FIRMWARE_TYPE 4

/* Define Hardware */
#define AFE_CONFIG_HARDWARE_SWITCH
#define AFE_CONFIG_HARDWARE_LED
#define AFE_CONFIG_HARDWARE_MCP23XXX
#define AFE_CONFIG_HARDWARE_BINARY_SENSOR
#define AFE_CONFIG_HARDWARE_DS18B20
#define AFE_CONFIG_HARDWARE_I2C


/* Max number of hardware items, per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BINARY_SENSORS 32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_MCP23017 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20 1
#ifdef AFE_ESP32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_I2C 2
#endif


/* Max number of hardware items per specyfic hardware device */
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23XXX AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_MCP23017
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BINARY_SENSORS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20
#ifdef AFE_ESP32
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_I2C
#endif

/* Generic version */
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BINARY_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_MCP23XXX 0
#ifdef AFE_ESP32
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 0
#endif


/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_ADC

#ifdef AFE_CONFIG_FUNCTIONALITY_ADC
#ifdef AFE_ESP32
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS AFE_NUMBER_OF_ADC_GPIOS
#else // ESP8266
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS 1
#endif // ESP32/ESP8266

#define AFE_CONFIG_HARDWARE_NUMBER_OF_ADCS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ADCS 0

#endif // AFE_CONFIG_FUNCTIONALITY_ADC


#endif // _AFE_Configuration_T_h