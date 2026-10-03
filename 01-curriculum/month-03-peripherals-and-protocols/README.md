# 🟡 Month 3: Communication Protocols & Hardware Peripherals

> Focus: UART, SPI, I2C, Timers, PWM, NVIC Priority Grouping, and Direct Memory Access (DMA).

---

## 🎯 Monthly Objectives
1. Implement serial communication protocols at register level (UART, SPI, I2C).
2. Configure Direct Memory Access (DMA) to stream data with zero CPU utilization.
3. Master ARM NVIC interrupt priority grouping and ISR execution rules.
4. Interface with physical sensors and parse sensor trimming calibration data.
5. Complete **Project 2: I2C Sensor Driver & DMA UART CLI Shell**.

---

## 📅 Weekly Breakdown

### Week 9: UART with Circular DMA
- Asynchronous framing, baud rate division, overrun errors, and RTS/CTS flow control.
- DMA Stream configuration (Peripheral-to-Memory circular mode).
- Lab: Implement an interrupt-driven UART driver with a ring buffer.

### Week 10: Serial Peripheral Interface (SPI)
- Synchronous master/slave, the 4 clock modes (CPOL/CPHA), chip select handling.
- Interfacing with SPI Flash memories (Winbond W25Qxx) and SPI OLED displays.
- Lab: Read JEDEC ID from an SPI Flash chip over hardware SPI.

### Week 11: Inter-Integrated Circuit (I2C) & Project 2 Milestone
- Open-drain bus, START/STOP conditions, ACK/NACK, bus lockup 9-clock recovery.
- Parsing Bosch BMP280 factory calibration registers and computing fixed-point math.
- Lab: Deliver [Project 2](../../04-portfolio-projects/project-02-i2c-sensor-uart-dma/).

### Week 12: Timers, PWM & Nested Vectored Interrupt Controller (NVIC)
- Advanced and general-purpose timers, auto-reload, prescalers, input capture.
- PWM generation with variable duty cycle.
- NVIC preemption priorities and subpriorities.
- Lab: Build an LED breathing PWM driver with hardware timer.
