# Arduino Binary Counter Code Explanation

This document provides a detailed explanation of the Arduino binary counter code. The code implements an 8-bit binary counter with up, down, and reset buttons, displaying the counter value on 8 LEDs.

## Pin Definitions and Global Variables

```c
const int ledPins[] = {12, 11, 10, 5, 6, 7, 8, 9};
const int numLEDs = 8;
```
- Defines an array of 8 pins (12, 11, 10, 5, 6, 7, 8, 9) that connect to LEDs
- `numLEDs` specifies the total number of LEDs (8)

```c
const int upButtonPin = 2;
const int downButtonPin = 3;
const int resetButtonPin = 4;
```
- Defines pins for three buttons:
  - Pin 2: Button to increment the counter
  - Pin 3: Button to decrement the counter
  - Pin 4: Button to reset the counter to zero

```c
int upLastState = LOW, downLastState = LOW, resetLastState = LOW;
```
- Tracks the previous state of each button
- Initialized to LOW (not pressed)
- Used to detect button state changes

```c
int counter = 0;
```
- Declares and initializes the main counter variable to 0
- This variable will store a value between 0 and 255 (8-bit)

## Setup Function

```c
void setup() {
    Serial.begin(9600);
    Serial.println("System Initialized...");
```
- Initializes serial communication at 9600 baud rate
- Prints an initialization message to the Serial Monitor

```c
    for (int i = 0; i < numLEDs; i++) pinMode(ledPins[i], OUTPUT);
    pinMode(upButtonPin, INPUT);
    pinMode(downButtonPin, INPUT);
    pinMode(resetButtonPin, INPUT);
```
- Loops through each LED pin (0 to 7) and sets them as OUTPUT
- Sets the three button pins as INPUT

```c
    updateLEDs(counter);
}
```
- Calls the `updateLEDs()` function with the initial counter value (0)
- Updates the LED display to match the counter's value

## Loop Function

```c
void loop() {
    int upReading = digitalRead(upButtonPin);
    int downReading = digitalRead(downButtonPin);
    int resetReading = digitalRead(resetButtonPin);
```
- Reads the current state of all three buttons
- Each variable will be either HIGH (button pressed) or LOW (button not pressed)

### Button Logic

#### Up Button Logic

```c
    if (upReading && !upLastState) {
        counter = (counter + 1) % 256;
        Serial.print("UP: "); Serial.println(counter);
        updateLEDs(counter);
    }
```
- Detects a rising edge on the up button (button was just pressed)
- Increments the counter by 1, using modulo 256 to wrap around from 255 to 0
- Prints the new counter value to the Serial Monitor
- Updates the LED display

#### Down Button Logic

```c
    else if (downReading && !downLastState) {
        counter = (counter - 1 + 256) % 256;
        Serial.print("DOWN: "); Serial.println(counter);
        updateLEDs(counter);
    }
```
- Detects a rising edge on the down button
- Decrements the counter by 1, adding 256 before the modulo operation ensures proper wrapping from 0 to 255
- Prints the new counter value to the Serial Monitor
- Updates the LED display

#### Reset Button Logic

```c
    else if (resetReading && !resetLastState) {
        counter = 0;
        Serial.println("RESET");
        updateLEDs(counter);
    }
```
- Detects a rising edge on the reset button
- Sets the counter to 0
- Prints a reset message to the Serial Monitor
- Updates the LED display

#### Update Button States

```c
    upLastState = upReading;
    downLastState = downReading;
    resetLastState = resetReading;

    delay(200); // Debounce delay
}
```
- Stores the current button state for the next comparison
- Adds a 200ms delay for button debouncing

## LED Update Function

```c
void updateLEDs(int value) {
    Serial.print("LEDs: ");
```
- Defines a function that takes an integer value (the counter)
- Begins printing an LED update message

```c
    for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], (value >> i) & 1);
        Serial.print((value >> i) & 1);
    }
```
- Loops through each of the 8 LED positions
- Extracts the bit value at position `i` from the counter value:
  - `value >> i` shifts the bits to the right by `i` positions
  - `& 1` performs a bitwise AND with 1 to isolate the least significant bit
  - Result is either 0 or 1
- Sets the corresponding LED pin HIGH (1) or LOW (0)
- Prints the bit value to the Serial Monitor

```c
    Serial.println();
}
```
- Prints a newline character to end the LED update line in the Serial Monitor

## Summary

This code implements an 8-bit binary counter (0-255) with the following functionality:
1. Press the up button to increment the counter
2. Press the down button to decrement the counter
3. Press the reset button to set the counter back to 0
4. The counter value is displayed in binary on the 8 LEDs
5. All actions are logged to the Serial Monitor

The LEDs represent the binary digits of the counter value, with each LED corresponding to a power of 2 (1, 2, 4, 8, 16, 32, 64, 128).

