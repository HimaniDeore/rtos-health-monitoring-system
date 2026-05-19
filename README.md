# RTOS-Based Smart Health Monitoring System with Fall Detection and Emergency Alerts

A real-time embedded health monitoring system developed using STM32F401 and FreeRTOS for monitoring heart rate, SpO₂, and fall detection. The system uses multitasking, interrupt handling, mutex, and semaphores to efficiently manage multiple operations simultaneously.

The project continuously monitors patient health conditions and generates emergency alerts during abnormal situations such as fall detection or invalid sensor readings.

## Overview

Traditional embedded systems using a single-loop architecture struggle to handle multiple tasks simultaneously. In healthcare applications, delayed responses or improper synchronization can reduce reliability and safety.

This project solves these limitations using FreeRTOS by dividing the system into multiple concurrent tasks with priority-based scheduling.

The system performs:
- Real-time heart rate monitoring
- Blood oxygen saturation (SpO₂) monitoring
- Accelerometer-based fall detection
- OLED display updates
- Interrupt-based touch control
- Emergency alert generation

## Features

- FreeRTOS-based multitasking
- Heart rate and SpO₂ monitoring using MAX30100
- Fall detection using MPU6050
- OLED real-time display
- Interrupt-driven touch sensor control
- Mutex-protected I2C communication
- Semaphore synchronization
- Emergency alert handling
- Real-time task scheduling

## Hardware Components

| Component | Description |
|---|---|
| STM32F401 | ARM Cortex-M4 Microcontroller |
| MAX30100 | Pulse Oximeter and Heart Rate Sensor |
| MPU6050 | Accelerometer and Gyroscope Module |
| OLED Display | I2C OLED Display |
| Touch Sensor | Capacitive Touch Input |
| LED/Buzzer | Emergency Alert Indication |

## Software Used

- STM32CubeIDE
- Embedded C
- FreeRTOS
- STM32 HAL Drivers

## System Architecture

The system is implemented using multiple FreeRTOS tasks.

| Task | Priority | Function |
|---|---|---|
| Touch Task | High | Handles touch interrupt and system control |
| MPU6050 Task | Above Normal | Detects fall conditions |
| MAX30100 Task | Normal | Reads BPM and SpO₂ values |
| Display Task | Low | Updates OLED display |

## Working Principle

### Heart Rate and SpO₂ Monitoring

The MAX30100 sensor measures heart rate and oxygen saturation using IR and Red light signals.

The system:
- Reads sensor values through I2C
- Applies signal processing
- Detects pulse peaks
- Calculates BPM
- Estimates SpO₂ values

### Fall Detection

The MPU6050 continuously monitors acceleration values.

The fall detection algorithm:
1. Detects sudden free-fall
2. Detects impact acceleration
3. Verifies no movement after impact
4. Triggers emergency alert

This helps reduce false detection.

### Interrupt-Based Touch Control

The touch sensor uses external interrupt (EXTI) for fast response.

Flow:
Touch Sensor → EXTI Interrupt → ISR → Semaphore → Touch Task

Advantages:
- Fast response
- Reduced CPU usage
- No continuous polling

### OLED Display

The OLED display continuously updates:
- Heart Rate
- SpO₂
- System State
- Emergency Alerts
- Fall Detection Status

## Hardware Connections

### MAX30100 Connections

| MAX30100 | STM32F401 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | I2C3 SDA |
| SCL | I2C3 SCL |

### MPU6050 Connections

| MPU6050 | STM32F401 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | I2C1 SDA |
| SCL | I2C1 SCL |

### OLED Display Connections

| OLED | STM32F401 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | I2C1 SDA |
| SCL | I2C1 SCL |

### Touch Sensor Connections

| Touch Sensor | STM32F401 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| OUT | GPIO EXTI Pin |

## Synchronization Mechanisms

### Mutex
Used to protect I2C communication and prevent simultaneous bus access by multiple tasks.

### Semaphore
Used for communication between interrupt service routine (ISR) and Touch Task.

# Project Structure

The complete STM32 FreeRTOS project is originally organized inside the ZIP file generated from STM32CubeIDE.  
Only the important source files (`.c`, `.h`, `.s`) and result images are uploaded to this repository for cleaner presentation and easier understanding.

The actual project configuration inside STM32CubeIDE follows the structure below:

```bash
RTOS_Health_Monitoring_System/
│
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── freertos.h
│   │   ├── max30100.h
│   │   ├── mpu6050.h
│   │   ├── oled.h
│   │   ├── stm32f4xx_it.h
│   │   └── stm32f4xx_hal_conf.h
│   │
│   └── Src/
│       ├── main.c
│       ├── freertos.c
│       ├── max30100.c
│       ├── mpu6050.c
│       ├── oled.c
│       ├── stm32f4xx_it.c
│       ├── stm32f4xx_hal_msp.c
│       └── system_stm32f4xx.c
│
├── Drivers/
│   ├── CMSIS/
│   └── STM32F4xx_HAL_Driver/
│
├── Middlewares/
│   └── Third_Party/
│       └── FreeRTOS/
│
├── Startup/
│   └── startup_stm32f401xx.s
│
├── Debug/
│   └── (Build files generated automatically)
│
├── Code/
│   ├── main.c
│   ├── main.h
│   ├── freertos.c
│   ├── freertos.h
│   ├── max30100.c
│   ├── max30100.h
│   ├── mpu6050.c
│   ├── mpu6050.h
│   ├── oled.c
│   ├── oled.h
│   ├── stm32f4xx_it.c
│   ├── stm32f4xx_it.h
│   ├── stm32f4xx_hal_msp.c
│   ├── stm32f4xx_hal_conf.h
│   ├── FreeRTOSConfig.h
│   ├── startup_stm32f401xx.s
│   └── system_stm32f4xx.c
│
├── Results/
│   ├── block_diagram.png
│   ├── hardware_setup.jpg
│   ├── oled_output.jpg
│   ├── fall_detection.jpg
│   └── system_running.jpg
│
└── README.md
```

## Note
- The `Core`, `Drivers`, `Middlewares`, and `Startup` folders represent the original STM32CubeIDE project structure.
- The `Code` folder in this repository contains the essential source files extracted from the original project for simplified viewing.
- Build/debug files are not included to keep the repository clean and lightweight.`

## Results

The system was successfully tested under multiple operating conditions.

Successful observations:
- Stable BPM and SpO₂ readings
- Real-time OLED updates
- Fast interrupt response
- Successful fall detection
- Smooth RTOS task execution
- Reliable multitasking performance

## Future Improvements

- GSM-based emergency alerts
- Bluetooth/WiFi connectivity
- Cloud health monitoring
- Mobile application integration
- AI-based health analysis
- Wearable implementation

## Applications

- Elderly patient monitoring
- Smart healthcare devices
- Wearable health systems
- Emergency monitoring systems
- Real-time patient monitoring

## Author

Himani Deore  
Electronics and Telecommunication Engineering

## License

This project is developed for educational and research purposes.
