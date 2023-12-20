#ifdef AFE_CONFIG_HARDWARE_MCP23XXX

/* ---------Headers ---------*/

//void initializeMCP23017(void);

/* --------- Body -----------

void initializeMCP23017(void) {

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfMCP23xxx; i++) {
#ifdef AFE_ESP32
    MCP23017Broker->begin(Firmware->API->Flash, Firmware->Device,
                          Firmware->Hardware->WirePort0,
                          Firmware->Hardware->WirePort1);
#else
    MCP23017Broker->begin(Firmware->API->Flash, Firmware->Device,
                          Firmware->Hardware->WirePort0);
#endif
  }
}; */
#endif // AFE_CONFIG_HARDWARE_MCP23XXX
