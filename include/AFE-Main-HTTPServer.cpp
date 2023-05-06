/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

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
  if (!HTTPServer->generate()) {
    HttpAPI->listener();
  }
}
void handleUpload(void) { HTTPServer->generate(true); }

void handleOnNotFound(void) {
  HTTPServer->server.send(404, "text/plain", F(L_404));
}

void initializeHTTPServer(void) {
  /* Initializing HTTP HTTPServer */
  HTTPServer->handle("/", handleHTTPRequests);
  HTTPServer->handle("/favicon.ico", handleFavicon);
  HTTPServer->handleFirmwareUpgrade("/upgrade", handleHTTPRequests,
                                    handleUpload);
  HTTPServer->onNotFound(handleOnNotFound);
  HTTPServer->begin(Firmware);

#ifdef DEBUG
  Serial << endl << F("INFO: BOOT: HTTP Server initialized");
#endif
}
