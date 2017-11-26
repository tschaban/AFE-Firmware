/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

#include "AFE-EEPROM.h"

AFEEEPROM::AFEEEPROM() { EEPROM.begin(EEPROM_SIZE); }

String AFEEEPROM::read(uint16_t address, uint16_t size) {
  String _return;
  for (uint16_t i = address; i < address + size; ++i) {
    if (EEPROM.read(i) != 255) {
      _return += char(EEPROM.read(i));
    }
  }
  return _return;
}

boolean AFEEEPROM::read(uint16_t address) {
  return (read(address, 1).toInt() == 1 ? true : false);
}

uint8_t AFEEEPROM::readUInt8(uint16_t address) { return EEPROM.read(address); }

void AFEEEPROM::write(uint16_t address, uint16_t size, String in) {
  for (uint16_t i = 0; i < size; i++) {
    writeUInt8(address + i, (i < in.length() ? in[i] : 255));
  }
  EEPROM.commit();
}

IPAddress AFEEEPROM::readIP(uint16_t address) {
  IPAddress ip(readUInt8(address), readUInt8(address + 1),
               readUInt8(address + 2), readUInt8(address + 3));
  return ip;
}

void AFEEEPROM::writeUInt8(uint16_t address, uint8_t in) {
  /* Method compares what's in EEPROM already to save lifecycle of it */
  if (EEPROM.read(address) != in) {
    EEPROM.write(address, in);
    EEPROM.commit();
  }
}

void AFEEEPROM::write(uint16_t address, boolean in) {
  write(address, 1, String(in ? 1 : 0));
}

void AFEEEPROM::write(uint16_t address, uint16_t size, long in) {
  write(address, size, String(in));
}

void AFEEEPROM::write(uint16_t address, uint16_t size, float in) {
  write(address, size, String(in));
}

void AFEEEPROM::writeIP(uint16_t address, IPAddress ip) {
  writeUInt8(address, ip[0]);
  writeUInt8(address + 1, ip[1]);
  writeUInt8(address + 2, ip[2]);
  writeUInt8(address + 3, ip[3]);
}

void AFEEEPROM::clear(uint16_t address, uint16_t size) {
  for (uint16_t i = 0; i < size; ++i) {
    EEPROM.write(i + address, 255);
  }
  EEPROM.commit();
}

void AFEEEPROM::erase() { clear(0, EEPROM_SIZE); }
