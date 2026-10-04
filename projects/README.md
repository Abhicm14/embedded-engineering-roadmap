# 🚀 The 7 Portfolio Projects for Embedded Engineers

> Seven progressive, production-grade portfolio projects designed to build an industry-ready GitHub profile.

---

## 🧭 Projects Matrix at a Glance

| Level | Project # & Name | Silicon Platform | Core Technologies | Target Guide |
| :---: | :--- | :--- | :--- | :--- |
| **Beginner** | **1. GPIO Control Board** | STM32F4 (Cortex-M4) | Bare-metal C (No HAL), Linker script, SysTick timer, Debounced switch FSM. | [`beginner.md`](beginner.md#project-1-gpio-control-board) |
| **Beginner** | **2. UART Command Console** | STM32F4 / ESP32 | Circular FIFO buffer, interrupt-driven UART, command parser shell. | [`beginner.md`](beginner.md#project-2-uart-command-console) |
| **Intermediate** | **3. Sensor Data Logger** | STM32F4 / Black Pill | I2C sensor driver (BMP280), SPI Flash memory (W25Qxx), DMA transfers. | [`intermediate.md`](intermediate.md#project-3-sensor-data-logger) |
| **Intermediate** | **4. PWM Fan/Motor Controller** | STM32F4 / N-FET | Hardware timer PWM, ADC tachometer input, closed-loop PID control. | [`intermediate.md`](intermediate.md#project-4-pwm-fanmotor-controller) |
| **Intermediate** | **5. FreeRTOS Environmental Monitor** | STM32 / FreeRTOS | Preemptive multitasking, Queues, Mutex with Priority Inheritance, low-power. | [`intermediate.md`](intermediate.md#project-5-freertos-environmental-monitor) |
| **Advanced** | **6. Connected IoT Node** | ESP32 / STM32+WiFi | TLS 1.3 encryption, MQTT pub/sub, Dual-bank Flash A/B fail-safe OTA updates. | [`advanced.md`](advanced.md#project-6-connected-iot-node) |
| **Advanced** | **7. TinyML Edge Device** | Cortex-M4F / IMU | TensorFlow Lite Micro, CMSIS-NN Quantized INT8, real-time gesture classification. | [`advanced.md`](advanced.md#project-7-tinyml-edge-device) |

> [!TIP]
> **Looking for 8-Bit Architecture Projects?**  
> Check out the [**PIC Microcontroller Projects**](beginner.md#pic-beginner-projects) covering the PIC16F877A, including Digital I/O, Timers, ADC, PWM, UART, I2C, and EEPROM storage.
>
> 🛑 **Construction Rule:** Build every project using the [**7-Step Code Construction Workflow**](../code-examples/README.md#the-7-step-code-construction-methodology). Do not copy-paste code; write it register-by-register after inspecting the hardware schematic and datasheet.

---

## 🏆 Recommended Final Portfolio Showcase

For your resume and GitHub profile, we strongly recommend showcasing:
1. **One Bare-Metal STM32 Project:** Demonstrates understanding of registers, linker scripts, and clock trees (Project 1 or 2).
2. **One FreeRTOS Multi-Tasking Project:** Demonstrates real-time synchronization, queue pipelines, and priority management (Project 5).
3. **One Advanced Specialization Project:** Demonstrates domain depth in Automotive CAN, Secure IoT, or TinyML (Project 4, 6, or 7).
4. *(Optional)* **One Embedded Linux Driver:** If applying for Linux BSP / Systems roles.
