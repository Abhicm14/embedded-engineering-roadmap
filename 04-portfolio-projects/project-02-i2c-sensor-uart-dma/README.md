# 📁 Project 02: I2C Sensor Driver & DMA UART CLI Shell

> Target: STM32F401 / STM32F411 / ESP32  
> Language: Embedded C (C99)  
> Key Technologies: I2C Master, Bosch BMP280 Digital Barometric Sensor, DMA UART with Ring Buffer, Command Line Parser

---

## 🎯 Project Overview

In this project, you build an interactive serial Command-Line Interface (CLI) backed by Direct Memory Access (DMA) and a custom register-level I2C sensor driver for the Bosch BMP280 temperature and pressure sensor.

### Key Learnings:
1. **DMA RX/TX Buffering:** Eliminating CPU polling overhead by transferring serial bytes directly to/from SRAM in hardware.
2. **Lock-Free Ring Buffer:** Managing asynchronous data bursts from the UART DMA without dropping bytes or requiring critical sections.
3. **I2C Protocol Implementation:** Managing START, Repeated-START, 7-bit addressing, ACK/NACK, and bus lockup recovery.
4. **Sensor Calibration Compensation:** Parsing factory trimming parameters stored in sensor ROM and computing integer/fixed-point compensation formulas.
5. **Interactive Command Shell:** Non-blocking CLI command dispatcher parsing user inputs like `temp`, `pressure`, `reset`, and `stream`.

---

## 🔌 Hardware Setup & Pinout

| Signal | STM32 Pin | BMP280 Sensor Pin | USB-UART Pin | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **I2C1_SCL** | PB6 | SCL | - | Pull-up $4.7\text{ k}\Omega$ to 3.3V |
| **I2C1_SDA** | PB7 | SDA | - | Pull-up $4.7\text{ k}\Omega$ to 3.3V |
| **USART2_TX**| PA2 | - | RXD | 115200 baud, 8-N-1 |
| **USART2_RX**| PA3 | - | TXD | Connected to DMA1 Stream 5 |
| **GND / 3.3V**| GND / 3.3V | GND / VCC | GND | Common ground reference |

---

## 🏗️ Architecture Diagram

```
[ PC Terminal (115200) ]
        │
   USB-to-UART
        │
   USART2 RX Pin (PA3)
        │
        ▼
[ DMA Stream ] ──(Pushes bytes)──► [ Circular Ring Buffer ]
                                              │
                                        (Pops bytes)
                                              ▼
                                    [ CLI Command Parser ]
                                              │
                      ┌───────────────────────┴───────────────────────┐
                      ▼                                               ▼
             [ "temp" Command ]                             [ "press" Command ]
                      │                                               │
                      └──────────────► [ BMP280 Driver ] ◄────────────┘
                                              │
                                         I2C1 Engine
                                         (PB6 / PB7)
                                              │
                                              ▼
                                    [ BMP280 Sensor HW ]
```
