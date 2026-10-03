# ⚠️ Top 15 Common Embedded Engineering Mistakes

> The classic traps that cost firmware engineers hundreds of hours of debugging.

---

## 1. Firmware & C Programming Traps

1. **Missing `volatile` on ISR Flags:** The compiler optimizes the polling loop into an infinite loop because it assumes no other code can alter the variable.
2. **Calling `printf()` or `malloc()` Inside an ISR:** Non-reentrant library functions corrupt global heap pointers and cause unbounded latency or stack overflows inside interrupt handlers.
3. **Forgetting to Clear Peripheral Interrupt Flags:** If the pending bit is not cleared in hardware before exiting the ISR, the CPU re-triggers the exact same interrupt immediately upon return.
4. **Stack Overflow from Huge Local Buffers:** Declaring `uint8_t buffer[2048];` inside a function on a microcontroller with 4KB of RAM causes silent stack collision with the `.bss` or heap section.
5. **Using C Bit-Fields for Hardware Registers:** Struct bitfields have compiler-dependent bit-ordering and generate non-atomic read-modify-write assembly instructions.
6. **Ignoring Compiler Warnings:** Compiling without `-Wall -Wextra -Werror` lets silent type truncation, uninitialized variables, and signed/unsigned comparison bugs slip into production.

---

## 2. Hardware & Circuit Traps

7. **Floating Digital Input Pins:** Leaving a GPIO input pin without an internal or external pull-up/pull-down resistor causes the pin to float, picking up RF noise and toggling randomly.
8. **Probing High-Speed Signals with 1X Oscilloscope Probes:** A 1X probe adds ~100pF of parasitic capacitance, severely distorting SPI clock edges and halting crystal oscillators. Always use **10X**.
9. **Driving Inductive Loads (Relays/Motors) Without Flyback Diodes:** When a coil turns off, the collapsing magnetic field generates a massive reverse voltage spike ($V = -L \frac{di}{dt}$) that destroys switching MOSFETs and MCU pins.
10. **Swapping UART TX and RX:** Remember: Target MCU `TX` connects to Adapter `RX`; Target MCU `RX` connects to Adapter `TX`.
11. **I2C Bus Lockup on MCU Reset:** If the MCU reboots while a slave is holding `SDA` LOW, the bus remains permanently stuck. Implement the 9-clock SCL toggling recovery sequence on boot.
12. **Missing Decoupling Capacitors:** Omitting a 100nF ceramic capacitor within 3mm of every MCU $V_{DD}$ pin causes intermittent brown-out resets and erratic ADC readings.

---

## 3. RTOS & Concurrency Traps

13. **Using Binary Semaphores for Resource Protection:** Binary semaphores do **not** support Priority Inheritance! If a medium-priority task preempts, you will suffer Mars Pathfinder Unbounded Priority Inversion. Always use a Mutex.
14. **Calling Standard FreeRTOS APIs in an ISR:** Standard APIs (e.g. `xQueueSend`) attempt to execute task scheduling logic. Always use `xQueueSendFromISR` with `portYIELD_FROM_ISR()`.
15. **Under-Sizing FreeRTOS Task Stacks:** Remember that `configMINIMAL_STACK_SIZE` in FreeRTOS is defined in **32-bit words**, not bytes, but complex math, string formatting, or deep call stacks quickly exceed minimal allocations.
