#include <IRremote.h>

#define IR_RECEIVE_PIN 15

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Ready to capture detailed raw IR signals...");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println("Received Raw Data:");
    IrReceiver.printIRResultRawFormatted(&Serial);
    IrReceiver.resume();  // Prepare for the next signal
  }
}
