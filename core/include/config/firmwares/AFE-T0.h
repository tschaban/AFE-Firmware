#ifndef _AFE_Configuration_T_h
#define _AFE_Configuration_T_h

#define AFE_FIRMWARE_VERSION "3.3.0"
#define AFE_FIRMWARE_TYPE 0

/* Define Hardware */
#define AFE_CONFIG_HARDWARE_SWITCH

#ifndef AFE_DEVICE_SHELLY_1
#define AFE_CONFIG_HARDWARE_LED
#endif

/* Max number of hardware items, per AFE version */
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_SWITCHES 5
#ifdef AFE_CONFIG_HARDWARE_LED
#define AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_LEDS 5
#endif


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
#define AFE_CONFIG_HARDWARE_SWITCH_GPIO_DIGIT_INPUT // Sets switch Digitial PIN
// to INPUT
/* Generic version */
#else
#define AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES 5
#define AFE_CONFIG_HARDWARE_NUMBER_OF_LEDS 5
#endif

/* Default values for hardware items per specyfic hardware device */
#if defined(AFE_DEVICE_SONOFF_BASIC_V1)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_4CH)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 4
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 4
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_1G)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_2G)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 2
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 2
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SONOFF_TOUCH_3G)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 3
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 3
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#elif defined(AFE_DEVICE_SHELLY_1)
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 0
#define AFE_CONFIG_HARDWARE_SWITCH_GPIO_DIGIT_INPUT // Sets switch DigitialPin
// to INPUT
/* Generic version */
#else
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_RELAYS 0
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_SWITCHES 1
#define AFE_CONFIG_HARDWARE_DEFAULT_NUMBER_OF_LEDS 1
#endif

/* Functionalities */
#define AFE_CONFIG_FUNCTIONALITY_RELAY
#define AFE_CONFIG_FUNCTIONALITY_RELAY_AUTOONOFF

/* ADC Is not applicable for Shelly-1 */
#ifndef AFE_DEVICE_SHELLY_1
#define AFE_CONFIG_FUNCTIONALITY_ADC
#endif // AFE_DEVICE_SHELLY_1

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