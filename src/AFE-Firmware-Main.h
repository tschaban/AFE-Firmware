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

#include <AFE-Firmware.h>
#include <AFE-Hardware.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>

AFEFirmware *Firmware = new AFEFirmware();
AFEWebServer *HTTPServer = new AFEWebServer();
AFEHardware *Hardware = new AFEHardware(Firmware);

#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-MCP23017-Broker.h>
AFEMCP23017Broker *MCP23017Broker = new AFEMCP23017Broker();
#endif

#ifdef AFE_CONFIG_HARDWARE_CLED
#include <AFE-CLED.h>
AFECLED *CLEDStrip = new AFECLED();
#endif // AFE_CONFIG_HARDWARE_CLED




#ifdef AFE_CONFIG_FUNCTIONALITY_REGULATOR
#include <AFE-Regulator.h>
AFERegulator Regulator[AFE_CONFIG_HARDWARE_NUMBER_OF_REGULATORS];
#endif

#ifdef AFE_CONFIG_FUNCTIONALITY_THERMAL_PROTECTOR
#include <AFE-Thermal-Protector.h>
AFEThermalProtector
    ThermalProtector[AFE_CONFIG_HARDWARE_NUMBER_OF_THERMAL_PROTECTORS];
#endif



#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Sensor-PN532.h>
AFESensorPN532 PN532Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_PN532_SENSORS];
#include <AFE-MiFare-Card.h>
AFEMiFareCard MiFareCard[AFE_CONFIG_HARDWARE_NUMBER_OF_MIFARE_CARDS];
#endif

#include <AFE-Main-APIs.cpp>


#ifdef AFE_CONFIG_HARDWARE_MCP23XXX
#include <AFE-Main-MCP23017.cpp>
#endif


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

#ifdef AFE_CONFIG_HARDWARE_FS3000
#include <AFE-Main-FS3000.cpp>
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
