#ifndef _AFE_Hardware_DHT_h
#define _AFE_Hardware_DHT_h

#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#ifndef AFE_CONFIG_HUMIDITY
#define AFE_CONFIG_HUMIDITY
#endif

#define AFE_CONFIG_HARDWARE_DHT_TYPE_AUTO 0
#define AFE_CONFIG_HARDWARE_DHT_TYPE_DHT11 1
#define AFE_CONFIG_HARDWARE_DHT_TYPE_DHT22 2
#define AFE_CONFIG_HARDWARE_DHT_TYPE_AM2302 3
#define AFE_CONFIG_HARDWARE_DHT_TYPE_RHT03 4

#define AFE_CONFIG_HARDWARE_DHT_DEFAULT_GPIO 14
#define AFE_CONFIG_HARDWARE_DHT_DEFAULT_INTERVAL 60
#define AFE_CONFIG_HARDWARE_DHT_DEFAULT_SENDING_ONLY_CHANGES false
#define AFE_CONFIG_HARDWARE_DHT_DEFAULT_TEMPERATURE_CORRECTION 0
#define AFE_CONFIG_HARDWARE_DHT_DEFAULT_HUMIDITY_CORRECTION 0
#define AFE_CONFIG_API_JSON_DHT_DATA_LENGTH  420 
/* 
 {"temperature":{"value":218.4,"unit":"C"},"humidity":{"value":317.0,"unit":"%"},"absoluteHumidity":{"value":110.29,"unit":"%"},"heatIndex":{"value":217.81,"unit":"C"},"dewPoint":{"value":112.33,"unit":"C"},"perception":{"value":1,"description":"W porządku dla większości, ale wszyscy odczuwają wilgoć przy górnej krawędzi"},"comfort":{"value":0,"ratio":100.00,"unit":"%","description":"Gorąco i wilgotno"}}
*/
#define AFE_CONFIG_API_JSON_DHT_COMMAND_LENGTH 200

#endif // _AFE_Hardware_DHT_h