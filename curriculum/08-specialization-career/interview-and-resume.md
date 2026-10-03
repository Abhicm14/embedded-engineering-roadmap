# 💼 Embedded Engineering Resume, Portfolio & Interview Mastery

> Packaging your projects, passing technical phone screens, whiteboard coding, and acing the system design interview.

---

## 1. Structuring an Embedded Engineering Resume

Top tech and semiconductor firms look for evidence of real hardware interaction, not just abstract software buzzwords:

### Strong Bullet Points Follow the Formula:
$$\text{[Accomplished X]} \text{ using [Hardware/Protocol Y]}, \text{ measured by [Metric Z]}$$

- **Weak:** *"Wrote C code to read sensor data on an STM32 board."*
- **Strong:** *"Architected a non-blocking bare-metal I2C driver for Bosch BMP280 on STM32F401, implementing a circular DMA ring buffer and achieving 400 kbps throughput with zero CPU polling overhead."*
- **Weak:** *"Used FreeRTOS to manage tasks."*
- **Strong:** *"Designed a 4-task environmental data logger in FreeRTOS, eliminating priority inversion on the shared SPI bus via mutexes with Priority Inheritance Protocol and reducing sleep current to $45\,\mu\text{A}$ using Tickless Idle."*

---

## 2. Whiteboard Coding Drills for Embedded

Be prepared to code the following on a whiteboard or shared Google Doc in plain C:
1. **Circular FIFO Buffer:** Push and pop operations using pointer arithmetic and modulo/bitwise wrapping.
2. **Bit Manipulation:** Reversing 32 bits, counting set bits, and packing protocol header bitfields.
3. **Custom `memcpy` / `memset`:** Handling overlapping memory regions (like `memmove`) and 32-bit word alignment optimizations.
4. **State Machine Dispatcher:** Implementing a switch-case or table-driven FSM with event transitions.

---

## 3. The 45-Minute Embedded System Design Interview

When asked: *"Design a connected battery-powered asset tracker"* or *"Design a medical infusion pump"*:
1. **Clarify Constraints (5 mins):** Update rate, battery capacity, temperature range, wireless range, functional safety targets.
2. **Hardware Architecture (10 mins):** Choose MCU vs Linux SoC, power management (LDO vs Buck), battery chemistry, sensors, and transceivers.
3. **Firmware Architecture (15 mins):** Bare-metal vs FreeRTOS, task priority matrix, IPC mechanisms, watchdog strategy, fault detection.
4. **Failure Modes & Edge Cases (10 mins):** What happens if the flash is corrupted? Brown-out detection? Power loss mid-write?
5. **Trade-offs (5 mins):** Cost vs reliability, bandwidth vs battery lifetime.
