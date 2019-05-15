#include <AFE-Sensor-DS18B20.h>
#include <Streaming.h>

AFESensorDS18B20      TemperatureSensor(14);
void setup() {
   Serial.begin(115200);
}

void loop() {
    Serial << endl << "Temp C : " << TemperatureSensor.get(UNIT_CELCIUS);
    Serial << endl << "Temp F : " << TemperatureSensor.get(UNIT_FAHRENHEIT);
    delay(10000);
}
