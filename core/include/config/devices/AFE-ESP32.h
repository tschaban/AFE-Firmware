#ifndef _AFE_devices_ESP32_h
#define _AFE_devices_ESP32_h

#ifdef AFE_ESP32_30_PINS
#define AFE_NUMBER_OF_GPIOS 26
#else // AFE_ESP32_38_PINS
#define AFE_NUMBER_OF_GPIOS 34
#endif // AFE_ESP32_30_PINS
#define AFE_NUMBER_OF_ADC_GPIOS 4

#ifndef AFE_ESP_FLASH_4MB
#define AFE_ESP_FLASH_4MB
#endif

#define AFE_MAX_RAM 327680


#endif // _AFE_devices_ESP32_h