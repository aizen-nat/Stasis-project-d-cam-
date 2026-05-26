# Stasis Project D-Cam
Smart Attendance & Access Verification System built with Raspberry Pi, RFID, Fingerprint Authentication, and ESP32-CAM.

## Overview
Stasis Project D-Cam is a multi-factor attendance management and smart access control system designed for schools, universities, labs, and restricted environments.
The system combines RFID verification, fingerprint authentication, camera capture, and physical door control into one embedded hardware platform powered by Raspberry Pi.
Instead of relying on traditional attendance methods, the device verifies identity physically before granting access or marking attendance.

## Features
- RFID attendance scanning
- Fingerprint verification
- ESP32-CAM face capture
- Door lock automation
- Real-time attendance logging
- LCD status display
- OLED status monitor
- Audio feedback system
- Relay-controlled access system
- Local data storage
- Offline-ready operation
- Raspberry Pi powered control system

## Hardware Used
   Component                                 Purpose    
   Raspberry Pi 3B+                  Main processing unit   
   RC522 RFID Module                 Card scanning   
   ESP32-CAM                          Face capture   
   R307 Fingerprint Sensor            Biometric verification   
   DFPlayer Mini                      Audio feedback   
   LCD 16x2                          Main display   
   OLED Display                      Secondary status display   
   Relay Module                      Door lock control   
   Electronic Door Lock              Physical access system   
   MicroSD Card                      Data storage   
   LEDs + Buzzer                      Status indicators   
   Custom PCB                          Hardware integration   
   Custom Enclosure                  Device housing   

## System Flow

1. User scans RFID card
2. System reads student/staff information
3. Fingerprint verification starts
4. ESP32-CAM captures image
5. Raspberry Pi validates request
6. Attendance gets logged
7. Door unlocks if verified
8. LCD displays result

## Project Goals

- Reduce proxy attendance
- Improve classroom security
- Create an offline-capable attendance platform
- Build a low-cost smart verification system
- Design a scalable university-grade hardware solution

## Software Stack
- Python
- Raspberry Pi OS
- SPI / I2C / UART Communication
- GPIO Control
- OpenCV
- RFID Libraries
- Fingerprint Sensor Libraries

## Repository Structure
code(firmware)/
cart(hardware parts)/
imagesandpcb/

## Future Plans
- Web dashboard integration
- Cloud sync support
- Face recognition matching
- Mobile notifications
- Thermal receipt printing
- Battery backup integration
- Multi-device classroom networking
- PCB revision v2

## Current Status
Prototype development in progress.
Hardware integration, PCB planning, and enclosure design are actively being tested and improved.

## Images

### Overall PCB Concept
![PCB](imagesandpcb/dcampcb.png)

## Author
Natnaiel B

## License
MIT License
