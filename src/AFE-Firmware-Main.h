/************************************************************************
 * AFE Firmware for devices based on ESP8266/ESP8285/ESP32 chips
 * This code combains AFE Firmware versions:
 *  - T0 (Basic version: relay)
 *  - T1 (DS18B20)
 *  - T2 (DHTxx)
 *  - T3 (PIRs)
 *  - T4 (With expander MCP23017)
 *  - T5 Gate
 *  - T6 Wheater station
 *  - T7 RGB LED Controller
 *
 * More info: https://afe.smartnydom.pl
 * LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
**************************************************************************/

/**
 * Work LOG T4
 * - lokalizacje
 * - to set fr ESP32 size of buffer for MCP INFO: JSON: Buffer size: 200, actual
 * JSON size: 40
 * - ESP32
 */

#include <AFE-Configuration.h>


/* Includes libraries for debugging in development compilation only */
#ifdef DEBUG
#include <Streaming.h>
#endif

//#include <AFE-API-JSONRPC.h>
//#include <AFE-Data-Access.h>
//#include <AFE-Device.h>
#include <AFE-Firmware.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
//#include <AFE-WiFi.h>

//AFEDataAccess *Data = new AFEDataAccess();
AFEFirmware *FirmwarePro = new AFEFirmware();
//AFEDevice *Device = new AFEDevice();
//AFEWiFi *Network = new AFEWiFi();
AFEWebServer *HTTPServer = new AFEWebServer();
//AFEJSONRPC *RestAPI = new AFEJSONRPC();

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-MCP23017-Broker.h>
AFEMCP23017Broker *MCP23017Broker = new AFEMCP23017Broker();
#endif

//#ifdef AFE_CONFIG_HARDWARE_LED
//#include <AFE-LED.h>
//AFELED *Led = new AFELED();
//#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-CLED.h>
AFECLED *CLEDStrip = new AFECLED();
#endif // AFE_CONFIG_HARDWARE_CLED

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
AFERelay Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
AFESwitch Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#include <AFE-Analog-Input.h>
#ifdef AFE_ESP32
AFEAnalogInput AnalogInput[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_ADCS];
#else
AFEAnalogInput AnalogInput;
#endif // AFE_ESP32
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
AFEGate Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
GATES_CURRENT_STATE *GatesCurrentStates = new GATES_CURRENT_STATE;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Contactron.h>
AFEContactron Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
//byte lastPublishedContactronState[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Regulator.h>
AFERegulator Regulator[AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Thermal-Protector.h>
AFEThermalProtector
    ThermalProtector[AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Sensor-DS18B20.h>
AFESensorDS18B20 DS18B20Sensor[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DS18B20];
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Sensor-DHT.h>
AFESensorDHT DHTSensor[AFE_CONFIG_HARDWARE_MAX_NUMBER_OF_DHT];
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Sensor-BH1750.h>
AFESensorBH1750 BH1750Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BH1750];
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <AFE-Sensor-TSL2561.h>
AFESensorTSL2561 TSL2561Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_TSL2561];
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Sensor-BMEX80.h>
AFESensorBMEX80 BMEX80Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BMEX80];
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Sensor-Binary.h>
AFESensorBinary BinarySensor[AFE_CONFIG_HARDWARE_NUMBER_OF_BINARY_SENSORS];
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Sensor-HPMA115S0.h>
AFESensorHPMA115S0 HPMA115S0Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_HPMA115S0];
#endif

#if defined(AFE_CONFIG_HARDWARE_ANEMOMETER) ||                                 \
    defined(AFE_CONFIG_HARDWARE_RAINMETER)
#include <AFE-Impulse-Catcher.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-Sensor-Anemometer.h>
AFEImpulseCatcher *WindImpulse = new AFEImpulseCatcher();
AFEAnemometer *AnemometerSensor = new AFEAnemometer();
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Sensor-Rainmeter.h>
AFEImpulseCatcher *RainImpulse = new AFEImpulseCatcher();
AFERainmeter *RainSensor = new AFERainmeter();
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Sensor-PN532.h>
AFESensorPN532 PN532Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_PN532_SENSORS];
#include <AFE-MiFare-Card.h>
AFEMiFareCard MiFareCard[AFE_CONFIG_HARDWARE_NUMBER_OF_MIFARE_CARDS];
#endif

#include <AFE-Main-APIs.cpp>

#ifdef AFE_CONFIG_HARDWARE_I2C
#include <AFE-Main-I2C.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-Main-MCP23017.cpp>
#endif

//#ifdef AFE_CONFIG_HARDWARE_LED
//#include <AFE-Main-LED.cpp>
//#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-Main-CLED.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Main-Relay.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Main-Switch.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANALOG_INPUT
#include <AFE-Main-ADC.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Main-Contactron.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Main-Gate.cpp>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Main-Regulator.cpp>
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Main-Thermal-Protector.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_DS18B20
#include <AFE-Main-DS18B20.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_BMEX80
#include <AFE-Main-BMX80.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_BH1750
#include <AFE-Main-BH1750.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_TSL2561
#include <AFE-Main-TSL2561.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_DHT
#include <AFE-Main-DHT.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Main-Binary-Sensor.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_HPMA115S0
#include <AFE-Main-HPMA115S0.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Main-Rainmeter.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_ANEMOMETER
#include <AFE-Main-Anemometer.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Main-PN532.cpp>
#endif

#if defined(DEBUG) && defined(AFE_CONFIG_HARDWARE_I2C)
#include <AFE-I2C-Scanner.h>
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-Sensor-AS3935.h>
AFESensorAS3935 AS3935Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935];
#endif

#include <AFE-Main-HTTPServer.cpp>

#ifdef DEBUG
#include <AFE-Debug.cpp>
#endif

#include <AFE-Event.h>
AFEEvent *Event = new AFEEvent();
