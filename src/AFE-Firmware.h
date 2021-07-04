/*
AFE Firmware for smarthome devices based on ESP8266/ESP8285 chips

This code combains AFE Firmware versions:
   - T0 and T0 for Shelly-1
   - T1 (DS18B20)
   - T2 (DHTxx)
   - T3 (PIRs)
   - T4 - decommissioned, T0 took over 100% of it's functionality
   - T5 Gate
   - T6 Wheater station

More info: https://afe.smartnydom.pl
LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
*/

#include <AFE-Configuration.h>

/* Includes libraries for debugging in development compilation only */
#ifdef DEBUG
#include <Streaming.h>
#endif

#include <AFE-API-JSONRPC.h>
#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-Firmware-Pro.h>
#include <AFE-Upgrader.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>

AFEDataAccess Data;
AFEFirmwarePro FirmwarePro;
AFEDevice Device;
AFEWiFi Network;
AFEWebServer WebServer;
AFEJSONRPC RestAPI;

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-MCP23017-Broker.h>
AFEMCP23017Broker MCP23017Broker;
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-LED.h>
AFELED Led;
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Relay.h>
AFERelay Relay[AFE_CONFIG_HARDWARE_NUMBER_OF_RELAYS];
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Switch.h>
AFESwitch Switch[AFE_CONFIG_HARDWARE_NUMBER_OF_SWITCHES];
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
#include <AFE-Analog-Input.h>
AFEAnalogInput AnalogInput;
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
#include <AFE-Gate.h>
AFEGate Gate[AFE_CONFIG_HARDWARE_NUMBER_OF_GATES];
GATES_CURRENT_STATE GatesCurrentStates;
#endif

#ifdef AFE_CONFIG_HARDWARE_CONTACTRON
#include <AFE-Contactron.h>
AFEContactron Contactron[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
byte lastPublishedContactronState[AFE_CONFIG_HARDWARE_NUMBER_OF_CONTACTRONS];
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
AFEImpulseCatcher WindImpulse;
AFEAnemometer AnemometerSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_RAINMETER
#include <AFE-Sensor-Rainmeter.h>
AFEImpulseCatcher RainImpulse;
AFERainmeter RainSensor;
#endif

#ifdef AFE_CONFIG_HARDWARE_PN532_SENSOR
#include <AFE-Sensor-PN532.h>
AFESensorPN532 PN532Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_PN532_SENSORS];
#include <AFE-MiFare-Card.h>
AFEMiFareCard MiFareCard[AFE_CONFIG_HARDWARE_NUMBER_OF_MIFARE_CARDS];
#endif

#include <AFE-Main-APIs.cpp>

#ifdef AFE_CONFIG_HARDWARE_MCP23017
#include <AFE-Main-MCP23017.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_LED
#include <AFE-Main-LED.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_RELAY
#include <AFE-Main-Relay.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_SWITCH
#include <AFE-Main-Switch.cpp>
#endif

#ifdef AFE_CONFIG_HARDWARE_ADC_VCC
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
AFEI2CScanner I2CScanner;
#endif

#ifdef AFE_CONFIG_HARDWARE_AS3935
#include <AFE-Sensor-AS3935.h>
AFESensorAS3935 AS3935Sensor[AFE_CONFIG_HARDWARE_NUMBER_OF_AS3935];
#endif

#ifdef AFE_CONFIG_HARDWARE_BINARY_SENSOR
#include <AFE-Sensor-Binary.h>
#endif

#include <AFE-Events-Handler.cpp>
#include <AFE-Main-HTTPServer.cpp>
/*
#include <FastLED.h>
#define LED_PIN 14
#define COLOR_ORDER RGB
#define CHIPSET WS2811
#define NUM_LEDS 8

#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME 3

#define AFE_CONFIG_HARDWARE_WS2811_LED_WAVE_TIMEOUT 50
#define AFE_CONFIG_HARDWARE_WS2811_LED_FADE_TIMEOUT 1
#define AFE_CONFIG_HARDWARE_WS2811_LED_MAX_BRIGHTNESS 255
#define AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP 20

unsigned long timer = millis();
uint8_t currentLed = 1;
int8_t increment = 1;

uint8_t currentBrightness = AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP + 1;

void setAllLedsColor(uint16_t color) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void On(uint16_t color) {
  setAllLedsColor(color);
  FastLED.show();
}
void Off() {
  setAllLedsColor(CRGB::Black);
  FastLED.show();
}

void FadeInOut() {
  if (millis() - timer > AFE_CONFIG_HARDWARE_WS2811_LED_WAVE_TIMEOUT) {
    if (currentBrightness >=
            AFE_CONFIG_HARDWARE_WS2811_LED_MAX_BRIGHTNESS -
                AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP - 1 ||
        currentBrightness <
            AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP + 1) {

      increment = increment > 0
                      ? -1 * AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP
                      : AFE_CONFIG_HARDWARE_WS2811_LED_BRIGHTNESS_STEP;
    }
    currentBrightness += increment;
    FastLED.setBrightness(currentBrightness);
    FastLED.show();
    timer = millis();
  }
}

void OneLedWave() {

  if (millis() - timer > AFE_CONFIG_HARDWARE_WS2811_LED_WAVE_TIMEOUT) {
    if (currentLed == NUM_LEDS - 1 || currentLed == 0) {
      increment *= -1;
    }
    leds[currentLed] = CRGB::Black;
    currentLed += increment;
    leds[currentLed] = CRGB::DarkBlue;
    FastLED.show();
    timer = millis();
  }
}
*/