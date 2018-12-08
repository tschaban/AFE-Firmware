#if defined(DEBUG)

#define DEBUG_INTERVAL 10
unsigned long debugStartTime = 0;

void debugListener() {
  unsigned long time = millis();

  if (debugStartTime == 0) { // starting timer. used for switch sensitiveness
    debugStartTime = time;
  }

  if (time - debugStartTime >= DEBUG_INTERVAL * 1000) {
    getAvailableMem();
    debugStartTime = 0;
  }
}

void getAvailableMem() {
  Serial << endl << endl << "----------- DEBUGGER ------------";
  Serial << endl
         << "Free Memory = " << system_get_free_heap_size() / 1024 << "kB";
  Serial << endl << "---------------------------------" << endl;
}

#endif
