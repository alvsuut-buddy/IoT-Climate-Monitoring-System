# 🌡️ IoT Climate Monitoring System

IoT Climate Monitoring System is a smart device designed to monitor temperature and humidity in real-time, specifically optimized for classroom environments to ensure comfort and productivity. This system integrates with Firebase for data logging and Blynk for remote monitoring and notifications.

## 📹 Demo & Documentation
Watch the system in action here:

**[Watch Project Demo Video on YouTube](https://youtu.be/cKLOGbdeinw?si=KpArHYNi9_JWAdfO)**

## Key Features
- Real-time Monitoring: Measures temperature and humidity continuously using the DHT11 sensor.
- Visual Indicators:
  - Green LED: Temperature is normal (< 28 C).
  - Yellow/Red LED: Temperature is high (> 28 C).
- Interactive LCD Display:
  - Default: Shows current temperature & humidity.
  - Button Press 1x: Shows Maximum recorded values.
  - Button Press 2x: Shows Minimum recorded values.
- Cloud Data Logging: Automatically uploads sensor data to Google Firebase every minute for historical analysis.
- Remote Monitoring & Alerts: Integrates with Blynk to view data via mobile app/web and sends notifications (Email/Push) if the temperature exceeds 28 C.
- Daily Reset Logic: Automatically resets Max/Min values when a new day is detected via NTP Server.

## Hardware Architecture
The project is built using the following components:
- NodeMCU ESP8266 (Wi-Fi enabled).
- DHT11 (Sensor Temperature & Humidity).
- LCD 16x2 with I2C Module.
- LED (Green and Yellow/Red)
- Resistors (220 Ohm).
- Push Button.
- Breadboard & Jumper Wires.

## 🔌 Pin Mapping
| Component | ESP8266 Pin |
| :--- | :--- |
| LED Green | D8 |
| LED Green GND | GND |
| LED Red | D7 |
| LED red GND | GND |
| Push Button | D3 |
| Push Button GND | GND |
| LCD SCL | D1 |
| LCD SDA | D0 |
| LCD VDD | 3V3 |
| LCD VSS | GND |
| DHT11 Data | D4 |
| DHT11 VDD | 3V3 |
| DHT11 GND | GND |

## How It Works
1. Initialization: The system connects to Wi-Fi, Firebase, Blynk, and syncs time via NTP Server.
2. Sensing: The DHT11 sensor reads temperature and humidity every 2 seconds.
3. Processing:
   - The ESP8266 checks if the temperature exceeds the threshold (28 C).
   - It updates the Max/Min values stored in the system.
4. Output:
   - LEDs: Switch between Green/Yellow based on the threshold.
   - LCD: Displays data based on button interaction.
   - Cloud: Uploads JSON data to Firebase every 60 seconds.
   - Blynk: Pushes data to Virtual Pins (V0-V5) and triggers alerts if necessary.
