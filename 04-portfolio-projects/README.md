# 🚀 7 Production-Grade Embedded Portfolio Projects

> Seven end-to-end, resume-defining portfolio projects demonstrating bare-metal register manipulation, peripheral drivers, RTOS multitasking, Embedded Linux drivers, Automotive CAN, Secure IoT, and TinyML.

---

## 🎯 Projects Overview Matrix

| # | Project Name | Complexity | Primary Silicon Target | Key Technologies Demonstrated | Directory |
| :-: | :--- | :---: | :--- | :--- | :--- |
| **01** | **Bare-Metal GPIO & SysTick FSM** | Beginner | STM32F401 / F411 (Cortex-M4) | Pure register C (No HAL), Vector table, Linker script, Makefile, Debounced Button FSM | [`project-01-baremetal-gpio-systick/`](project-01-baremetal-gpio-systick/) |
| **02** | **I2C Sensor Driver & DMA UART Shell** | Beginner-Int | STM32F4 / ESP32 | I2C register driver, BMP280 compensation math, Circular Ring Buffer, DMA RX/TX, CLI Shell | [`project-02-i2c-sensor-uart-dma/`](project-02-i2c-sensor-uart-dma/) |
| **03** | **FreeRTOS Multi-Task Environmental Logger** | Intermediate | STM32F4 / FreeRTOS | Preemptive multitasking, Queues, Mutexes, SPI Flash logging, OLED SSD1306, Low-power tickless | [`project-03-freertos-data-logger/`](project-03-freertos-data-logger/) |
| **04** | **Automotive CAN Gateway & Telemetry Node** | Int-Advanced | STM32F4 / ESP32 CAN | CAN 2.0B / CAN-FD, Filter masks, OBD-II PID parser, Arbitration, Bus-off recovery | [`project-04-automotive-can-node/`](project-04-automotive-can-node/) |
| **05** | **Embedded Linux Character Driver & App** | Advanced | Raspberry Pi / BeagleBone / QEMU | Linux Kernel Module (LKM), `file_operations`, `sysfs`, `copy_to_user`, User-space POSIX daemon | [`project-05-embedded-linux-driver/`](project-05-embedded-linux-driver/) |
| **06** | **Secure Edge IoT Client with MQTT & OTA** | Advanced | ESP32 / STM32 + Wi-Fi | TLS 1.3 encryption, X.509 mTLS, MQTT pub/sub, Dual-bank Flash A/B rollback, Secure boot | [`project-06-secure-iot-mqtt-ota/`](project-06-secure-iot-mqtt-ota/) |
| **07** | **TinyML Motion / Gesture Classifier** | Capstone | STM32F401 / Cortex-M4F | TensorFlow Lite Micro, CMSIS-NN Quantized INT8, 6-Axis IMU DSP windowing, Real-time inference | [`project-07-tinyml-gesture-classifier/`](project-07-tinyml-gesture-classifier/) |

---

## 💡 How to Build and Showcase These in Your Portfolio

1. **Clean Code & Hardware Evidence:** For each project you complete, commit the full code, include a clean wiring schematic, and take a photo or screenshot of the physical board working alongside a logic analyzer / terminal trace.
2. **Architecture-First README:** Every project folder includes an architecture diagram, pinout table, register explanations, and step-by-step flashing instructions.
3. **Resume Impact:** Add each project under your "Technical Projects" section, highlighting the specific hardware protocols, memory management techniques, and debugging tools used.
