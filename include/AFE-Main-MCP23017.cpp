#ifdef AFE_CONFIG_HARDWARE_MCP23017

/* ---------Headers ---------*/

void initializeMCP23017(void);

/* --------- Body -----------*/

void initializeMCP23017(void) {

#ifdef AFE_ESP32
    MCP23017Broker.begin(&Data, &WirePort0, &WirePort1);
#else
    MCP23017Broker.begin(&Data, &WirePort0);
#endif

};
#endif // AFE_CONFIG_HARDWARE_MCP23017
