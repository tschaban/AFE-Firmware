/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef DEBUG

#define DEBUG_INTERVAL 60

/* DEBUG Messages */
#define AFE_D_WELCOME_MESSAGE "\n\n################################ BOOTING ################################\nINFO: All classes and global variables initialized\nINFO: Initializing device\n"


unsigned long debugStartTime = 0;


void debugListener();
void getAvailableMem();
#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
void getFileSystemInfo();
#endif
void getESPInformation();

void debugListener() {
  unsigned long time = millis();

  if (debugStartTime == 0) { // starting timer. used for switch sensitiveness
    debugStartTime = time;
  }

  if (time - debugStartTime >= DEBUG_INTERVAL * 1000) {
    getAvailableMem();
#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
    getFileSystemInfo();
#endif
    debugStartTime = 0;
  }
}

void getAvailableMem() {
  
  #ifndef AFE_ESP32
  Serial << endl << F("INFO: RAM: ") << system_get_free_heap_size() / 1024 << F("kB");
  #else
  Serial << endl << F("WARN: RAM: Free? Not implemented yet for ESP32");
  #endif
}

#if AFE_FILE_SYSTEM == AFE_FS_SPIFFS
void getFileSystemInfo() {
  FSInfo fileSystem;
  SPIFFS.info(fileSystem);
  Serial << endl
         << F("INFO: FILES SYSTEM. Used: ") << fileSystem.usedBytes / 1024
         << F("kB from ") << fileSystem.totalBytes / 1024 << F("kB")
         << F(" Max.open files ") << fileSystem.maxOpenFiles;
}
#endif

void getESPInformation() {

#ifndef AFE_ESP32 /* ESP82xx */
  getAvailableMem();
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
