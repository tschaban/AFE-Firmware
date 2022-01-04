#ifndef _AFE_devices_ESP32_h
#define _AFE_devices_ESP32_h

#ifdef ESP32_30_PINS
#define AFE_NUMBER_OF_GPIOS 25
#else // ESP32_38_PINS
#define AFE_NUMBER_OF_GPIOS 34
#endif // ESP32_30_PINS
#define AFE_NUMBER_OF_ADC_GPIOS 4

#ifndef ESP_4MB
#define ESP_4MB
#endif


#endif // _AFE_devices_ESP32_h