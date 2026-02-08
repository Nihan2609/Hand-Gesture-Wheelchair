# Hand Gesture Controlled Wheelchair

A smart wheelchair system controlled using hand gestures.  
The system uses **two ESP32 boards** communicating via **ESP-NOW**.

## 🔧 System Overview

- **Hand Unit**
  - ESP32
  - MPU6050 (gesture detection)
  - Sends X/Y gesture data wirelessly

- **Chair Unit**
  - ESP32
  - L298N motor driver
  - Ultrasonic sensor (obstacle detection)
  - Buzzer (alert)
  - Receives gesture data and controls motors

## 🚀 Features

- Hand gesture based movement
- Wireless communication using ESP-NOW
- Obstacle detection with automatic stop
- Tilt detection for safety
- Buzzer alert on obstacle or tilt
- Signal loss protection (auto stop)

## 📂 Project Structure

Hand-Gesture-Wheelchair/
│
├── chair_unit/
│ └── chair_unit.ino
│
├── hand_unit/
│ └── hand_unit.ino
│
└── README.md

markdown
Copy code

## 🛠 Hardware Used

- ESP32 (2x)
- MPU6050
- L298N Motor Driver
- Ultrasonic Sensor (HC-SR04)
- DC Motors
- Buzzer
- Battery / Power Bank

## 📡 Communication

- ESP-NOW (ESP32 ↔ ESP32)
- No internet required

## ▶️ How to Run

1. Upload `hand_unit.ino` to the hand ESP32
2. Upload `chair_unit.ino` to the chair ESP32
3. Power both units
4. Control wheelchair using hand gestures

## 📌 Notes

- Chair unit code does not require modification
- Hand unit can be extended with Bluetooth or mobile notifications

## 👤 Author

**AL-FARHAN NIHAN**

Computer Science Student
