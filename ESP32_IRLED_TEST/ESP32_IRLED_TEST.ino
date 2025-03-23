#define IR_SEND_PIN 4

void setup() {
  pinMode(IR_SEND_PIN, OUTPUT);
}

void loop() {
  digitalWrite(IR_SEND_PIN, HIGH);  // LED On
  delay(500);
  digitalWrite(IR_SEND_PIN, LOW);   // LED Off
  delay(500);
}
