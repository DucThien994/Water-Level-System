# 💧 IoT-based Dual-Core Smart Water Level Monitoring & Control System

An embedded IoT system built on the **ESP32 Dual-Core SoC** designed for real-time liquid level monitoring, automated threshold alerting, and remote telemetry dashboard over local Wi-Fi networks.

[![Watch the Demo Video](https://img.shields.io/badge/YouTube-Demo%20Video-red?logo=youtube)](https://www.youtube.com/watch?v=W6BsUYRWWLE)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)

---

## 📌 Project Overview

This project provides an end-to-end embedded IoT solution for monitoring water tanks and industrial reservoirs. Utilizing the dual-core capability of the **ESP32**, time-critical tasks (ultrasonic measurement, digital signal filtering, local OLED rendering, alarm actuation) are isolated from network-heavy operations (Wi-Fi management, time synchronization, asynchronous AJAX HTTP Web Server).

---

## 🚀 Key Features

* **Multi-Threaded Architecture (FreeRTOS Dual-Core)**:
  * **Core 0**: Dedicated background task for Wi-Fi connection health, auto-reconnection logic, RSSI tracking, and RTC network time synchronization.
  * **Core 1**: Real-time ultrasonic pulse timing, outlier filtering, threshold detection, OLED screen update, and HTTP client request handling.
* **Non-Blocking Embedded Web Server**:
  * Built-in HTTP server hosting a responsive web dashboard with live visual status indicators.
  * Real-time telemetry updates using asynchronous **AJAX polling (XMLHttpRequest)** without reloading the webpage.
* **Robust Signal Processing**:
  * 40ms microsecond-level timeout protection preventing system lockups on lost ultrasonic echoes.
  * Noise rejection and outlier filtering algorithm to eliminate water surface disturbance anomalies.
* **Local Real-Time Dashboard**:
  * High-speed I2C communication (400kHz Fast-Mode) with SSD1306 0.96" OLED display.
  * Real-time display of water height (cm), percentage (%), Wi-Fi RSSI strength, and timestamp.
* **Actuation & Safety Control**:
  * Transistor/Relay switching for pump/valve control with automatic HIGH/LOW overflow/dry-run alarms.

---

## 🛠️ Hardware & Pinout Configuration

| Component | Interface / Pin | ESP32 GPIO | Description |
| :--- | :--- | :--- | :--- |
| **Ultrasonic Sensor (JSN-SR04M / HC-SR04)** | `TRIG` | `GPIO 5` | Trigger ultrasonic pulse (10µs pulse) |
| | `ECHO` | `GPIO 18` | Echo time measurement |
| **OLED Display (SSD1306 128x64)** | `SDA` | `GPIO 33` | I2C Data line (400 kHz) |
| | `SCL` | `GPIO 32` | I2C Clock line (400 kHz) |
| **Alert LED / Buzzer** | `Digital Out` | `GPIO 2` | Visual / Acoustic alarm indicator |
| **Actuator / Pump Driver (Transistor)** | `Base` | `GPIO 13` | High-current load switching |

---

## 📂 Software Architecture & OOP Structure

The codebase is organized following Object-Oriented Programming (OOP) principles:

```
Water-Level-System/
├── main.cpp          # System initialization, FreeRTOS tasks, and main execution loop
├── main.h            # Pin definitions, system parameters, and global configs
├── Ultrasonic.cpp/.h # Ultrasonic driver with noise filtering & timeout safeguards
├── WIFI.cpp/.h       # Wi-Fi connection manager & RSSI calculation
├── Website.cpp/.h    # Web server handlers & dynamic HTML/AJAX generation
├── RTC.cpp/.h        # Real-time clock & NTP time synchronization
├── oled.cpp/.h       # OLED SSD1306 I2C display UI engine
└── README.md         # Project documentation
```

---

## 🌐 Web Telemetry Dashboard

The embedded HTTP server delivers a web dashboard accessible from any browser on the local network:
* **Endpoint `/`**: Serves responsive HTML/CSS dashboard with live metric cards.
* **Endpoint `/readings`**: Returns real-time CSV telemetry data (`[WaterLevel%],[LED_State],[Status]`) consumed by client-side JavaScript.

---

## 🎥 Video Demonstration

Click the link below to watch the physical prototype in action:
* 📺 **[YouTube Demo: Smart Water Level Monitoring System](https://www.youtube.com/watch?v=W6BsUYRWWLE)**
