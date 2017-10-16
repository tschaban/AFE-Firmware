#ifndef _AFE_Web_Handlers_h
#define _AFE_Web_Handlers_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Streaming.h>

void handleFavicon() {}

void handleHTTPRequests() { WebServer.generate(); }

#endif
