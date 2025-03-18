#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* wifiList[][2] = {
    {"RHJM 5G", "donottry@12345"},
    {"Google Pixel 8", "easyentry"},
    {"RHJM 4G", "donottry@12345"},
    {"HT Nanpura", "123@@123"},
    {"Hatimi Relay", "hatimi@5253"}
};
const int numNetworks = sizeof(wifiList) / sizeof(wifiList[0]);

WebServer server(80);
String message = "Hello!";
String connectedSSID = "";  // Store the WiFi name

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

// Display text on OLED
void displayText(String text, int delayTime = 3000) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print(text);
    display.display();
    delay(delayTime);
}

// Connect to WiFi
bool connectToWiFi() {
    Serial.println("Scanning WiFi networks...");
    int numNetworksFound = WiFi.scanNetworks();
    if (numNetworksFound == 0) {
        Serial.println("No WiFi networks found.");
        return false;
    }

    for (int i = 0; i < numNetworks; i++) {
        for (int j = 0; j < numNetworksFound; j++) {
            if (WiFi.SSID(j) == wifiList[i][0]) {
                Serial.print("Trying to connect to ");
                Serial.println(wifiList[i][0]);
                WiFi.begin(wifiList[i][0], wifiList[i][1]);

                int timeout = 40;
                while (WiFi.status() != WL_CONNECTED && timeout-- > 0) {
                    delay(500);
                    Serial.print(".");
                }

                Serial.println();
                if (WiFi.status() == WL_CONNECTED) {
                    connectedSSID = wifiList[i][0];  // Store the connected SSID

                    Serial.println("Connected to WiFi!");
                    Serial.print("WiFi Name: ");
                    Serial.println(connectedSSID);
                    Serial.print("IP Address: ");
                    Serial.println(WiFi.localIP());

                    // Show WiFi name on OLED
                    displayText("Connected to:", 2000);
                    displayText(connectedSSID, 3000);
                    displayText("IP: " + WiFi.localIP().toString(), 5000);
                    return true;
                } else {
                    Serial.println("Failed to connect to " + String(wifiList[i][0]));
                }
            }
        }
    }
    return false;
}

// Start AP Mode
void startAccessPoint() {
    Serial.println("Starting Access Point...");
    WiFi.softAP("ESP_OLED_AP", "12345678");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP Mode started. Connect to 'ESP_OLED_AP' and visit: ");
    Serial.println(myIP);

    displayText("AP Mode Active", 2000);
    displayText("ESP_OLED_AP", 3000);
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

    if (!connectToWiFi()) {
        startAccessPoint();
    }

    // Start server
    server.on("/", handleRoot);
    server.on("/send", HTTP_POST, handleTextInput);
    server.begin();
    Serial.println("Server started.");
}

void loop() {
    server.handleClient();
}
