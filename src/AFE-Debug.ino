#if defined(DEBUG)

#define DEBUG_INTERVAL 10
unsigned long debugStartTime = 0;

void debugListener() {
  unsigned long time = millis();

  if (debugStartTime == 0) { // starting timer. used for switch sensitiveness
    debugStartTime = time;
  }

  if (time - debugStartTime >= DEBUG_INTERVAL * 1000) {
    Serial << endl << endl << "----------- DEBUGGER ------------";
    getAvailableMem();
#ifdef CONFIG_HARDWARE_SPIFFS
    getFileSystemInfo();
#endif
    debugStartTime = 0;
    Serial << endl << "---------------------------------" << endl;
  }
}

void getAvailableMem() {
  Serial << endl
         << "- Free Memory = " << system_get_free_heap_size() / 1024 << "kB";
}

#ifdef CONFIG_HARDWARE_SPIFFS
void getFileSystemInfo() {
  FSInfo fileSystem;
  SPIFFS.info(fileSystem);
  Serial << endl
         << "- File system. Used: " << fileSystem.usedBytes / 1024 << "kB from "
         << fileSystem.totalBytes / 1024 << "kB"
         << " Max.open files " << fileSystem.maxOpenFiles;
}
#endif

#endif
