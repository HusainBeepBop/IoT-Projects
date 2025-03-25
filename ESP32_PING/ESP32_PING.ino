#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi credentials
struct WiFiCredential {
  const char* ssid;
  const char* password;
};

WiFiCredential wifiList[] = {
  {"ssid", "password"},
};

const int numNetworks = sizeof(wifiList) / sizeof(wifiList[0]);

// MQTT server details
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopic = "topic_name";
WiFiClient espClient;
PubSubClient client(espClient);

// Pin Definitions
const int buttonPin = 32;
const int ledPin = 2;
const int vibrationMotorPin = 4;
const char* deviceName = "name_ESP";
const char* ignoredDevices[] = {"name_ESP", }; // Add more devices as needed

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationMotorPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialization failed");
    while (true);
  }
  display.clearDisplay();
  display.display();

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  if (digitalRead(buttonPin) == HIGH) {
    Serial.println("Button Pressed! Sending Ping...");
    displayMessage("Button Pressed!\nSending Ping...");
    String msg = String("PING from ") + deviceName;
    client.publish(mqttTopic, msg.c_str());
    heartbeatEffect();
  }
}

void connectToWiFi() {
  for (int i = 0; i < numNetworks; i++) {
    Serial.print("Connecting to: ");
    Serial.println(wifiList[i].ssid);
    displayMessage((String("Connecting to: ") + wifiList[i].ssid).c_str());
    WiFi.begin(wifiList[i].ssid, wifiList[i].password);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 10) {
      delay(1000);
      Serial.print(".");
      attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to Wi-Fi!");
      displayMessage("Connected to Wi-Fi!");
      return;
    }
    Serial.println("\nFailed to connect. Trying next network...");
  }
  Serial.println("Could not connect to any Wi-Fi. Restarting in 10 seconds...");
  displayMessage("Wi-Fi Failed! Restarting...");
  delay(10000);
  ESP.restart();
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    displayMessage("Connecting to MQTT...");
    if (client.connect(String("ESP32_Client_" + String(random(0, 1000))).c_str())) {
      Serial.println("Connected to MQTT!");
      displayMessage("Connected to MQTT!");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      displayMessage("MQTT Failed! Retrying...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  for (const char* ignoredDevice : ignoredDevices) {
    if (message.indexOf(ignoredDevice) != -1) {
      Serial.println("Ignored message from: " + String(ignoredDevice));
      return;
    }
  }

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(message);
  displayMessage((String("From MQTT:\n") + message).c_str());

  heartbeatEffect();
}

void displayMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}

void heartbeatEffect() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(vibrationMotorPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    digitalWrite(vibrationMotorPin, LOW);
    delay(300);
    digitalWrite(ledPin, HIGH);
    digitalWrite(vibrationMotorPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    digitalWrite(vibrationMotorPin, LOW);
    delay(400);
  }
  displayMessage("Waiting for Ping...");
}
