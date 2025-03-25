#include <IRremote.h>

#define IR_SEND_PIN 4

// Raw data for Power Off
uint16_t powerOffSignal[] = {
  2890950, 1050, 550, 1050, 500, 1050, 550, 1050, 500, 1050, 2550,
  1050, 500, 1050, 500, 1050, 550, 1050, 2500, 1050, 550, 1000, 2550,
  1050, 2550, 1050, 550, 1000, 550, 1000, 550, 1000, 2600, 1000
};

// Raw data for Power On
uint16_t powerOnSignal[] = {
  2890950, 1050, 550, 1050, 500, 1050, 2550, 1050, 2500, 1050, 550,
  1050, 500, 1050, 2550, 1050, 2500, 1050, 2500, 1050, 550, 1050, 500,
  1050, 500, 1050, 2550, 1050, 500, 1050, 550, 1050, 500, 1050, 2550,
  1050, 500, 1050, 500, 1050, 550, 1050, 500, 1050, 550, 1050, 500,
  1050, 500, 1050, 550, 1050, 500, 1050, 500, 1050, 550, 1050, 500,
  1050, 500, 1050, 550, 1050, 500, 1050, 2550, 1050, 500, 1050, 550,
  1050, 500, 1050, 500, 1050, 550, 1050, 500, 1050, 500, 1050, 550,
  1000, 550, 1000, 550, 1000, 550, 1000, 2600, 1000, 550, 1000, 600,
  1000, 550, 1000, 550, 1000, 600, 1000, 2550, 1050, 2550, 1000, 550,
  1050, 550, 1000, 550, 1000, 2600, 1000
};

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_SEND_PIN);
  Serial.println("IR Transmitter Ready. Type 'on' or 'off' to test.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("on")) {
      Serial.println("Sending ON Signal...");
      IrSender.sendRaw(powerOnSignal, sizeof(powerOnSignal) / sizeof(powerOnSignal[0]), 38);
    } else if (command.equalsIgnoreCase("off")) {
      Serial.println("Sending OFF Signal...");
      IrSender.sendRaw(powerOffSignal, sizeof(powerOffSignal) / sizeof(powerOffSignal[0]), 38);
    } else {
      Serial.println("Invalid command. Please type 'on' or 'off'.");
    }
  }
}
