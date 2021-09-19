/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifndef _AFE_I2C_Structure_h
#define _AFE_I2C_Structure_h


struct I2CPORT {
  uint8_t SDA;
  uint8_t SCL;
#ifdef AFE_ESP32  
  uint32_t frequency;
#endif  
};

#endif
