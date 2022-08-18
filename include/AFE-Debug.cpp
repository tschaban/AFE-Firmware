/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#if defined(DEBUG)

#define DEBUG_INTERVAL 60
unsigned long debugStartTime = 0;

void debugListener();
void getAvailableMem(); 
void getFileSystemInfo();
void getESPInformation();

void debugListener() {
  unsigned long time = millis();

  if (debugStartTime == 0) { // starting timer. used for switch sensitiveness
    debugStartTime = time;
  }

  if (time - debugStartTime >= DEBUG_INTERVAL * 1000) {
    getAvailableMem();
    getFileSystemInfo();
    debugStartTime = 0;
  }
}

void getAvailableMem() {
  Serial << endl
         << F("INFO: RAM: ") << system_get_free_heap_size() / 1024
         << F("kB");
}

void getFileSystemInfo() {
  FSInfo fileSystem;
  SPIFFS.info(fileSystem);
  Serial << endl
         << F("INFO: FILES SYSTEM. Used: ") << fileSystem.usedBytes / 1024
         << F("kB from ") << fileSystem.totalBytes / 1024 << F("kB")
         << F(" Max.open files ") << fileSystem.maxOpenFiles;
}

void getESPInformation() {

#ifndef AFE_ESP32 /* ESP82xx */
  Serial << endl
         << F("INFO: RAM: ") << system_get_free_heap_size() / 1024
         << F("kB: at start");
  Serial << endl << F("INFO: ESP: ID ") << ESP.getFlashChipId();
  Serial << endl << F("INFO: ESP: Real flash size: ");
  if (ESP.getFlashChipRealSize() >= 1048576) {
    Serial << (ESP.getFlashChipRealSize() / 1048576) << F(" Mbits");
  } else {
    Serial << (ESP.getFlashChipRealSize() / 1024) << F(" Kbits");
  }

  Serial << endl << F("INFO: ESP: Flesh size: ");
  if (ESP.getFlashChipSize() >= 1048576) {
    Serial << (ESP.getFlashChipSize() / 1048576) << F(" Mbits");
  } else {
    Serial << (ESP.getFlashChipSize() / 1024) << F(" Kbits");
  }

  Serial << endl
         << F("INFO: ESP: Speed ") << (ESP.getFlashChipSpeed() / 1000000)
         << F(" MHz");
  Serial << endl << F("INFO: ESP: Mode ") << ESP.getFlashChipMode() << endl;

#else  /* ESP32 */
  Serial << endl << F("INFO: ESP: Chip Model ") << ESP.getChipModel();
  Serial << endl << F("INFO: ESP: Cores: ") << ESP.getChipCores();
  Serial << endl
         << F("INFO: ESP: CPU Frequency: ") << ESP.getCpuFreqMHz() << F("Mhz");
  Serial << endl << F("INFO: ESP: Flesh size: ");
  if (ESP.getFlashChipSize() >= 1048576) {
    Serial << (ESP.getFlashChipSize() / 1048576) << F(" Mbits");
  } else {
    Serial << (ESP.getFlashChipSize() / 1024) << F(" Kbits");
  }
  Serial << endl
         << F("INFO: ESP: Speed ") << (ESP.getFlashChipSpeed() / 1000000)
         << F(" MHz");
  Serial << endl << F("INFO: ESP: Mode ") << ESP.getFlashChipMode() << endl;
  Serial << endl << F("INFO: ESP: Firmware size: ") << ESP.getSketchSize();
  Serial << endl
         << F("INFO: ESP: Firmware free space size: ")
         << ESP.getFreeSketchSpace();
#endif // ESP32
}

#endif
