#ifndef _AFE_Hardware_Gate_h
#define _AFE_Hardware_Gate_h

/* Gate's states */
#define AFE_GATE_OPEN 0           // Open
#define AFE_GATE_CLOSED 1         // Closed
#define AFE_GATE_PARTIALLY_OPEN 2 // Being opened
#define AFE_GATE_UNKNOWN 9        // Unknown state

/* Gate MQTT messages */
#define AFE_MQTT_GATE_OPEN "open"
#define AFE_MQTT_GATE_CLOSED "closed"
#define AFE_MQTT_GATE_PARTIALLY_OPEN "partiallyOpen"
#define AFE_MQTT_GATE_UNKNOWN "unknown"

#define AFE_CONFIG_API_JSON_GATE_DATA_LENGTH  200 // Not checked, used by HTTP API

#endif // _AFE_Hardware_Gate_h