/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_Hardwware_h
#define _AFE_Hardwware_h

#include <AFE-Configuration.h>
#include <AFE-Firmware.h>

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-Wire-Container.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-MCP23017-Broker.h>
#endif

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

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Regulator.h>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Thermal-Protector.h>
#endif

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFEHardware {

private:
  AFEFirmware *Firmware;

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

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
  void initMCP23XXX(void);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void initRegulator(void);
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  void initThermalProtector(void);
#endif

public:
#ifdef AFE_CONFIG_HARDWARE_I2C
#ifdef DEBUG
  AFEWireContainer *WirePort = new AFEWireContainer(
      Firmware->Device, Firmware->API->Flash, Firmware->Debugger);
#else
  AFEWireContainer *WirePort =
      new AFEWireContainer(Firmware->Device, Firmware->API->Flash);
#endif
#endif

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
#ifdef DEBUG
  AFEImpulseCatcher *WindImpulse = new AFEImpulseCatcher(Firmware->Debugger);
#else
  AFEImpulseCatcher *WindImpulse = new AFEImpulseCatcher();
#endif
  AFEAnemometer *AnemometerSensor = new AFEAnemometer();
  boolean initAnemometer(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#ifdef DEBUG
  AFEImpulseCatcher *RainImpulse = new AFEImpulseCatcher(Firmware->Debugger);
#else
  AFEImpulseCatcher *RainImpulse = new AFEImpulseCatcher();
#endif
  AFERainmeter *RainmeterSensor = new AFERainmeter();
  boolean initRainmeter(void);
#endif

#ifdef AFE_CONFIG_HARDWARE_FS3000
  AFESensorFS3000 *FS3000Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_FS3000];
#endif /*                                                                      \ \
   \                                                                             \
   \ \                                                                               \
   AFEMCP23017Broker(AFEDataAccess *_Data, AFEDevice *_Device, TwoWire \ \ \                                                                               \
   *_WirePort0, \ \ \                                                                               \
                     TwoWire *_WirePort1, AFEDebugger *_Debugger); \ \ \                                                                               \
 */
#ifdef AFE_CONFIG_HARDWARE_MCP23XXX

#ifdef DEBUG
  AFEMCP23017Broker *MCP23017Broker = new AFEMCP23017Broker(
      Firmware->API->Flash, Firmware->Device, WirePort, Firmware->Debugger);
#else
  AFEMCP23017Broker *MCP23017Broker =
      new AFEMCP23017Broker(Firmware->API->Flash, Firmware->Device, WirePort);
#endif
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
  AFERegulator *Regulator[AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
  AFEThermalProtector
      *ThermalProtector[AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS];
#endif

  /* Constructor */
  AFEHardware(AFEFirmware *);

  void initPriorityHardwareItems(void);
  void initHardwareItems(void);

  void begin(void);
};

#endif
