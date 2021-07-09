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
void handleHTTPRequests(void) { WebServer.generate(); }
void handleUpload(void) { WebServer.generate(true); }

void handleOnNotFound(void) {
  String page = "<head><meta http-equiv=\"refresh\" content=\"0; "
                "url=http://192.168.5.1/\" /></head><body><p>";
  page += L_INDEX_OPENING_CONFIG_PANEL;
  page += "</p></body>";
  WebServer.publishHTML(page);
}

void initializeHTTPServer(void) {
  /* Initializing HTTP HTTPServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.handleFirmwareUpgrade("/upgrade", handleHTTPRequests, handleUpload);
  if (Device.getMode() == AFE_MODE_NETWORK_NOT_SET) {
    WebServer.onNotFound(handleOnNotFound);
  }
#if defined(AFE_CONFIG_HARDWARE_LED) && !defined(AFE_CONFIG_HARDWARE_I2C)
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led);
#elif defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led, &WirePort0,
                  &WirePort1);
#else
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &Led, &WirePort0);
#endif // AFE_ESP32
#elif !defined(AFE_CONFIG_HARDWARE_LED) && defined(AFE_CONFIG_HARDWARE_I2C)
#ifdef AFE_ESP32
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &WirePort0,
                  &WirePort1);
#else
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI, &WirePort0);
#endif // AFE_ESP32
#else
  WebServer.begin(&Data, &Device, &FirmwarePro, &RestAPI);
#endif

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: HTTP Server initialized");
#endif
}
