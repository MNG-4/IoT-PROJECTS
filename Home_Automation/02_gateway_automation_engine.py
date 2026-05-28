import json
import time
from datetime import datetime

class HomeAutomationGateway:
    def __init__(self):
        self.device_registry = {}
        self.automation_rules_enabled = True

    def process_incoming_telemetry(self, topic: str, raw_payload: str):
        """Processes sensor logs arriving from edge hardware nodes."""
        try:
            data = json.loads(raw_payload)
            room = topic.split("/")[-1]
            
            # Enrich data packets with metadata timestamps
            data["timestamp"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            self.device_registry[room] = data
            
            print(f"📥 [Telemetry Received] Room: {room.upper()} | Data: {data}")
            
            # Trigger Rule Engine Evaluation
            if self.automation_rules_enabled:
                self.evaluate_rules(room, data)
                
        except json.JSONDecodeError:
            print(f"❌ Failed to parse malformed payload on topic: {topic}")

    def evaluate_rules(self, room: str, metrics: dict):
        """Rule engine simulating localized automation logic conditions."""
        # Rule 1: Energy Saver Mode (Auto-shutoff on vacancy)
        if metrics.get("motion") == 0:
            print(f"💡 [Rule Triggered] No occupancy detected in {room}. Issuing power-saving shutdown instruction to relay module.")
            self.send_actuator_command(f"home/actuators/relay", "OFF")
            
        # Rule 2: High Temperature HVAC Threshold Automation
        if metrics.get("temperature", 0) > 26.0:
            print(f"🚨 [Rule Triggered] Critical Temperature threshold exceeded ({metrics['temperature']}°C) in {room}. Initializing climate control overrides.")
            self.send_actuator_command(f"home/actuators/relay", "ON")

    def send_actuator_command(self, topic: str, command: str):
        """Simulates outgoing control frames dispatching back out over network channels."""
        print(f"📤 [Control Dispatch] Channel: {topic} ----> Target State: [{command}]")


if __name__ == "__main__":
    print("🚀 Initializing Local Edge Gateway Core Automation Engine...")
    gateway = HomeAutomationGateway()
    
    # Simulating a live telemetry stream loop over network connections
    mock_stream = [
        ('home/sensors/livingroom', '{"temperature": 23.4, "humidity": 58.2, "motion": 1}'),
        ('home/sensors/livingroom', '{"temperature": 27.1, "humidity": 62.0, "motion": 1}'),
        ('home/sensors/livingroom', '{"temperature": 24.0, "humidity": 55.4, "motion": 0}'),
    ]
    
    for topic, sample in mock_stream:
        time.sleep(2)
        gateway.process_incoming_telemetry(topic, sample)
