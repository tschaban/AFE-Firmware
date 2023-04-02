/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */


#include "AFE-I2C-Scanner.h"
#ifdef AFE_CONFIG_HARDWARE_I2C

AFEI2CScanner::AFEI2CScanner() {};

 void AFEI2CScanner::begin(TwoWire *_WirePort) {
  AFEDataAccess Data;
  WirePort = _WirePort; 
 }

#ifdef DEBUG
void AFEI2CScanner::scanAll() {

  uint8_t numberOfDeficesFound = 0;
  boolean searchStatus;

  Serial << endl
         << endl
         << F("------------------ I2C Scanner ------------------");

  for (uint8_t address = 1; address < 127; address++) {

    searchStatus = scan(address);

    if (searchStatus)
      numberOfDeficesFound++;
  }

  if (numberOfDeficesFound == 0) {
    Serial << endl << F("No I2C devices found");
  } else {
    Serial << endl << F("Scanning completed");
  }

  Serial << endl
         << F("---------------------------------------------------") << endl;
}
#endif

boolean AFEI2CScanner::scan(byte address) {
  byte status;
  WirePort->beginTransmission(address);
  status = WirePort->endTransmission();
  if (status == 0) {
#ifdef DEBUG
    Serial << endl << F(" - Sensor Found [0x");
    if (address < 16) {
      Serial << F("0");
    }
    Serial << _HEX(address) << F("] : ") << getName(address);
#endif
    return true;
  } else {
    return false;
  }
}

const __FlashStringHelper *AFEI2CScanner::getName(byte deviceAddress) {
  /* WARN: Description can't be longer than 70chars, used by addDeviceI2CAddressSelectionItem in AFE-Site-Gnerator.h */
  if (deviceAddress == 0x00)
    return F("AS3935");
  else if (deviceAddress == 0x01)
    return F("AS3935");
  else if (deviceAddress == 0x02)
    return F("AS3935");
  else if (deviceAddress == 0x03)
    return F("AS3935");
  else if (deviceAddress == 0x0A)
    return F("SGTL5000");
  else if (deviceAddress == 0x0B)
    return F("SMBusBattery?");
  else if (deviceAddress == 0x0C)
    return F("AK8963");
  else if (deviceAddress == 0x10)
    return F("CS4272");
  else if (deviceAddress == 0x11)
    return F("Si4713");
  else if (deviceAddress == 0x13)
    return F("VCNL4000,AK4558");
  else if (deviceAddress == 0x18)
    return F("LIS331DLH");
  else if (deviceAddress == 0x19)
    return F("LSM303,LIS331DLH");
  else if (deviceAddress == 0x1A)
    return F("WM8731");
  else if (deviceAddress == 0x1C)
    return F("LIS3MDL");
  else if (deviceAddress == 0x1D)
    return F("LSM303D,LSM9DS0,ADXL345,MMA7455L,LSM9DS1,LIS3DSH");
  else if (deviceAddress == 0x1E)
    return F("LSM303D,HMC5883L,FXOS8700,LIS3DSH");
  else if (deviceAddress == 0x20)
    //return F("MCP23017,MCP23008,PCF8574,FXAS21002,SoilMoisture");
    return F("MCP23017");
  else if (deviceAddress == 0x21)
    //return F("MCP23017,MCP23008,PCF8574");
    return F("MCP23017");
  else if (deviceAddress == 0x22)
    //return F("MCP23017,MCP23008,PCF8574");
    return F("MCP23017");
  else if (deviceAddress == 0x23)
    return F("BH1750, MCP23017");
    //return F("BH1750,MCP23017,MCP23008,PCF8574");
  else if (deviceAddress == 0x24)
    //return F("MCP23017,MCP23008,PCF8574");
    return F("MCP23017,PN532");
  else if (deviceAddress == 0x25)
    //return F("MCP23017,MCP23008,PCF8574");
    return F("MCP23017");
  else if (deviceAddress == 0x26)
    //return F("MCP23017,MCP23008,PCF8574");
    return F("MCP23017");
  else if (deviceAddress == 0x27)
    //return F("MCP23017,MCP23008,PCF8574,LCD16x2,DigoleDisplay");
    return F("MCP23017");
  else if (deviceAddress == 0x28)
    return F("FS3000");
  else if (deviceAddress == 0x29)
    return F("TSL25xx,VL6180,BNO055,CAP1188");
  else if (deviceAddress == 0x2A)
    return F("SGTL5000,CAP1188");
  else if (deviceAddress == 0x2B)
    return F("CAP1188");
  else if (deviceAddress == 0x2C)
    return F("MCP44XX");
  else if (deviceAddress == 0x2D)
    return F("MCP44XX");
  else if (deviceAddress == 0x2E)
    return F("MCP44XX");
  else if (deviceAddress == 0x2F)
    return F("MCP44XX");
  else if (deviceAddress == 0x38)
    return F("RA8875,FT6206");
  else if (deviceAddress == 0x39)
    return F("TSL2561");
  else if (deviceAddress == 0x3C)
    return F("SSD1306,DigisparkOLED");
  else if (deviceAddress == 0x3D)
    return F("SSD1306");
  else if (deviceAddress == 0x40)
    return F("PCA9685,Si7021");
  else if (deviceAddress == 0x41)
    return F("STMPE610,PCA9685");
  else if (deviceAddress == 0x42)
    return F("PCA9685");
  else if (deviceAddress == 0x43)
    return F("PCA9685");
  else if (deviceAddress == 0x44)
    return F("PCA9685");
  else if (deviceAddress == 0x45)
    return F("PCA9685");
  else if (deviceAddress == 0x46)
    return F("PCA9685");
  else if (deviceAddress == 0x47)
    return F("PCA9685");
  else if (deviceAddress == 0x48)
    return F("ADS1115,PN532,TMP102,PCF8591");
  else if (deviceAddress == 0x49)
    return F("ADS1115,TSL2561,PCF8591");
  else if (deviceAddress == 0x4A)
    return F("ADS1115");
  else if (deviceAddress == 0x4B)
    return F("ADS1115,TMP102");
  else if (deviceAddress == 0x50)
    return F("EEPROM");
  else if (deviceAddress == 0x51)
    return F("EEPROM");
  else if (deviceAddress == 0x52)
    return F("Nunchuk,EEPROM");
  else if (deviceAddress == 0x53)
    return F("ADXL345,EEPROM");
  else if (deviceAddress == 0x54)
    return F("EEPROM");
  else if (deviceAddress == 0x55)
    return F("EEPROM");
  else if (deviceAddress == 0x56)
    return F("EEPROM");
  else if (deviceAddress == 0x57)
    return F("EEPROM");
  else if (deviceAddress == 0x58)
    return F("TPA2016,MAX21100");
  else if (deviceAddress == 0x5A)
    return F("MPR121");
  else if (deviceAddress == 0x5C)
    return F("BH1750");
  else if (deviceAddress == 0x60)
    return F("MPL3115,MCP4725,MCP4728,TEA5767,Si5351");
  else if (deviceAddress == 0x61)
    return F("MCP4725,AtlasEzoDO");
  else if (deviceAddress == 0x62)
    return F("LidarLite,MCP4725,AtlasEzoORP");
  else if (deviceAddress == 0x63)
    return F("MCP4725,AtlasEzoPH");
  else if (deviceAddress == 0x64)
    return F("AtlasEzoEC");
  else if (deviceAddress == 0x66)
    return F("AtlasEzoRTD");
  else if (deviceAddress == 0x68)
    return F("DS1307,DS3231,MPU6050,MPU9050,MPU9250,ITG3200,ITG3701,"
           "LSM9DS0,L3G4200D");
  else if (deviceAddress == 0x69)
    return F("MPU6050,MPU9050,MPU9250,ITG3701,L3G4200D");
  else if (deviceAddress == 0x6A)
    return F("LSM9DS1");
  else if (deviceAddress == 0x6B)
    return F("LSM9DS0");
  else if (deviceAddress == 0x70)
    return F("AdafruitLED");
  else if (deviceAddress == 0x71)
    return F("SFE7SEG,AdafruitLED");
  else if (deviceAddress == 0x72)
    return F("AdafruitLED");
  else if (deviceAddress == 0x73)
    return F("AdafruitLED");
  else if (deviceAddress == 0x76)
    return F("BMx280");
    //return F("BMx280,MS5607,MS5611,MS5637");
  else if (deviceAddress == 0x77)
    //return F("BMx085,BMx180,BMx280,BMx680,MS5611");
    return F("BMx085,BMx180,BMx280,BMx680");
  else
    return F("UNKNOWN");
}

#endif // AFE_CONFIG_HARDWARE_I2C