#ifdef AFE_CONFIG_HARDWARE_MCP23017

/* ---------Headers ---------*/

void initializeMCP23017(void);

/* --------- Body -----------*/

void initializeMCP23017(void) {

  for (uint8_t i = 0; i < AFE_CONFIG_HARDWARE_NUMBER_OF_MCP23017; i++) {
#ifdef AFE_ESP32
    MCP23017Broker[i].begin(&Data, &WirePort0, &WirePort1);
#else
    MCP23017Broker[i].begin(&Data, &WirePort0);
#endif
  }
};
#endif // AFE_CONFIG_HARDWARE_MCP23017
