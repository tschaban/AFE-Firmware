#ifdef AFE_CONFIG_HARDWARE_LED

/* ---------Headers ---------*/

void initializeLED(void);

/* --------- Body -----------*/

void initializeLED(void) {
  uint8_t id = Data.getSystemLedID();
#ifdef DEBUG
  boolean initialized = false;
#endif
  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (Device.configuration.noOfLEDs - 1 >= id) {
#ifdef AFE_CONFIG_HARDWARE_MCP23017
      // @TODO T4 Led.addMCP23017Reference(&MCP23017Broker);
#endif // AFE_CONFIG_HARDWARE_MCP23017
      if (Led.begin(&Data, id)) {
        Led.on();
#ifdef DEBUG
        initialized = true;
#endif
      }
    }
#ifdef DEBUG
    if (initialized) {
      Serial << endl << F("INFO: BOOT: System LED initialized");
    } else {
      Serial << endl << F("WARN: BOOT: System LED NOT initialized");
    }
#endif // DEBUG
  }
};
#endif // AFE_CONFIG_HARDWARE_LED
