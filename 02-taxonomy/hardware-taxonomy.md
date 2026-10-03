# 🔌 Hardware Topic Taxonomy for Embedded Systems

> A detailed taxonomy of electronic hardware, microcontroller silicon architectures, board-level circuit design, and laboratory instrumentation required for embedded firmware and hardware engineers.

---

## 1. Electronics Physics & Component Engineering

### 1.1 Passive Components
- **Resistors:**
  - Standard E-series values (E12, E24, E96), tolerance ratings (1%, 5%), power dissipation ($P = I^2 R = V^2 / R$).
  - Applications: Current-limiting (LEDs), pull-up / pull-down (I2C, switches), voltage dividers, current sense shunts (Kelvin connections).
- **Capacitors:**
  - Dielectric types: Ceramic (MLCC - X5R, X7R, C0G/NP0), Electrolytic, Tantalum.
  - DC bias derating of ceramic capacitors (effective capacitance drop under voltage).
  - Parasitics: Equivalent Series Resistance (ESR) and Equivalent Series Inductance (ESL).
  - Applications: Decoupling / bypass caps (bulk 10µF + local 100nF per VDD pin), AC coupling, RC timing, filter tanks.
- **Inductors & Ferrite Beads:**
  - Self-inductance, core saturation current ($I_{sat}$), DC resistance (DCR).
  - High-frequency impedance suppression: Ferrite beads for analog power rail isolation ($AVDD$).

### 1.2 Active Components & Semiconductors
- **Diodes:**
  - P-N junction vs Schottky diodes (low forward drop $\approx 0.3\text{V}$, fast recovery for reverse polarity protection and freewheeling).
  - Zener diodes for transient overvoltage clamping and simple voltage references.
  - TVS (Transient Voltage Suppressors) for ESD protection on exposed USB / CAN lines.
- **Transistors & Switches:**
  - Bipolar Junction Transistors (BJT): NPN / PNP, base saturation current, switching speed.
  - MOSFETs: N-Channel vs P-Channel; Enhancement vs Depletion mode.
  - Key parameters: Gate threshold voltage ($V_{GS(th)}$), on-resistance ($R_{DS(on)}$), gate capacitance ($Q_g$), drain-source breakdown ($V_{DS}$).
  - Topologies: High-side switching (P-FET or N-FET with charge pump) vs Low-side switching (N-FET).
- **Operational Amplifiers (Op-Amps) & Comparators:**
  - Rail-to-rail input/output (RRIO), gain-bandwidth product (GBWP), slew rate, input offset voltage.
  - Configurations: Voltage follower (buffer), non-inverting amplifier, differential amplifier, active low-pass filtering.

---

## 2. Power Supply Architecture & Energy Management

- **Power Regulation Topologies:**
  - Linear Regulators (LDO - Low Dropout): Low output noise, zero switching ripple, poor efficiency when $(V_{in} - V_{out})$ is large ($P_{loss} = (V_{in} - V_{out}) \times I_{load}$). Ideal for analog sensors and clean MCU rails.
  - Switched-Mode Power Supplies (SMPS):
    - Buck (Step-down), Boost (Step-up), Buck-Boost.
    - High efficiency (85-95%), switching frequencies (500 kHz - 2 MHz), inductor ripple current sizing, output capacitor ESR impact on ripple.
- **Battery Management & Power Budgeting:**
  - Cell chemistries: Li-Ion / Li-Po (nominal 3.7V, cut-off 3.0V, charging 4.2V CC/CV profile).
  - Primary cells: CR2032 coin cells (high internal resistance, pulsed load handling).
  - Protection ICs (overvoltage, undervoltage, overcurrent) and Fuel Gauges (Coulomb counters).
- **MCU Low-Power Modes:**
  - Sleep / Wait: Core clock stopped, peripherals active, fast wake-up.
  - Stop / Deep-Sleep: Core and peripheral clocks off, SRAM retained, wake-up via external interrupt or RTC.
  - Standby / Hibernation: Regulators powered down, SRAM lost, only backup domain / RTC powered ($< 1\mu\text{A}$).

---

## 3. Microcontroller & Processor Silicon Architectures

- **Core Microcontroller Families:**
  - **ARM Cortex-M Series:**
    - Cortex-M0/M0+: Ultra-low power, 2-stage pipeline, von Neumann architecture.
    - Cortex-M3: Balanced 32-bit computing, 3-stage pipeline, Harvard architecture, hardware divide.
    - Cortex-M4: DSP instructions, Single Precision Floating-Point Unit (FPU).
    - Cortex-M7: Superscalar, dual-issue pipeline, 6-stage pipeline, L1 caches, double-precision FPU.
    - Cortex-M33: ARMv8-M with TrustZone hardware security isolation.
  - **ARM Cortex-A Series (Embedded Linux):** Application processors (Cortex-A7, A53, A72), MMU, multi-core SMP, NEON SIMD.
  - **RISC-V:** Open standard ISA (RV32I, RV32IMAC), customizable extensions.
  - **Espressif (ESP32):** Xtensa dual-core / RISC-V single-core, integrated 2.4 GHz Wi-Fi and Bluetooth baseband.
  - **AVR / 8-bit:** Classic 8-bit Harvard RISC architectures (ATmega328P).
- **Internal Silicon Subsystems:**
  - Phase-Locked Loops (PLL) and internal/external clock oscillators (HSE, HSI, LSE 32.768 kHz for RTC).
  - Direct Memory Access (DMA) controllers: FIFO modes, circular mode, memory-to-peripheral, peripheral-to-memory, memory-to-memory.
  - Analog-to-Digital Converters (ADC): SAR (Successive Approximation Register), resolution (10/12/16-bit), sampling rate, sampling time, input impedance, analog reference voltage ($V_{REF+}$).
  - Digital-to-Analog Converters (DAC) and Analog Comparators.
  - Hardware Timers: Advanced control timers (complementary PWM with dead-time), general-purpose timers, watchdog timers (Independent Watchdog `IWDG`, Window Watchdog `WWDG`).

---

## 4. Board-Level Hardware Design (Schematics & PCB)

- **Schematic Capture (KiCad / Altium):**
  - Hierarchical schematics, net labeling, power flags, decoupling capacitor placement next to IC power pins.
  - Bill of Materials (BOM) management and component sourcing (LCSC, Mouser, Digikey).
- **Printed Circuit Board (PCB) Layout:**
  - Layer stackups: 2-layer vs 4-layer (Signal-GND-Power-Signal).
  - Ground planes: Continuous solid ground return paths, avoiding split planes under high-speed traces.
  - Trace impedance control: $50\Omega$ single-ended (RF/antenna traces), $90\Omega$ differential pairs (USB D+/D-), $120\Omega$ differential pairs (CAN bus).
  - Current capacity & thermal vias: Sizing power traces using IPC-2152 standards.
- **Design for Manufacturing & Test (DFM / DFT):**
  - SMD footprint clearances, silkscreen polarity markings, fiducial markers for pick-and-place machines.
  - Test points on critical nets (SWD, UART, Reset, Power rails).

---

## 5. Signal Integrity, EMI/EMC & Noise Mitigation

- **High-Speed Phenomena:**
  - Signal rise time ($t_r$) vs trace length: When a trace becomes a transmission line ($l > \frac{t_r}{6 \times t_{pd}}$).
  - Reflections, ringing, and series termination resistors (e.g. $22\Omega - 33\Omega$ on high-speed SPI / SDIO lines).
  - Capacitive and inductive crosstalk between parallel traces.
- **Electromagnetic Compatibility (EMC):**
  - Radiated vs Conducted emissions.
  - Ground loops and minimizing loop area for current return paths.
  - Shielding, ferrite chokes, and common-mode chokes for USB / Ethernet / CAN interfaces.
- **Electrostatic Discharge (ESD):**
  - IEC 61000-4-2 standard (air discharge vs contact discharge).
  - Placement of clamping TVS arrays directly at the connector edge before signals enter ICs.

---

## 6. Laboratory Test & Measurement Equipment

- **Digital Storage Oscilloscope (DSO):**
  - Bandwidth specification: $\text{Bandwidth} \ge 5 \times f_{signal}$ for accurate square-wave rise-time measurement.
  - Probe calibration: Compensating $10\text{X}$ passive probes with trimmer capacitor to prevent over/undershoot.
  - Triggering mastery: Normal, Auto, Single-shot, Edge, Pulse width, Runt triggers.
  - Math channels: FFT for spectral noise analysis, $A - B$ for differential voltage.
- **Logic Analyzers & Protocol Decoders:**
  - Asynchronous vs synchronous state capture.
  - Using open-source Sigrok / PulseView for multi-channel protocol analysis.
  - Triggering on protocol packets (e.g., trigger when I2C slave address matches `0x68`).
- **Precision DC Power Supplies & Current Monitors:**
  - Constant Voltage (CV) vs Constant Current (CC) mode.
  - Measuring dynamic current profiles (active vs sleep states) using high-side sense amplifiers or source-measure units (SMUs).
