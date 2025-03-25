const int buttonPin = 32;  
const int ledPin = 2;      

void setup() {
  pinMode(buttonPin, INPUT);  
  pinMode(ledPin, OUTPUT);   
}

void loop() {
  int buttonState = digitalRead(buttonPin); 

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);  
  } else {
    digitalWrite(ledPin, LOW);   
  }
}
