/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#if defined(DEBUG)

#define DEBUG_INTERVAL 60
unsigned long debugStartTime = 0;

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
         << F("INFO: MEMORY = ") << system_get_free_heap_size() / 1024 << F("kB");
}

void getFileSystemInfo() {
  FSInfo fileSystem;
  SPIFFS.info(fileSystem);
  Serial << endl
         << F("INFO: FILES SYSTEM. Used: ") << fileSystem.usedBytes / 1024 << F("kB from ")
         << fileSystem.totalBytes / 1024 << F("kB")
         << F(" Max.open files ") << fileSystem.maxOpenFiles;
}

#endif
