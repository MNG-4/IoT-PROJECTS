#include <WiFi.h>
#include <PubSubClient.h>

// --- Network & Gateway Settings ---
const char* ssid     = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
const char* brokerUser = "power_gateway_admin";
const char* brokerPass = "secure_energy_pass";

#define CURRENT_SENSOR_PIN 34  // ESP32 Analog Pin connected to CT Sensor
const float MAINS_VOLTAGE  = 230.0; // Nominal grid voltage (Kenya standard)
const float CALIBRATION_FACTOR = 30.0; // Calibration coefficient for SCT-013-000

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
  setup_wifi();
}

void setup_wifi() {
  delay(10);
  Serial.println("\nInitializing grid-tie Wi-Fi subsystem...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Link Established.");
}

float calculate_RMS_Current() {
  int readValue;
  int maxValue = 0;
  int minValue = 4095;
  
  uint32_t start_time = millis();
  // Sample for 200ms to span multiple 50Hz AC cycles perfectly
  while((millis() - start_time) < 200) {
    readValue = analogRead(CURRENT_SENSOR_PIN);
    if (readValue > maxValue) maxValue = readValue;
    if (readValue < minValue) minValue = readValue;
  }
  
  // Calculate Peak-to-Peak Voltage from the ADC values
  float voltagePeakToPeak = ((maxValue - minValue) * 3.3) / 4095.0;
  // Convert Peak-to-Peak to Root-Mean-Square (RMS) Current
  float currentRMS = (voltagePeakToPeak / 2.0) * 0.707 * CALIBRATION_FACTOR;
  
  return currentRMS;
}

void loop() {
  if (!client.connected()) {
    // Basic MQTT reconnection pattern logic
    if (client.connect("ESP32_Power_Meter", brokerUser, brokerPass)) {
      Serial.println("Telemetry link active.");
    } else {
      delay(5000);
      return;
    }
  }
  client.loop();

  // Power System Instrumentation calculations
  float current = calculate_RMS_Current();
  if (current < 0.08) current = 0.0; // Noise threshold floor cutoff
  float activePower = MAINS_VOLTAGE * current; // P = V * I

  // Generate payload string
  String payload = "{\"voltage\":" + String(MAINS_VOLTAGE) + 
                   ",\"current\":" + String(current, 3) + 
                   ",\"active_power\":" + String(activePower, 1) + "}";

  client.publish("grid/meter/billing_node_01", payload.c_str());
  Serial.println("Data Dispatched: " + payload);
  delay(3000); // Send telemetry updates every 3 seconds
}
