# 🌡️ IoT Climate Monitoring System

[cite_start]**IoT Climate Monitoring System** is a smart device designed to monitor temperature and humidity in real-time, specifically optimized for classroom environments to ensure comfort and productivity[cite: 56]. [cite_start]This system utilizes the **ESP8266 NodeMCU** microcontroller, **DHT11** sensor, and integrates with **Firebase** for data logging and **Blynk** for remote monitoring and notifications[cite: 300, 303].

## 📋 Table of Contents
- [Overview](#-overview)
- [Key Features](#-key-features)
- [Hardware Architecture](#-hardware-architecture)
- [Software & Libraries](#-software--libraries)
- [Pin Mapping](#-pin-mapping)
- [How It Works](#-how-it-works)
- [Team Members](#-team-members)

## 📖 Overview
Maintaining an optimal learning environment is crucial for concentration. [cite_start]This project solves the issue of uncontrolled classroom temperatures by providing an efficient, practical, and affordable monitoring system[cite: 56]. 

[cite_start]The system reads environmental data, displays it locally on an LCD, provides visual LED alerts, logs historical data to a cloud database (Firebase), and sends critical alerts to users via smartphones (Blynk)[cite: 64, 66, 305].

## ✨ Key Features
* [cite_start]**Real-time Monitoring:** Measures temperature and humidity continuously using the DHT11 sensor[cite: 290].
* [cite_start]**Visual Indicators:** * 🟢 **Green LED:** Temperature is normal (< 28°C)[cite: 137].
    * [cite_start]🟡 **Yellow/Red LED:** Temperature is high (> 28°C)[cite: 138, 251].
* **Interactive LCD Display:** * Default: Shows current temperature & humidity.
    * Button Press 1x: Shows **Maximum** recorded values.
    * [cite_start]Button Press 2x: Shows **Minimum** recorded values[cite: 298, 299].
* [cite_start]**Cloud Data Logging:** Automatically uploads sensor data to **Google Firebase** every minute for historical analysis[cite: 254, 255].
* [cite_start]**Remote Monitoring & Alerts:** Integrates with **Blynk** to view data via mobile app/web and sends notifications (Email/Push) if the temperature exceeds 28°C[cite: 305, 346].
* [cite_start]**Daily Reset Logic:** Automatically resets Max/Min values when a new day is detected via NTP Server[cite: 242].

## 🛠️ Hardware Architecture
[cite_start]The project is built using the following components [cite: 116-148]:
* **Microcontroller:** NodeMCU ESP8266 (Wi-Fi enabled).
* **Sensor:** DHT11 (Temperature & Humidity).
* **Display:** LCD 16x2 with I2C Module.
* **Indicators:** LED (Green and Yellow/Red) + Resistors (220Ω).
* **Input:** Push Button.
* **Connectivity:** Breadboard & Jumper Wires.

## 💻 Software & Libraries
The firmware is written in C++ using the Arduino IDE. [cite_start]Required libraries include[cite: 212]:
* `ESP8266WiFi.h`
* `BlynkSimpleEsp8266.h`
* `DHT.h`
* `LiquidCrystal_I2C.h`
* `FirebaseESP8266.h`
* `NTPClient.h` & `WiFiUdp.h`

## 🔌 Pin Mapping
[cite_start]Based on the implementation code and circuit design [cite: 180-184, 221-224]:

| Component | ESP8266 Pin | Notes |
| :--- | :--- | :--- |
| **DHT11 Data** | D4 (GPIO 2) | Sensor Input |
| **Push Button** | D3 | Input for LCD Control |
| **Yellow LED** | D7 | High Temp Indicator |
| **Green LED** | D8 | Normal Temp Indicator |
| **LCD SCL** | D1 | I2C Clock |
| **LCD SDA** | D0 | I2C Data |

## ⚙️ How It Works
1.  [cite_start]**Initialization:** The system connects to Wi-Fi, Firebase, Blynk, and syncs time via NTP Server [cite: 231-236].
2.  [cite_start]**Sensing:** The DHT11 sensor reads temperature and humidity every 2 seconds[cite: 244].
3.  **Processing:** * The ESP8266 checks if the temperature exceeds the threshold (28°C).
    * [cite_start]It updates the Max/Min values stored in the system[cite: 253].
4.  **Output:**
    * [cite_start]**LEDs:** Switch between Green/Yellow based on the threshold[cite: 251].
    * [cite_start]**LCD:** Displays data based on button interaction[cite: 248].
    * [cite_start]**Cloud:** Uploads JSON data to Firebase every 60 seconds[cite: 254].
    * [cite_start]**Blynk:** Pushes data to Virtual Pins (V0-V5) and triggers alerts if necessary[cite: 256].

## 👥 Team Members
[cite_start]This project was developed by **Kelompok 8**[cite: 5]:
1.  **Jonathan Sidebang** (Technical Writer & QA)
2.  **Baskoro Jatmiko Adi Raharjo** (Electrical & Software Engineer)
3.  **Husna Nurul Farida** (Technical Writer & Data Analyst)
4.  **Alvin Oktavian Surya Saputra** (Electrical/Software Engineer & Design)
5.  **Luthfie Yannuardy** (Project Manager & Full Stack Engineer)

---
[cite_start]*Developed for the Embedded Systems Course, Department of Electrical Engineering and Informatics, Vocational College, Universitas Gadjah Mada (2024).* [cite: 6-9]
