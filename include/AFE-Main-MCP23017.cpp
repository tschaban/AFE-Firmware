#ifdef AFE_CONFIG_HARDWARE_MCP23017

/* ---------Headers ---------*/

void initializeMCP23017(void);

/* --------- Body -----------*/

void initializeMCP23017(void) { MCP23017Broker.begin(&Data, &Device); };
#endif // AFE_CONFIG_HARDWARE_MCP23017
