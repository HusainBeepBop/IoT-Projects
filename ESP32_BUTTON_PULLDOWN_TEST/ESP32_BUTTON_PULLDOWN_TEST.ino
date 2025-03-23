const int buttonPin = 32;  // GPIO15 for the button
const int ledPin = 2;      // Onboard LED on GPIO2

void setup() {
  pinMode(buttonPin, INPUT);  // Button as input
  pinMode(ledPin, OUTPUT);    // LED as output
}

void loop() {
  int buttonState = digitalRead(buttonPin); // Read button state

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);  // Turn on LED if button is pressed
  } else {
    digitalWrite(ledPin, LOW);   // Turn off LED if button is not pressed
  }
}
