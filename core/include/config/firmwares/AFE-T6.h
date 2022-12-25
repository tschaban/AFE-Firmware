#ifndef _AFE_Configuration_T_h
#define _AFE_Configuration_T_h

#define AFE_FIRMWARE_VERSION "3.5.3"
#define AFE_FIRMWARE_TYPE 6

/**
 * @brief Functionalities & Hardware : E1 custome version
 * 
 */
 
#if defined(AFE_T6_CUSTOM_E1)
#define AFE_CONFIG_HARDWARE_DS18B20

/**
 * @brief Functionalities & Hardware : ECS Wheater station
 * 
 */
#elif defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_ADC
#define AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#define AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_HARDWARE_BH1750
#define AFE_CONFIG_HARDWARE_TSL2561
#define AFE_CONFIG_HARDWARE_ANEMOMETER
#define AFE_CONFIG_HARDWARE_RAINMETER

/**
 * @brief Functionalities & Hardware : Generic version
 * 
 */
#else

#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF
#define AFE_CONFIG_FUNCTIONALITY_ADC
#define AFE_CONFIG_FUNCTIONALITY_BATTERYMETER
#define AFE_CONFIG_HARDWARE_HPMA115S0
#define AFE_CONFIG_HARDWARE_BH1750
#define AFE_CONFIG_HARDWARE_TSL2561
#define AFE_CONFIG_HARDWARE_ANEMOMETER
#define AFE_CONFIG_HARDWARE_RAINMETER
#define AFE_CONFIG_HARDWARE_DS18B20
#define AFE_CONFIG_HARDWARE_DHT

#endif


/**
 * @brief Functionalities & Hardware : all versions
 * 
 */
#define AFE_CONFIG_HARDWARE_I2C
#define AFE_CONFIG_HARDWARE_SWITCH
#define AFE_CONFIG_HARDWARE_LED
#define AFE_CONFIG_HARDWARE_BMEX80

//#define AFE_CONFIG_HARDWARE_AS3935 decommissioned


/**
 * @brief Max number of hardware items for all version
 * 
 */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_I2C 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 2
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_BH1750 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_TSL2561 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_AS3935 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ANEMOMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RAINMETER_SENSORS 1
#if !(defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21))
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20 12
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT 1
#endif

/**
 * @brief Max number of hardware items per specyfic hardware device
 * ECS Wheater station
 * 
 */
#if defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561 1
//#define AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C 1

/**
 * @brief Max number of hardware items per specyfic hardware device
 * E1: Custome verstion
 * 
 */
#elif defined(AFE_T6_CUSTOM_E1)
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20 12
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C 1

/**
 * @brief Max number of hardware items per specyfic hardware device
 * Generic version
 * 
 */
#else
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561 1
//#define AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_NUMBER_OF_ANEMOMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RAINMETER_SENSORS 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20 2
#define AFE_CONFIG_HARDWARE_NUMBER_OF_DHT 1
#define AFE_CONFIG_HARDWARE_NUMBER_OF_I2C 2
#endif // AFE_DEVICE_iECS_WHEATER_STATION_20



/**
 * @brief Default values for hardware items per specyfic hardware device
 * ECS Wheater station
 * 
 */
#if defined(AFE_DEVICE_iECS_WHEATER_STATION_20) || defined(AFE_DEVICE_iECS_WHEATER_STATION_21)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_TSL2561 0
//#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 0
/**
 * @brief Default values for hardware items per specyfic hardware device
 * E1 Custome version
 * 
 */
#elif defined(AFE_T6_CUSTOM_E1)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20 12
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 1
/**
 * @brief Default values for hardware items per specyfic hardware device
 * Generic version
 * 
 */
#else
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BMEX80 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_HPMA115S0 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_BH1750 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_TSL2561 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_AS3935 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_ANEMOMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RAINMETER_SENSORS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DS18B20 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_DHT 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_I2C 0
#endif 

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