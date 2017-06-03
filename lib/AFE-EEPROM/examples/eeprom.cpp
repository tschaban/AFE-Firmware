#include <AFE-EEPROM.h>
#include <Streaming.h>

AFEEEPROM Eeprom;

void setup() {
        Serial.begin(115200);
        delay(10);

        Serial << endl << "INFO: Erasing EEPROM: ";
        Eeprom.erase();
        Serial << "done";

        Serial << endl << "INFO: Writing to EEPROM text Hello Word! at 0x000: ";
        Eeprom.write(0,12,"Hello World!");
        Serial << "done";

        Serial << endl << "INFO: Writing to EEPROM boolean: true at 0x013: ";
        Eeprom.write(13,true);
        Serial << "done";

        Serial << endl << "INFO: Writing to EEPROM int: 123 at 0x014: ";
        Eeprom.write(14,3,(long)123);
        Serial << "done";

        Serial << endl << "INFO: Writing to EEPROM float: -12.345 at 0x017: ";
        Eeprom.write(17,8,(float)-12.345);
        Serial << "done";


        Serial << endl << "-------------------------------------------------";
        Serial << endl << "INFO: Reading to EEPROM text from 0x000";
        Serial << endl << "INFO: Text from EEPROM: " << Eeprom.read(0,12);

        Serial << endl << "INFO: Reading to EEPROM boolean at 0x013";
        Serial << endl << "INFO: Boolean from EEPROM: " << (Eeprom.read(13) ? "TRUE" : "FALSE");

        Serial << endl << "INFO: Reading to EEPROM int from 0x014";
        Serial << endl << "INFO: Int from EEPROM: " << Eeprom.read(14,3);

        Serial << endl << "INFO: Reading to EEPROM int from 0x017";
        Serial << endl << "INFO: Float from EEPROM: " << Eeprom.read(17,8);

}

void loop() {
}
