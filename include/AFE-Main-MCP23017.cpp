#ifdef AFE_CONFIG_HARDWARE_MCP23XXX

/* ---------Headers ---------*/

void initializeMCP23017(void);

/* --------- Body -----------*/

void initializeMCP23017(void) {

  for (uint8_t i = 0; i < Device->configuration.noOfMCP23xxx; i++) {
#ifdef AFE_ESP32
    MCP23017Broker->begin(Data, Device, &WirePort0, &WirePort1);
#else
    MCP23017Broker->begin(Data, Device, &WirePort0);
#endif
  }
};
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
