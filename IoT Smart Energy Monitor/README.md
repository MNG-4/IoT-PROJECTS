# IoT-Smart-Energy-Monitor

An instrumentation and telemetry monitoring platform engineered to capture alternating current (AC) metrics at the edge and track performance metrics via an uncoupled edge gateway analytical pipeline.

---

## ⚡ Power System Instrumentation Theory

The system uses standard alternating current instrumentation frameworks to capture active loading metrics safely:

* **Non-Invasive Sensing:** Utilizes split-core Current Transformer (CT) sensors (`SCT-013-000`) to extract galvanic isolation safety monitoring across live system loads without severing primary conductor paths.
* **Root-Mean-Square (RMS) Conversion:** The edge system samples the continuous alternating voltage curve at high frequencies, running mathematical root-mean-square transformations over full $50\text{Hz}$ sinusoidal wave patterns to derive true operating current ($I_{RMS}$).
* **Active Power Integration ($P = V \times I$):** Telemetry packets integrate real-time current against nominal voltage specifications to continuously compute true instantaneous system load demands.

---

## 📂 Repository Architecture

* **`energy_monitor_node.ino`**: Microcontroller firmware configuration written in C++ that processes analog current signals, handles mathematical calibrations, and outputs payload telemetry metrics across local interfaces.
* **`power_analytics_engine.py`**: A python-based gateway monitoring system that ingests load metrics, integrates demand tracking loops over active windows to compile Kilowatt-hour ($\text{kWh}$) consumption metrics, and triggers automatic load-shedding warning notifications.

---

## 🛠️ Environmental Requirements

### Edge Layer Components
* **Hardware Profile:** ESP32, ESP8266, or equivalent ADC microcontrollers.
* **Core Libraries:** `WiFi.h`, `PubSubClient.h`

### Application Analytics Core
* **Language Runtime:** Python 3.8+
* **System Tools:** JSON processing protocols, runtime scheduling systems.
