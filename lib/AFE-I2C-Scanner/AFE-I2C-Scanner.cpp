/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#include "AFE-I2C-Scanner.h"

AFEI2CScanner::AFEI2CScanner() {
  AFEDataAccess Data;
  I2CPORT I2C = Data.getI2CPortConfiguration();
  WirePort.begin(I2C.SDA, I2C.SCL);
};

#ifdef DEBUG
void AFEI2CScanner::scanAll() {

  uint8_t numberOfDeficesFound = 0;
  boolean searchStatus;

  Serial << endl
         << endl
         << "------------------ I2C Scanner ------------------";

  for (uint8_t address = 1; address < 127; address++) {

    searchStatus = scan(address);

    if (searchStatus)
      numberOfDeficesFound++;
  }

  if (numberOfDeficesFound == 0) {
    Serial << endl << "No I2C devices found";
  } else {
    Serial << endl << "Scanning completed";
  }

  Serial << endl
         << "---------------------------------------------------" << endl;
}
#endif

boolean AFEI2CScanner::scan(byte address) {
  byte status;
  WirePort.beginTransmission(address);
  status = WirePort.endTransmission();
  if (status == 0) {
#ifdef DEBUG
    Serial << endl << "Sensor Found [0x";
    if (address < 16) {
      Serial << "0";
    }
    Serial << _HEX(address) << "] : " << getName(address);
#endif
    return true;
  } else {
    return false;
  }
}

const char *AFEI2CScanner::getName(byte deviceAddress) {
  if (deviceAddress == 0x00)
    return "AS3935";
  else if (deviceAddress == 0x01)
    return "AS3935";
  else if (deviceAddress == 0x02)
    return "AS3935";
  else if (deviceAddress == 0x03)
    return "AS3935";
  else if (deviceAddress == 0x0A)
    return "SGTL5000";
  else if (deviceAddress == 0x0B)
    return "SMBusBattery?";
  else if (deviceAddress == 0x0C)
    return "AK8963";
  else if (deviceAddress == 0x10)
    return "CS4272";
  else if (deviceAddress == 0x11)
    return "Si4713";
  else if (deviceAddress == 0x13)
    return "VCNL4000,AK4558";
  else if (deviceAddress == 0x18)
    return "LIS331DLH";
  else if (deviceAddress == 0x19)
    return "LSM303,LIS331DLH";
  else if (deviceAddress == 0x1A)
    return "WM8731";
  else if (deviceAddress == 0x1C)
    return "LIS3MDL";
  else if (deviceAddress == 0x1D)
    return "LSM303D,LSM9DS0,ADXL345,MMA7455L,LSM9DS1,LIS3DSH";
  else if (deviceAddress == 0x1E)
    return "LSM303D,HMC5883L,FXOS8700,LIS3DSH";
  else if (deviceAddress == 0x20)
    return "MCP23017,MCP23008,PCF8574,FXAS21002,SoilMoisture";
  else if (deviceAddress == 0x21)
    return "MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x22)
    return "MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x23)
    return "BH1750,MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x24)
    return "MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x25)
    return "MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x26)
    return "MCP23017,MCP23008,PCF8574";
  else if (deviceAddress == 0x27)
    return "MCP23017,MCP23008,PCF8574,LCD16x2,DigoleDisplay";
  else if (deviceAddress == 0x28)
    return "BNO055,EM7180,CAP1188";
  else if (deviceAddress == 0x29)
    return "TSL2561,VL6180,TSL2561,TSL2591,BNO055,CAP1188";
  else if (deviceAddress == 0x2A)
    return "SGTL5000,CAP1188";
  else if (deviceAddress == 0x2B)
    return "CAP1188";
  else if (deviceAddress == 0x2C)
    return "MCP44XX ePot";
  else if (deviceAddress == 0x2D)
    return "MCP44XX ePot";
  else if (deviceAddress == 0x2E)
    return "MCP44XX ePot";
  else if (deviceAddress == 0x2F)
    return "MCP44XX ePot";
  else if (deviceAddress == 0x38)
    return "RA8875,FT6206";
  else if (deviceAddress == 0x39)
    return "TSL2561";
  else if (deviceAddress == 0x3C)
    return "SSD1306,DigisparkOLED";
  else if (deviceAddress == 0x3D)
    return "SSD1306";
  else if (deviceAddress == 0x40)
    return "PCA9685,Si7021";
  else if (deviceAddress == 0x41)
    return "STMPE610,PCA9685";
  else if (deviceAddress == 0x42)
    return "PCA9685";
  else if (deviceAddress == 0x43)
    return "PCA9685";
  else if (deviceAddress == 0x44)
    return "PCA9685";
  else if (deviceAddress == 0x45)
    return "PCA9685";
  else if (deviceAddress == 0x46)
    return "PCA9685";
  else if (deviceAddress == 0x47)
    return "PCA9685";
  else if (deviceAddress == 0x48)
    return "ADS1115,PN532,TMP102,PCF8591";
  else if (deviceAddress == 0x49)
    return "ADS1115,TSL2561,PCF8591";
  else if (deviceAddress == 0x4A)
    return "ADS1115";
  else if (deviceAddress == 0x4B)
    return "ADS1115,TMP102";
  else if (deviceAddress == 0x50)
    return "EEPROM";
  else if (deviceAddress == 0x51)
    return "EEPROM";
  else if (deviceAddress == 0x52)
    return "Nunchuk,EEPROM";
  else if (deviceAddress == 0x53)
    return "ADXL345,EEPROM";
  else if (deviceAddress == 0x54)
    return "EEPROM";
  else if (deviceAddress == 0x55)
    return "EEPROM";
  else if (deviceAddress == 0x56)
    return "EEPROM";
  else if (deviceAddress == 0x57)
    return "EEPROM";
  else if (deviceAddress == 0x58)
    return "TPA2016,MAX21100";
  else if (deviceAddress == 0x5A)
    return "MPR121";
  else if (deviceAddress == 0x5C)
    return "BH1750";
  else if (deviceAddress == 0x60)
    return "MPL3115,MCP4725,MCP4728,TEA5767,Si5351";
  else if (deviceAddress == 0x61)
    return "MCP4725,AtlasEzoDO";
  else if (deviceAddress == 0x62)
    return "LidarLite,MCP4725,AtlasEzoORP";
  else if (deviceAddress == 0x63)
    return "MCP4725,AtlasEzoPH";
  else if (deviceAddress == 0x64)
    return "AtlasEzoEC";
  else if (deviceAddress == 0x66)
    return "AtlasEzoRTD";
  else if (deviceAddress == 0x68)
    return "DS1307,DS3231,MPU6050,MPU9050,MPU9250,ITG3200,ITG3701,"
           "LSM9DS0,L3G4200D";
  else if (deviceAddress == 0x69)
    return "MPU6050,MPU9050,MPU9250,ITG3701,L3G4200D";
  else if (deviceAddress == 0x6A)
    return "LSM9DS1";
  else if (deviceAddress == 0x6B)
    return "LSM9DS0";
  else if (deviceAddress == 0x70)
    return "AdafruitLED";
  else if (deviceAddress == 0x71)
    return "SFE7SEG,AdafruitLED";
  else if (deviceAddress == 0x72)
    return "AdafruitLED";
  else if (deviceAddress == 0x73)
    return "AdafruitLED";
  else if (deviceAddress == 0x76)
    return "BMx280,MS5607,MS5611,MS5637";
  else if (deviceAddress == 0x77)
    return "BMx085,BMx180,BMx280,BMx680,MS5611";
  else
    return "UNKNOWN";
}
