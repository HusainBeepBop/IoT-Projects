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
String message = "Hello!";

// Define your Access Point (AP) credentials
const char* apSSID = "ESP_OLED_AP";
const char* apPassword = "12345678";

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 OLED Display</title>
</head>
<body>
    <h2>Enter Text to Display:</h2>
    <form action="/send" method="POST">
        <input type="text" name="text">
        <input type="submit" value="Send">
    </form>
</body>
</html>
)rawliteral";

// Function to display text on OLED
void displayText(String text, int delayTime = 3000) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print(text);
    display.display();
    delay(delayTime);
}

// Function to start ESP32 in Access Point (AP) mode
void startAccessPoint() {
    Serial.println("Starting Access Point...");
    WiFi.softAP(apSSID, apPassword);
    IPAddress myIP = WiFi.softAPIP();
    
    Serial.print("AP Mode Active. Connect to: ");
    Serial.println(apSSID);
    Serial.print("Go to: ");
    Serial.println(myIP);

    displayText("AP Mode Active", 2000);
    displayText(apSSID, 3000);
    displayText("Go to: " + myIP.toString(), 5000);
}

// Handle root page
void handleRoot() {
    Serial.println("Handling root request...");
    server.send(200, "text/html", htmlPage);
}

// Handle text input
void handleTextInput() {
    if (server.hasArg("text")) {
        message = server.arg("text");
        Serial.print("Received message: ");
        Serial.println(message);
        displayText(message);
        server.send(200, "text/html", "<h2>Message Updated!</h2><a href='/'>Go Back</a>");
    } else {
        Serial.println("No text received.");
        server.send(400, "text/plain", "Bad Request");
    }
}

void setup() {
    Serial.begin(9600);

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    displayText("Starting...", 2000);

    // Always start in AP mode
    startAccessPoint();

    // Start web server
    server.on("/", handleRoot);
    server.on("/send", HTTP_POST, handleTextInput);
    server.begin();
    Serial.println("Server started.");
}

void loop() {
    server.handleClient();
}
