#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Joystick Pins
const int VRX_PIN = 34;
const int VRY_PIN = 35;
const int SW_PIN = 15;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialization failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Initialize Joystick
  pinMode(SW_PIN, INPUT_PULLUP);
}

void loop() {
  // Read Joystick Values
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);
  int buttonState = digitalRead(SW_PIN);

  // Print to Serial Monitor
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Button: ");
  Serial.println(buttonState == HIGH ? "Not Pressed" : "Pressed");

  // Display on OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("X: ");
  display.println(xValue);
  display.print("Y: ");
  display.println(yValue);
  display.print("Button: ");
  display.println(buttonState == HIGH ? "Not Pressed" : "Pressed");
  display.display();

  delay(100);
}
