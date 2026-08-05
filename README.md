# 🗑️ Smart Trash Bin

An Arduino Uno-based Smart Trash Bin that automatically opens the lid when a person approaches and monitors the trash level using ultrasonic sensors. The system also uses an LCD I2C, LED indicators, and a buzzer to provide real-time status.

## 📌 Overview

This project was developed as a university IoT project to demonstrate the implementation of sensors and automation using Arduino Uno. The smart trash bin improves hygiene and convenience by opening the lid automatically and notifying users when the bin is full.

## ✨ Features

- Automatic lid opening using an ultrasonic sensor
- Trash level monitoring with an ultrasonic sensor
- LCD I2C displays system status
- LED indicator for trash status
- Buzzer notification when the trash bin is full
- Real-time monitoring

## 🛠️ Components

- Arduino Uno R3
- 2 × HC-SR04 Ultrasonic Sensors
- Servo Motor SG90
- LCD 16x2 I2C
- Buzzer
- LED
- Breadboard
- Jumper Wires
- Power Supply

## ⚙️ How It Works

1. The ultrasonic sensor detects a person approaching the trash bin.
2. The servo motor automatically opens the lid.
3. After a few seconds, the lid closes automatically.
4. The second ultrasonic sensor measures the trash level.
5. The LCD displays the current system status.
6. When the trash is full, the LED and buzzer are activated.

## 📂 Project Structure

```
Smart-Trash-Bin/
├── Code/
├── Images/
├── Video/
└── README.md
```

## 📸 Project Documentation

Project photos are available in the **images/** folder.

Demo videos are available in the **video/** folder.

## 💻 Technologies Used

- Arduino IDE
- C/C++ (Arduino)
- Embedded Systems
- Internet of Things (IoT)

## 👨‍💻 My Role

This project was developed as a university group assignment.

My contributions included:
- Designing the Arduino program
- Developing the system logic
- Integrating sensors and hardware
- Testing and debugging
- Preparing project documentation

## 🚀 Future Improvements

- IoT monitoring using ESP32
- Mobile application integration
- Cloud-based data monitoring
- Automatic notification when the bin is full

## 📸 Preview

![Smart Trash Bin](Images/Hasil.jpg)

## 📄 License

This project is intended for educational and portfolio purposes.
