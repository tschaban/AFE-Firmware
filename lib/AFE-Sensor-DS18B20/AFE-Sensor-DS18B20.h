/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_DS18B20_h
#define _AFE_Sensor_DS18B20_h

#include <AFE-Configuration.h>

#ifdef AFE_CONFIG_HARDWARE_DS18B20

#include <AFE-Data-Access.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#ifdef DEBUG
#include <AFE-Debugger.h>
#endif

class AFESensorDS18B20
{

private:
  AFEDataAccess *Data;
  #ifdef DEBUG
  AFEDebugger *Debugger;
  #endif
  OneWire *WireBUS = new OneWire();
  DallasTemperature *Sensor = new DallasTemperature();
  boolean _initialized = false;
  unsigned long startTime = 0;
  unsigned long readTimeOut = 0;
  float currentTemperature = DEVICE_DISCONNECTED_C;

#ifdef DEBUG
  void begin(AFEDataAccess *, uint8_t id);
#endif

public:
  DS18B20 *configuration = new DS18B20;

  AFESensorDS18B20();

#ifdef DEBUG
  void begin(AFEDataAccess *, AFEDebugger *, uint8_t id);
#else
  void begin(AFEDataAccess *, uint8_t id);
#endif

  /* Read and returns current temperature from the sensor */
  float getCurrentTemperature();

  /* Return temperature stored in the buffer, it's the latest */
  float getTemperature();

  /* Method has to be added to the loop in order to listen for sensor value
   * changes */
  boolean listener();

  uint8_t scan(uint8_t gpio, DS18B20Addresses &addresses);
  /* Converts DeviceAddress to Char */
  void addressToChar(DeviceAddress &address, char *addressString);
  /* Converts Char to DeviceAddress */
  void addressToInt(char *addressString, DeviceAddress &address);
  /* Returns {0,0,0,0,0,0,0,0} Device address */
  void addressNULL(DeviceAddress &address);

  /* Returns the sensor data in JSON format */
  void getJSON(char *json);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
  /* Return relay IDX in Domoticz */
  unsigned long getDomoticzIDX();
#endif
};

#endif // AFE_CONFIG_HARDWARE_DS18B20
#endif // _AFE_Sensor_DS18B20_h
