#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "SSID";
const char* password = "PASSWORD";

const int LED_PIN = 2;  // Change if not using the onboard LED
WebServer server(80);

void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Recording started → blink 3 times fast, then stay ON
  server.on("/record/on", []() {
    blinkLED(3, 100);
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/plain", "Recording started");
  });

  // Recording stopped → blink 2 times slow, then OFF
  server.on("/record/off", []() {
    digitalWrite(LED_PIN, LOW);
    blinkLED(2, 300);
    server.send(200, "text/plain", "Recording stopped");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
