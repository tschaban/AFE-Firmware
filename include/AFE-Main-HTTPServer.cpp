/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

/* ---------Headers ---------*/

void handleFavicon(void);
void handleHTTPRequests(void);
void handleUpload(void);
void handleOnNotFound(void);
void initializeHTTPServer(void);

/* --------- Body -----------*/

/* Method handles favicon.ico request */
void handleFavicon(void) {}

/* Method handles all HTTP request */
void handleHTTPRequests(void) {
  if (!HTTPServer.generate()) {
    HttpAPI.listener();
  }
}
void handleUpload(void) { HTTPServer.generate(true); }

void handleOnNotFound(void) {
  HTTPServer.server.send(404, "text/plain", F(L_404));
}

void initializeHTTPServer(void) {
  /* Initializing HTTP HTTPServer */
  HTTPServer.handle("/", handleHTTPRequests);
  HTTPServer.handle("/favicon.ico", handleFavicon);
  HTTPServer.handleFirmwareUpgrade("/upgrade", handleHTTPRequests,
                                   handleUpload);
  HTTPServer.onNotFound(handleOnNotFound);
#if defined(AFE_CONFIG_HARDWARE_LED) && !defined(AFE_CONFIG_HARDWARE_I2C)
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led);
#elif defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led, &WirePort0,
                   &WirePort1);
#else
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led, &WirePort0);
#endif // AFE_ESP32
#elif !defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &WirePort0,
                   &WirePort1);
#else
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &WirePort0);
#endif // AFE_ESP32
#else
  HTTPServer.begin(&Data, &Device, &FirmwarePro, &RestAPI);
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: HTTP Server initialized");
#endif
}
