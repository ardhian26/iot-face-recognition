#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Dinas Kominfo 2.4GHz";
const char* password = "17kominfo";
const char* serverName = "http://10.10.111.191/motion_sim/detect.php";

const int pirPin = 27;

bool motionDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long interval = 5000; // 5 detik

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT_PULLDOWN);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  int pirState = digitalRead(pirPin);
  unsigned long currentMillis = millis();

  if (pirState == HIGH &&
      !motionDetected &&
      (currentMillis - lastTriggerTime >= interval)) {

    motionDetected = true;
    lastTriggerTime = currentMillis;

    Serial.println("Gerakan terdeteksi!");

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST("pir=1");
    http.end();
  }

  if (pirState == LOW) {
    motionDetected = false;
  }
}
