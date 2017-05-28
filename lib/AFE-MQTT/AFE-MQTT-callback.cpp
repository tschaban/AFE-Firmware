#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Streaming.h>

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  Serial << endl << "INFO: MQTT message recieved: " << topic << " \\ ";
  if (length >= 1) { // command arrived
    for (uint16_t i=0;i<length;i++) {
      Serial << (char)payload[i];
    }
  }
}
