# 🎛️ Step 4: Essential Microcontroller Peripherals

> **Pillar:** FIRMWARE (STM32 + Peripherals)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*

---

## 🎯 Learning Objectives

Peripherals are the dedicated silicon hardware engines that allow the CPU to interact with the physical world. For every peripheral in this module, you will learn:
1. **What to Learn:** Registers, physical wiring, timing diagrams, clock prescaling, and error flags.
2. **Failure Modes:** Line capacitance, bus contention, overrun errors, floating inputs, and clock stretching lockups.
3. **Starter Exercise:** A concrete hands-on lab exercise.

---

## 🧭 Peripheral Guides & Starter Exercises

| Peripheral | Core Concepts | Starter Exercise |
| :--- | :--- | :--- |
| [**1. GPIO**](gpio.md) | Push-pull vs Open-drain, internal pull-ups, slew rate (`OSPEEDR`), atomic `BSRR`. | **Debounced LED Toggle:** Write a state machine reading a noisy button on PA0 to toggle PC13. |
| [**2. Timers & PWM**](timers-and-pwm.md) | Prescalers, Auto-Reload (`ARR`), input capture, PWM duty cycle calculation. | **Breathing LED:** Generate 1 kHz PWM on TIM2 Ch1, dynamically sweeping duty cycle from 0% to 100%. |
| [**3. Interrupts & NVIC**](interrupts-and-nvic.md) | External interrupts (`EXTI`), priority grouping, ISR execution time constraints. | **Rotary Encoder Decoder:** Decode quadrature pulses on 2 EXTI lines with zero missed steps. |
| [**4. UART / USART**](uart.md) | Baud rate divisor ($USARTDIV$), start/stop framing, circular DMA RX/TX buffers. | **Interactive Command Console:** Parse serial strings (`help`, `status`) at 115200 baud without blocking. |
| [**5. SPI Master/Slave**](spi.md) | The 4 clock modes (CPOL/CPHA), chip select (`CS`), full-duplex shift registers. | **Flash JEDEC ID Reader:** Read manufacturer ID from an external SPI Flash memory (W25Qxx). |
| [**6. I2C Bus**](i2c.md) | Open-drain, 7-bit addressing, ACK/NACK, pull-up sizing, 9-clock bus lockup recovery. | **BMP280 Sensor Driver:** Read raw temperature/pressure registers and compute factory compensation math. |
| [**7. ADC & DAC**](adc-dac.md) | Successive Approximation (SAR), sampling time, reference voltage ($V_{REF}$), continuous DMA. | **Analog Battery Monitor:** Sample potentiometer or battery voltage via ADC DMA with rolling average filter. |

---

## 🛠️ Associated Projects

- **[Project 2: UART Command Console](../../projects/beginner.md#project-2-uart-command-console)**
- **[Project 3: Sensor Data Logger](../../projects/intermediate.md#project-3-sensor-data-logger)**
- **[Project 4: PWM Fan/Motor Controller](../../projects/intermediate.md#project-4-pwm-fanmotor-controller)**

---

## ✅ Step 4 Completion Checklist

- [ ] Can configure a GPIO pin for atomic set/reset without race conditions.
- [ ] Can calculate timer prescaler and auto-reload registers for exact millisecond or microsecond intervals.
- [ ] Can explain the difference between SPI Mode 0 and Mode 3.
- [ ] Can implement the 9-clock I2C bus recovery routine to clear stuck slave devices.
- [ ] Can stream ADC conversion samples directly to an SRAM buffer using circular DMA.

➡️ **Next Step:** [Step 5: Engineering Workflow](../05-engineering-workflow/README.md)
