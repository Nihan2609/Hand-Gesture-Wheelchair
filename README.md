# Hand Gesture Controlled Wheelchair

A wireless hand gesture-controlled wheelchair system using ESP32 and Arduino, designed for enhanced mobility and independence.

![Project Status](https://img.shields.io/badge/status-active-success.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Circuit Diagrams](#circuit-diagrams)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This project enables wheelchair control through intuitive hand gestures using an MPU6050 accelerometer. The system consists of two main units:

- **Hand Unit**: ESP32 (NodeMCU-32S) with MPU6050 sensor that detects hand tilt
- **Wheelchair Unit**: Arduino Uno with motor control, obstacle detection, and safety features

**Communication**: Bluetooth (HC-05) for wireless control with ~10m range

## ✨ Features

### Gesture Control
- ✅ **Forward**: Tilt hand forward
- ✅ **Backward**: Tilt hand backward
- ✅ **Left Turn**: Tilt hand left
- ✅ **Right Turn**: Tilt hand right
- ✅ **Stop**: Keep hand level

### Safety Features
- 🛡️ **Obstacle Detection**: Ultrasonic sensor stops wheelchair before collision (30cm default)
- 🛡️ **Tilt Protection**: Prevents tipping by monitoring wheelchair angle (30° threshold)
- 🛡️ **Connection Timeout**: Emergency stop if signal lost (1 second timeout)
- 🛡️ **Buzzer Alerts**: Audio feedback for different states

### Additional Features
- 🔋 Battery-powered (3S 18650 Li-ion)
- 🔊 Audio feedback via buzzer
- 📊 Real-time Serial Monitor debugging
- ⚡ Adjustable motor speed and sensitivity

## 🛠️ Hardware Requirements

### Hand Unit
| Component | Quantity | Notes |
|-----------|----------|-------|
| ESP32 (NodeMCU-32S) | 1 | Any ESP32 board works |
| MPU6050 Accelerometer | 1 | I2C 6-axis sensor |
| USB Cable | 1 | For power/programming |

### Wheelchair Unit
| Component | Quantity | Notes |
|-----------|----------|-------|
| Arduino Uno | 1 | Or compatible board |
| HC-05 Bluetooth Module | 1 | Classic Bluetooth |
| L298N Motor Driver | 1 | Dual H-Bridge |
| DC Motors | 2 | 12V recommended |
| HC-SR04 Ultrasonic Sensor | 1 | For obstacle detection |
| MPU6050 Accelerometer | 1 | For tilt detection |
| Buzzer | 1 | Active or passive |
| 18650 Li-ion Batteries | 3 | 3.7V each (11.1V total) |
| 3S BMS Module | 1 | Battery protection |
| 5V Buck Converter | 1 | LM2596 or similar |
| Resistors | 2 | 1kΩ and 2kΩ for voltage divider |
| Jumper Wires | - | Male-to-female/male-to-male |
| Breadboard (optional) | 1 | For prototyping |

## 💻 Software Requirements

- **Arduino IDE** v1.8.x or newer
- **ESP32 Board Support** (via Board Manager)
- **Libraries**:
  - MPU6050 by Electronic Cats
  - Wire (built-in)
  - SoftwareSerial (built-in)
  - BluetoothSerial (ESP32 built-in)

## 📐 Circuit Diagrams

### Hand Unit Wiring (ESP32 + MPU6050)

```
MPU6050        ESP32 (NodeMCU-32S)
-------        -------------------
VCC      →     3.3V
GND      →     GND
SDA      →     GPIO 21
SCL      →     GPIO 22
```

### Wheelchair Unit Wiring (Arduino Uno)

#### HC-05 Bluetooth Module
```
HC-05          Arduino Uno
-----          -----------
VCC      →     5V
GND      →     GND
TX       →     Pin 2 (RX)
RX       →     Pin 3 (TX) via voltage divider*
EN/KEY   →     Not Connected
```

**Voltage Divider (CRITICAL):**
```
Arduino Pin 3 ──[1kΩ]──┬──→ HC-05 RX
                        │
                    [2kΩ]
                        │
                       GND
```

#### L298N Motor Driver
```
L298N          Arduino Uno          Connection
-----          -----------          ----------
ENA      →     Pin 5 (PWM)          Left motor speed
IN1      →     Pin 6                Left motor direction
IN2      →     Pin 7                Left motor direction
IN3      →     Pin 8                Right motor direction
IN4      →     Pin 9                Right motor direction
ENB      →     Pin 10 (PWM)         Right motor speed
12V      →     Battery 11.1V        Motor power
GND      →     Common GND           Ground
OUT1/2   →     Left Motor           Motor connections
OUT3/4   →     Right Motor          Motor connections
```

#### HC-SR04 Ultrasonic Sensor
```
HC-SR04        Arduino Uno
-------        -----------
VCC      →     5V
GND      →     GND
TRIG     →     Pin 11
ECHO     →     Pin 12
```

#### MPU6050 (Wheelchair)
```
MPU6050        Arduino Uno
-------        -----------
VCC      →     5V
GND      →     GND
SDA      →     A4
SCL      →     A5
```

#### Buzzer
```
Buzzer         Arduino Uno
------         -----------
(+)      →     Pin 13
(-)      →     GND
```

### Power Distribution
```
[3x 18650 in Series: 11.1V]
         |
         |-----> [L298N Motor Driver] ---> Motors
         |            |
         |            GND (Common Ground)
         |
         |-----> [Buck Converter (5V)]
                      |
                      |---> Arduino Uno 5V
                      |---> HC-05 VCC
                      |---> MPU6050 VCC
                      |---> Ultrasonic VCC
                      |
                      GND (Common Ground)
```

## 🚀 Installation

### 1. Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Add ESP32 Board Support
1. Open Arduino IDE
2. Go to `File → Preferences`
3. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools → Board → Board Manager`
5. Search "ESP32" and install "ESP32 by Espressif Systems"

### 3. Install Required Libraries
1. Go to `Tools → Manage Libraries`
2. Search and install:
   - **MPU6050** by Electronic Cats

### 4. Upload Code

#### Hand Unit (ESP32)
1. Open `hand_unit_esp32.ino`
2. Select `Tools → Board → ESP32 Arduino → NodeMCU-32S`
3. Select correct COM port
4. Click Upload
5. Open Serial Monitor (115200 baud)

#### Wheelchair Unit (Arduino Uno)
1. Open `wheelchair_unit_arduino.ino`
2. Select `Tools → Board → Arduino AVR Boards → Arduino Uno`
3. Select correct COM port
4. Click Upload
5. Open Serial Monitor (9600 baud)

## 📱 Usage

### Initial Setup

1. **Power Up Sequence**:
   - Turn ON wheelchair unit (Arduino + HC-05)
   - Wait 5 seconds
   - Turn ON hand unit (ESP32)
   - Wait 10 seconds for auto-connection

2. **Pairing (First Time)**:
   - On phone/laptop Bluetooth: Pair with "HC-05" (PIN: 1234)
   - Pair with "ESP32_Hand" (PIN: 1234)
   - Turn OFF phone/laptop Bluetooth
   - Devices will auto-connect to each other

3. **Verify Connection**:
   - ESP32 Serial Monitor: "✓✓✓ CONNECTED!"
   - Arduino Serial Monitor: "✓✓✓ ESP32 CONNECTED!"
   - HC-05 LED: Slow blinking (every 2 seconds)
   - Buzzer: 1 beep on connection

### Operating the Wheelchair

| Hand Gesture | Wheelchair Action |
|--------------|-------------------|
| Tilt Forward (>15°) | Move Forward |
| Tilt Backward (<-15°) | Move Backward |
| Tilt Left (>15°) | Turn Left |
| Tilt Right (<-15°) | Turn Right |
| Keep Level | Stop |

### Buzzer Alerts

| Beeps | Meaning |
|-------|---------|
| 2 beeps | System ready, waiting for connection |
| 1 beep | Connection established |
| 3 beeps | Obstacle detected |
| 5 beeps | Dangerous tilt detected |

### Safety Features in Action

- **Obstacle Detection**: Wheelchair automatically stops 30cm before obstacles
- **Tilt Protection**: Stops if wheelchair tilts more than 30°
- **Signal Loss**: Stops if no command received for 1 second

## ⚙️ Configuration

### Adjustable Parameters

Edit these values in `wheelchair_unit_arduino.ino`:

```cpp
// Motor speed (0-255)
const int MOTOR_SPEED = 200;  // Increase for faster, decrease for slower

// Obstacle detection distance (cm)
const int OBSTACLE_DISTANCE = 30;  // Stop distance from obstacle

// Tilt safety angle (degrees)
const int TILT_ALERT_ANGLE = 30;  // Maximum safe tilt

// Connection timeout (milliseconds)
const unsigned long COMMAND_TIMEOUT = 1000;  // Stop if no signal
```

Edit these values in `hand_unit_esp32.ino`:

```cpp
// Gesture sensitivity (degrees)
const int TILT_THRESHOLD = 15;  // Increase if too sensitive (20-25)
                                // Decrease for more sensitivity (10-12)
```

### HC-05 Configuration

If you need to reconfigure HC-05, use AT commands:

```
AT+NAME=HC-05          // Set device name
AT+PSWD=1234          // Set pairing PIN
AT+UART=9600,0,0      // Set baud rate
AT+ROLE=0             // Slave mode
```

## 🔧 Troubleshooting

### Connection Issues

| Problem | Solution |
|---------|----------|
| ESP32 won't connect to HC-05 | Verify HC-05 MAC address, try pairing via phone first |
| HC-05 LED fast blinking | Not connected - check power, verify AT commands |
| "Connection timeout" message | Check Bluetooth range (<10m), verify power supply |
| No response from HC-05 | Check voltage divider, verify TX/RX wiring |

### Motor Issues

| Problem | Solution |
|---------|----------|
| Motors don't spin | Check L298N power (11.1V), verify ENA/ENB jumpers removed |
| Motors spin backwards | Swap motor wires on L298N (OUT1↔OUT2 or OUT3↔OUT4) |
| Motors too fast/slow | Adjust MOTOR_SPEED (0-255) |
| One motor not working | Check individual motor connections and wiring |

### Sensor Issues

| Problem | Solution |
|---------|----------|
| MPU6050 connection failed | Check I2C wiring (SDA/SCL), verify 3.3V/5V power |
| Ultrasonic sensor false triggers | Mount away from vibrations, adjust OBSTACLE_DISTANCE |
| Gestures not detected | Calibrate TILT_THRESHOLD, check MPU6050 orientation |

### Power Issues

| Problem | Solution |
|---------|----------|
| System resets randomly | Check battery charge, verify BMS connections |
| HC-05 dim LED | Insufficient 5V supply, check buck converter output |
| Motors weak | Check battery voltage (should be >9V), verify connections |

## 📊 Serial Monitor Output

### Normal Operation

**ESP32 (115200 baud):**
```
================================
  ESP32 HAND GESTURE CONTROL   
================================
✓ Bluetooth initialized (SLAVE MODE)
✓ MPU6050 initialized
Waiting for connection...
✓✓✓ CONNECTED! ✓✓✓
>>> Sent: F | Pitch: 25.3° | Roll: 2.1°
>>> Sent: L | Pitch: 3.2° | Roll: 20.5°
```

**Arduino (9600 baud):**
```
================================
  WHEELCHAIR CONTROL SYSTEM    
================================
✓ MPU6050 initialized
Waiting for ESP32 connection...
✓✓✓ ESP32 CONNECTED! ✓✓✓
CMD: F → FORWARD
CMD: L → TURN LEFT
⚠ OBSTACLE: 25 cm - STOPPED
CMD: S → STOP
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### How to Contribute
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Ideas for Improvement
- [ ] Add variable speed control (multiple tilt angles)
- [ ] Implement voice commands
- [ ] Add LED status indicators
- [ ] Create mobile app for monitoring
- [ ] Add GPS tracking
- [ ] Implement route memory/playback
- [ ] Add emergency stop button
- [ ] Battery level indicator

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- Your Name - Initial work

## 🙏 Acknowledgments

- MPU6050 library by Electronic Cats
- ESP32 community for Bluetooth examples
- Arduino community for motor control references

## 📞 Support

For issues and questions:
- Open an issue on GitHub
- Check the [Troubleshooting](#troubleshooting) section
- Review Serial Monitor output for debugging

## 🔐 Safety Disclaimer

**IMPORTANT**: This is an assistive technology project. Please ensure:

- ⚠️ Always test in a safe, controlled environment first
- ⚠️ Never rely solely on automated systems for safety
- ⚠️ Keep manual override controls accessible
- ⚠️ Regularly inspect all electrical connections
- ⚠️ Use proper battery protection (BMS required)
- ⚠️ Maintain the wheelchair according to manufacturer guidelines
- ⚠️ Consult with medical/mobility professionals before use

---

**Star ⭐ this repository if you find it helpful!**

**Made with ❤️ for accessibility and independence**
