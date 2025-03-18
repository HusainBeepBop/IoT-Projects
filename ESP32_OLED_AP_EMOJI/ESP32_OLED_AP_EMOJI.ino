#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WebServer server(80);
String message = "";

const unsigned char heart_bitmap[] PROGMEM = {
    0b00001100, 0b00110000,
    0b00111110, 0b01111100,
    0b01111111, 0b11111110,
    0b11111111, 0b11111111,
    0b11111111, 0b11111111,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,
    0b00011111, 0b11111000,
    0b00001111, 0b11110000,
    0b00000111, 0b11100000,
    0b00000011, 0b11000000,
    0b00000001, 0b10000000
};

const unsigned char star_bitmap[] PROGMEM = {
    0b00000100, 0b00000000,
    0b00000100, 0b00000000,
    0b00010101, 0b01000000,
    0b00001110, 0b00000000,
    0b00011111, 0b10000000,
    0b00111111, 0b11000000,
    0b01111111, 0b11100000,
    0b00001110, 0b00000000,
    0b00001110, 0b00000000,
    0b00000100, 0b00000000
};

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 OLED</title>
</head>
<body>
    <h2>Send Text or Emoji:</h2>
    <form action="/send" method="POST">
        <input type="text" name="text">
        <input type="submit" value="Send">
    </form>
    <br>
    <form action="/emoji" method="POST">
        <button name="emoji" value="heart">❤️ Heart</button>
        <button name="emoji" value="star">⭐ Star</button>
    </form>
</body>
</html>
)rawliteral";

void displayText(String text) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.print(text);
    display.display();
}

void displayBitmap(const unsigned char *bitmap, int width, int height) {
    display.clearDisplay();
    display.drawBitmap((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2, bitmap, width, height, WHITE);
    display.display();
}

void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

void handleTextInput() {
    if (server.hasArg("text")) {
        message = server.arg("text");
        displayText(message);
        server.send(200, "text/html", "<h2>Text Updated!</h2><a href='/'>Go Back</a>");
    } else {
        server.send(400, "text/plain", "Bad Request");
    }
}

void handleEmojiInput() {
    if (server.hasArg("emoji")) {
        String emoji = server.arg("emoji");
        if (emoji == "heart") {
            displayBitmap(heart_bitmap, 16, 11);
        } else if (emoji == "star") {
            displayBitmap(star_bitmap, 16, 10);
        }
        server.send(200, "text/html", "<h2>Emoji Updated!</h2><a href='/'>Go Back</a>");
    }
}

void setup() {
    Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    displayText("Ready");

    WiFi.softAP("ESP_OLED_AP", "12345678");
    Serial.println("Access Point Started");

    server.on("/", handleRoot);
    server.on("/send", HTTP_POST, handleTextInput);
    server.on("/emoji", HTTP_POST, handleEmojiInput);
    server.begin();
    Serial.println("Server started.");
}

void loop() {
    server.handleClient();
}
