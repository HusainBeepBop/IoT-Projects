#include <Servo.h>

Servo myServo;  // Create servo object
const int servoPin = 9; // Pin connected to the servo on Arduino Uno

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos += 10) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }

  // Sweep from 180 to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 10) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }
}
