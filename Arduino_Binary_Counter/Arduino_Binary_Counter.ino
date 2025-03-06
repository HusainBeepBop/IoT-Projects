const int ledPins[] = {12, 11, 10, 5, 6, 7, 8, 9}; 
const int numLEDs = 8;

const int upButtonPin = 2;
const int downButtonPin = 3;
const int resetButtonPin = 4;

int upLastState = LOW, downLastState = LOW, resetLastState = LOW;
int counter = 0;

void updateLEDs(int value) {
    Serial.print("LEDs: ");
    for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], (value >> i) & 1);
        Serial.print((value >> i) & 1);
    }
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    Serial.println("System Initialized...");

    for (int i = 0; i < numLEDs; i++) pinMode(ledPins[i], OUTPUT);
    pinMode(upButtonPin, INPUT);
    pinMode(downButtonPin, INPUT);
    pinMode(resetButtonPin, INPUT);

    updateLEDs(counter);
}

void loop() {
    int upReading = digitalRead(upButtonPin);
    int downReading = digitalRead(downButtonPin);
    int resetReading = digitalRead(resetButtonPin);

    if (upReading && !upLastState) {
        counter = (counter + 1) % 256;
        Serial.print("UP: "); Serial.println(counter);
        updateLEDs(counter);
    } 
    else if (downReading && !downLastState) {
        counter = (counter - 1 + 256) % 256;
        Serial.print("DOWN: "); Serial.println(counter);
        updateLEDs(counter);
    } 
    else if (resetReading && !resetLastState) {
        counter = 0;
        Serial.println("RESET");
        updateLEDs(counter);
    }

    upLastState = upReading;
    downLastState = downReading;
    resetLastState = resetReading;

    delay(200); // Debounce delay
}
