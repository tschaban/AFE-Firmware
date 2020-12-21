#ifdef AFE_CONFIG_HARDWARE_LED

/* ---------Headers ---------*/

void initializeLED(void);

/* --------- Body -----------*/

void initializeLED(void) {
  uint8_t id = Data.getSystemLedID();
#ifdef DEBUG
  boolean initialized = false;
#endif

#ifdef AFE_CONFIG_HARDWARE_MCP23017
  uint8_t MCP23017Id = AFE_HARDWARE_ITEM_NOT_EXIST;
#endif

  if (id != AFE_HARDWARE_ITEM_NOT_EXIST) {
    if (Device.configuration.noOfLEDs - 1 >= id) {
      Led.initialize(&Data, id);
#ifdef AFE_CONFIG_HARDWARE_MCP23017
      if (Led.configuration.mcp23017.address != AFE_HARDWARE_ITEM_NOT_EXIST) {
        MCP23017Id = MCP23017Broker.getId(Led.configuration.mcp23017.address);
        if (MCP23017Id != AFE_HARDWARE_ITEM_NOT_EXIST) {
          Led.addMCP23017Reference(&MCP23017Broker.MCP[MCP23017Id]);
        }
      }
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
      Serial << endl << F("INFO: System LED initialized");
    } else {
      Serial << endl << F("WARN: System LED NOT initialized");
    }
#endif // DEBUG
  }
};
#endif // AFE_CONFIG_HARDWARE_LED
