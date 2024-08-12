# IoT Based Streetlight automation and Fault Detection

# Smart Lighting Control System

This project is a collaborative effort to develop a smart lighting control system using sensors and a dimmer. The system is designed to automate lighting based on ambient light levels, motion detection, and time of day. The primary goal is to optimize energy consumption while ensuring adequate lighting conditions.

## Features

- **Ambient Light Detection**: Utilizes three LDR sensors to monitor the ambient light levels in different areas. The system automatically adjusts the light intensity based on these readings.

- **Motion Detection**: An ultrasonic sensor detects motion within a specified range. If motion is detected during nighttime, the system increases the light intensity. If no motion is detected for a set period, the light intensity is reduced to conserve energy.

- **Time-Based Control**: The system distinguishes between day and night using a timer. Lights are automatically turned on at night and off during the day.

- **AC Current Measurement**: The system includes an ACS712 current sensor to measure and calculate the True RMS (TRMS) of the current flowing through the lights. This data is used to monitor power consumption.

- **Data Logging**: The system sends a string of data through the serial interface, which includes the status of each bulb, the dimmer power level, and the measured current. This allows for monitoring and analysis of the system's performance.

## Components Used

- **LDR Sensors**: For detecting ambient light levels.
- **Ultrasonic Sensor**: For detecting motion.
- **ACS712 Current Sensor**: For measuring AC current.
- **Dimmer**: For adjusting the light intensity.
- **Arduino Board**: As the main controller for the system.

## Code Overview

The code is structured to handle multiple tasks simultaneously:

- **Light Control**: Based on LDR readings and time of day.
- **Motion Detection**: Uses the ultrasonic sensor to detect movement and adjust light intensity.
- **AC Current Measurement**: Calculates and logs the TRMS value of the current.

The code also includes a `createDataString()` function, which formats the sensor data and system status into a string that is printed to the serial monitor for easy monitoring.

## Setup Instructions

1. **Hardware Connections**:
    - Connect the LDR sensors to analog pins A0, A1, and A2.
    - Connect the ACS712 sensor to analog pin A4.
    - Connect the ultrasonic sensor to digital pins 6 (echo) and 7 (trig).
    - Connect the dimmer to digital pin 10.
    - Connect the zero-cross detection to digital pin 2.

2. **Software**:
    - Load the provided code onto an Arduino board.
    - Use the Serial Monitor to view the system's data output.

## Contributors

1. Melroy Desouza
2. Nasim Ansari
3. Leonne Desouza
4. Faimaan Aga