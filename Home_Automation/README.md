# Edge-IoT-Home-Automation

An event-driven, hardware-agnostic Smart Home Automation subsystem implementing local edge telemetry analytics, parameter-controlled automated feedback triggers, and network communication handling.

---

## 🏗️ System Architecture

The ecosystem relies on an uncoupled architecture linking thin firmware edge clients directly to a central edge coordination gateway engine:

```text
┌─────────────────┐             Network Protocol            ┌──────────────────┐
│   ESP32 Edge    │ ───────── Telemetry Stream ───────────> │ Central Gateway  │
│  Sensor Cluster │ <──────── Control Commands ──────────── │   Rule Engine    │
└─────────────────┘                                         └──────────────────┘
