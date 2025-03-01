#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Wi-Fi Credentials
const char* ssid = "Zahid";
const char* password = "12345678";

// LED Pin (GPIO2 is the built-in LED on many ESP32 boards)
const int ledPin = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// HTML as a raw string literal
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Web Server</title>
  <style>
    body { text-align: center; font-family: Arial; }
    button { padding: 10px 20px; font-size: 18px; margin: 5px; }
  </style>
</head>
<body>
  <h2>ESP32 LED Control</h2>
  <p><button onclick="toggleLED('on')">Turn ON</button></p>
  <p><button onclick="toggleLED('off')">Turn OFF</button></p>
  <p id="status">LED is OFF</p>

  <script>
    function toggleLED(state) {
      fetch(`/${state}`)
        .then(response => response.text())
        .then(text => {
          document.getElementById("status").innerText = text;
        });
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Initialize LED off

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Define server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", htmlPage);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, HIGH);
    request->send(200, "text/plain", "LED is ON");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, LOW);
    request->send(200, "text/plain", "LED is OFF");
  });

  // Start server
  server.begin();
}

void loop() {
  // Async server handles requests automatically
}