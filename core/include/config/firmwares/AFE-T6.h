#ifndef _AFE_Configuration_T_h
#define _AFE_Configuration_T_h

#define AFE_FIRMWARE_VERSION "3.3.4.RC1"
#define AFE_FIRMWARE_TYPE 6

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_ADC
#define AFE_CONFIG_FUNCTIONALITY_BATTERYMETER

#define AFE_CONFIG_HARDWARE_I2C
#define AFE_CONFIG_HARDWARE_SWITCH
#define AFE_CONFIG_HARDWARE_LED
#define AFE_CONFIG_HARDWARE_BMEX80
//#define AFE_CONFIG_HARDWARE_HPMA115S0
//#define AFE_CONFIG_HARDWARE_BH1750
//#define AFE_CONFIG_HARDWARE_AS3935
//#define AFE_CONFIG_HARDWARE_ANEMOMETER
//#define AFE_CONFIG_HARDWARE_RAINMETER

#if !(defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21))
#define AFE_CONFIG_HARDWARE_DS18B20
#define AFE_CONFIG_HARDWARE_DHT
#endif

/* Max number of hardware items, per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_I2C 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_AS3935 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ANEMOMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RAINMETER_SENSORS 1
#if !(defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21))
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT 1
#endif

/* Max number of hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C 1
#else
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80
#define AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750
#define AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_AS3935
#define AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ANEMOMETER_SENSORS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RAINMETER_SENSORS
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20 AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DHT AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_I2C
#endif // AFE_DEVICE_iECS_WHEATER_STATION_20

/* Default values for hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
/* Whater Station V2 */
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 0
#else
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 0
#endif // AFE_DEVICE_iECS_WHEATER_STATION_20

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