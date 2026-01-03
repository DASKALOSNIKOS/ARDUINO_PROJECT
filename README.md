# Obstacle Avoiding Robot using Arduino

This repository contains the complete implementation of an autonomous obstacle avoiding robotic vehicle, developed as part of a postgraduate-level Arduino assignment.

The project demonstrates the design and implementation of an embedded system that combines sensors, actuators and control logic to achieve real-time autonomous navigation and obstacle avoidance.

---

## 📌 Project Overview

The robotic vehicle is based on an **Arduino Uno** microcontroller and uses:
- an **HC-SR04 ultrasonic sensor** for distance measurement,
- a **servo motor** to rotate the sensor and scan the environment,
- an **L298N motor driver** to control two DC motors.

The robot moves forward autonomously and, when an obstacle is detected, it stops, scans its surroundings and selects an alternative direction with more available space.

---

## ⚙️ Hardware Components

- Arduino Uno
- L298N Dual H-Bridge Motor Driver
- HC-SR04 Ultrasonic Distance Sensor
- Servo Motor (for sensor rotation)
- 2 DC Motors (2WD chassis)
- Battery pack (4×AA)
- Jumper wires and mechanical mounting materials

---

## 🔌 Final Wiring Summary

- **L298N**
  - ENA / ENB: Enabled via onboard jumpers
  - IN1 → D7  
  - IN2 → D6  
  - IN3 → D5  
  - IN4 → D4  

- **HC-SR04 Ultrasonic Sensor**
  - TRIG → D9  
  - ECHO → D8  
  - VCC → 5V (L298N regulator)  
  - GND → GND (common ground)

- **Servo Motor**
  - Signal → D10  
  - VCC → 5V (Arduino)  
  - GND → GND (Arduino)

- **Power Supply**
  - Battery + → L298N VMS and Arduino VIN  
  - Battery − → Common GND

---

## 🧠 Software Description

The software is written in **Arduino C/C++** and is structured in a modular way:
- Distance measurement is handled by a dedicated function.
- Motor control is abstracted into helper functions.
- The main control loop implements a decision-making algorithm based on distance thresholds.

The robot operates as a cyclic state-based system with the following logical states:
1. Initialization
2. Forward Motion
3. Obstacle Detection
4. Environment Scanning
5. Decision and Maneuver Execution

A detailed flowchart and state model are provided in the accompanying report.

---

## ▶️ How It Works

1. The robot continuously measures the distance in front of it.
2. If the path is clear, it moves forward.
3. When an obstacle is detected:
   - the robot stops,
   - scans left and right using the servo-mounted sensor,
   - compares available space,
   - reverses briefly and turns toward the freer direction.
4. The process repeats continuously, allowing autonomous navigation.

---

## 🛠️ Development Environment

- Arduino IDE
- Arduino Uno board package
- Servo library (included with Arduino IDE)

---

## 📄 Documentation

The full system design, architecture, flowchart, state model and detailed explanation of the software are documented in the accompanying academic report submitted as part of the course requirements.

---

## 📜 License

This project is provided for educational purposes.

---

## 🎥 Demo Video

A short demonstration of the robot’s autonomous obstacle avoidance behavior is available here:

▶️ [youtube.com/watch?v=SBTemglAMME&feature=youtu.be](https://www.youtube.com/watch?v=SBTemglAMME)


## 👤 Author

NIKOLAOS DASKALOS  

---
