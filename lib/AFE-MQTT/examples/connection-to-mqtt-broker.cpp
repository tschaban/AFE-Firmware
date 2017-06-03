#include <AFE-WiFi.h>
#include <AFE-MQTT.h>

AFEWiFi Network;
AFEMQTT Mqtt;

boolean send = true;

void setup() {
        Serial.begin(115200);
        delay(10);

        Mqtt.begin("192.168.2.231",1883,"user","password","/device/","AFE");
        Mqtt.setReconnectionParams(10,1,5);
        Network.setReconnectionParams(10,1,5);
        Network.begin("SSID","password","AFE-DEVICE");

}

void loop() {
        if (Network.connected()) {
                if (!Mqtt.connected()) {
                        Mqtt.connect();
                } else {
                        if (send) {
                                Mqtt.publish("state","ON");
                                Mqtt.publish("/device/","state","OFF");
                                send=false;
                        }
                        Mqtt.loop();
                }
        } else {
                Network.connect();
        }
}
