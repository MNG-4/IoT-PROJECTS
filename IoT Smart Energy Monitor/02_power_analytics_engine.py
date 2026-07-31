import json
import time
from datetime import datetime

class EnergyAnalyticsEngine:
    def __init__(self):
        self.cumulative_kwh = 0.0
        self.last_timestamp = time.time()
        self.max_load_threshold = 3000.0 # 3kW threshold safety limit

    def ingest_meter_data(self, payload_str: str):
        """Ingests raw digitized power grid data frames."""
        try:
            data = json.loads(payload_str)
            now = time.time()
            time_delta_hours = (now - self.last_timestamp) / 3600.0
            self.last_timestamp = now
            
            power = data.get("active_power", 0.0)
            current = data.get("current", 0.0)
            
            # Integrate power over time to calculate Energy Consumption (Kilowatt-hours)
            consumed_kwh = (power / 1000.0) * time_delta_hours
            self.cumulative_kwh += consumed_kwh
            
            print(f"[{datetime.now().strftime('%H:%M:%S')}] P: {power:.1f}W | I: {current:.2f}A | Total Accumulation: {self.cumulative_kwh:.6f} kWh")
            
            # Power System Safety Rule Enforcement
            if power > self.max_load_threshold:
                print(f"[OVERLOAD ALERT] Demand threat detected! Peak load exceeded safety margins: {power}W")
                
        except json.JSONDecodeError:
            print("Malformed packet structure dropped.")

if __name__ == "__main__":
    print("Activating Grid Energy Telemetry Analytics Processing Suite...")
    engine = EnergyAnalyticsEngine()
    
    # Simulating a live streaming power telemetry loop
    mock_grid_stream = [
        '{"voltage": 230.0, "current": 1.25, "active_power": 287.5}',
        '{"voltage": 230.0, "current": 14.50, "active_power": 3335.0}', # Triggers safety rule
        '{"voltage": 230.0, "current": 0.00, "active_power": 0.0}'
    ]
    
    for telemetry_frame in mock_grid_stream:
        time.sleep(2)
        engine. ingest_meter_data(telemetry_frame)
