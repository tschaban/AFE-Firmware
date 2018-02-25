/* Method processes gate related events */
void mainGate() {
  if (Gate.event()) {
    Led.on();
    MQTTPublishGateState();
    Led.off();
  }
}
