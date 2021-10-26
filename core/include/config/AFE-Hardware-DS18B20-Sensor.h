#ifndef _AFE_Hardware_DS18B20_h
#define _AFE_Hardware_DS18B20_h

#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif  

#define AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_GPIO 14
#define AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_INTERVAL 60
#define AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_SENDING_ONLY_CHANGES false
#define AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_TEMPERATURE_CORRECTION 0
#define AFE_CONFIG_HARDWARE_DS18B20_DEFAULT_RESOLUTION 12
#define AFE_CONFIG_HARDWARE_DS18B20_ADDRESS_LENGTH 8
#define AFE_CONFIG_HARDWARE_DS18B20_READ_TIMEOUT 10000

#define AFE_CONFIG_API_JSON_DS18B20_DATA_LENGTH  46 // {"temperature":{"value":-999.999,"unit":"C"}}

#endif // _AFE_Hardware_DS18B20_h