# 🔌 Step 2: Electronics & Computer Fundamentals

> **Pillar:** FOUNDATION (C + Electronics)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*  
> **Prerequisites:** Review [**`PREREQUISITES.md`**](../../PREREQUISITES.md). If you haven't wired a circuit yet, check our [**🌱 True Beginner On-Ramp (`beginner-onramp/`)**](../../beginner-onramp/README.md). Unfamiliar with any term? Check our [**📖 Beginner Glossary (`cheatsheets/glossary.md`)**](../../cheatsheets/glossary.md).

---

## 🎯 Learning Objectives

Microcontrollers do not operate in a vacuum of pure mathematics; they interact with analog physics, voltages, currents, and electromagnetic fields. By the end of this module, you should be able to:
1. Apply Ohm's Law and Kirchhoff's Laws to calculate current limiting, voltage dividers, and pull-up resistors.
2. Select and wire bipolar transistors (BJT) and N/P-channel MOSFETs for high-speed switching.
3. Understand binary, hexadecimal, Boolean algebra, flip-flops, and finite state machines in hardware.
4. Explain CPU core microarchitecture (ALU, Program Counter, Stack Pointer, core registers, and interrupts).
5. Diagnose signal rise-time, noise, and bus impedance using a Multimeter, Oscilloscope, and Logic Analyzer.
6. Read and extract electrical limits, pinout mappings, and timing diagrams from vendor datasheets.

---

## 🧭 Topic Guides in This Module

| Topic Document | Type | Key Concepts |
| :--- | :---: | :--- |
| [**1. Circuit Basics & Passive Components**](circuit-basics-and-components.md) | `[INTUITION]` | Ohm's law, pull-up/pull-down resistors, capacitor decoupling, diodes, MOSFET switches. |
| [**2. Digital Logic & Boolean Algebra**](digital-logic-and-gates.md) | `[INTUITION]` | Logic gates, truth tables, latches, flip-flops, propagation delays, setup/hold times. |
| [**3. CPU & Memory Architecture**](cpu-and-memory-architecture.md) | `[INTUITION]` | Registers, ALU, PC, SP, pipelining, cache, Flash vs SRAM vs EEPROM, endianness. |
| [**4. Signals, Noise & Grounding**](signals-and-noise.md) | `[DEEP DIVE]` | Frequency, duty cycle, slew rate, ringing, ground bounce, ESD protection. |
| [**5. Lab Instruments & Probing Mastery**](lab-instruments-and-probing.md) | `[INTUITION]` | Multimeter continuity, 10X oscilloscope probe attenuation, logic analyzer triggering. |

---

## 🛠️ Hands-on Lab Practice

1. **Pull-Up Dimensioning Lab:** Calculate the exact pull-up resistor required for a $400\text{ kHz}$ I2C line with $120\text{ pF}$ estimated trace capacitance.
2. **MOSFET Low-Side Switch:** Wire an N-channel MOSFET (e.g. 2N7000 or AO3400) to control a $12\text{V}$ DC relay from a $3.3\text{V}$ MCU GPIO pin. Add a freewheeling flyback diode across the inductive coil.
3. **Logic Analyzer Protocol Decoding:** Using **PulseView** and a USB 8-channel logic analyzer, capture an asynchronous serial packet and measure bit-time to verify baud rate accuracy.

---

## ✅ Step 2 Completion Checklist

- [ ] Can calculate resistor values for LED current limiting and I2C pull-ups.
- [ ] Understands the difference between $1\text{X}$ and $10\text{X}$ oscilloscope probes.
- [ ] Can explain why ground bounce occurs when multiple GPIOs switch simultaneously.
- [ ] Can read a timing diagram and identify setup time ($t_{su}$) and hold time ($t_h$).

➡️ **Next Step:** [Step 3: STM32 & Microcontrollers](../03-stm32-microcontrollers/README.md)
