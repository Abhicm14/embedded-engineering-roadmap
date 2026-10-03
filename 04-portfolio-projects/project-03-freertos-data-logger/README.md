# 📁 Project 03: Multitasking Environmental Data Logger with FreeRTOS

> Target: STM32F4 / ESP32 / FreeRTOS Simulator (POSIX / Win32)  
> Language: Embedded C (C99)  
> Key Technologies: FreeRTOS Preemptive Kernel, Inter-Task Queues, Mutexes, Software Timers, SPI Flash/SD Card, OLED Display

---

## 🎯 Project Overview

This project implements a multi-tasking environmental monitor and persistent data logger using the FreeRTOS real-time kernel. Multiple concurrent threads handle high-rate sensor sampling, display refresh, persistent SPI logging, and power management without priority inversions or race conditions.

### Architectural Highlights:
1. **Producer-Consumer Queue Pipeline:** High-rate sensor telemetry is pushed into an RTOS queue by producer tasks, decoupling sampling timing from slower Flash/SD storage writes.
2. **Mutual Exclusion:** Shared communication buses (I2C/SPI) are guarded by FreeRTOS Mutexes with **Priority Inheritance** to prevent Martian Priority Inversion disasters.
3. **Software Timers:** Periodic health-checks and system heartbeat LED blinking run on the FreeRTOS timer daemon task, avoiding dedicated thread RAM overhead.
4. **Tickless Low-Power Operation:** Demonstrating RTOS sleep mode entering `WFI` (Wait For Interrupt) during idle periods.

---

## 📊 Task Breakdown & Priority Matrix

| Task Name | Priority | Frequency | Stack Depth | Responsibility |
| :--- | :---: | :---: | :---: | :--- |
| **`vSensorTask`** | 3 (High) | 100 ms | 256 words | Polls environmental sensor, packages data, pushes to Queue |
| **`vLoggerTask`** | 2 (Medium) | Event-Driven | 512 words | Blocks on Queue; writes incoming records to SPI Flash / SD |
| **`vDisplayTask`**| 1 (Low) | 500 ms | 256 words | Updates OLED screen with current temperature/pressure |
| **`vHeartbeat`** | Timer Daemon | 1000 ms | Shared | Toggles green system health LED |

---

## 🏗️ Multi-Thread Dataflow Diagram

```
 [ Hardware Sensor ]
         │ (I2C)
         ▼
 ┌───────────────┐
 │  SensorTask   │ (Priority: High)
 └───────┬───────┘
         │
         │ xQueueSend(xSensorQueue)
         ▼
 ┌─────────────────────────────────────────┐
 │       Sensor Data FIFO Queue            │
 └───────────────────┬─────────────────────┘
                     │
                     │ xQueueReceive(portMAX_DELAY)
                     ▼
         ┌───────────────────────┐
         │      LoggerTask       │ (Priority: Medium)
         └───────────┬───────────┘
                     │
                     ▼ (Guarded by Mutex)
             [ SPI Flash / SD ]
```
