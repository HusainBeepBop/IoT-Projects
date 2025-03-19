#include <WiFi.h>

// Define struct for Wi-Fi credentials
struct WiFiCredential {
  const char* ssid;
  const char* password;
};

// Add your Wi-Fi networks here
WiFiCredential wifiList[] = {
  {"RHJM 5G", "donottry@12345"},
    {"Google Pixel 8", "easyentry"},
    {"RHJM 4G", "donottry@12345"},
    {"HT Nanpura", "123@@123"},
    {"Hatimi Relay", "hatimi@5253"}
};

const int numNetworks = sizeof(wifiList) / sizeof(wifiList[0]);

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost. Reconnecting...");
    connectToWiFi();
  }
  delay(5000); // Check connection every 5 seconds
}

void connectToWiFi() {
  for (int i = 0; i < numNetworks; i++) {
    Serial.print("Connecting to: ");
    Serial.println(wifiList[i].ssid);

    WiFi.begin(wifiList[i].ssid, wifiList[i].password);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 10) {
      delay(1000);
      Serial.print(".");
      attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to Wi-Fi!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return;
    }
    Serial.println("\nFailed to connect. Trying next network...");
  }
  Serial.println("Could not connect to any Wi-Fi. Retrying in 10 seconds...");
  delay(10000);
}
