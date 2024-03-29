/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Hardwware_h
#define _AFE_Hardwware_h

#include <AFE-Configuration.h>
#include <AFE-Firmware.h>

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-CLED.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#include <AFE-Analog-Input.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Contactron.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <AFE-Sensor-TSL2561.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Sensor-BH1750.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Sensor-Binary.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-HPMA115S0.h>
#endif

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) ||                                 \
    defined(AFE_CONFIG_HARDWARE_RAINMETER)
#include <AFE-Impulse-Catcher.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-Sensor-Anemometer.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Sensor-Rainmeter.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
#include <AFE-Sensor-FS3000.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEHardware {

private:
  AFEFirmware *_Firmware;

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  void initSwitch(void);
#endif
#ifdef AFE_CONFIG_HARDWARE_RELAY
  void initRelay(void);
#endif
#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
  void initADC(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  void initRGBLED(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
  void initGate(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  void initContactron(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  void initTSL2562(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  void initBH1750(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  void initBMX80(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  void initBinarySensor(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  void initDS18B20(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  void initDHT(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  void initHPMA115S0(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  void initFS3000(void);
#endif

public:
#ifdef AFE_CONFIG_HARDWARE_RELAY
  AFERelay *Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
  AFESwitch *Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#ifdef AFE_ESP32
  AFEAnalogInput *AnalogInput[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS];
#else
  AFEAnalogInput *AnalogInput;
#endif // AFE_ESP32
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
  AFECLED *RGBLEDStrip = new AFECLED();
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_GATE
  AFEGate *Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
  GATES_CURRENT_STATE *GatesCurrentStates = new GATES_CURRENT_STATE;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
  AFEContactron *Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
  byte lastPublishedContactronState[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
  AFESensorTSL2561 *TSL2561Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561];
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
  AFESensorBH1750 *BH1750Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750];
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
  AFESensorBMEX80 *BMEX80Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80];
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
  AFESensorBinary *BinarySensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
  AFESensorDS18B20 *DS18B20Sensor[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20];
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
  AFESensorDHT *DHTSensor[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
  AFESensorHPMA115S0 *HPMA115S0Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0];
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
  AFEImpulseCatcher *WindImpulse = new AFEImpulseCatcher();
  AFEAnemometer *AnemometerSensor = new AFEAnemometer();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
  AFEImpulseCatcher *RainImpulse = new AFEImpulseCatcher();
  AFERainmeter *RainmeterSensor = new AFERainmeter();
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  AFESensorFS3000 *FS3000Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_FS3000];
#endif

  /* Constructor */
  AFEHardware(AFEFirmware *Firmware);

  void initPriorityHardwareItems(void);
  void initHardwareItems(void);

  void begin(void);
};

#endif
