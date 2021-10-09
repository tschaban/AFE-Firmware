/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

/* Class
- establishes connection to MQTT Broker
- is a proxy between AFE-APIs and MQTT Broker
- it also stories referencies to global classes that can be used for AFE-APIs */

#ifndef _AFE_API_h
#define _AFE_API_h

#include <AFE-Configuration.h>
#include <AFE-Device.h>
#include <AFE-MQTT-Structure.h>
#include <AFE-MQTT.h>

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-HPMA115S0.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Sensor-BH1750.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-Sensor-AS3935.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-Sensor-Anemometer.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Sensor-Rainmeter.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Contactron.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Regulator.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Thermal-Protector.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Sensor-Binary.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-MiFare-Card.h>
#include <AFE-Sensor-PN532.h>
#endif // AFE_CONFIG_HARDWARE_PN532_SENSOR

/* Not yet implemented 
#if defined(AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT) ||                   \
    defined(AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT)
#include <AFE-CLED.h>
#endif
*/

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <AFE-Sensor-TSL2561.h>
#endif // AFE_CONFIG_HARDWARE_TSL2561

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEAPI {
private:
public:
  AFEMQTT Mqtt;
  AFEDevice *_Device;
  AFEDataAccess *_Data;
#ifdef AFE_CONFIG_HARDWARE_LED
  AFELED *_Led;
#endif

  AFEAPI();

#ifdef AFE_CONFIG_HARDWARE_LED
  void begin(AFEDataAccess *, AFEDevice *, AFELED *);
#else
  void begin(AFEDataAccess *, AFEDevice *);
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
  virtual void addClass(AFERelay *);
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  virtual void addClass(AFESwitch *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  virtual void addClass(AFEAnalogInput *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  virtual void addClass(AFESensorBMEX80 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  virtual void addClass(AFESensorHPMA115S0 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  virtual void addClass(AFESensorBH1750 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  virtual void addClass(AFESensorAS3935 *);
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  virtual void addClass(AFEAnemometer *);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  virtual void addClass(AFERainmeter *);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  virtual void addClass(AFEGate *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  virtual void addClass(AFEContactron *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  virtual void addClass(AFESensorDS18B20 *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  virtual void addClass(AFERegulator *);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  virtual void addClass(AFEThermalProtector *);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  virtual void addClass(AFESensorDHT *);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  virtual void addClass(AFESensorBinary *);
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  virtual void addClass(AFESensorPN532 *);
#endif
  virtual void addClass(AFEMiFareCard *);
#endif
/* Not yet implemented 
#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
  virtual void addClassEffectDeviceLight(AFECLED *);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  virtual void addClassEffecPN532Sensor(AFECLED *);
#endif
*/

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  virtual void addClass(AFESensorTSL2561 *);
#endif

protected:
  /* Is API enabled, set in begin() */
  boolean enabled = false;

#ifdef AFE_CONFIG_HARDWARE_RELAY
  /* Stories reference to global Relay class */
  AFERelay *_Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  /* Stories reference to global Switch class */
  AFESwitch *_Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
  /* Stories reference to global ADC class */
  #ifdef AFE_ESP32
  AFEAnalogInput *_AnalogInput[AFE_CONFIG_HARDWARE_NUMBER_OF_ADCS];
  #else
  AFEAnalogInput *_AnalogInput;
  #endif
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  AFESensorBMEX80 *_BMx80Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  AFESensorHPMA115S0 *_HPMA115S0Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0];
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  AFESensorBH1750 *_BH1750Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750];
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
  AFESensorAS3935 *_AS3935Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935];
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  AFEAnemometer *_AnemometerSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  AFERainmeter *_RainmeterSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  AFEGate *_Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  AFEContactron *_Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  AFESensorDS18B20 *_DS18B20Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_DS18B20];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  AFERegulator *_Regulator[AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  AFEThermalProtector
      *_ThermalProtector[AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  AFESensorDHT *_DHTSensor[AFE_CONFIG_HARDWARE_NUMBER_OF_DHT];
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  AFESensorBinary *_BinarySensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#ifndef AFE_CONFIG_API_DOMOTICZ_ENABLED
  AFESensorPN532 *_PN532Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_PN532_SENSORS];
#endif
  AFEMiFareCard *_MiFareCard[AFE_CONFIG_HARDWARE_NUMBER_OF_MIFARE_CARDS];
#endif
/* Not yet implemented 
#ifdef AFE_CONFIG_HARDWARE_CLED_BACKLIGHT_EFFECT
  AFECLED *_CLEDBacklight;
#endif
#ifdef AFE_CONFIG_HARDWARE_CLED_ACCESS_CONTROL_EFFECT
  AFECLED *_CLEDAccessControl;
#endif
*/

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  AFESensorTSL2561 *_TSL2561Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561];
#endif

};

#endif // _AFE_API_h