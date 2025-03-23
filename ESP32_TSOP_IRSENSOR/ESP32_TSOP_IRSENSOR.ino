#include <IRremote.h>

// Define the GPIO pin connected to the TSOP sensor
#define IR_RECEIVE_PIN 15

void setup() {
  Serial.begin(9600);
  Serial.println("Starting IR Receiver...");

  // Initialize IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver is ready. Waiting for signals...");
}

void loop() {
  // Check if IR signal is received
  if (IrReceiver.decode()) {
    Serial.print("Decoded Protocol: ");
    Serial.print(IrReceiver.decodedIRData.protocol);
    Serial.print(", Code: 0x");
    Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print(", Bits: ");
    Serial.println(IrReceiver.decodedIRData.numberOfBits);

    // Resume receiving for the next signal
    IrReceiver.resume();
  }
}
