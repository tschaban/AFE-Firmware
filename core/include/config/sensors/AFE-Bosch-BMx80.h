#ifndef _AFE_Hardware_Bosch_BMx80_h
#define _AFE_Hardware_Bosch_BMx80_h

#ifndef AFE_CONFIG_TEMPERATURE
#define AFE_CONFIG_TEMPERATURE
#endif
#ifndef AFE_CONFIG_HUMIDITY
#define AFE_CONFIG_HUMIDITY
#endif
#ifndef AFE_CONFIG_PRESSURE
#define AFE_CONFIG_PRESSURE
#endif
#ifndef AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#define AFE_CONFIG_DOMOTICZ_CUSTOME_SENSOR
#endif

#define AFE_CONFIG_HARDWARE_BMEX80_DEFAULT_INTERVAL 60
/**
 * @brief AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH
 * {"temperature":{"value":999.9999,"unit":"C","correction":99.9999},"pressure":{"value":9999.9999,"unit":"hPa","correction":9999.9999},"relativePressure":{"value":9999.999,"unit":"hPa"},"dewPoint":{"value":999.999999,"unit":"C"},"humidity":{"value":999.99999,"unit":"%H","correction":999.99999,"rating":3},"absoluteHumidity":{"value":6.480335,"unit":"%H"},"heatIndex":{"value":999.99999,"unit":"C"},"perception":{"value":0,"description":"W porządku dla większości, ale wszyscy odczuwają wilgoć przy górnej krawędzi"},"comfort":{"value":4,"ratio":999.99999,"unit":"%","description":"Gorąc i  wilgotno"},"iaq":{"value":9999,"rating":1,"accuracy":0},"staticIaq":{"value":9999,"rating":1,"accuracy":0},"co2Equivalent":{"value":99999,"unit":"ppm","rating":1,"accuracy":0},"breathVocEquivalent":{"value":99.999999,"unit":"?","accuracy":0},"gasResistance":{"value":99999.999,"unit":"kOm"}}
 * 
 */
#define AFE_CONFIG_API_JSON_BMEX80_DATA_LENGTH 1687 
#define AFE_BMX_UNKNOWN_SENSOR 255
#define AFE_BMP180_SENSOR 1
#define AFE_BME280_SENSOR 2
#define AFE_BMP280_SENSOR 3
#define AFE_BME680_SENSOR 6

#endif // _AFE_Hardware_Bosch_BMx80_h