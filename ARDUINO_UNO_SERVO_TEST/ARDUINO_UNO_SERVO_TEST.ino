#include <Servo.h>

Servo myServo;  
const int servoPin = 9; 

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  
  for (int pos = 0; pos <= 180; pos += 10) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }

  
  for (int pos = 180; pos >= 0; pos -= 10) {
    myServo.write(pos);
    Serial.print("Angle: ");
    Serial.println(pos);
    delay(50);
  }
}
