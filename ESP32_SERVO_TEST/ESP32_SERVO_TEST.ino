#include <ESP32Servo.h>

Servo myServo;
const int servoPin = 15; 

void setup() {
  myServo.attach(servoPin);
  Serial.begin(115200);
  Serial.println("Servo Test Starting...");
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos += 5) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }

  // Sweep from 180 to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 5) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }
}
