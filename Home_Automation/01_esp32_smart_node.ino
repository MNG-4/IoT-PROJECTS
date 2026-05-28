#include <WiFi.h>
#include <PubSubClient.h>

// --- Network Configuration ---
const char* ssid     = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
const char* mqttUser = "iot_gateway_user";
const char* mqttPass = "secure_gateway_password";

// GPIO Pin Definitions
#define RELAY_PIN    26
#define DHT_PIN      4
#define MOTION_PIN   27

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with appliance turned off

  setup_wifi();
}

void setup_wifi() {
  delay(10);
  Serial.println("\nConnecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected successfully.");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt connection with a fallback unique client ID
    if (client.connect("ESP32_Home_Node", mqttUser, mqttPass)) {
      Serial.println("connected.");
      client.subscribe("home/actuators/relay");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Mock Sensor Readings (Simulating hardware values)
  float temperature = 24.5 + (random(-10, 10) / 10.0);
  float humidity    = 60.0 + (random(-20, 20) / 10.0);
  int motion        = digitalRead(MOTION_PIN);

  // Payload Construction
  String payload = "{\"temperature\":" + String(temperature) + 
                   ",\"humidity\":" + String(humidity) + 
                   ",\"motion\":" + String(motion) + "}";

  client.publish("home/sensors/livingroom", payload.c_str());
  delay(5000); // Send telemetry updates every 5 seconds
}
